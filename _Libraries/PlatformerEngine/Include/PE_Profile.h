
#ifndef _PE_PROFILE_H_
#define _PE_PROFILE_H_

#include <string.h>
#include <omp.h>
#include "PE_Lib.h"

typedef struct PE_Profile_s
{
    double m_step;
    double m_broadPhase;
    double m_narrowPhase;
    double m_timeOfImpact;
    double m_solver;
    double m_integration;

    double m_startStep;
    double m_startBP;
    double m_startNP;
    double m_startTOI;
    double m_startSolver;
    double m_startInt;

    int m_nbSteps;
    int m_nbCollisions;
    int m_nbIslands;
    int m_nbSolverIterations;
} PE_Profile;

PE_INLINE void PE_Profile_reset(PE_Profile *profile)
{
    memset(profile, 0, sizeof(PE_Profile));
}

PE_INLINE void PE_Profile_startStep(PE_Profile *profile)
{
    profile->m_startStep = omp_get_wtime();
}

PE_INLINE void PE_Profile_startIntegration(PE_Profile *profile)
{
    profile->m_startInt = omp_get_wtime();
}

PE_INLINE void PE_Profile_startBroadPhase(PE_Profile *profile)
{
    profile->m_startBP = omp_get_wtime();
}

PE_INLINE void PE_Profile_startNarrowPhase(PE_Profile *profile)
{
    profile->m_startNP = omp_get_wtime();
}

PE_INLINE void PE_Profile_startTOI(PE_Profile *profile)
{
    profile->m_startTOI = omp_get_wtime();
}

PE_INLINE void PE_Profile_startSolver(PE_Profile *profile)
{
    profile->m_startSolver = omp_get_wtime();
}

PE_INLINE void PE_Profile_stopStep(PE_Profile *profile)
{
    profile->m_step += omp_get_wtime() - profile->m_startStep;
    profile->m_nbSteps++;
}

PE_INLINE void PE_Profile_stopIntegration(PE_Profile *profile)
{
    profile->m_integration += omp_get_wtime() - profile->m_startInt;
}

PE_INLINE void PE_Profile_stopBroadPhase(PE_Profile *profile)
{
    profile->m_broadPhase += omp_get_wtime() - profile->m_startBP;
}

PE_INLINE void PE_Profile_stopNarrowPhase(PE_Profile *profile)
{
    profile->m_narrowPhase += omp_get_wtime() - profile->m_startNP;
}

PE_INLINE void PE_Profile_stopTOI(PE_Profile *profile)
{
    profile->m_timeOfImpact += omp_get_wtime() - profile->m_startTOI;
}

PE_INLINE void PE_Profile_stopSolver(PE_Profile *profile)
{
    profile->m_solver += omp_get_wtime() - profile->m_startSolver;
}

PE_INLINE void PE_Profile_addCollisions(PE_Profile *profile, int nbCollisions)
{
    profile->m_nbCollisions += nbCollisions;
}

PE_INLINE void PE_Profile_addIslands(PE_Profile *profile, int nbIslands)
{
    profile->m_nbIslands += nbIslands;
}

PE_INLINE void PE_Profile_addSolverIterations(PE_Profile *profile, int nbSolverIterations)
{
    profile->m_nbSolverIterations += nbSolverIterations;
}

void PE_Profile_print(PE_Profile *profile);

#endif
