#include "Player.h"
#include "../Scene.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

// M?thodes du GameObject
int Player_free(GameObject *object);
int Player_onStart(GameObject *object);
int Player_onRespawn(GameObject *object);
int Player_update(GameObject *object);
int Player_fixedUpdate(GameObject *object);
int Player_render(GameObject *object);

// Fonction locales
int Player_createAnimator(Player *player);
void Player_aboveTriggerStay(PE_Trigger *trigger);
void Player_aboveTriggerExit(PE_Trigger *trigger);
void Player_belowTriggerStay(PE_Trigger *trigger);
void Player_belowTriggerExit(PE_Trigger *trigger);
void Player_onCollisionStay(PE_Collision *collision);
void Player_shoot(Player *player, int type);
void Player_internal_switchGravity(Player *player);

PE_Collider *Player_makeDefaultCollider(Player *player)
{
    PE_ColliderDef colliderDef;
    PE_ColliderDef_setDefault(&colliderDef);
    PE_Shape_setAsBox(&colliderDef.shape, -0.35f, 0.01f, 0.35f, 0.95f);
    colliderDef.filter.categoryBits = FILTER_PLAYER | FILTER_VISIBLE | FILTER_GRAVITY;
    colliderDef.filter.maskBits = FILTER_BLOCK | FILTER_CAMERA | FILTER_COLLECTABLE | FILTER_TOOL;
    return PE_Body_createCollider(GameObject_getBody(Player_getObject(player)), &colliderDef);
}

PE_Collider *Player_makeDamageableCollider(Player *player)
{
    PE_ColliderDef colliderDef;
    PE_ColliderDef_setDefault(&colliderDef);
    PE_Shape_setAsBox(&colliderDef.shape, -0.35f, 0.01f, 0.35f, 0.95f);
    colliderDef.filter.categoryBits = FILTER_DAMAGEABLE | FILTER_VISIBLE;
    colliderDef.filter.maskBits = FILTER_DAMAGER;
    return PE_Body_createCollider(GameObject_getBody(Player_getObject(player)), &colliderDef);
}

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

void Player_onTakeDamage(PE_Collision *collision)
{
    PE_Body *thisBody = PE_Collision_getBody(collision);
    PE_Body *otherBody = PE_Collision_getOtherBody(collision);
    GameObject *thisObject = PE_Body_getUserData(thisBody);
    GameObject *otherObject = PE_Body_getUserData(otherBody);
    Player* player = GameObject_getPlayer(thisObject);

    if (!thisObject || !otherObject)
    {
        printf("ERROR - Player_onCollisionStay()\n");
        return;
    }

    if (GameObject_getType(otherObject) == GAME_ENEMY)
    {
        int relPos = PE_Collision_getRelativePosition(collision);
        PE_Vec2 velocity;

        switch (relPos)
        {
        case PE_ABOVE:
            // Take damages
            velocity.y = 2.0f;
            if(Player_getGravityDirection(player) == INVERTED)
            {
                Player_damage(GameObject_getPlayer(thisObject));
            }
            else
            {
                Player_bounce(GameObject_getPlayer(thisObject));
                Enemy_damage(GameObject_getEnemy(otherObject));
            }
            PE_Body_setCollisionResponse(thisBody, &velocity);
            break;
        case PE_LEFT:
            // Take damages and get bounced to the right
            velocity.y = 10.0f;
            velocity.x = -8.0f;
            Player_damage(GameObject_getPlayer(thisObject));
            PE_Body_setCollisionResponse(thisBody, &velocity);
        case PE_RIGHT:
            //Take damages and get bounced to the left
            velocity.y = 10.f;
            velocity.x = -2.0f;
            Player_damage(GameObject_getPlayer(thisObject));
            PE_Body_setCollisionResponse(thisBody, &velocity);
            break;
        case PE_BELOW:
            //Kill the enemy
            if(Player_getGravityDirection(player) == INVERTED)
            {
                Player_bounce(GameObject_getPlayer(thisObject));
                Enemy_damage(GameObject_getEnemy(otherObject));
            }
            else
            {
                Player_damage(GameObject_getPlayer(thisObject));
            }
            PE_Body_setCollisionResponse(thisBody, &velocity);
            break;
        default:
            break;
        }
    }
}

