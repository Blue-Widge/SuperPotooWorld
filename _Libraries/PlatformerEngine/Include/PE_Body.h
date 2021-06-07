/// @file PE_Body.h
/// @defgroup PE_Body Fonctions relatives à un corps

#ifndef _PE_BODY_H_
#define _PE_BODY_H_

#include "PE_Settings.h"
#include "PE_Collider.h"
#include "PE_Math.h"
#include "PE_List.h"
#include "PE_Collision.h"
#include "PE_Sleep.h"

typedef struct PE_World_s PE_World;
typedef struct PE_SolverBody_s PE_SolverBody;

/// @ingroup PE_Body
/// @brief Enumération donnant les types possibles pour un corps.
typedef enum PE_BodyType_e
{
    /// @brief Un corps statique est conçu pour ne jamais se déplacer sous simulation,
    /// même en cas de collision. Un corps statique ne peut entrer en collision qu'avec un
    /// corps dynamique.
    PE_STATIC_BODY = 0,

    /// @brief Un corps cinématique est conçu pour se déplacer sous simulation,
    /// mais seulement sous le contrôle explicite de l'utilisateur.
    /// Il n'est pas soumis à la gravité et ne peut entrer en collision qu'avec un
    /// corps dynamique.
    PE_KINEMATIC_BODY,

    /// @brief Un corps dynamique est conçu pour se déplacer sous simulation.
    /// Il est soumis à la gravité et peut potentiellement enter en collision avec n'importe
    /// quel autre corps.
    PE_DYNAMIC_BODY
} PE_BodyType;

/// @ingroup PE_Body
/// @brief Structure regroupant les informations nécessaires à la création d'un corps.
/// Une même définition peut servir à créer plusieurs corps.
typedef struct PE_BodyDef_s
{
    /// @brief Le type du corps.
    PE_BodyType type;

    /// @brief La position du corps exprimée dans le référentiel monde.
    PE_Vec2 position;

    /// @brief La vitesse linéaire de l'origine du corps exprimée dans le référentiel monde.
    PE_Vec2 velocity;

    /// @brief L'amortissement linéaire du corps sur l'axe des abscisses.
    /// Ce paramètre permet de réduire la vitesse linéaire horizontale du corps.
    float xDamping;

    /// @brief L'amortissement linéaire du corps sur l'axe des ordonnées.
    /// Ce paramètre permet de réduire la vitesse linéaire verticale du corps.
    float yDamping;

    /// @brief L'échelle de gravité du corps.
    float gravityScale;

    /// @brief Pointeur vers les données de l'utilisateur.
    /// La librairie ne le modifie pas. Il est défini à NULL par défaut.
    void *userData;
} PE_BodyDef;

/// @ingroup PE_Body
/// @brief Initialise les valeurs par défaut de la définition de corps.
/// @param[in] def la définition à initialiser.
extern PE_API void PE_BodyDef_setDefault(PE_BodyDef *def);

typedef enum PE_BodyFlags_e
{
    PE_BODY_AWAKE     = 1 << 0,
    PE_BODY_SIMULATED = 1 << 1,
    PE_BODY_IN_ISLAND = 1 << 2,
} PE_BodyFlags;

// Différence entre SIMULATED et AWAKE
// not SIMULATED : aucune participation au moteur physique (les colliders ne sont pas dans la broadphase)
// not AWAKE : Pas de mise à jour de la position (plus d'intégration ni de modification par le solver)
//             Les collisions sont possibles.

