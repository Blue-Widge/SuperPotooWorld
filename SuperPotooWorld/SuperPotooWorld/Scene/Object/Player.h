#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "../../Settings.h"
#include "GameObject.h"
#include "Camera.h"

typedef struct Scene_s Scene;

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

void Player_setStartPosition(Player *player, PE_Vec2 *position);


void Player_damage(Player *player);
void Player_bounce(Player *player);
void Player_kill(Player *player);
void Player_addFirefly(Player *player);
void Player_addHeart(Player *player);

#endif