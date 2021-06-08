#include "Player.h"
#include "../Scene.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

// Méthodes du GameObject
int Player_free(GameObject *object);
int Player_onStart(GameObject *object);
int Player_onRespawn(GameObject *object);
int Player_update(GameObject *object);
int Player_fixedUpdate(GameObject *object);
int Player_render(GameObject *object);

// Fonction locales
int  Player_createAnimator(Player *player);
void Player_aboveTriggerStay(PE_Trigger *trigger);
void Player_aboveTriggerExit(PE_Trigger *trigger);
void Player_belowTriggerStay(PE_Trigger *trigger);
void Player_belowTriggerExit(PE_Trigger *trigger);
void Player_onCollisionStay(PE_Collision *collision);

void Player_aboveTriggerStay(PE_Trigger *trigger)
{
    PE_Body *thisBody = PE_Trigger_getBody(trigger);
    GameObject *thisObject = PE_Body_getUserData(thisBody);

    if (!thisObject)
    {
        printf("ERROR - Player_aboveTriggerStay()\n");
        return;
    }

    Player *player = GameObject_getPlayer(thisObject);
    if (player)
    {
        player->m_aboveDetector = TRUE;
    }
}

void Player_aboveTriggerExit(PE_Trigger *trigger)
{
    PE_Body *thisBody = PE_Trigger_getBody(trigger);
    GameObject *thisObject = PE_Body_getUserData(thisBody);
    if (!thisObject)
    {
        printf("ERROR - Player_aboveTriggerExit()\n");
        return;
    }

    Player *player = GameObject_getPlayer(thisObject);
    if (player)
    {
        player->m_aboveDetector = FALSE;
    }
}

void Player_belowTriggerStay(PE_Trigger *trigger)
{
    PE_Body *thisBody = PE_Trigger_getBody(trigger);
    GameObject *thisObject = PE_Body_getUserData(thisBody);
    if (!thisObject)
    {
        printf("ERROR - Player_belowTriggerStay()\n");
        return;
    }

    Player *player = GameObject_getPlayer(thisObject);
    if (player)
    {
        PE_Vec2 velocity = GameObject_getVelocity(thisObject);
        if (velocity.y <= 0.f)
            player->m_belowDetector = TRUE;
    }
}

void Player_belowTriggerExit(PE_Trigger *trigger)
{
    PE_Body *thisBody = PE_Trigger_getBody(trigger);
    GameObject *thisObject = PE_Body_getUserData(thisBody);
    if (!thisObject)
    {
        printf("ERROR - Player_belowTriggerExit()\n");
        return;
    }

    Player *player = GameObject_getPlayer(thisObject);
    if (player)
    {
        player->m_belowDetector = FALSE;
    }
}

void Player_onCollisionStay(PE_Collision *collision)
{
    PE_Body *thisBody = PE_Collision_getBody(collision);
    PE_Body *otherBody = PE_Collision_getOtherBody(collision);
    GameObject *thisObject = PE_Body_getUserData(thisBody);
    GameObject *otherObject = PE_Body_getUserData(otherBody);

    if (!thisObject || !otherObject)
    {
        printf("ERROR - Player_onCollisionStay()\n");
        return;
    }

    // TODO
    // Vous pouvez infliger des dommages aux ennemis dans cette fonction
    // Utilisez GameObject_getEnemy(otherObject) et testez le pointeur

    if (GameObject_getType(otherObject) == GAME_BLOCK)
    {
        int relPos = PE_Collision_getRelativePosition(collision);
        PE_Vec2 velocity;
        PE_Body_getVelocity(thisBody, &velocity);

        switch (relPos)
        {
        case PE_ABOVE:
            if (velocity.y < 0.f)
                velocity.y = 0.f;
            break;

        case PE_BELOW:
            if (velocity.y > 0.f)
                velocity.y = 0.f;
            Block_hit(GameObject_getBlock(otherObject));
            break;

        case PE_RIGHT:
        case PE_LEFT:
            velocity.x = 0.f;
            break;

        default:
            break;
        }
        PE_Body_setCollisionResponse(thisBody, &velocity);
    }
}

