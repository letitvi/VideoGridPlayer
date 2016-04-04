
#pragma once
#ifndef VGP_EXPORT_HPP
#define VGP_EXPORT_HPP


#include <vgp/config.hpp>

/* Define portable import / export macros */
#if defined(VGP_EXPORTS)
#define VGP_API VGP_API_EXPORT
#else
#define VGP_API VGP_API_IMPORT
#endif


#endif // VGP_EXPORT_HPP

