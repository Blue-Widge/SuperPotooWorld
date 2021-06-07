
#ifndef _PE_SETTINGS_H_
#define _PE_SETTINGS_H_

#include "PE_Profile.h"
#include "PE_Lib.h"
#include <stdint.h>

typedef int Bool;
#define TRUE 1
#define FALSE 0

#define PE_TIME_TO_SLEEP          0.25f
#define PE_LINEAR_SLEEP_TOLERANCE 0.05f
#define PE_SKIN_RADIUS            1.0f / 256.0f
#define PE_COLLISION_TIME         3.0f / 60.0f
#define PE_TRIGGER_TIME           2.0f / 60.0f
#define PE_SOLVER_EPSILON         1.0f / 256.0f

//#define _PE_DEFAULT_ALLOCATOR_
//#define PE_PROFILE

PE_Profile g_profile;

typedef unsigned char  uint8;  /// sizeof(uint8)  must be 1
typedef signed short   sint16; /// sizeof(sint16) must be 2
typedef unsigned short uint16; /// sizeof(uint16) must be 2
typedef signed int     sint32; /// sizeof(sint32) must be 4
typedef unsigned int   uint32; /// sizeof(uint32) must be 4

/// @brief Equivalent du pointeur null pour les ID sur 32 bits
#define	PE_INVALID_ID 0xffffffff 

#endif