int Player_createAnimator(Player *player)
{
    Scene *scene = GameObject_getScene(player->m_object);
    GameTextures *textures = Scene_getTextures(scene);
    RE_Animator *animator = NULL;
    RE_TextureAnim *texAnim = NULL;
    RE_ParamAnim *parAnim = NULL;
    int exitStatus = EXIT_SUCCESS;

    animator = RE_Animator_new();
    if (!animator) goto ERROR_LABEL;

    player->m_animator = animator;

    // Définition de l'animation "Running"
    texAnim = RE_Animator_createTextureAnim(animator, textures->playerRunning, "Running");
    if (!texAnim) goto ERROR_LABEL;

    RE_TextureAnim_setCycleTime(texAnim, 0.3f);

    // Définition de l'animation "Falling"
    texAnim = RE_Animator_createTextureAnim(animator, textures->playerFalling, "Falling");
    if (!texAnim) goto ERROR_LABEL;

    RE_TextureAnim_setCycleTime(texAnim, 0.2f);

    // TODO
    // Définissez d'autres animations

    // Initialisation de l'animator
    exitStatus = RE_Animator_playTextureAnim(player->m_animator, "Running");

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Player_createAnimator()\n");
    return EXIT_FAILURE;
}

Player *Player_new(Scene *scene, PE_Vec2 *position)
{
    GameObject *object = NULL;
    Player *player = NULL;
    int exitStatus = EXIT_SUCCESS;

    object = GameObject_new(scene, GAME_PLAYER);
    if (!object) goto ERROR_LABEL;

    player = (Player *)calloc(1, sizeof(Player));
    if (!player) goto ERROR_LABEL;

    player->m_object = object;
    player->m_startPos = *position;

    // Initialisation de la classe mère
    object->m_data = (void *)player;
    object->m_layer = 3;
    object->cm_free = Player_free;
    object->cm_onStart = Player_onStart;
    object->cm_onRespawn = Player_onRespawn;
    object->cm_update = Player_update;
    object->cm_fixedUpdate = Player_fixedUpdate;
    object->cm_render = Player_render;

    // Allocations supplémentaires
    exitStatus = Player_createAnimator(player);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return player;

ERROR_LABEL:
    printf("ERROR - Player_new()\n");
    GameObject_free(object);
    return NULL;
}


int Player_free(GameObject *object)
{
    Player *player = NULL;

    if (!object || !object->m_data) goto ERROR_LABEL;

    player = GameObject_getPlayer(object);
    if (!player) goto ERROR_LABEL;

    RE_Animator_free(player->m_animator);

    free(player);

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Player_free()\n");
    return EXIT_FAILURE;
}

