
/// @file PE_Collider.h
/// @defgroup PE_Collider Fonctions relatives à un Collider

#ifndef _PE_COLLIDER_H_
#define _PE_COLLIDER_H_

#include "PE_Settings.h"
#include "PE_Shape.h"
#include "PE_Math.h"
#include "PE_List.h"
#include "PE_Allocator.h"

typedef struct PE_Body_s PE_Body;
typedef struct PE_BPID_s PE_BPID;
typedef struct PE_BroadPhase_s PE_BroadPhase;
typedef struct PE_Collision_s PE_Collision;
typedef struct PE_Trigger_s PE_Trigger;
typedef struct PE_CollisionDetector_s PE_CollisionDetector;
typedef struct PE_World_s PE_World;

typedef void PE_CollisionCallback(PE_Collision *);
typedef void PE_TriggerCallback(PE_Trigger *);

/// @ingroup PE_Collider
/// @brief Structure permettant de filtrer les collisions.
typedef struct PE_Filter_s
{
    /// @brief Les bits représentant les catégories d'un collider.
    /// Normalement, on ne définit qu'un seul bit à un.
    int categoryBits;

    /// @brief Les bits servant de masque pour les collisions.
    /// Ce membre définit les catégories avec lesquelles le collider peut entrer en collision.
    int maskBits;
} PE_Filter;

PE_INLINE Bool PE_Filter_shouldCollide(PE_Filter *filterA, PE_Filter *filterB)
{
    return
        (filterA->categoryBits & filterB->maskBits) &&
        (filterB->categoryBits & filterA->maskBits);
}

/// @ingroup PE_Collider
/// @brief Structure regroupant les informations nécessaires à la création d'un collider.
/// Une même définition peut servir à créer plusieurs colliders.
typedef struct PE_ColliderDef_s
{
    /// @brief Forme du collider.
    PE_Shape shape;

    /// @brief Indique si le collider est un déclencheur.
    Bool isTrigger;
    
    /// @brief Indique si le collider a un comportement "one-way".
    /// Autrement dit, s'il autorise uniquement les collisions dans une seule direction.
    Bool isOneWay;

    /// @brief Filtre à appliquer au collider pendant la détection des collisions.
    PE_Filter filter;

    /// @brief Pointeur vers les données de l'utilisateur.
    /// La librairie ne le modifie pas. Il est défini à NULL par défaut.
    void *userData;
} PE_ColliderDef;

/// @ingroup PE_Collider
/// @brief Initialise les valeurs par défaut de la définition de colliders.
/// @param[in] def la définition à initialiser.
extern PE_API void PE_ColliderDef_setDefault(PE_ColliderDef *def);

typedef enum PE_ColliderFlag_e
{
    PE_COLLIDER_TRIGGER = 1 << 0,
    PE_COLLIDER_ACTIVE  = 1 << 1,
    PE_COLLIDER_ONE_WAY = 1 << 2,
} PE_ColliderFlag;

/// @ingroup PE_Collider
/// @brief Structure représentant un collider dans le moteur physique.
/// Un collider peut être affecté à un corps pour la détection de collisions.
/// Si le collider est un détecteur (trigger), il n'applique aucune force aux autres colliders/corps.
typedef struct PE_Collider_s
{
    /// @protected
    /// @brief Noeud contenant un pointeur vers ce collider dans la listes des collider du corps parent.
    /// Ce membre appartient au corps parent.
    PE_Node m_bodyNode;

    /// @brief Identifiant du collider dans la broad phase.
    /// Ce membre appartient à la broad phase.
    uint32 m_bpBoxID;

    /// @protected
    /// @brief Flags du collider.
    int m_flags;

    /// @protected
    /// @brief Forme du collider.
    PE_Shape m_shape;

    /// @protected
    /// @brief Boîte englobante du collider pour le balayage courant.
    /// Cette AABB est mise à jour par le corps parent et est utilisée dans la broad phase.
    PE_AABB m_aabb;

    /// @protected
    /// @brief Filtre à appliquer au collider pendant la détection des collisions.
    PE_Filter m_filter;

    /// @protected
    /// @brief Liste des collisions impliquant ce collider.
    PE_List m_collisionList;

    /// @protected
    /// @brief Liste des détections impliquant ce collider.
    PE_List m_triggerList;

    /// @protected
    /// @brief Pointeur vers le corps parent.
    PE_Body *m_body;

    /// @protected
    /// @brief Pointeur vers la fonction de rappel "onCollisionEnter" de ce collider.
    PE_CollisionCallback *m_onCollisionEnter;

    /// @protected
    /// @brief Pointeur vers la fonction de rappel "onCollisionStay" de ce collider.
    PE_CollisionCallback *m_onCollisionStay;

    /// @protected
    /// @brief Pointeur vers la fonction de rappel "onCollisionExit" de ce collider.
    PE_CollisionCallback *m_onCollisionExit;

    /// @protected
    /// @brief Pointeur vers la fonction de rappel "onTriggerEnter" de ce collider.
    PE_TriggerCallback *m_onTriggerEnter;

    /// @protected
    /// @brief Pointeur vers la fonction de rappel "onTriggerStay" de ce collider.
    PE_TriggerCallback *m_onTriggerStay;

    /// @protected
    /// @brief Pointeur vers la fonction de rappel "onTriggerExit" de ce collider.
    PE_TriggerCallback *m_onTriggerExit;

    /// @protected
    /// @brief Pointeur vers les données de l'utilisateur.
    /// La librairie ne le modifie pas. Il est défini à NULL par défaut.
    void *m_userData;
} PE_Collider;

