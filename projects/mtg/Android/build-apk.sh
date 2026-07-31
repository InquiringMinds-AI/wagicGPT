#!/usr/bin/env bash
# Modern command-line APK build for Wagic (replaces the dead ant flow).
# Needs: Android SDK (build-tools, platform android-36+), NDK output in libs/,
# java 17+. Run ndk-build first; then this script produces bin/wagic-debug.apk.
set -euo pipefail
cd "$(dirname "$0")"

SDK="${ANDROID_SDK_ROOT:-$HOME/Android/Sdk}"
BT="$SDK/build-tools/36.0.0"
PLATFORM="$SDK/platforms/android-36/android.jar"
OUT=bin-modern
rm -rf "$OUT" && mkdir -p "$OUT/gen" "$OUT/classes" "$OUT/apk"

# 1. Resources -> proto APK + R.java
"$BT/aapt2" compile --dir res -o "$OUT/res.zip"
"$BT/aapt2" link -o "$OUT/base.apk" -I "$PLATFORM" \
    --manifest AndroidManifest.xml -R "$OUT/res.zip" \
    --java "$OUT/gen" --auto-add-overlay

# 2. Java -> classes (bundled jars: zip4j, jsoup, json-simple)
JARS=$(ls libs/*.jar | tr '\n' ':')
find src "$OUT/gen" -name '*.java' > "$OUT/sources.txt"
javac -source 8 -target 8 -bootclasspath "$PLATFORM" \
    -classpath "$PLATFORM:$JARS" -d "$OUT/classes" @"$OUT/sources.txt" 2> >(grep -v 'source value 8' >&2 || true)

# 3. Dex (app classes + bundled jars)
"$BT/d8" --release --lib "$PLATFORM" --output "$OUT/apk" \
    $(find "$OUT/classes" -name '*.class') libs/*.jar

# 4. Assemble: start from the aapt2 base, add dex + native libs (+ assets if any)
cp "$OUT/base.apk" "$OUT/apk/wagic-unaligned.apk"
(cd "$OUT/apk" && zip -q -j wagic-unaligned.apk classes.dex)
# arm64-v8a ONLY: libs/armeabi-v7a holds TRACKED UPSTREAM PREBUILTS (not this
# fork's engine) - shipping them would give 32-bit devices the wrong engine.
mkdir -p "$OUT/apk/lib/arm64-v8a"
cp libs/arm64-v8a/*.so "$OUT/apk/lib/arm64-v8a/"
(cd "$OUT/apk" && zip -q -r wagic-unaligned.apk lib)
# assets/_keystore holds upstream's release keystore - never package it.
if [ -d assets ] && [ -n "$(ls -A assets 2>/dev/null)" ]; then
    (cd . && zip -q -r "$OUT/apk/wagic-unaligned.apk" assets -x 'assets/_keystore/*')
fi

# 5. Align + sign (debug key, auto-generated once)
KEYSTORE="$HOME/.android/wagic-debug.keystore"
if [ ! -f "$KEYSTORE" ]; then
    keytool -genkeypair -keystore "$KEYSTORE" -storepass wagicdebug -keypass wagicdebug \
        -alias wagicdebug -keyalg RSA -keysize 2048 -validity 10000 \
        -dname "CN=Wagic Debug"
fi
mkdir -p bin
"$BT/zipalign" -f 4 "$OUT/apk/wagic-unaligned.apk" bin/wagic-debug.apk
"$BT/apksigner" sign --ks "$KEYSTORE" --ks-pass pass:wagicdebug \
    --key-pass pass:wagicdebug bin/wagic-debug.apk
echo "OK: $(ls -la bin/wagic-debug.apk | awk '{print $5}') bytes -> bin/wagic-debug.apk"
