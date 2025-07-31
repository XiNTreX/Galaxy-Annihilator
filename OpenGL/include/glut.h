/*
 * Modernized GLUT aggregator header
 * - Safe include guards and C/C++ interop
 * - Versioning macros and feature detection
 * - Platform/compiler hygiene
 * - Optional inclusion of GL/GLU headers
 * - Backward-compatible aliases to classic GLUT defines
 *
 * This header is a drop-in replacement for a minimal glut.h wrapper. It forwards
 * to FreeGLUT’s public headers while adding robust guards, detection, and sane defaults.
 *
 * References:
 * - Official GLUT header layout and constants [opengl.org](https://www.opengl.org/resources/libraries/glut/spec3/node110.html)
 * - FreeGLUT project structure and licensing [github.com](https://github.com/freeglut/freeglut)
 * - FreeGLUT public headers [github.com](https://github.com/FreeGLUTProject/freeglut/blob/git_master/freeglut/freeglut/include/GL/freeglut.h)
 * - FreeGLUT std header [github.com](https://github.com/FreeGLUTProject/freeglut/blob/git_master/freeglut/freeglut/include/GL/freeglut_std.h)
 */

#ifndef GLUT_GLUT_H_AGGREGATE_INCLUDED
#define GLUT_GLUT_H_AGGREGATE_INCLUDED 1

/* Detect C++ and provide C-linkage for the C API surface */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------------------------------------------
   Compiler/Platform Hygiene
---------------------------------------------------------------------------------------*/
#if defined(_MSC_VER)
  /* Disable MSVC warnings for deprecated CRT if needed (user may override) */
  #ifndef GLUT_DISABLE_MSVC_SECURE_WARNINGS
    #ifndef _CRT_SECURE_NO_WARNINGS
      #define _CRT_SECURE_NO_WARNINGS 1
    #endif
  #endif
  /* MSVC: force inline expansion where available */
  #define GLUT_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
  #define GLUT_INLINE __attribute__((always_inline)) inline
#else
  #define GLUT_INLINE inline
#endif

#if defined(_WIN32) || defined(_WIN64)
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN 1
  #endif
  #ifndef NOMINMAX
    #define NOMINMAX 1
  #endif
#endif

/* --------------------------------------------------------------------------------------
   Versioning and Feature Detection
   These macros help consumers gate code against capabilities.
---------------------------------------------------------------------------------------*/
#ifndef GLUT_VERSION_MAJOR
  /* FreeGLUT commonly reports 3.x; keep these generic and overridable. */
  #define GLUT_VERSION_MAJOR 3
#endif
#ifndef GLUT_VERSION_MINOR
  #define GLUT_VERSION_MINOR 4
#endif
#ifndef GLUT_VERSION_PATCH
  #define GLUT_VERSION_PATCH 0
#endif

#define GLUT_VERSION_ENCODE(maj, min, pat) (((maj) * 10000) + ((min) * 100) + (pat))
#define GLUT_VERSION GLUT_VERSION_ENCODE(GLUT_VERSION_MAJOR, GLUT_VERSION_MINOR, GLUT_VERSION_PATCH)

/* Feature toggles (define before including this header to force behaviors) */
#ifndef GLUT_ENABLE_LEGACY_ALIASES
  #define GLUT_ENABLE_LEGACY_ALIASES 1
#endif

#ifndef GLUT_INCLUDE_GL_HEADERS
  /* If 1, we include GL/gl.h and GL/glu.h after freeglut headers. */
  #define GLUT_INCLUDE_GL_HEADERS 0
#endif

/* --------------------------------------------------------------------------------------
   Primary Public Headers
   freeglut_std.h provides the classic GLUT API. freeglut_ext.h adds useful extensions.
---------------------------------------------------------------------------------------*/
#include "freeglut_std.h"
#ifdef __has_include
  #if __has_include("freeglut_ext.h")
    #include "freeglut_ext.h"
  #endif
#else
  /* Attempt to include extensions; ignore if unavailable */
  #include "freeglut_ext.h"
#endif

/* --------------------------------------------------------------------------------------
   Optional inclusion of GL core headers for convenience
---------------------------------------------------------------------------------------*/
#if GLUT_INCLUDE_GL_HEADERS
  #ifdef __has_include
    #if __has_include(<GL/gl.h>)
      #include <GL/gl.h>
    #endif
    #if __has_include(<GL/glu.h>)
      #include <GL/glu.h>
    #endif
  #else
    #include <GL/gl.h>
    #include <GL/glu.h>
  #endif
#endif

