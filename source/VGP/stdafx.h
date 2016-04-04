#pragma once
#ifndef VGP_STDAFX_HPP
#define VGP_STDAFX_HPP



#if defined(_WIN32)
#include <SDKDDKVer.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif

#endif

#ifdef _DEBUG
#define SPDLOG_TRACE_ON
#define SPDLOG_DEBUG_ON
#endif


#endif
