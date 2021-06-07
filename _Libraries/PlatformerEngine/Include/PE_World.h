
/// @file PE_World.h
/// @defgroup PE_World Fonctions relatives au monde

#ifndef _PE_WORLD_H_
#define _PE_WORLD_H_

#include "PE_Lib.h"
#include "PE_Settings.h"
#include "PE_Math.h"
#include "PE_List.h"
#include "PE_CollisionDetector.h"
#include "PE_BroadPhase.h"
#include "PE_Allocator.h"
#include "PE_Profile.h"

typedef struct PE_Body_s PE_Body;
typedef struct PE_BodyDef_s PE_BodyDef;
typedef struct PE_Solver_s PE_Solver;

/// @ingroup PE_World
/// @struct PE_World_s
/// @brief Le monde gère et crée tous les corps physiques.
typedef struct PE_World_s
{
    /// @protected
    /// @brief Allocateur principal utilisé pour toutes les allocations mémoire dans le monde.
    PE_Allocator *m_allocator;

    /// @protected
    /// @brief Pas de temps utilisé pour chaque mise à jour du monde.
    /// Le pas de temps est fixe et varie typiquement entre 1.f/50.f et 1.f/60.f.
    float m_timeStep;

    /// @protected
    /// @brief Vecteur de gravité global du monde.
    /// Note that the gravity of a body can be scaled.
    PE_Vec2 m_gravity;

    /// @protected
    /// @brief Liste des corps du monde.
    PE_List m_bodyList;

    /// @protected
    /// @brief Détecteur de collisions.
    PE_CollisionDetector *m_collisionDetector;

    /// @protected
    /// @brief Solver utilisé pour résoudre toutes les collisions dans ce monde.
    PE_Solver *m_solver;

    /// @protected
    /// @brief Booléen permettant de bloquer l'accès à certaines fonctions lors de callback.
    Bool m_locked;
} PE_World;

/// @ingroup PE_World
/// @brief Crée un nouveau monde.
/// @param[in] gravity le vecteur de gravité du monde.
/// @param[in] timeStep le pas de temps utilisé pour chaque mise à jour du monde.
/// @return Le monde créé ou NULL en cas d'erreur.
extern PE_API PE_World *PE_World_new(PE_Vec2 gravity, float timeStep);

/// @ingroup PE_World
/// @brief Détruit le monde.
/// Chaque entité physique appartenant à ce monde est également détruite.
/// Le pointeur world ne doit pas être déréférencé après cet appel.
/// Il est conseillé de l'affecter ensuite à NULL.
/// @param[in,out] world le monde à détruire.
extern PE_API void PE_World_free(PE_World *world);

/// @ingroup PE_World
/// @brief Crée un corps indéformable dans le monde.
/// Cette fonction est verrouillée dans les fonctions de rappels (callback).
/// @param[in,out] world le monde.
/// @param[in] def la définition du corps.
/// @return Un pointeur vers le corps créé ou NULL en cas d'erreur.
extern PE_API PE_Body *PE_World_createBody(PE_World *world, PE_BodyDef *def);

/// @ingroup PE_World
/// @brief Détruit un corps indéformable.
/// Le pointeur world ne doit pas être déréférencé après cet appel.
/// Il est conseillé de l'affecter ensuite à NULL.
/// Cette fonction est verrouillée dans les fonctions de rappels (callback).
/// @param[in,out] world le monde.
/// @param[in,out] body le corps à détruire.
extern PE_API void PE_World_removeBody(PE_World *world, PE_Body *body);

/// @ingroup PE_World
/// @brief Renvoie le nombre de corps présents dans le monde.
/// @param[in] world le monde.
/// @return Le nombre de corps.
extern PE_API int PE_World_getNbBodies(PE_World *world);

/// @ingroup PE_World
/// @brief Définit le vecteur de gravité global appliqué à tous les corps dynamiques du monde.
/// @param[in,out] world le monde.
/// @param[in] gravity le nouveau vecteur de gravité.
extern PE_API void PE_World_setGravity(PE_World *world, PE_Vec2 *gravity);

/// @ingroup PE_World
/// @brief Renvoie le vecteur de gravité global appliqué dans le monde.
/// @param[in] world le monde.
/// @return Le vecteur de gravité global.
extern PE_API PE_Vec2 *PE_World_getGravity(PE_World *world);