/// @ingroup PE_Body
/// @brief Structure représentant un corps indéformable dans le moteur physique.
typedef struct PE_Body_s
{
    /// @protected
    /// @brief Noeud contenant un pointeur vers ce corps dans la listes des corps du monde parent.
    /// Ce membre appartient au monde parent.
    PE_Node m_worldNode;

    /// @protected
    /// @brief Type du corps.
    PE_BodyType m_type;

    /// @protected
    /// @brief Flags du corps.
    int m_flags;

    /// @protected
    /// @brief Position du corps exprimée dans le référentiel monde.
    PE_Vec2 m_position;

    /// @protected
    /// @brief Vitesse linéaire de l'origine du corps exprimée dans le référentiel monde.
    PE_Vec2 m_velocity;

    /// @protected
    /// @brief Somme des forces appliquées au corps, exprimée dans le référentiel monde.
    PE_Vec2 m_forces;

    /// @protected
    /// @brief Amortissement linéaire du corps sur l'axe des abscisses.
    /// Ce paramètre permet de réduire la vitesse linéaire horizontale du corps.
    float m_xDamping;

    /// @protected
    /// @brief Amortissement linéaire du corps sur l'axe des ordonnées.
    /// Ce paramètre permet de réduire la vitesse linéaire verticale du corps.
    float m_yDamping;

    /// @protected
    /// @brief Echelle de gravité du corps.
    float m_gravityScale;

    /// @protected
    /// @brief Pointeur vers le monde parent de ce corps.
    PE_World *m_world;

    /// @protected
    /// @brief Liste des colliders attachés à ce corps.
    PE_List m_colliderList;

    /// @protected
    /// @brief Décrit le mouvement du corps pendant un pas d'intégration.
    PE_Sweep m_sweep;

    /// @protected
    /// @brief Etat d'endormissment du corps.
    PE_Sleep *m_sleep;

    /// @protected
    /// @brief Pointeur vers les données du solver associées à ce corps.
    PE_SolverBody *m_solverBody;

    /// @protected
    /// @brief Pointeur vers les données de l'utilisateur.
    /// La librairie ne le modifie pas. Il est défini à NULL par défaut.
    void *m_userData;
} PE_Body;

/// @brief Crée un nouveau corps.
/// Cette fonction est utilisée par PE_World_createBody() pour créer un corps.
/// Elle ne doit pas être utilisée directement.
/// @param[in] def la définition du corps.
/// @param[in] world le monde parent.
/// @return Un pointeur vers le corps créé ou NULL en cas d'erreur.
PE_Body *PE_Body_new(PE_BodyDef *def, PE_World *world);

/// @brief Détruit un corps.
/// Cette fonction est utilisée par PE_World_removeBody() pour détruire un corps.
/// Elle ne doit pas être utilisée directement.
/// @param[in,out] body le corps à détruire.
void PE_Body_free(PE_Body *body);

/// @ingroup PE_Body
/// @brief Crée un nouveau collider et l'attache à un corps.
/// La forme du collider est décrite dans le référentiel local au corps.
/// @param[in,out] body le corps auquel attacher le collider.
/// @param[in] def la définition du collider.
/// @return Un pointeur vers le collider créé ou NULL en cas d'erreur.
extern PE_API PE_Collider *PE_Body_createCollider(PE_Body *body, PE_ColliderDef *def);

/// @ingroup PE_Body
/// @brief Supprime un collider attaché à un corps.
/// Le pointeur world ne doit pas être déréférencé après cet appel.
/// Il est conseillé de l'affecter ensuite à NULL.
/// @param[in,out] body le corps.
/// @param[in,out] collider le collider à supprimer du corps.
extern PE_API void PE_Body_removeCollider(PE_Body *body, PE_Collider *collider);

/// @ingroup PE_Body
/// @brief Renvoie le nombre de colliders attachés à un corps.
/// @param[in] body le corps.
/// @return Le nombre de colliders du corps.
PE_API PE_INLINE int PE_Body_getNbColliders(PE_Body *body)
{
    return PE_List_getSize(&body->m_colliderList);
}

/// @brief Integrate the body's position using its velocity.
/// @param[in,out] body this.
/// @param[in] timeStep the step. 
int PE_Body_integrate(PE_Body *body, float timeStep);

int PE_Body_synchronize(PE_Body *body);

PE_INLINE void PE_Body_addFlags(PE_Body *body, int flags)
{
    body->m_flags |= flags;
}

PE_INLINE void PE_Body_removeFlags(PE_Body *body, int flags)
{
    body->m_flags &= ~flags;
}

PE_INLINE Bool PE_Body_hasOneFlag(PE_Body *body, int flags)
{
    return (body->m_flags & flags);
}

PE_INLINE Bool PE_Body_hasAllFlags(PE_Body *body, int flags)
{
    return ((body->m_flags & flags) == flags);
}