int Player_onStart(GameObject *object)
{
    Scene *scene = NULL;
    Player *player = NULL;
    PE_World *world = NULL;
    PE_Body *body = NULL;
    PE_BodyDef bodyDef;
    PE_Collider *mainCollider = NULL;
    PE_Collider *aboveDetector = NULL;
    PE_Collider *belowDetector = NULL;
    PE_ColliderDef colliderDef;
    int exitStatus;

    if (!object || !object->m_data) goto ERROR_LABEL;

    player = GameObject_getPlayer(object);
    if (!player) goto ERROR_LABEL;

    // Initialisation
    player->m_state = PLAYER_RUNNING;

    scene = GameObject_getScene(object);
    world = Scene_getWorld(scene);

    // Création du corps physique du joueur
    PE_BodyDef_setDefault(&bodyDef);
    bodyDef.type = PE_DYNAMIC_BODY;
    bodyDef.position = scene->m_startPos;
    body = PE_World_createBody(world, &bodyDef);
    if (!body) goto ERROR_LABEL;

    GameObject_setBody(player->m_object, body);

    // Création du collider principal
    PE_ColliderDef_setDefault(&colliderDef);
    PE_Shape_setAsBox(&colliderDef.shape, -0.35f, 0.01f, 0.35f, 0.95f);
    colliderDef.filter.categoryBits = FILTER_PLAYER | FILTER_VISIBLE;
    colliderDef.filter.maskBits = FILTER_BLOCK | FILTER_CAMERA | FILTER_COLLECTABLE | FILTER_ENEMY | FILTER_TOOL;
    mainCollider = PE_Body_createCollider(body, &colliderDef);
    if (!mainCollider) goto ERROR_LABEL;

    // Création du détecteur en dessous du sol
    PE_ColliderDef_setDefault(&colliderDef);
    PE_Shape_setAsBox(&colliderDef.shape, -0.35f, -0.1f, 0.35f, 0.f);
    colliderDef.isTrigger = TRUE;
    colliderDef.filter.categoryBits = FILTER_PLAYER;
    colliderDef.filter.maskBits = FILTER_BLOCK;

    belowDetector = PE_Body_createCollider(body, &colliderDef);
    if (!belowDetector) goto ERROR_LABEL;

    // Création du détecteur au dessus du sol
    PE_ColliderDef_setDefault(&colliderDef);
    PE_Shape_setAsBox(&colliderDef.shape, -0.3f, 0.1f, 0.3f, 0.2f);
    colliderDef.isTrigger = TRUE;
    colliderDef.filter.categoryBits = FILTER_PLAYER;
    colliderDef.filter.maskBits = FILTER_BLOCK;
    aboveDetector = PE_Body_createCollider(body, &colliderDef);
    if (!aboveDetector) goto ERROR_LABEL;

    // Ajout des callbacks
    PE_Collider_setOnCollisionStay(mainCollider, Player_onCollisionStay);

    PE_Collider_setOnTriggerStay(aboveDetector, Player_aboveTriggerStay);
    PE_Collider_setOnTriggerExit(aboveDetector, Player_aboveTriggerExit);
    PE_Collider_setOnTriggerStay(belowDetector, Player_belowTriggerStay);
    PE_Collider_setOnTriggerExit(belowDetector, Player_belowTriggerExit);

    // Activation des méthodes de la classe mère
    exitStatus = Scene_setToUpdate(scene, object);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    exitStatus = Scene_setToFixedUpdate(scene, object);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    exitStatus = Scene_setToRespawn(scene, object);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Player_onStart()\n");
    return EXIT_FAILURE;
}

int Player_onRespawn(GameObject *object)
{
    Scene *scene = NULL;
    Player *player = NULL;
    PE_Body *body = NULL;
    PE_Vec2 velocity;
    int exitStatus = EXIT_SUCCESS;

    if (!object || !object->m_data) goto ERROR_LABEL;

    player = GameObject_getPlayer(object);
    if (!player) goto ERROR_LABEL;

    scene = GameObject_getScene(object);
    body = GameObject_getBody(object);

    // TODO
    // Réinitialisez les paramètres du joueur ici lorsqu'il réapparait après avoir perdu une vie

    exitStatus = PE_Body_setPosition(body, &scene->m_startPos);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    PE_Vec2_set(&velocity, 0.f, 0.f);
    exitStatus = PE_Body_setVelocity(body, &velocity);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    player->m_state = PLAYER_RUNNING;

    exitStatus = Scene_setToRespawn(scene, object);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Player_onRespawn()\n");
    return EXIT_FAILURE;
}

void Player_setStartPosition(Player *player, PE_Vec2 *position)
{
    PE_Body *body = GameObject_getBody(player->m_object);
    PE_Body_setPosition(body, position);
}

void Player_bounce(Player *player)
{
    // TODO
    // Améliorez cette fonction pour que le joueur rebondisse quand elle est appelée
    // C'est l'ennemi qui appelle cette fonction quand il entre en collision avec le joueur et qu'il meurt.

    printf("Player_bounce() : Po rebondit sur une noisette magique !\n");
}

void Player_damage(Player *player)
{
    Scene *scene = GameObject_getScene(player->m_object);

    // TODO
    // Améliorez cette fonction avec une gestion des coeurs ou des vies
    // Vous pouvez modifier les stats du joueur
    // Vous pouvez quitter le jeu en appelant Scene_gameOver(scene);

    printf("Player_damage() : Po a mal !\n");

    Scene_respawn(scene);
}

