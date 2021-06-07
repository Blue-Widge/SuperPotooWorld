
#ifndef _PE_DISTANCE_H_
#define _PE_DISTANCE_H_

#include "PE_Settings.h"
#include "PE_Shape.h"
#include "PE_Math.h"

float PE_Distance_Shape(
    PE_Shape *shapeA, PE_Sweep *sweepA,
    PE_Shape *shapeB, PE_Sweep *sweepB, float alpha);

#endif