/// @brief Crée un nouveau collider.
/// Cette fonction est utilisée par PE_Body_createCollider() pour créer un collider.
/// Elle ne doit pas être utilisée directement.
/// @param[in] def la définition du collider.
/// @param[in] allocator l'allocateur principal du monde.
/// @return Un pointeur vers le collider créé ou NULL en cas d'erreur.
PE_Collider *PE_Collider_new(PE_ColliderDef *def, PE_Allocator *allocator);

/// @brief Détruit un collider.
/// Cette fonction est utilisée par PE_Body_removeCollider() pour détruire un collider.
/// Elle ne doit pas être utilisée directement.
/// @param[in,out] collider la collider à supprimer.
/// @param[in] allocator l'allocateur principal du monde.
void PE_Collider_free(PE_Collider *collider, PE_Allocator *allocator);

//extern PE_API void PE_Collider_setFilter(PE_Collider *collider, PE_Filter *filter);

/// @ingroup PE_Collider
/// @brief Renvoie le filtre d'un collider.
/// @param[in] collider le collider.
/// @return Le filtre du collider.
extern PE_API PE_Filter PE_Collider_getFilter(PE_Collider *collider);

PE_INLINE void PE_Collider_addFlags(PE_Collider *collider, int flags)
{
    collider->m_flags |= flags;
}

PE_INLINE void PE_Collider_removeFlags(PE_Collider *collider, int flags)
{
    collider->m_flags &= ~flags;
}

PE_INLINE Bool PE_Collider_hasOneFlag(PE_Collider *collider, int flags)
{
    return (collider->m_flags & flags);
}

PE_INLINE Bool PE_Collider_hasAllFlags(PE_Collider *collider, int flags)
{
    return ((collider->m_flags & flags) == flags);
}

/// @ingroup PE_Collider
/// @brief Définit si un collider est un détecteur.
/// @param[in,out] collider le collider.
/// @param trigger peut valoir TRUE ou FALSE.
PE_API PE_INLINE void PE_Collider_setTrigger(PE_Collider *collider, Bool trigger)
{
    if (trigger)
        PE_Collider_addFlags(collider, PE_COLLIDER_TRIGGER);
    else
        PE_Collider_removeFlags(collider, PE_COLLIDER_TRIGGER);
}

/// @ingroup PE_Collider
/// @brief Indique si un collider est un détecteur.
/// @param[in] collider le collider.
/// @return TRUE si le collider est un détecteur, FALSE sinon.
PE_API PE_INLINE Bool PE_Collider_isTrigger(PE_Collider *collider)
{
    return PE_Collider_hasOneFlag(collider, PE_COLLIDER_TRIGGER);
}

/// @ingroup PE_Collider
/// @brief Renvoie le corps parent d'un collider.
/// @param[in] collider le collider.
/// @return Un pointeur vers le corps parent du collider.
extern PE_API PE_Body *PE_Collider_getBody(PE_Collider *collider);

/// @ingroup PE_Collider
/// @brief Renvoie la forme d'un collider.
/// @param[in] collider le collider.
/// @return La forme du collider.
PE_API PE_INLINE PE_Shape *PE_Collider_getShape(PE_Collider *collider)
{
    return &collider->m_shape;
}

