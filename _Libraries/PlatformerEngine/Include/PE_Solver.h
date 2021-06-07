
#ifndef _PE_SOLVER_H_
#define _PE_SOLVER_H_

#include "PE_Settings.h"
#include "PE_List.h"
#include "PE_Collision.h"
#include "PE_Body.h"
#include "PE_Allocator.h"

typedef struct PE_Island_s PE_Island;

///////////////////////////////////////////////////////////////////////////////////////////////////
//  PE_SolverBody

/// @brief A SolverBody contains the solver data associated with a body.
/// Each body contains this object.
typedef struct PE_SolverBody_s
{
    /// @brief Pointer to the associated body.
    PE_Body *m_body;

    /// @brief Number of times the solver has moved the body during the current fixed update.
    int m_nbIterations;
} PE_SolverBody;

/// @brief Contructs the slover data attached to a body.
/// @param[in] body the body associated with this data.
/// @param[in] allocator the world allocator.
/// @return A pointer to the solver data initialized.
PE_SolverBody *PE_SolverBody_new(PE_Body *body, PE_Allocator *allocator);

/// @brief Destructs the solver data associated with a body.
/// @param solverBody the solver data to destruct.
/// @param allocator the world allocator.
void PE_SolverBody_free(PE_SolverBody *solverBody, PE_Allocator *allocator);

/// @brief Resets the solver data associated with a body.
/// This function is called once by fixed update of the physics engine.
/// @param solverBody the solver data to reset.
void PE_SolverBody_reset(PE_SolverBody *solverBody);

///////////////////////////////////////////////////////////////////////////////////////////////////
//  PE_SolverCollisionPair

/// @brief A SolverCollisionPair contains the solver data associated with a collision pair.
/// Each collision pair contains this object.
typedef struct PE_SolverCollisionPair_s
{
    /// @brief Pointer to the associated collision pair.
    PE_CollisionPair *m_collisionPair;

    /// @brief Number of times the solver has treated the collision pair during the current fixed update.
    int m_nbIterations;
} PE_SolverCollisionPair;

/// @brief Contructs the slover data attached to a collision pair.
/// @param collisionPair the collision pair associated with this data.
/// @param allocator the world allocator.
/// @return A pointer to the solver data initialized.
PE_SolverCollisionPair *PE_SolverCollisionPair_new(PE_CollisionPair *collisionPair, PE_Allocator *allocator);

/// @brief Destructs the solver data associated with a collision pair.
/// @param solverCP the solver data to destruct.
/// @param allocator the world allocator.
void PE_SolverCollisionPair_free(PE_SolverCollisionPair *solverCP, PE_Allocator *allocator);

/// @brief Resets the solver data associated with a collision pair.
/// This function is called once by fixed update of the physics engine.
/// @param solverCP the solver data to reset.
void PE_SolverCollisionPair_reset(PE_SolverCollisionPair *solverCP);

///////////////////////////////////////////////////////////////////////////////////////////////////
//  PE_Solver

/// @brief The solver is used to solved the collisions computed by the collision detector.
typedef struct PE_Solver_s
{
    /// @protected
    /// @brief Pointer to the world allocator.
    PE_Allocator *m_allocator;

    /// @brief Pour la liste des PE_Body
    PE_World *m_world;

    /// @brief Pour la liste de collisions;
    PE_CollisionDetector *m_collisionDetector;

    /// @protected
    /// @brief Dynamic stack of bodies used to build an island.
    PE_Vector *m_bodyStack;

    /// @protected
    /// @brief Pointer to the island used to slove the collisions.
    /// The island is allocated only once during the solver allocation
    /// and cleared once the first collision is sloved.
    PE_Island *m_island;
} PE_Solver;

/// @brief Contructs the slover of the physics engine.
/// @param world the world of the physics engine.
/// @return A pointer to the new solver.
PE_Solver *PE_Solver_new(PE_World *world);

/// @brief Destructs the solver.
/// @param solver the solver to destruct.
void PE_Solver_free(PE_Solver *solver);

/// @brief Solves the collisions computed by the collision detector.
/// @param solver the solver of the physics engine.
void PE_Solver_solve(PE_Solver *solver);

/// @brief Resets the solver data associated with each body and each collision pair in the world.
/// @param solver the solver of the physics engine.
void PE_Solver_reset(PE_Solver *solver);

/// @brief Removes the island flag of each body and each collision pair in the world.
/// @param solver the solver of the physics engine.
void PE_Solver_removeIslandFlags(PE_Solver *solver);

// ................................................................................................
// Helper methods

/// @brief Helper method used to solve one collision pair.
/// @param solver the solver of the physics engine.
/// @param collisionPair the collision pair to solve.
/// @return EXIT_SUCCESS or EXIT_FAILURE.
int PE_Solver_solveCollisionPair(PE_Solver *solver, PE_CollisionPair *collisionPair);

/// @brief Helper method used by the solver to move a body in order to resolve a collision.
/// @param solver the solver of the physics engine.
/// @param body the body to move.
/// @param displacement the displacement vector to add to the body's position.
void PE_Solver_moveBody(PE_Body *body, PE_Vec2 *displacement);

#endif