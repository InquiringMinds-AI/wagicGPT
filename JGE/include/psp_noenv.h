#ifndef WAGIC_PSP_NOENV_H
#define WAGIC_PSP_NOENV_H
/* Force-included into every PSP TU (see the -include flag in both Makefiles).
 *
 * On real PSP hardware newlib's environ points into UNINITIALIZED memory, so
 * getenv() returns garbage non-NULL pointers - nondeterministically, since it
 * depends on RAM contents at boot. PPSSPP zeroes memory, so the emulator
 * always returns NULL and the bug is invisible there. Field-diagnosed on a
 * PSP-2000 (2026-07-31): getenv("WAGIC_SELFPLAY") came back non-NULL and the
 * selfplay harness hijacked every real game entry.
 *
 * There is no environment on the PSP: make every getenv call answer NULL.
 */
#include <stdlib.h>
static inline char * wagic_psp_getenv(const char * name) { (void)name; return 0; }
#define getenv(x) wagic_psp_getenv(x)
#endif