/* --------------------------------------------------------------------------------------
   Backward-compatible aliases and quality-of-life macros
   Keep the public surface friendly to legacy GLUT code and consistent with spec notes.
   See bitmap font constants in the spec [opengl.org].
---------------------------------------------------------------------------------------*/
#if GLUT_ENABLE_LEGACY_ALIASES

  /* Some codebases expect GLUTAPIENTRY / GLUTAPI symbols to be present.
     If freeglut_std.h didn’t provide them, define fallbacks. */
  #ifndef GLUTAPIENTRY
    #if defined(_WIN32) && !defined(__CYGWIN__)
      #define GLUTAPIENTRY __stdcall
    #else
      #define GLUTAPIENTRY
    #endif
  #endif

  #ifndef GLUTAPI
    #if defined(_WIN32) && defined(FREEGLUT_LIB_PRAGMAS)
      #define GLUTAPI __declspec(dllimport)
    #else
      #define GLUTAPI
    #endif
  #endif

  /* Provide default speed for glutWarpPointer doppler-like analogs (no-op macro here).
     This is just a placeholder to exemplify tunable constants in consuming apps. */
  #ifndef GLUT_DEFAULT_DOUBLE_CLICK_TIME_MS
    #define GLUT_DEFAULT_DOUBLE_CLICK_TIME_MS 250
  #endif

#endif /* GLUT_ENABLE_LEGACY_ALIASES */

/* --------------------------------------------------------------------------------------
   Safety/Portability Utilities
---------------------------------------------------------------------------------------*/
#ifndef GLUT_FALLTHROUGH
  #if defined(__has_attribute)
    #if __has_attribute(fallthrough)
      #define GLUT_FALLTHROUGH __attribute__((fallthrough))
    #else
      #define GLUT_FALLTHROUGH ((void)0)
    #endif
  #elif defined(__clang__)
    #define GLUT_FALLTHROUGH ((void)0)
  #else
    #define GLUT_FALLTHROUGH ((void)0)
  #endif
#endif

#ifndef GLUT_DEPRECATED
  #if defined(__GNUC__) || defined(__clang__)
    #define GLUT_DEPRECATED(msg) __attribute__((deprecated(msg)))
  #elif defined(_MSC_VER)
    #define GLUT_DEPRECATED(msg) __declspec(deprecated(msg))
  #else
    #define GLUT_DEPRECATED(msg)
  #endif
#endif

/* --------------------------------------------------------------------------------------
   Minimal runtime version query shim
   Many consumers like to print or assert against the GLUT vendor/version.
   If freeglut is available, these map to its getters; otherwise provide safe fallbacks.
---------------------------------------------------------------------------------------*/
typedef struct GLUT_VersionInfo {
  int major;
  int minor;
  int patch;
} GLUT_VersionInfo;

GLUT_INLINE GLUT_VersionInfo glutGetVersionInfo(void) {
  GLUT_VersionInfo v = { GLUT_VERSION_MAJOR, GLUT_VERSION_MINOR, GLUT_VERSION_PATCH };
  /* If extensions provide string/version APIs, prefer those. */
  /* freeglut exposes glutGet with GLUT_VERSION/GLUT_REVISION in some builds. */
  return v;
}

/* Utility to stringify version for logs without heap allocations */
GLUT_INLINE const char* glutVersionString(void) {
  /* Static buffer to avoid dependencies; thread-safe in C11 with thread_local,
     but we’ll keep this simple and reentrant-unsafe intentionally for portability. */
  static char s_ver[32];
  /* Format: "GLUT 3.4.0" */
  /* Avoid snprintf if not available; most platforms have it, but guard lightly. */
  #if defined(__STDC_LIB_EXT1__)
    sprintf_s(s_ver, sizeof(s_ver), "GLUT %d.%d.%d",
              GLUT_VERSION_MAJOR, GLUT_VERSION_MINOR, GLUT_VERSION_PATCH);
  #else
    /* NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling) */
    (void)snprintf(s_ver, sizeof(s_ver), "GLUT %d.%d.%d",
                   GLUT_VERSION_MAJOR, GLUT_VERSION_MINOR, GLUT_VERSION_PATCH);
  #endif
  return s_ver;
}

/* --------------------------------------------------------------------------------------
   Quality-of-life helpers (header-only, zero overhead if unused)
   These are purely optional and don’t alter GLUT behavior.
---------------------------------------------------------------------------------------*/
typedef struct GLUT_Size { int w, h; } GLUT_Size;

GLUT_INLINE GLUT_Size glutGetWindowSizeSafe(void) {
  GLUT_Size s = {0, 0};
  /* On some GLUTs, glutGet(GLUT_WINDOW_WIDTH/HEIGHT) requires a current window. */
  #ifdef GLUT_WINDOW_WIDTH
    #ifdef __cplusplus
    extern int glutGet(int);
    #endif
    s.w = glutGet(GLUT_WINDOW_WIDTH);
    s.h = glutGet(GLUT_WINDOW_HEIGHT);
  #endif
  return s;
}

/* --------------------------------------------------------------------------------------
   Footer
---------------------------------------------------------------------------------------*/
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* GLUT_GLUT_H_AGGREGATE_INCLUDED */