/// @ingroup PE_Collider
/// @brief Renvoie la boîte englobante d'un collider.
/// @param[in] collider le collider.
/// @return Un pointeur vers la boîte englobante du collider.
extern PE_API PE_AABB *PE_Collider_getAABB(PE_Collider *collider);


/// @ingroup PE_Collider
/// @brief Définit les données de l'utilisateur d'un collider.
/// Ce pointeur sert à stocker les données spécifiques de votre application en lien avec un collider.
/// La librairie ne modifie pas ces données.
/// @param[in] collider le collider.
/// @param[in] data les données de l'utilisateur.
extern PE_API void PE_Collider_setUserData(PE_Collider *collider, void *data);

/// @ingroup PE_Collider
/// @brief Renvoie les données de l'utilisateur affectées à un collider.
/// @param[in] collider le collider.
/// @return Les données de l'utilisateur affectées au collider.
extern PE_API void *PE_Collider_getUserData(PE_Collider *collider);

/// @ingroup PE_Collider
/// @brief Définit la fonction de rappel (callback) appelée à chaque fois qu'un collider
//  entre en collision avec un autre collider.
/// Les références vers les objets donnés dans un callback ne doivent pas être sauvegardées,
/// elle sont invalidées après la fin de la fonction.
/// @param[in,out] collider le collider auquel affecter la fonction de rappel.
/// @param[in] onCollisionEnter la fonction de rappel.
PE_API PE_INLINE void PE_Collider_setOnCollisionEnter(
    PE_Collider *collider,
    PE_CollisionCallback *onCollisionEnter)
{
    collider->m_onCollisionEnter = onCollisionEnter;
}

/// @ingroup PE_Collider
/// @brief Définit la fonction de rappel (callback) appelée à chaque fois qu'un collider
/// touche un autre collider.
/// Les références vers les objets donnés dans un callback ne doivent pas être sauvegardées,
/// elle sont invalidées après la fin de la fonction.
/// @param[in,out] collider le collider auquel affecter la fonction de rappel.
/// @param[in] onCollisionStay la fonction de rappel.
PE_API PE_INLINE void PE_Collider_setOnCollisionStay(
    PE_Collider *collider,
    PE_CollisionCallback *onCollisionStay)
{
    collider->m_onCollisionStay = onCollisionStay;
}

/// @ingroup PE_Collider
/// @brief Définit la fonction de rappel (callback) appelée à chaque fois qu'un collider
/// cesse d'être en collision avec un autre collider.
/// Les références vers les objets donnés dans un callback ne doivent pas être sauvegardées,
/// elle sont invalidées après la fin de la fonction.
/// @param[in,out] collider le collider auquel affecter la fonction de rappel.
/// @param[in] onCollisionExit la fonction de rappel.
PE_API PE_INLINE void PE_Collider_setOnCollisionExit(
    PE_Collider *collider,
    PE_CollisionCallback *onCollisionExit)
{
    collider->m_onCollisionExit = onCollisionExit;
}

/// @ingroup PE_Collider
/// @brief Définit la fonction de rappel (callback) appelée à chaque fois qu'un collider
//  commence à détecter ou à être détecté par un autre collider.
/// Les références vers les objets donnés dans un callback ne doivent pas être sauvegardées,
/// elle sont invalidées après la fin de la fonction.
/// @param[in,out] collider le collider auquel affecter la fonction de rappel.
/// @param[in] onTriggerEnter la fonction de rappel.
PE_API PE_INLINE void PE_Collider_setOnTriggerEnter(
    PE_Collider *collider,
    PE_TriggerCallback *onTriggerEnter)
{
    collider->m_onTriggerEnter = onTriggerEnter;
}

/// @ingroup PE_Collider
/// @brief Définit la fonction de rappel (callback) appelée à chaque fois qu'un collider
//  commence détecte ou est détecté par un autre collider.
/// Les références vers les objets donnés dans un callback ne doivent pas être sauvegardées,
/// elle sont invalidées après la fin de la fonction.
/// @param[in,out] collider le collider auquel affecter la fonction de rappel.
/// @param[in] onTriggerStay la fonction de rappel.
PE_API PE_INLINE void PE_Collider_setOnTriggerStay(
    PE_Collider *collider,
    PE_TriggerCallback *onTriggerStay)
{
    collider->m_onTriggerStay = onTriggerStay;
}

