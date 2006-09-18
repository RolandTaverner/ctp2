#ifndef __os_include_ctp2_config_h__
#define __os_include_ctp2_config_h__ 1

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif

#if defined(HAVE_CONFIG_H)
#include "config.h"
#elif defined(_MSC_VER)
#include "config_win32.h"
#else 
#error "Unsupported build environment."
#endif

#if defined(_MSC_VER) && (_MSC_VER < 1400)
// MSVC7 has no knowledge about functions that do not return, 
// and will report missing return statements.
#define UNREACHABLE_RETURN(x) return(x)
#else
#define UNREACHABLE_RETURN(x)
#endif

#endif