PE_API PE_INLINE Bool PE_Body_isAwake(PE_Body *body)
{
    return PE_Body_hasOneFlag(body, PE_BODY_AWAKE);
}

PE_API PE_INLINE Bool PE_Body_isSleeping(PE_Body *body)
{
    return !PE_Body_isAwake(body);
}

/// @ingroup PE_Body
/// @brief Réveille un corps.
/// @param[in,out] body le corps à réveiller.
PE_API PE_INLINE void PE_Body_wakeUp(PE_Body *body)
{
    PE_Body_addFlags(body, PE_BODY_AWAKE);
    PE_Sleep_wakeUp(body->m_sleep);
}

/// @ingroup PE_Body
/// @brief Endort un corps.
/// @param[in,out] body le corps à endormir.
PE_API PE_INLINE void PE_Body_sleep(PE_Body *body)
{
    PE_Body_removeFlags(body, PE_BODY_AWAKE);

    body->m_sweep.position0 = body->m_position;
    body->m_sweep.position1 = body->m_position;

    PE_Body_synchronize(body);
}

//PE_API PE_INLINE void PE_Body_setType(PE_Body *body, PE_BodyType type)

/// @ingroup PE_Body
/// @brief Renvoie le type d'un corps.
/// @param[in] body le corps.
/// @return Le type du corps.
PE_API PE_INLINE int PE_Body_getType(PE_Body *body)
{
    return body->m_type;
}

/// @ingroup PE_Body
/// @brief Définit la position de l'origine du corps (dans le référentiel monde).
/// Les collisions seront mises à jour lors du prochain appel à PE_World_fixedUpdate().
/// Cette fonction est verrouillée dans les fonctions de rappels (callback).
/// @param[in,out] body le corps.
/// @param[in] position la nouvelle position de l'origine du corps.
extern PE_API int PE_Body_setPosition(PE_Body *body, PE_Vec2 *position);

/// @ingroup PE_Body
/// @brief Renvoie la position de l'origine du corps (dans le référentiel monde).
/// Cette position correspond à la position du corps lors du dernier appel à PE_World_fixedUpdate().
/// @param[in] body le corps.
/// @param[out] position pointeur vers le vecteur dans lequel copier la position du corps.
PE_API PE_INLINE void PE_Body_getPosition(PE_Body *body, PE_Vec2 *position)
{
    PE_Memcpy(position, sizeof(PE_Vec2), &body->m_position, sizeof(PE_Vec2));
}

/// @ingroup PE_Body
/// @brief Renvoie la position interpolée de l'origine du corps (dans le référentiel monde).
/// Cette position correspond l'interpolation linéaire entre la position du corps lors
/// du dernier appel à PE_World_fixedUpdate() et celle du prochain appel.
/// @param[in] body le corps.
/// @param[in] alpha ratio entre 0.f et 1.f servant à l'interpolation.
/// @param[out] position pointeur vers le vecteur dans lequel copier la position du corps.
PE_API PE_INLINE void PE_Body_getInterpolation(PE_Body *body, float alpha, PE_Vec2 *position)
{
    PE_Sweep_getTransform(&body->m_sweep, alpha, position);
}

/// @ingroup PE_Body
/// @brief Définit la vitesse de l'origine du corps (dans le référentiel monde).
/// Les collisions seront mises à jour lors du prochain appel à PE_World_fixedUpdate().
/// Cette fonction est verrouillée dans les fonctions de rappel (callback).
/// @param[in,out] body le corps.
/// @param[in] velocity la nouvelle vitesse de l'origine du corps.
/// @sa PE_Body_setVelocity()
extern PE_API int PE_Body_setVelocity(PE_Body *body, PE_Vec2 *velocity);

/// @ingroup PE_Body
/// @brief Renvoie la vitesse de l'origine du corps (dans le référentiel monde).
/// Cette vitesse correspond à la vitesse du corps lors du dernier appel à PE_World_fixedUpdate().
/// @param[in] body le corps.
/// @param[out] velocity pointeur vers le vecteur dans lequel copier la vitesse du corps.
PE_API PE_INLINE void PE_Body_getVelocity(PE_Body *body, PE_Vec2 *velocity)
{
    PE_Memcpy(velocity, sizeof(PE_Vec2), &body->m_velocity, sizeof(PE_Vec2));
}

