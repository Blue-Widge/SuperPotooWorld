#ifndef _RE_LIB_H_
#define _RE_LIB_H_

#include <string.h>

#ifdef _WIN32
// -------------------------------------------------------------------------------------------------
// D�clarations pour window

#define RE_INLINE inline

#ifdef RENDERINGENGINE_EXPORTS
#define RE_API __declspec(dllexport)
#else
#define RE_API __declspec(dllimport)
#endif

#else
// -------------------------------------------------------------------------------------------------
// D�clarations pour linux

#define RE_INLINE static inline

#define RE_API

#endif

// -------------------------------------------------------------------------------------------------
// Fonctions diff�rentes window/linux

RE_INLINE void RE_Memcpy(
    void *destination, long long destinationSize,
    void *source, long long sourceSize)
{
#ifdef _WIN32
    memcpy_s(destination, destinationSize, source, sourceSize);
#else
    memcpy(destination, source, sourceSize);
#endif
}

#endif