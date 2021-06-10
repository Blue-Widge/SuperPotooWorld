#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "../../Settings.h"
#include "GameObject.h"
#include "Camera.h"

typedef struct Scene_s Scene;

typedef enum GravityDirection_e
{
    NORMAL = 1,
    INVERTED = -1
} GravityDirection;

// AJOUT
typedef enum PlayerPowerUP_e
{
    PLAYER_NORMAL,
    PLAYER_FIRE

}PlayerPowerUP;

typedef enum PlayerState_e
{
    PLAYER_IDLE,
    PLAYER_WALKING,
    PLAYER_RUNNING,
    PLAYER_SKIDDING,
    PLAYER_FALLING,
    PLAYER_DYING
} PlayerState;

typedef struct PlayerStats_s
{
    int nbFireflies;
    int nbHearts;
    int nbLives;
    PlayerPowerUP PowerUP;

} PlayerStats;

typedef struct Player_s
{
    GameObject *m_object;
    PlayerStats m_stats;
    Bool m_aboveDetector;
    Bool m_belowDetector;
    Bool m_onGround;
    Bool m_jump;
    int m_state;
    float m_hDirection;

    RE_Animator *m_animator;
    PE_Vec2 m_startPos;
    
    float m_immune_time;
    int m_immune;

    int m_should_bounce;

    PE_Collider* m_damageableCollider;
    Bool m_shoot;
} Player;

Player *Player_new(Scene *scene, PE_Vec2 *position);

INLINE GameObject *Player_getObject(Player *player)
{
    return player->m_object;
}

INLINE PlayerStats *Player_getStats(Player *player)
{
    return &player->m_stats;
}

INLINE GravityDirection Player_getGravityDirection(Player* player)
{
    return PE_Body_getGravityScale(GameObject_getBody(Player_getObject(player))) >= 0 ? NORMAL : INVERTED;
} 

void Player_setStartPosition(Player *player, PE_Vec2 *position);


void Player_damage(Player *player);
void Player_bounce(Player *player);
void Player_kill(Player *player);
void Player_addFirefly(Player *player);
void Player_addHeart(Player *player);
void Player_addLives(Player* player);
void Player_PowerUP(Player* player, int type);
#endif