#ifndef _TILEMAP_H_
#define _TILEMAP_H_

#include "../Settings.h"

typedef struct Scene_s Scene;

typedef enum TileType_e
{
    TILE_EMPTY,
    TILE_GROUND,
    TILE_WOOD,
    TILE_ONE_WAY
} TileType;

typedef struct Tile_s
{
    TileType m_type;
    int m_partIdx;
} Tile;

typedef struct Tilemap_s
{
    Scene *m_scene;
    int m_width;
    int m_height;
    Tile **m_tiles;
} Tilemap;

Tilemap *Tilemap_new(Scene *scene, int width, int height);
void Tilemap_free(Tilemap *tilemap);

int Tilemap_setTile(Tilemap *tilemap, int x, int y, int type);

/// @brief Initialise les colliders de la tilemap dans le moteur physique.
/// @param[in,out] tilemap la tilemap.
int Tilemap_initCollider(Tilemap *tilemap);

/// @brief Initialise les tuiles de la tilemap.
/// Cette fonction calcule les textures à afficher pour le sol.
/// @param[in,out] tilemap la tilemap.
void Tilemap_initTiles(Tilemap *tilemap);

void Tilemap_render(Tilemap *tilemap);

#endif