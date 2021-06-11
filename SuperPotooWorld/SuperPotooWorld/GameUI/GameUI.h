#ifndef _GAME_UI_H_
#define _GAME_UI_H_

typedef struct Scene_s Scene;
typedef struct PlayerStats_s PlayerStats;

void GameUI_render(Scene *scene, PlayerStats *stats);

#endif
