#include "Tilemap.h"
#include "Scene.h"

#include <stdio.h>
#include <stdlib.h>

static const int BINARY_TO_PART_INDEX[256] = {
    0 ,  1,  0,  1,  2,  3,  2,  4,  0,  1,  0,  1,  2,  3,  2,  4,
    5 ,  6,  5,  6,  7,  8,  7,  9,  5,  6,  5,  6, 10, 11, 10, 12,
    0 ,  1,  0,  1,  2,  3,  2,  4,  0,  1,  0,  1,  2,  3,  2,  4,
    5 ,  6,  5,  6,  7,  8,  7,  9,  5,  6,  5,  6, 10, 11, 10, 12,
    13, 14, 13, 14, 15, 16, 15, 17, 13, 14, 13, 14, 15, 16, 15, 17,
    18, 19, 18, 19, 20, 21, 20, 22, 18, 19, 18, 19, 23, 24, 23, 25,
    13, 14, 13, 14, 15, 16, 15, 17, 13, 14, 13, 14, 15, 16, 15, 17,
    26, 27, 26, 27, 28, 29, 28, 30, 26, 27, 26, 27, 31, 32, 31, 33,
    0 ,  1,  0,  1,  2,  3,  2,  4,  0,  1,  0,  1,  2,  3,  2,  4,
    5 ,  6,  5,  6,  7,  8,  7,  9,  5,  6,  5,  6, 10, 11, 10, 12,
    0 ,  1,  0,  1,  2,  3,  2,  4,  0,  1,  0,  1,  2,  3,  2,  4,
    5 ,  6,  5,  6,  7,  8,  7,  9,  5,  6,  5,  6, 10, 11, 10, 12,
    13, 34, 13, 34, 15, 35, 15, 36, 13, 34, 13, 34, 15, 35, 15, 36,
    18, 37, 18, 37, 20, 38, 20, 39, 18, 37, 18, 37, 23, 40, 23, 41,
    13, 34, 13, 34, 15, 35, 15, 36, 13, 34, 13, 34, 15, 35, 15, 36,
    26, 42, 26, 42, 28, 43, 28, 44, 26, 42, 26, 42, 31, 45, 31, 46
};

Tilemap *Tilemap_new(Scene *scene, int width, int height)
{
    Tilemap *tilemap = NULL;

    tilemap = (Tilemap *)calloc(1, sizeof(Tilemap));
    if (!tilemap) goto ERROR_LABEL;

    tilemap->m_scene = scene;
    tilemap->m_width = width;
    tilemap->m_height = height;

    // Allocation des tuiles
    tilemap->m_tiles = (Tile **)calloc(width, sizeof(Tile *));
    if (!tilemap->m_tiles) goto ERROR_LABEL;

    for (int x = 0; x < width; x++)
    {
        tilemap->m_tiles[x] = (Tile *)calloc(height, sizeof(Tile));
        if (!tilemap->m_tiles[x]) goto ERROR_LABEL;
    }

    return tilemap;

ERROR_LABEL:
    printf("ERROR - Tilemap_new()\n");
    Tilemap_free(tilemap);
    return NULL;
}

void Tilemap_free(Tilemap *tilemap)
{
    if (!tilemap)
        return;

    if (tilemap->m_tiles)
    {
        int width = tilemap->m_width;
        for (int x = 0; x < width; x++)
        {
            free(tilemap->m_tiles[x]);
        }
        free(tilemap->m_tiles);
    }

    free(tilemap);
}

int Tilemap_setTile(Tilemap *tilemap, int x, int y, int type)
{
    if (x < 0 || x >= tilemap->m_width ||
        y < 0 || y >= tilemap->m_height)
        return EXIT_FAILURE;

    Tile *tile = &(tilemap->m_tiles[x][y]);
    tile->m_partIdx = 0;
    tile->m_type = type;

    return EXIT_SUCCESS;
}