/// @ingroup PE_Body
/// @brief Définit la nouvelle vitesse de l'origine du corps (dans le référentiel monde)
/// en réponse à une collision.
/// Contrairement à PE_Body_setVelocity(), cette fonction n'est pas verrouillée dans les fonctions de rappel.
/// Elle ne réveille pas un corps endormi.
/// @param[in,out] body le corps.
/// @param[in] velocity la nouvelle vitesse de l'origine du corps.
/// @sa PE_Body_setVelocity()
PE_API PE_INLINE void PE_Body_setCollisionResponse(PE_Body *body, PE_Vec2 *velocity)
{
    PE_Memcpy(&body->m_velocity, sizeof(PE_Vec2), velocity, sizeof(PE_Vec2));
}

/// @ingroup PE_Body
/// @brief Applique une force à un corps.
/// Comme tous les corps ont une masse unitaire, la somme des forces appliquées à un corps
/// correspond à son accélération.
/// @param[in,out] body le corps.
/// @param[in] force la force à appliquer au corps.
PE_API PE_INLINE void PE_Body_applyForce(PE_Body *body, PE_Vec2 *force)
{
    if (body->m_type != PE_DYNAMIC_BODY)
        return;

    PE_Body_wakeUp(body);
    PE_Vec2_addTo(&body->m_forces, force);
}

/// @ingroup PE_Body
/// @brief Applique une impulsion à un corps.
/// L'impulsion s'ajoute directement à la vitesse du corps.
/// @param[in,out] body le corps.
/// @param[in] impulse l'impulsion à appliquer au corps.
PE_API PE_INLINE void PE_Body_applyImpulse(PE_Body *body, PE_Vec2 *impulse)
{
    if (body->m_type != PE_DYNAMIC_BODY)
        return;

    PE_Body_wakeUp(body);
    PE_Vec2_addTo(&body->m_velocity, impulse);
}

/// @ingroup PE_Body
/// @brief Annule les forces appliquées à un corps.
/// @param[in,out] body le corps.
PE_API PE_INLINE void PE_Body_clearForces(PE_Body *body)
{
    PE_Vec2_set(&body->m_forces, 0.0f, 0.0f);
}

/// @ingroup PE_Body
/// @brief Définit l'amortissement linéaire d'un corps sur l'axe des abscisses.
/// Ce paramètre permet de réduire la vitesse linéaire horizontale du corps.
/// @param[in,out] body le corps.
/// @param[in] xDamping l'amortissement linéaire sur l'axe des abscisses.
PE_API PE_INLINE void PE_Body_setXDamping(PE_Body *body, float xDamping)
{
    body->m_xDamping = xDamping;
}

/// @ingroup PE_Body
/// @brief Définit l'amortissement linéaire d'un corps sur l'axe des ordonnées.
/// Ce paramètre permet de réduire la vitesse linéaire verticale du corps.
/// @param[in,out] body le corps.
/// @param[in] yDamping l'amortissement linéaire sur l'axe des ordonnées.
PE_API PE_INLINE void PE_Body_setYDamping(PE_Body *body, float yDamping)
{
    body->m_yDamping = yDamping;
}

/// @ingroup PE_Body
/// @brief Renvoie l'amortissement linéaire d'un corps sur l'axe des abscisses.
/// @param[in] body le corps.
/// @return L'amortissement linéaire du corps sur l'axe des abscisses.
PE_API PE_INLINE float PE_Body_getXDamping(PE_Body *body)
{
    return body->m_xDamping;
}

/// @ingroup PE_Body
/// @brief Renvoie l'amortissement linéaire d'un corps sur l'axe des ordonnées.
/// @param[in] body le corps.
/// @return L'amortissement linéaire du corps sur l'axe des ordonnées.
PE_API PE_INLINE float PE_Body_getYDamping(PE_Body *body)
{
    return body->m_yDamping;
}