/// @ingroup PE_Collider
/// @brief Définit la fonction de rappel (callback) appelée à chaque fois qu'un collider
//  cesse de détecter ou d'être détecté par un autre collider.
/// Les références vers les objets donnés dans un callback ne doivent pas être sauvegardées,
/// elle sont invalidées après la fin de la fonction.
/// @param[in,out] collider le collider auquel affecter la fonction de rappel.
/// @param[in] onTriggerExit la fonction de rappel.
PE_API PE_INLINE void PE_Collider_setOnTriggerExit(
    PE_Collider *collider,
    PE_TriggerCallback *onTriggerExit)
{
    collider->m_onTriggerExit = onTriggerExit;
}

/// @ingroup PE_Collider
/// @brief Renvoie la fonction de rappel "onCollisionEnter" affectée à un collider.
/// @param[in] collider le collider.
/// @return La fonction de rappel "onCollisionEnter" du collider.
PE_API PE_INLINE PE_CollisionCallback *PE_Collider_getOnCollisionEnter(PE_Collider *collider)
{
    return collider->m_onCollisionEnter;
}

/// @ingroup PE_Collider
/// @brief Renvoie la fonction de rappel "onCollisionStay" affectée à un collider.
/// @param[in] collider le collider.
/// @return La fonction de rappel "onCollisionStay" du collider.
PE_API PE_INLINE PE_CollisionCallback *PE_Collider_getOnCollisionStay(PE_Collider *collider)
{
    return collider->m_onCollisionStay;
}

/// @ingroup PE_Collider
/// @brief Renvoie la fonction de rappel "onCollisionExit" affectée à un collider.
/// @param[in] collider le collider.
/// @return La fonction de rappel "onCollisionExit" du collider.
PE_API PE_INLINE PE_CollisionCallback *PE_Collider_getOnCollisionExit(PE_Collider *collider)
{
    return collider->m_onCollisionExit;
}

/// @ingroup PE_Collider
/// @brief Renvoie la fonction de rappel "onTriggerEnter" affectée à un collider.
/// @param[in] collider le collider.
/// @return La fonction de rappel "onTriggerEnter" du collider.
PE_API PE_INLINE PE_TriggerCallback *PE_Collider_getOnTriggerEnter(PE_Collider *collider)
{
    return collider->m_onTriggerEnter;
}

/// @ingroup PE_Collider
/// @brief Renvoie la fonction de rappel "onTriggerStay" affectée à un collider.
/// @param[in] collider le collider.
/// @return La fonction de rappel "onTriggerStay" du collider.
PE_API PE_INLINE PE_TriggerCallback *PE_Collider_getOnTriggerStay(PE_Collider *collider)
{
    return collider->m_onTriggerStay;
}

/// @ingroup PE_Collider
/// @brief Renvoie la fonction de rappel "onTriggerExit" affectée à un collider.
/// @param[in] collider le collider.
/// @return La fonction de rappel "onTriggerExit" du collider.
PE_API PE_INLINE PE_TriggerCallback *PE_Collider_getOnTriggerExit(PE_Collider *collider)
{
    return collider->m_onTriggerExit;
}

PE_INLINE void PE_Collider_onCollisionEnter(PE_Collider *collider, PE_Collision *collision)
{
    if (collider->m_onCollisionEnter)
        collider->m_onCollisionEnter(collision);
}

PE_INLINE void PE_Collider_onCollisionStay(PE_Collider *collider, PE_Collision *collision)
{
    if (collider->m_onCollisionStay)
        collider->m_onCollisionStay(collision);
}

PE_INLINE void PE_Collider_onCollisionExit(PE_Collider *collider, PE_Collision *collision)
{
    if (collider->m_onCollisionExit)
        collider->m_onCollisionExit(collision);
}

PE_INLINE void PE_Collider_onTriggerEnter(PE_Collider *collider, PE_Trigger *trigger)
{
    if (collider->m_onTriggerEnter)
        collider->m_onTriggerEnter(trigger);
}

PE_INLINE void PE_Collider_onTriggerStay(PE_Collider *collider, PE_Trigger *trigger)
{
    if (collider->m_onTriggerStay)
        collider->m_onTriggerStay(trigger);
}

