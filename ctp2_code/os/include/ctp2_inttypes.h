/**
 * $Id$
 */
#ifdef HAVE_PRAGMA_ONCE
#pragma once
#endif
#ifndef __os_include_ctp2_inttypes_h__
#define __os_include_ctp2_inttypes_h__ 1

#include "os/include/ctp2_config.h"

#if defined(_MSC_VER)

#include <stdint.h>

typedef char          schar;
typedef unsigned char uchar;

typedef int8_t   sint8;
typedef uint8_t  uint8;
typedef int16_t  sint16;
typedef uint16_t uint16;
typedef int32_t  sint32;
typedef uint32_t uint32;
typedef int64_t  sint64;
typedef uint64_t uint64;

typedef char          CHAR;
#else

#if defined(HAVE_INTTYPES_H)
#include <inttypes.h>
#endif
#if defined(HAVE_STDINT_H)
#include <stdint.h>
#endif

typedef signed char          schar;
typedef unsigned char        uchar;

typedef int8_t               sint8;
typedef uint8_t              uint8;
typedef int16_t              sint16;
typedef uint16_t             uint16;
typedef int32_t              sint32;
typedef uint32_t             uint32;
typedef int64_t              sint64;
typedef uint64_t             uint64;

#endif// !defined(_MSC_VER)

#endif