/// @ingroup PE_Body
/// @brief Définit l'échelle de gravité d'un corps.
/// La force de gravité appliquée à un corps est égale au vecteur gravité du monde
/// multiplié par l'échelle de gravité du corps.
/// @param[in] body le corps.
/// @param[in] gravityScale l'échelle de gravité du corps. 
PE_API PE_INLINE void PE_Body_setGravityScale(PE_Body *body, float gravityScale)
{
    body->m_gravityScale = gravityScale;
}

/// @ingroup PE_Body
/// @brief Renvoie l'échelle de gravité d'un corps.
/// @param[in] body le corps.
/// @return L'échelle de gravité du corps.
PE_API PE_INLINE float PE_Body_getGravityScale(PE_Body *body)
{
    return body->m_gravityScale;
}

PE_INLINE PE_SolverBody *PE_Body_getSolverData(PE_Body *body)
{
    return body->m_solverBody;
}

/// @ingroup PE_Body
/// @brief Définit les données de l'utilisateur d'un corps.
/// Ce pointeur sert à stocker les données spécifiques de votre application en lien avec un corps.
/// La librairie ne modifie pas ces données.
/// @param[in] body le corps.
/// @param[in] data les données de l'utilisateur.
PE_API PE_INLINE void PE_Body_setUserData(PE_Body *body, void *data)
{
    body->m_userData = data;
}

/// @ingroup PE_Body
/// @brief Renvoie les données de l'utilisateur affectées à un corps.
/// @param[in] body le corps.
/// @return Les données de l'utilisateur affectées au corps.
PE_API PE_INLINE void *PE_Body_getUserData(PE_Body *body)
{
    return body->m_userData;
}

/// @ingroup PE_Body
/// @brief Renvoie le monde parent d'un corps.
/// @param[in] body le corps.
/// @return Le monde parent du corps.
PE_API PE_INLINE PE_World *PE_Body_getWorld(PE_Body *body)
{
    return body->m_world;
}

PE_INLINE Bool PE_Body_shouldCollide(PE_Body *bodyA, PE_Body *bodyB)
{
    return (PE_Body_isAwake(bodyA) || PE_Body_isAwake(bodyB))
        && (bodyA->m_type == PE_DYNAMIC_BODY || bodyB->m_type == PE_DYNAMIC_BODY);
}

PE_INLINE PE_Sweep *PE_Body_getSweep(PE_Body *body)
{
    return &body->m_sweep;
}

PE_INLINE void PE_Body_moveNextPosition(PE_Body *body, PE_Vec2 *displacement)
{
    body->m_sweep.position1.x += displacement->x;
    body->m_sweep.position1.y += displacement->y;

    PE_Body_synchronize(body);
}

void PE_Body_finalizeStep(PE_Body *body, float timeStep);

////////////////////////////////////////////////////////////////////////////////////////////////////
// Iterator

typedef PE_Iterator PE_ColliderIterator;

/// @ingroup PE_Body
/// @brief Renvoie un itérateur sur la collection des colliders attaché à un corps dans le monde.
/// @param[in] body le corps.
/// @param[out] it pointeur vers l'itérateur à initialiser.
/// 
/// @sa PE_BodyIterator_moveNext(), PE_BodyIterator_current()
PE_API PE_INLINE void PE_Body_getColliderIterator(PE_Body *body, PE_ColliderIterator *it)
{
    PE_List_getIterator(&body->m_colliderList, it);
}

/// @ingroup PE_Body
/// @brief Avance l'itérateur pour qu'il pointe sur le prochain élément de la collection.
/// @param[in,out] it l'itérateur.
/// @return FALSE si l'itérateur pointe sur le dernier élément de la collection, TRUE sinon.
PE_API PE_INLINE Bool PE_ColliderIterator_moveNext(PE_ColliderIterator *it)
{
    return PE_Iterator_moveNext(it);
}

/// @ingroup PE_Body
/// @brief Renvoie un pointeur vers l'élément de la collection à la position courante de l'itérateur.
/// @param[in] it l'itérateur.
/// @return L'élément de la collection à la position courante de l'itérateur.
PE_API PE_INLINE PE_Collider *PE_ColliderIterator_current(PE_ColliderIterator *it)
{
    return (PE_Collider *)PE_Iterator_current(it);
}

#endif
