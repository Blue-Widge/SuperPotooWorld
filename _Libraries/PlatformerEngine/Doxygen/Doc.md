# Librairie Platformer Engine (PE)

La librairie *Platformer Engine* est un moteur physique léger pour les jeux de plates-formes en 2D.
Si vous souhaitez accéder directement aux fonctionnalités de la librairie par thèmes, vous pouvez consulter les pages suivantes :

- [API de PE_World](@ref md_DocWorld)
- [API de PE_Body](@ref md_DocBody)
- [API de PE_Collider](@ref md_DocCollider)
- [API de PE_Collidsion et PE_Trigger](@ref md_DocCollision)

## Création du monde

La structure PE_World_s (accessible via typedef ::PE_World) constitue le point central de la librairie.
Le monde s'occupe de gestion de la mémoire, gère tous les objets physique et effectue la simulation physique.

Pour créer le monde physique, il suffit de définir le vecteur de gravité global ainsi que le pas de temps fixe utilisé pour la simulation.

    PE_World *world = NULL;
    PE_Vec2 gravity;
    float timeStep = 1.f / 50.f;
    
    PE_Vec2_set(&gravity, 0.f, -40.f);
    world = PE_World_new(gravity, timeStep);

Une fois la simulation terminée, on peut supprimer le monde avec la fonction PE_World_free(). Il n'est pas nécessaire de supprimer les différents objets présents dans le monde, cette fonction libère toute la mémoire allouée pendant la simulation.

Pour mettre à jour le monde et faire avancer la simulation du pas de temps spécifié lors de sa création, on utilise la fonction PE_World_fixedUpdate().

## Création d'un corps

La structure PE_Body_s (typedef ::PE_Body) représente un corps indéformable dans la simulation.
La création d'un corps s'effectue avec la structure PE_BodyDef_s (typedef ::PE_BodyDef) qui regroupe toutes les informations essentielle du corps.
On utilise généralement la fonction PE_BodyDef_setDefault() pour initialiser la définition avec ses valeurs par défaut, puis on modifie les membres que l'on souhaite.

    PE_BodyDef bodyDef;
    PE_BodyDef_setDefault(&bodyDef);
    bodyDef.type = PE_STATIC_BODY;

On peut ensuite créer le corps avec la fonction PE_World_createBody().

    PE_Body *body = PE_World_createBody(world, &bodyDef);

Si l'on souhaite supprimer un corps pendant la simulation, il suffit d'appeler la fonction PE_World_removeBody().
Attention cependant, les fonctions de création et de suppression d'un corps sont verrouillées pendant la mise à jour du monde.
Elles ne peuvent donc pas être utilisées au sien d'une fonction de rappel (callback) qui s'exécute lors d'une collision entre deux objets.

## Création d'un collider

La structure PE_Collider_s (typedef ::PE_Collider) représente une forme attachée à un corps qui peut détecter ou entrer en collision avec d'autres formes.
Un corps à besoin d'au moins un collider pour interagir avec d'autres corps.

La création d'un collider s'effectue avec la structure PE_ColliderDef_s (typedef ::PE_ColliderDef) qui regroupe toutes les informations essentielle du collider.
On utilise généralement la fonction PE_ColliderDef_setDefault() pour initialiser la définition avec ses valeurs par défaut, puis on modifie les membres que l'on souhaite.

    PE_ColliderDef colliderDef;
    PE_ColliderDef_setDefault(&colliderDef);
    PE_Shape_setAsBox(&colliderDef.shape, 0.f, 0.f, 1.f, 1.f);
    collider = PE_Body_createCollider(body, &colliderDef);

Si l'on souhaite supprimer un collider pendant la simulation, il suffit d'appeler la fonction PE_Body_removeCollider().
Comme pour les corps, la création et la suppressions de colliders sont verrouillées pendant la mise à jour du monde.

## Gestion des collisions

Quand le moteur physique détecte une collision entre deux colliders, il en informe l'utilisateur en appelant une fonction de rapel (callback).

- Dans le cas d'une collision, c'est à l'utilisateur de définir la réponse (modification de la vitesse) de chacun des corps en utilisant la fonction PE_Body_setCollisionResponse().
  Le solver du moteur physique empêchera autant que possible les colliders de se superposer.
- Dans le cas d'une détection (trigger), il n'y a aucune modification de la physique des corps et le solver n'entre pas en jeu.