void Player_kill(Player *player)
{
    Scene *scene = GameObject_getScene(player->m_object);
    // TODO
    // Améliorez cette fonction avec une gestion des vies
    // Vous pouvez modifier les stats du joueur
    // Vous pouvez quitter le jeu en appelant Scene_gameOver(scene);

    printf("Player_kill() : Po est mort !\n");

    Scene_respawn(scene);
}

void Player_addFirefly(Player *player)
{
    // TODO
    // Améliorez cette fonction avec une gestion lucioles et des vies

    printf("Player_addFirefly() : Po gobe une luciole !\n");
}

void Player_addHeart(Player *player)
{
    // TODO
    // Améliorez cette fonction

    printf("Player_addHeart() : Po gagne un coeur !\n");
}

int Player_update(GameObject *object)
{
    Player *player = NULL;
    Scene *scene = NULL;
    RE_Timer *time = NULL;
    GameInput *input = NULL;

    if (!object || !object->m_data) goto ERROR_LABEL;

    player = GameObject_getPlayer(object);
    if (!player) goto ERROR_LABEL;

    scene = GameObject_getScene(object);
    time = Scene_getTime(scene);
    input = Scene_getInput(scene);

    // Récupération des actions de l'utilisateur
    player->m_hDirection = input->hAxis;
    if (input->jumpPressed)
        player->m_jump = TRUE;

    // TODO
    // Améliorez cette fonction avec par exemple une meilleure gestion du saut

    // Mise à jour de l'animateur
    RE_Animator_update(player->m_animator, time);

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Player_update()\n");
    return EXIT_FAILURE;
}

int Player_fixedUpdate(GameObject *object)
{
    Player *player = NULL;
    Scene *scene = NULL;
    RE_Timer *time = NULL;
    GameInput *input = NULL;
    PE_Body *body = NULL;
    PE_Vec2 velocity;

    if (!object || !object->m_data) goto ERROR_LABEL;

    player = GameObject_getPlayer(object);
    if (!player) goto ERROR_LABEL;

    scene = GameObject_getScene(object);
    body = GameObject_getBody(object);
    time = Scene_getTime(scene);
    input = Scene_getInput(scene);

    // Initialisation du booléen indiquant si le joueur touche le sol
    player->m_onGround = player->m_belowDetector && !player->m_aboveDetector;

    // TODO
    // Améliorer cette fonction, et il y a beaucoup à faire !

    // Mise à jour de l'état du joueur
    if (player->m_onGround && (player->m_state == PLAYER_FALLING))
    {
        player->m_state = PLAYER_RUNNING;
        int exitStatus = RE_Animator_playTextureAnim(player->m_animator, "Running");
    }
    else if (!player->m_onGround && (player->m_state == PLAYER_RUNNING))
    {
        player->m_state = PLAYER_FALLING;
        int exitStatus = RE_Animator_playTextureAnim(player->m_animator, "Falling");
    }

    // Déplacement du joueur
    PE_Body_getVelocity(body, &velocity);
    velocity.x = player->m_hDirection * 8.f;
    if (player->m_jump)
    {
        velocity.y = 17.f;
        player->m_jump = FALSE;
    }
    PE_Body_setVelocity(body, &velocity);

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Player_fixedUpdate()\n");
    return EXIT_FAILURE;
}

int Player_render(GameObject *object)
{
    Player *player = NULL;
    Scene *scene = NULL;
    Camera *camera = NULL;
    GameTextures *textures = NULL;
    PE_Vec2 position;

    if (!object || !object->m_data) goto ERROR_LABEL;

    player = GameObject_getPlayer(object);
    if (!player) goto ERROR_LABEL;

    scene = GameObject_getScene(object);
    position = GameObject_getPosition(object);
    camera = Scene_getCamera(scene);
    textures = Scene_getTextures(scene);

    float viewX, viewY;
    Camera_worldToView(camera, &position, &viewX, &viewY);
    viewY -= RE_Texture_getPartHeight(textures->playerRunning, 0);
    viewX -= RE_Texture_getPartWidth(textures->playerRunning, 0) / 2;
    RE_Animator_renderF(player->m_animator, viewX, viewY);

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Player_render()\n");
    return EXIT_FAILURE;
}
