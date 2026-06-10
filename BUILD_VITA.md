# Wagic for PS Vita

Native PS Vita port of [Wagic: The Homebrew](https://github.com/WagicProject/wagic). Renders at 960x544 with vitaGL and SDL2. No PSP backward-compat layer.

The released VPK ships without bundled card art. Cards display with placeholder graphics. Wizards of the Coast card images cannot be redistributed. To play with bundled art, build the full VPK locally using the steps in "Personal build with card art" below.

## Prerequisites

1. VitaSDK with `$VITASDK` set. See https://vitasdk.org/, then run `vdpm`.

2. Libraries via `vdpm install ...`:

   ```
   zlib libpng libjpeg-turbo freetype
   SDL2 SDL2_mixer libvorbis libogg libmikmod libFLAC mpg123
   ```

3. vitaGL, pinned to commit `d4a8f9d` (Jan 10 2026). Build it locally and let CMake link against the local copy. The system `/usr/local/vitasdk` vitaGL is left untouched.

   ```bash
   mkdir -p vendor
   cd vendor
   git clone https://github.com/Rinnegatamante/vitaGL.git
   cd vitaGL
   git checkout d4a8f9d
   make NO_DEBUG=1 -j$(nproc)
   ```

   This produces `vendor/vitaGL/libvitaGL.a`. CMake auto-detects it and links against it before the system vitaGL. Do not run `make install` here. That would overwrite the system vitaGL.

4. `libshacccg.suprx` extracted on your Vita. vitaGL needs it for shader compilation. See the vitaGL README for extraction steps.

## Build

### Standard build (released VPK)

```bash
mkdir build_vita && cd build_vita
cmake -DCMAKE_TOOLCHAIN_FILE=$VITASDK/share/vita.toolchain.cmake ..
make -j$(nproc) wagic.self-self vpk
```

Output: `wagic.vpk`, around 31 MB. Contains the executable, LiveArea assets, and core game resources (UI graphics, sounds, card data, AI decks). Cards render with placeholder art.

The two-stage `wagic.self-self vpk` invocation is required. The vitasdk `vita_create_self` macro does not expose `wagic.self` as a transitive dependency of the VPK target, so `make vpk` alone will not relink when sources change.

### Personal build with card art

This path is local only. You supply the card images. The build bundles whatever you point it at.

First, download images. The included script pulls them from Scryfall:

```bash
# All ~75,000 images at normal quality (~5.7 GB)
python3 download_card_images.py -o ./card_images

# Smaller images (~1.1 GB)
python3 download_card_images.py -o ./card_images --size small

# Specific sets only
python3 download_card_images.py -o ./card_images --sets 10E,M15,LEA

# Resume a partial download
python3 download_card_images.py -o ./card_images --resume
```

Then build with the images included:

```bash
mkdir build_vita && cd build_vita
cmake -DCMAKE_TOOLCHAIN_FILE=$VITASDK/share/vita.toolchain.cmake \
      -DCARD_IMAGES_DIR=./card_images \
      ..
make -j$(nproc) wagic.self-self vpk-full
```

Output: `wagic-full.vpk`. Size scales with the number of sets bundled. The full Scryfall pull at small quality is around 880 MB for ~70,000 images across 290+ sets.

The card archive is built once into `wagic-card-images.zip` and reused on later `make vpk-full` runs. To force a rebuild after adding sets, run `rm wagic-card-images.zip && make vpk-full`.

### Card images directory layout

```
/path/to/card_images/
  sets/
    10E/
      129572.jpg
      129573.jpg
      ...
    M15/
      M15.zip          (per-set zip also works)
    ...
```

Each card is named by its Multiverse ID (`<MTGID>.jpg`). A per-set zip named `<SETCODE>.zip` inside the set folder is also accepted. The game checks both at runtime.

## Install

1. Transfer `wagic.vpk` to your Vita over FTP or USB.
2. Open VitaShell.
3. Navigate to the VPK and press Cross to install.
4. Launch from the LiveArea.

## Controls

| Input | Action |
|-------|--------|
| D-pad | Navigate menus and card grid |
| Left analog stick | Same as D-pad |
| Cross (X) | OK, confirm |
| Circle (O) | Secondary action |
| Triangle | Cancel, back |
| Square | Primary action |
| L trigger | Previous phase, previous item |
| R trigger | Open hand, next item |
| Start | Pause menu |
| Select | Control, context menu |
| Front touchscreen, tap | Select at finger position |
| Front touchscreen, drag | Hover and scroll |
| Front touchscreen, flick | Inertial scroll |

A tap is a touch shorter than 250 ms within a 50 px radius. Anything longer or further is treated as a drag. The mapping above is hard-coded. Rebinding is not supported.

## Technical notes

- Renders at 960x544. Game logic runs in PSP virtual coordinates (480x272) and scales up at draw time.
- Frame target: 30 fps.
- Graphics: vitaGL pinned to commit `d4a8f9d`, built with `NO_DEBUG=1`. VRAM-resident textures, cached system memory for pools, triple buffering, vsync on. 4 MB param buffer. 256 KB legacy pool for immediate-mode UI rendering.
- Audio: SDL2_mixer at 44.1 kHz stereo. Music is MP3, SFX is WAV.
- Input: native sceCtrl and sceTouch, not SDL. Left analog stick is converted to D-pad events with a deadzone.
- Filesystem: read-only `app0:Res/core.zip` for assets, read-write `ux0:data/Wagic/` for saves and settings. The zip-aware filesystem layer treats loose files and zip-bundled assets the same.
- No networking. All content ships offline.

## Save data location

| Path | Access | Contents |
|------|--------|----------|
| `ux0:data/Wagic/settings/options.txt` | RW | Volume, language, controls, tutorial flags |
| `ux0:data/Wagic/player/` | RW | Decks, collection, save profiles |
| `ux0:data/Wagic/debug.txt` | RW | Boot log, overwritten each launch |

To wipe progress, delete the `ux0:data/Wagic/` folder in VitaShell.

## Credits

Wagic engine and game by the [Wagic Project](https://github.com/WagicProject/wagic). PS Vita port glue code (vitaGL/SDL2 backend, native input, build) added on top of the upstream tree. See `CHANGELOG.md` for upstream history.