/// @ingroup PE_World
/// @brief Met à jour les corps présents dans le monde d'un pas de temps.
/// Cette fonction fait met à jour les positions et effectue la détection des collisions.
/// @param[in,out] world le monde.
/// @return EXIT_SUCCESS ou EXIT_FAILURE.
extern PE_API int PE_World_fixedUpdate(PE_World *world);

//.................................................................................................
// Fonctions de rappel globales

/// @ingroup PE_World
/// @brief Définit la fonction de rappel (callback) appelée à chaque fois que deux objets entrent en collision.
/// @param[in,out] world le monde.
/// @param[in] onCollisionEnter la fonction de rappel.
PE_API PE_INLINE void PE_World_setOnCollisionEnter(PE_World *world, PE_CollisionCallback *onCollisionEnter)
{
    PE_CollisionDetector_setOnCollisionEnter(world->m_collisionDetector, onCollisionEnter);
}

/// @ingroup PE_World
/// @brief Définit la fonction de rappel (callback) appelée à chaque fois que deux objets se touchent.
/// @param[in,out] world le monde.
/// @param[in] onCollisionStay la fonction de rappel.
PE_API PE_INLINE void PE_World_setOnCollisionStay(PE_World *world, PE_CollisionCallback *onCollisionStay)
{
    PE_CollisionDetector_setOnCollisionStay(world->m_collisionDetector, onCollisionStay);
}

/// @ingroup PE_World
/// @brief Définit la fonction de rappel (callback) appelée à chaque fois que deux objets cessent d'être en collision.
/// @param[in,out] world le monde.
/// @param[in] onCollisionExit la fonction de rappel.
PE_API PE_INLINE void PE_World_setOnCollisionExit(PE_World *world, PE_CollisionCallback *onCollisionExit)
{
    PE_CollisionDetector_setOnCollisionExit(world->m_collisionDetector, onCollisionExit);
}

//.................................................................................................
// Itérateur sur les corps présents dans le monde

typedef PE_Iterator PE_BodyIterator;

/// @ingroup PE_World
/// @brief Renvoie un itérateur sur la collection des corps présents dans le monde.
/// @param[in] world le monde.
/// @param[out] it pointeur vers l'itérateur à initialiser.
/// 
/// @sa PE_BodyIterator_moveNext(), PE_BodyIterator_current()
PE_API PE_INLINE void PE_World_getBodyIterator(PE_World *world, PE_BodyIterator *it)
{
    PE_List_getIterator(&world->m_bodyList, it);
}

/// @ingroup PE_World
/// @brief Avance l'itérateur pour qu'il pointe sur le prochain élément de la collection.
/// @param[in,out] it l'itérateur.
/// @return FALSE si l'itérateur pointe sur le dernier élément de la collection, TRUE sinon.
PE_API PE_INLINE Bool PE_BodyIterator_moveNext(PE_BodyIterator *it)
{
    return PE_Iterator_moveNext(it);
}

/// @ingroup PE_World
/// @brief Renvoie un pointeur vers l'élément de la collection à la position courante de l'itérateur.
/// @param[in] it l'itérateur.
/// @return L'élément de la collection à la position courante de l'itérateur.
PE_API PE_INLINE PE_Body *PE_BodyIterator_current(PE_BodyIterator *it)
{
    return (PE_Body *)PE_Iterator_current(it);
}

// ................................................................................................
// Méthodes internes

/// @brief Renvoie l'allocateur utilisé dans ce monde.
/// @param[in] world le monde.
/// @return Un pointeur vers l'allocateur du monde.
PE_INLINE PE_Allocator *PE_World_getAllocator(PE_World *world)
{
    return world->m_allocator;
}

/// @brief Renvoie le détecteur de collisions de ce monde.
/// @param[in] world le monde.
/// @return Un pointeur vers le détecteur de collisions.
PE_INLINE PE_CollisionDetector *PE_World_getCollisionDetector(PE_World *world)
{
    return world->m_collisionDetector;
}

/// @brief Indique si le monde est bloqué.
/// Cela permet d'interdir l'utilisation de certaines fonctions de la librairie pendant un callback.
/// @param[in] world le monde.
/// @return Un booléen indiquant si le monde est bloqué.
PE_INLINE Bool PE_World_isLocked(PE_World *world)
{
    return world->m_locked;
}


#endif
