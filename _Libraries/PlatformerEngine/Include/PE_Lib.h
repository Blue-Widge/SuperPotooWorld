#ifndef _PE_LIB_H_
#define _PE_LIB_H_

#include <string.h>

#ifdef _WIN32
// -------------------------------------------------------------------------------------------------
// D�clarations pour window

#define PE_INLINE inline

#ifdef PLATFORMERENGINE_EXPORTS
#define PE_API __declspec(dllexport)
#else
#define PE_API __declspec(dllimport)
#endif

#else
// -------------------------------------------------------------------------------------------------
// D�clarations pour linux

#define PE_INLINE static inline

#define PE_API

#endif

// -------------------------------------------------------------------------------------------------
// Fonctions diff�rentes window/linux

PE_API PE_INLINE void PE_Memcpy(
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