int Tilemap_initCollider(Tilemap *tilemap)
{
    Scene *scene = tilemap->m_scene;
    int width = tilemap->m_width;
    int height = tilemap->m_height;

    // TODO
    // Améliorez l'algorithme de création du collider

    // Création du sol
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            Tile *tile = &(tilemap->m_tiles[x][y]);

            // Création d'un bloc statique
            if ((tile->m_type == TILE_GROUND) ||
                (tile->m_type == TILE_WOOD))
            {
                PE_Vec2 position;
                PE_AABB aabb;
                PE_Vec2_set(&position, (float)x, (float)(y + 1));
                PE_AABB_set(&aabb, 0.f, -1.f, 1.f, 0.f);
                Block *block = Scene_createBlock(scene, BLOCK_GROUND, &position, &aabb);
                if (!block) goto ERROR_LABEL;
            }
        }
    }

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Tilemap_initCollider()\n");
    return EXIT_FAILURE;
}


int isGround(Tile** tiles, int x, int y, int width, int height)
{
    if ((x < 0) || (x >= width) || (y < 0))
        return 1;
    else if (y >= height)
        return 0;
    else
        return (tiles[x][y].m_type == TILE_GROUND);
}

void Tilemap_initTiles(Tilemap *tilemap)
{
    int width = tilemap->m_width;
    int height = tilemap->m_height;
    Tile **tiles = tilemap->m_tiles;

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if (tiles[x][y].m_type == TILE_GROUND)
            {
                // Calcule l'indice de la partie de la texture à afficher pour le sol
                int sum = 0;
                int minX = x > 0;
                int minY = y > 0;
                int maxX = x < width - 1;
                int maxY = y < height - 1;

                if (maxY) {

                    if (maxX && tiles[x+1][y+1].m_type == TILE_GROUND) {
                        sum += 2;
                    }

                    if (minX && tiles[x-1][y+1].m_type == TILE_GROUND) {
                        sum += 128;
                    }

                    if (tiles[x][y+1].m_type == TILE_GROUND) {
                        sum += 1;
                    }

                }

                if (minY) {

                    if (maxX && tiles[x+1][y-1].m_type == TILE_GROUND) {
                        sum += 8;
                    }

                    if (minX && tiles[x-1][y-1].m_type == TILE_GROUND) {
                        sum += 32;
                    }

                    if (tiles[x][y-1].m_type == TILE_GROUND) {
                        sum += 16;
                    }

                }

                if (minX && tiles[x-1][y].m_type == TILE_GROUND) {
                    sum += 64;
                }

                if (maxX && tiles[x + 1][y].m_type == TILE_GROUND) {
                    sum += 4;
                }

                tiles[x][y].m_partIdx = BINARY_TO_PART_INDEX[sum];
            }
            else
            {
                tiles[x][y].m_partIdx = 0;
            }
        }
    }
}

void Tilemap_render(Tilemap *tilemap)
{
    Camera *camera = Scene_getCamera(tilemap->m_scene);
    GameTextures *textures = Scene_getTextures(tilemap->m_scene);
    Tile **tiles = tilemap->m_tiles;
    int width = tilemap->m_width;
    int height = tilemap->m_height;

    int x0 = (int)camera->m_worldView.lower.x - 1;
    int y0 = (int)camera->m_worldView.lower.y - 1;
    int x1 = (int)camera->m_worldView.upper.x + 2;
    int y1 = (int)camera->m_worldView.upper.y + 2;

    x0 = x0 < 0 ? 0 : x0;
    y0 = y0 < 0 ? 0 : y0;
    x1 = x1 > width ? width : x1;
    y1 = y1 > height ? height : y1;

    for (int x = x0; x < x1; x++)
    {
        for (int y = y1 - 1; y >= y0; y--)
        {
            int partIdx;
            float viewX, viewY;
            PE_Vec2 position;
            position.x = (float)x;
            position.y = (float)(y + 1);
            Camera_worldToView(camera, &position, &viewX, &viewY);

            switch (tiles[x][y].m_type)
            {
            case TILE_GROUND:
                partIdx = tiles[x][y].m_partIdx;
                RE_Texture_render(textures->ground, partIdx, (int)viewX, (int)viewY);
                break;

            case TILE_WOOD:
                partIdx = tiles[x][y].m_partIdx;
                RE_Texture_renderF(textures->wood, partIdx, viewX, viewY);
                break;

            case TILE_ONE_WAY:
                partIdx = tiles[x][y].m_partIdx;
                RE_Texture_renderF(textures->oneWay, partIdx, viewX, viewY);
                break;

            default:
                break;
            }
        }
    }
}
