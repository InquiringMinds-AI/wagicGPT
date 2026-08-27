#!/bin/bash
#Run the worktree-built suite binary in the MAIN-TREE bin env (the only env
#where the suite works - worktree bin lacks User/profile state).
#Usage: run-suite.sh [testlist-relative-to-Res] ; no arg = full suite.
#
#GATE GAP FIX (2026-08-07): the binary resolves Res from the CWD (Res.txt
#points at a nonexistent ../../wagic_res/, so the fallback is ./Res), which
#means fixtures + manifests load from the MAIN tree - worktree-only fixtures
#were silently never run. Sync worktree fixture files ADDITIVELY (main-tree
#manifest stays unpolluted per the standing ruling) and default to the
#worktree manifest under its own name. List entries are loaded as
#"test/"+line (TestSuiteAI.cpp:1413) - never prefix entries with test/.
WT=/home/magi/Projects/wagicGPT-psp/projects/mtg
MAIN=/home/magi/Projects/wagicGPT/projects/mtg
for f in "$WT"/bin/Res/test/*.txt; do
    b=$(basename "$f")
    [ "$b" = "_tests.txt" ] && continue   #main manifest stays unpolluted
    cp -u "$f" "$MAIN"/bin/Res/test/"$b"
done
cp "$WT"/bin/Res/test/_tests.txt "$MAIN"/bin/Res/test/_tests_psp.txt
cd "$MAIN"/bin || exit 1
LIST="${1:-_tests_psp.txt}"
exec env WAGIC_HEADLESS=1 WAGIC_TESTSUITE=1 WAGIC_TESTSUITE_THREADS=1 WAGIC_TESTSUITE_FILE="test/$LIST" "$WT"/bin/wagic