void Player_onCollisionStay(PE_Collision *collision)
{
    PE_Body *thisBody = PE_Collision_getBody(collision);
    PE_Body *otherBody = PE_Collision_getOtherBody(collision);
    GameObject *thisObject = PE_Body_getUserData(thisBody);
    GameObject *otherObject = PE_Body_getUserData(otherBody);
    Player* player = GameObject_getPlayer(thisObject);

    if (!thisObject || !otherObject)
    {
        printf("ERROR - Player_onCollisionStay()\n");
        return;
    }

    if (GameObject_getType(otherObject) == GAME_ENEMY)
    {
        int relPos = PE_Collision_getRelativePosition(collision);
        PE_Vec2 velocity;

        switch (relPos)
        {
        case PE_ABOVE:
            // Take damages
            velocity.y = 2.0f;
            if(Player_getGravityDirection(player) == NORMAL)
            {
                Player_damage(GameObject_getPlayer(thisObject));
            }
            else
            {
                Player_bounce(GameObject_getPlayer(thisObject));
                Enemy_damage(GameObject_getEnemy(otherObject));
            }
            PE_Body_setCollisionResponse(thisBody, &velocity);
            break;
        case PE_LEFT:
            // Take damages and get bounced to the right
            velocity.y = 10.0f;
            velocity.x = -8.0f;
            Player_damage(GameObject_getPlayer(thisObject));
            PE_Body_setCollisionResponse(thisBody, &velocity);
        case PE_RIGHT:
            //Take damages and get bounced to the left
            velocity.y = 10.f;
            velocity.x = -2.0f;
            Player_damage(GameObject_getPlayer(thisObject));
            PE_Body_setCollisionResponse(thisBody, &velocity);
            break;
        case PE_BELOW:
            //Kill the enemy
            if(Player_getGravityDirection(player) == NORMAL)
            {
                Player_bounce(GameObject_getPlayer(thisObject));
                Enemy_damage(GameObject_getEnemy(otherObject));
            }
            else
            {
                Player_damage(GameObject_getPlayer(thisObject));
            }
            PE_Body_setCollisionResponse(thisBody, &velocity);
            break;
        default:
            break;
        }
    }
    if (GameObject_getType(otherObject) == GAME_BLOCK)
    {
        int relPos = PE_Collision_getRelativePosition(collision);
        PE_Vec2 velocity;
        PE_Body_getVelocity(thisBody, &velocity);
        Block *block = GameObject_getBlock(otherObject);

        if(block->m_type == BLOCK_KILL)
        {
            Player_kill(player);
            return;
        }
        
        switch (relPos)
        {
        case PE_ABOVE:
            if (velocity.y < 0.f)
                velocity.y = 0.f;
            break;

        case PE_BELOW:

            if (block->m_type != BLOCK_ONE_WAY)
            {
                if (velocity.y > 0.f)
                    velocity.y = 0.f;
                Block_hit(GameObject_getBlock(otherObject));
            }
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
    int exitStatus = EXIT_SUCCESS;

    animator = RE_Animator_new();
    if (!animator) goto ERROR_LABEL;

    player->m_animator = animator;

    // D?finition de l'animation "Running"
    texAnim = RE_Animator_createTextureAnim(animator, textures->playerRunning, "Running");
    if (!texAnim) goto ERROR_LABEL;

    RE_TextureAnim_setCycleTime(texAnim, 0.3f);

    // D?finition de l'animation "Falling"
    texAnim = RE_Animator_createTextureAnim(animator, textures->playerFalling, "Falling");
    if (!texAnim) goto ERROR_LABEL;

    RE_TextureAnim_setCycleTime(texAnim, 0.2f);

    texAnim = RE_Animator_createTextureAnim(animator, textures->playerRunningFire, "RunningFire");
    if (!texAnim) goto ERROR_LABEL;

    RE_TextureAnim_setCycleTime(texAnim, 0.3f);

    texAnim = RE_Animator_createTextureAnim(animator, textures->playerFallingFire, "FallingFire");
    if (!texAnim) goto ERROR_LABEL;
    RE_TextureAnim_setCycleTime(texAnim, 0.2f);

    texAnim = RE_Animator_createTextureAnim(animator, textures->playerRunningScream, "RunningScream");
    if (!texAnim) goto ERROR_LABEL;
    RE_TextureAnim_setCycleTime(texAnim, 0.3f);

    texAnim = RE_Animator_createTextureAnim(animator, textures->playerFallingScream, "FallingScream");
    if (!texAnim) goto ERROR_LABEL;
    RE_TextureAnim_setCycleTime(texAnim, 0.2f);

    texAnim = RE_Animator_createTextureAnim(animator, textures->IdlePlayer, "Idle");
    if (!texAnim) goto ERROR_LABEL;

    RE_TextureAnim_setCycleTime(texAnim, 0.2f);

    texAnim = RE_Animator_createTextureAnim(animator, textures->IdleFirePlayer, "IdleFire");
    if (!texAnim) goto ERROR_LABEL;

    RE_TextureAnim_setCycleTime(texAnim, 0.2f);

    texAnim = RE_Animator_createTextureAnim(animator, textures->IdleScreamPlayer, "IdleScream");
    if (!texAnim) goto ERROR_LABEL;
    RE_TextureAnim_setCycleTime(texAnim, 0.2f);
    
    RE_Animator_playTextureAnim(player->m_animator, "Idle");

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

    // Initialisation de la classe m?re
    object->m_data = (void *)player;
    object->m_layer = 3;
    object->cm_free = Player_free;
    object->cm_onStart = Player_onStart;
    object->cm_onRespawn = Player_onRespawn;
    object->cm_update = Player_update;
    object->cm_fixedUpdate = Player_fixedUpdate;
    object->cm_render = Player_render;

    // Allocations suppl?mentaires
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

    // AJOUT
    player->m_stats.PowerUP = PLAYER_NORMAL;

    scene = GameObject_getScene(object);
    world = Scene_getWorld(scene);

    // Cr?ation du corps physique du joueur
    PE_BodyDef_setDefault(&bodyDef);
    bodyDef.type = PE_DYNAMIC_BODY;
    bodyDef.position = scene->m_startPos;
    body = PE_World_createBody(world, &bodyDef);
    if (!body) goto ERROR_LABEL;

    GameObject_setBody(player->m_object, body);

    // Cr?ation du collider principal
    mainCollider = Player_makeDefaultCollider(player);
    if (!mainCollider) goto ERROR_LABEL;

    PE_Collider *damageableCollider = Player_makeDamageableCollider(player);
    if (!damageableCollider) goto ERROR_LABEL;

    // Cr?ation du d?tecteur en dessous du sol
    PE_ColliderDef_setDefault(&colliderDef);
    PE_Shape_setAsBox(&colliderDef.shape, -0.35f, -0.1f, 0.35f, 0.f);
    colliderDef.isTrigger = TRUE;
    colliderDef.filter.categoryBits = FILTER_PLAYER;
    colliderDef.filter.maskBits = FILTER_BLOCK;

    belowDetector = PE_Body_createCollider(body, &colliderDef);
    if (!belowDetector) goto ERROR_LABEL;

    // Cr?ation du d?tecteur au dessus du sol
    PE_ColliderDef_setDefault(&colliderDef);
    PE_Shape_setAsBox(&colliderDef.shape, -0.3f, 0.1f, 0.3f, 0.2f);
    colliderDef.isTrigger = TRUE;
    colliderDef.filter.categoryBits = FILTER_PLAYER;
    colliderDef.filter.maskBits = FILTER_BLOCK;
    aboveDetector = PE_Body_createCollider(body, &colliderDef);
    if (!aboveDetector) goto ERROR_LABEL;

    player->m_aboveCollider = aboveDetector;
    player->m_belowCollider = belowDetector;

    // Ajout des callbacks
    PE_Collider_setOnCollisionStay(mainCollider, Player_onCollisionStay);

    PE_Collider_setOnTriggerStay(player->m_aboveCollider, Player_aboveTriggerStay);
    PE_Collider_setOnTriggerExit(player->m_aboveCollider, Player_aboveTriggerExit);
    PE_Collider_setOnTriggerStay(player->m_belowCollider, Player_belowTriggerStay);
    PE_Collider_setOnTriggerExit(player->m_belowCollider, Player_belowTriggerExit);

    PE_Collider_setOnCollisionStay(damageableCollider, Player_onTakeDamage);

    player->m_damageableCollider = damageableCollider;

    // Activation des m?thodes de la classe m?re
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

    player->m_immune = FALSE;
    player->m_immune_time = -1;

    if (player->m_damageableCollider)
    {
        PE_Body_removeCollider(body, player->m_damageableCollider);
    }

    if (Player_getGravityDirection(player) == INVERTED)
    {
        Player_switchGravity(player);
    }

    player->m_damageableCollider = Player_makeDamageableCollider(player);
    PE_Collider_setOnCollisionStay(player->m_damageableCollider, Player_onTakeDamage);

    // R?initialisez les param?tres du joueur ici lorsqu'il r?apparait apr?s avoir perdu une vie

    player->m_state = PLAYER_IDLE;
    player->m_stats.nbFireflies = 0;
    player->m_stats.nbHearts = 2;
    player->m_stats.PowerUP = PLAYER_NORMAL;

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
    player->m_should_bounce = TRUE;
}

void Player_damage(Player *player)
{
    if (player->m_immune) { return; }

    Scene *scene = GameObject_getScene(player->m_object);
    Player_powerUp(player, PLAYER_NORMAL);


    // Am?liorez cette fonction avec une gestion des coeurs ou des vies
    // Vous pouvez modifier les stats du joueur
    // Vous pouvez quitter le jeu en appelant Scene_gameOver(scene);

    player->m_stats.nbHearts--;
    if (player->m_stats.nbHearts < 0)
    {
        Player_kill(player);
    }
    else
    {
        player->m_immune = TRUE;
        player->m_immune_time = RE_Timer_getElapsed(Scene_getTime(scene));
    }
}

void Player_kill(Player *player)
{
    Scene *scene = GameObject_getScene(player->m_object);

    // Am?liorez cette fonction avec une gestion des vies
    // Vous pouvez modifier les stats du joueur
    // Vous pouvez quitter le jeu en appelant Scene_gameOver(scene);

    player->m_state = PLAYER_DYING;
    player->m_stats.nbLives--;
    if (player->m_stats.nbLives < 0)
    {
        //Scene_gameOver(scene);
    }

    Scene_respawn(scene);
}

void Player_addFirefly(Player *player)
{
    // Am?liorez cette fonction avec une gestion lucioles et des vies
    player->m_stats.nbFireflies++;
}

void Player_addHeart(Player *player)
{
    // Am?liorez cette fonction
    if (player->m_stats.nbHearts < 2)
    {
        player->m_stats.nbHearts++;
    }
}

void Player_powerUp(Player *player, int type)
{
    player->m_state = PLAYER_IDLE;
    switch (type)
    {
    case POWERUP_FIRE:
        player->m_stats.PowerUP = PLAYER_FIRE;
        break;
    case POWERUP_SCREAM:
        player->m_stats.PowerUP = PLAYER_SCREAM;
        break;
    default:
        player->m_stats.PowerUP = PLAYER_NORMAL;
        break;
    }
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

    if (player->m_immune && RE_Timer_getElapsed(Scene_getTime(scene)) - player->m_immune_time > 2.0f)
    {
        player->m_immune = FALSE;
        player->m_immune_time = -1.0f;
        player->m_damageableCollider = Player_makeDamageableCollider(player);
        PE_Collider_setOnCollisionStay(player->m_damageableCollider, Player_onTakeDamage);
    }

    // R?cup?ration des actions de l'utilisateur
    player->m_hDirection = input->hAxis;
    if (input->jumpPressed)
        player->m_jump = TRUE;

    // Am?liorez cette fonction avec par exemple une meilleure gestion du saut
    if (player->m_jump == TRUE && player->m_state == PLAYER_FALLING)
        player->m_jump = FALSE;

    // Mise ? jour de l'animateur
    RE_Animator_update(player->m_animator, time);

    if (input->shootPressed)
    {
        player->m_shoot = TRUE;
    }

    if (input->skiddingPressed && !player->m_onGround)
    {
        player->m_state = PLAYER_SKIDDING;
    }
    else if (!(input->skiddingPressed) && !player->m_onGround)
    {
        player->m_state = PLAYER_FALLING;
    }
    
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

    if (player->m_switchGravity)
    {
        Player_internal_switchGravity(player);
        player->m_switchGravity = FALSE;
    }

    // Initialisation du bool?en indiquant si le joueur touche le sol
    player->m_onGround = player->m_belowDetector && !player->m_aboveDetector;

    if (player->m_immune && player->m_damageableCollider)
    {
        PE_Body_removeCollider(player->m_damageableCollider->m_body, player->m_damageableCollider);
        player->m_damageableCollider = NULL;
    }

    if (player->m_stats.PowerUP != PLAYER_NORMAL && player->m_shoot == TRUE)
    {
        Player_shoot(player, player->m_stats.PowerUP);
        player->m_shoot = FALSE;
    }
    else
    {
        player->m_shoot = FALSE;
    }

    PE_Body_getVelocity(body, &velocity);
    velocity.x = player->m_hDirection * 8.f;
    if (player->m_should_bounce)
    {
        velocity.y = Player_getGravityDirection(player) * 17.f;
        player->m_should_bounce = FALSE;
    }
    PE_Body_setVelocity(body, &velocity);

    // Mise ? jour de l'?tat du joueur
    if (player->m_onGround && (player->m_state == PLAYER_FALLING))
    {
        if (player->m_hDirection == 0)
        {
            player->m_state = PLAYER_IDLE;
            switch (player->m_stats.PowerUP)
            {
            case PLAYER_FIRE:
                RE_Animator_playTextureAnim(player->m_animator, "IdleFire");
                break;
            case PLAYER_SCREAM:
                 RE_Animator_playTextureAnim(player->m_animator, "IdleScream");
                break;
            default:
                RE_Animator_playTextureAnim(player->m_animator, "Idle");
                break;
            }
        }
        else
        {
            switch (player->m_stats.PowerUP)
            {
            case PLAYER_FIRE:
                RE_Animator_playTextureAnim(player->m_animator, "RunningFire");
                break;
            case PLAYER_SCREAM:
                RE_Animator_playTextureAnim(player->m_animator, "RunningScream");
                break;
            default:
                RE_Animator_playTextureAnim(player->m_animator, "Running");
                break;
            }

            player->m_state = PLAYER_RUNNING;
        }
    }
    else if (!player->m_onGround && (player->m_state != PLAYER_FALLING && player->m_state != PLAYER_SKIDDING))
    {
        player->m_state = PLAYER_FALLING;
        int exitStatus;

        switch (player->m_stats.PowerUP)
        {
        case PLAYER_FIRE:
            exitStatus = RE_Animator_playTextureAnim(player->m_animator, "FallingFire");
            break;
        case PLAYER_SCREAM:
            exitStatus = RE_Animator_playTextureAnim(player->m_animator, "FallingScream");
            break;
        default:
            exitStatus = RE_Animator_playTextureAnim(player->m_animator, "Falling");
            break;
        }
    }

    if (player->m_onGround && player->m_state != PLAYER_FALLING)
    {
        if (player->m_hDirection == 0)
        {
            player->m_state = PLAYER_IDLE;
            switch (player->m_stats.PowerUP)
            {
            case PLAYER_FIRE:
                RE_Animator_playTextureAnim(player->m_animator, "IdleFire");
                break;
            case PLAYER_SCREAM:
                RE_Animator_playTextureAnim(player->m_animator, "IdleScream");
                break;
            default:
                RE_Animator_playTextureAnim(player->m_animator, "Idle");
                break;
            }
        }
        else
        {
            if (player->m_state != PLAYER_RUNNING)
            {
                switch (player->m_stats.PowerUP)
                {
                case PLAYER_FIRE:
                    RE_Animator_playTextureAnim(player->m_animator, "RunningFire");
                    break;
                case PLAYER_SCREAM:
                    RE_Animator_playTextureAnim(player->m_animator, "RunningScream");
                    break;
                default:
                    RE_Animator_playTextureAnim(player->m_animator, "Running");
                    break;
                }
            }

            player->m_state = PLAYER_RUNNING;
        }
    }
    // D?placement du joueur
    PE_Body_getVelocity(body, &velocity);
    velocity.x = player->m_hDirection * 8.f;
    if (player->m_jump)
    {
        velocity.y = Player_getGravityDirection(player) * 17.f;
        player->m_jump = FALSE;
    }

    if (!player->m_onGround && player->m_state == PLAYER_SKIDDING)
    {
        if (Player_getGravityDirection(player) < 0)
        {
            if (velocity.y > 0)
            {
                velocity.y = Player_getGravityDirection(player) * -2.f;
            }  
        }
        else
        {
            if (velocity.y < 0)
            {
                velocity.y = Player_getGravityDirection(player) * -2.f;
            }  
        }
    }
    PE_Body_setVelocity(body, &velocity);

    if (player->m_hDirection == -1)
    {
        player->facingDirection = FACING_LEFT;
        RE_Transform transform;
        RE_Animator_getTransform(player->m_animator, &transform);
        transform.flipFlags |= RE_FLIP_HORIZONTAL;
        RE_Animator_setTransform(player->m_animator, &transform);
    }
    else if (player->m_hDirection == 1)
    {
        player->facingDirection = FACING_RIGHT;
        RE_Transform transform;
        RE_Animator_getTransform(player->m_animator, &transform);
        transform.flipFlags &= ~RE_FLIP_HORIZONTAL;
        RE_Animator_setTransform(player->m_animator, &transform);
    }
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

    if (Player_getGravityDirection(player) == INVERTED)
    {
        viewY += 35.0f;
    }

    RE_Animator_renderF(player->m_animator, viewX, viewY);

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Player_render()\n");
    return EXIT_FAILURE;
}
void Player_switchGravity(Player *player)
{
    player->m_switchGravity = TRUE;
}


void Player_internal_switchGravity(Player *player)
{
    GameObject *object = Player_getObject(player);
    PE_Body *body = GameObject_getBody(object);
    PE_ColliderDef colliderDef;
    RE_Transform transform;

    PE_Body_setGravityScale(GameObject_getBody(object), -Player_getGravityDirection(player));
    RE_Animator_getTransform(player->m_animator, &transform);

    PE_Body_removeCollider(body, player->m_aboveCollider);
    PE_Body_removeCollider(body, player->m_belowCollider);

    PE_ColliderDef_setDefault(&colliderDef);
    colliderDef.isTrigger = TRUE;
    colliderDef.filter.categoryBits = FILTER_PLAYER;
    colliderDef.filter.maskBits = FILTER_BLOCK;

    if (Player_getGravityDirection(player) == INVERTED)
    {
        transform.flipFlags |= RE_FLIP_VERTICAL;

        PE_Shape_setAsBox(&colliderDef.shape, -0.35f, 0.0f, 0.35f, 1.0f);
        player->m_belowCollider = PE_Body_createCollider(body, &colliderDef);

        PE_Shape_setAsBox(&colliderDef.shape, -0.3f, -1.0f, 0.3f, 0.0f);
        player->m_aboveCollider = PE_Body_createCollider(body, &colliderDef);
    }
    else
    {
        transform.flipFlags &= ~RE_FLIP_VERTICAL;

        PE_Shape_setAsBox(&colliderDef.shape, -0.35f, -1.0f, 0.35f, 0.f);
        player->m_belowCollider = PE_Body_createCollider(body, &colliderDef);

        PE_Shape_setAsBox(&colliderDef.shape, -0.3f, 0.1f, 0.3f, 0.2f);
        player->m_aboveCollider = PE_Body_createCollider(body, &colliderDef);
        
    }

    RE_Animator_setTransform(player->m_animator, &transform);

    PE_Collider_setOnTriggerStay(player->m_aboveCollider, Player_aboveTriggerStay);
    PE_Collider_setOnTriggerExit(player->m_aboveCollider, Player_aboveTriggerExit);
    PE_Collider_setOnTriggerStay(player->m_belowCollider, Player_belowTriggerStay);
    PE_Collider_setOnTriggerExit(player->m_belowCollider, Player_belowTriggerExit);

    player->m_aboveDetector = FALSE;
    player->m_belowDetector = FALSE;
}

void Player_shoot(Player *player, int type)
{
    GameObject *object = Player_getObject(player);
    Scene *scene = GameObject_getScene(object);
    PE_Vec2 position = GameObject_getPosition(object);
    position.y += 0.25;
    position.x += player->facingDirection * 1;
    switch (type)
    {
    case PLAYER_FIRE:
        Scene_createSkill(scene, FIREBALL, &position);
        break;
    case PLAYER_SCREAM:
        Scene_createSkill(scene, SCREAM, &position);
        break;
    default:
        break;
    }
}