PE_INLINE void PE_Collider_onTriggerExit(PE_Collider *collider, PE_Trigger *trigger)
{
    if (collider->m_onTriggerExit)
        collider->m_onTriggerExit(trigger);
}

int PE_Collider_synchronize(
    PE_Collider *collider, PE_CollisionDetector *collisionDetector,
    PE_Vec2 *transform0, PE_Vec2 *transform1);

PE_INLINE Bool PE_Collider_shouldCollide(PE_Collider *colliderA, PE_Collider *colliderB)
{
    return PE_Filter_shouldCollide(&colliderA->m_filter, &colliderB->m_filter)
        && (colliderA->m_body != colliderB->m_body);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Iterator

typedef PE_Iterator PE_CollisionIterator;

/// @ingroup PE_Collider
/// @brief Renvoie un itérateur sur la collection des collisions impliquant un collider.
/// @param[in] collider le collider.
/// @param[out] it pointeur vers l'itérateur à initialiser.
/// 
/// @sa PE_CollisionIterator_moveNext(), PE_CollisionIterator_current()
PE_API PE_INLINE void PE_Collider_getCollisionIterator(PE_Collider *collider, PE_CollisionIterator *it)
{
    PE_List_getIterator(&collider->m_collisionList, it);
}

/// @ingroup PE_Collider
/// @brief Avance l'itérateur pour qu'il pointe sur le prochain élément de la collection.
/// @param[in,out] it l'itérateur.
/// @return FALSE si l'itérateur pointe sur le dernier élément de la collection, TRUE sinon.
PE_API PE_INLINE Bool PE_CollisionIterator_moveNext(PE_CollisionIterator *it)
{
    return PE_Iterator_moveNext(it);
}

/// @ingroup PE_Collider
/// @brief Renvoie un pointeur vers l'élément de la collection à la position courante de l'itérateur.
/// @param[in] it l'itérateur.
/// @return L'élément de la collection à la position courante de l'itérateur.
PE_API PE_INLINE PE_Collision *PE_CollisionIterator_current(PE_CollisionIterator *it)
{
    return (PE_Collision *)PE_Iterator_current(it);
}

/// @ingroup PE_Collider
/// @brief Renvoie le nombre de collisions impliquant un collider.
/// @param[in] collider le collider.
/// @return Le nombre de collisions impliquant ce collider.
PE_API PE_INLINE int PE_Collider_getNbCollisions(PE_Collider *collider)
{
    return PE_List_getSize(&collider->m_collisionList);
}

//----------------------------


typedef PE_Iterator PE_TriggerIterator;


/// @ingroup PE_Collider
/// @brief Renvoie un itérateur sur la collection des détections (trigger)
/// impliquant un collider de type détecteur.
/// @param[in] collider le collider.
/// @param[out] it pointeur vers l'itérateur à initialiser.
/// 
/// @sa PE_TriggerIterator_moveNext(), PE_TriggerIterator_current()
PE_API PE_INLINE void PE_Collider_getTriggerIterator(PE_Collider *collider, PE_TriggerIterator *it)
{
    PE_List_getIterator(&collider->m_triggerList, it);
}

/// @ingroup PE_Collider
/// @brief Avance l'itérateur pour qu'il pointe sur le prochain élément de la collection.
/// @param[in,out] it l'itérateur.
/// @return FALSE si l'itérateur pointe sur le dernier élément de la collection, TRUE sinon.
PE_API PE_INLINE Bool PE_TriggerIterator_moveNext(PE_TriggerIterator *it)
{
    return PE_Iterator_moveNext(it);
}

/// @ingroup PE_Collider
/// @brief Renvoie un pointeur vers l'élément de la collection à la position courante de l'itérateur.
/// @param[in] it l'itérateur.
/// @return L'élément de la collection à la position courante de l'itérateur.
PE_API PE_INLINE PE_Trigger *PE_TriggerIterator_current(PE_TriggerIterator *it)
{
    return (PE_Trigger *)PE_Iterator_current(it);
}

/// @ingroup PE_Collider
/// @brief Renvoie le nombre de détections impliquant un collider de type détecteur.
/// @param[in] collider le collider.
/// @return Le nombre de détections impliquant ce collider.
PE_API PE_INLINE int PE_Collider_getNbTriggers(PE_Collider *collider)
{
    return PE_List_getSize(&collider->m_triggerList);
}
/// @}

#endif