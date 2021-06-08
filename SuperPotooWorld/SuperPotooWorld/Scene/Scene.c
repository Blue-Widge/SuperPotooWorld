#include "Scene.h"

#include <stdio.h>
#include <stdlib.h>

Player *Scene_createPlayer(Scene *scene, PE_Vec2 *position);
Camera *Scene_createCamera(Scene *scene, PE_AABB *worldView, PE_AABB *worldAABB);

int Scene_isValidChar(char c)
{
    char validChar[] = { '\n', '.', '#', '=', 'W', 'C', 'e', 'S', 'X', 'o', '?', 'F', 'H' };
    int nbValidChar = sizeof(validChar) / sizeof(char);
    int i;

    for (i = 0; i < nbValidChar; i++)
        if (c == validChar[i])
            return 1;

    return 0;
}

char *Scene_getLevelBuffer(FILE *levelFile, long *size)
{
    char *buffer = NULL, *stdBuffer = NULL;
    long fileSize, stdSize;
    int i;

    if (!levelFile) goto ERROR_LABEL;

    fseek(levelFile, 0, SEEK_END);
    fileSize = ftell(levelFile);
    rewind(levelFile);

    buffer = (char*)calloc(fileSize, sizeof(char));
    stdBuffer = (char*)calloc(fileSize, sizeof(char));
    if (!buffer || !stdBuffer) goto ERROR_LABEL;

    fread(buffer, 1, fileSize, levelFile);
    stdSize = 0;
    for (i = 0; i < fileSize; i++)
    {
        char c = buffer[i];
        if (Scene_isValidChar(c))
        {
            stdBuffer[stdSize] = c;
            stdSize++;
            break;
        }
    }
    for (i = i + 1; i < fileSize; i++)
    {
        char c = buffer[i];
        if (Scene_isValidChar(c))
        {
            if ((c == '\n') && (stdBuffer[stdSize - 1] == '\n'))
                break;

            stdBuffer[stdSize] = c;
            stdSize++;

        }
    }
    *size = stdSize;

    free(buffer);

    return stdBuffer;

ERROR_LABEL:
    printf("ERROR - Scene_getLevelBuffer()\n");
    free(buffer);
    free(stdBuffer);
    return NULL;
}

int Scene_getLevelDim(char *levelBuffer, long size, int *lvlWidth, int *lvlHeight)
{
    int Width = 0, Height = 0;
    for (int i = 0; i < size; i++)
    {
        char c = levelBuffer[i];
        
        if (Scene_isValidChar(c))
        {
            if (c == '\n')
            {
                Height++;
            } 
            else
                Width++;
        }
        
    }

    Width /= Height;

    *lvlWidth = Width;
    *lvlHeight = Height;

    return EXIT_SUCCESS;
}

int Scene_parseLevelBuffer(Scene *scene, char *levelBuffer, long size, int lvlWidth, int lvlHeight)
{
    Tilemap *tilemap = scene->m_tilemap;
    PE_Vec2 position = {0};
    PE_AABB aabb;

    int x = lvlWidth, y = 0;
    int w = lvlWidth;
    for (int i = size -1; i >=0; --i)
    {
        char c = levelBuffer[i];

        PE_Vec2_set(&position, x, y);
        PE_AABB_set(&aabb, 0, 0, 1, 1);

        switch (c)
        {
        case '.' : 
            Tilemap_setTile(tilemap, x, y, TILE_EMPTY);
            break;
        case '#':
            Tilemap_setTile(tilemap, x, y, TILE_GROUND);
            break;
        case 'W':
            Tilemap_setTile(tilemap, x, y, TILE_WOOD);
            break;
        case '=':
            Tilemap_setTile(tilemap, x, y, TILE_ONE_WAY);
            break;
        case 'S':
            PE_Vec2_set(&scene->m_startPos, x, y);
            break;
            // Bloc cassable
        case 'X':
            Scene_createBlock(scene, BLOCK_BRICK, &position, &aabb);
            break;
            // Arrivee
        case 'F':
            Scene_createTool(scene, TOOL_FINISH, &position, &aabb);
            break;
            // Checkpoint
        case 'C':
            Scene_createTool(scene, TOOL_CHECKPOINT, &position, &aabb);
            break;
            // Bonus
        case '?':
            Scene_createBlock(scene, BLOCK_BONUS, &position, &aabb);
            break;
            // Luciole
        case 'o':
            Scene_createCollectable(scene, COLLECTABLE_FIREFLY, &position);
            break;
        case 'H':
            Scene_createCollectable(scene, COLLECTABLE_HEART, &position);
            break;
            // Noisette
        case 'e':
            Scene_createEnemy(scene, ENEMY_NUT, &position);
            break;
        }

        if (x == 0)
        {
            ++y;
            x = lvlWidth;
        }
        else {
            --x;
        }

    }

    // Création d'un Hazard en dessous du niveau pour tuer le joueur quand il tombe
    PE_Vec2_set(&position, 0.f, 0.f);
    PE_AABB_set(&aabb, -2.f, -4.f, (float)lvlWidth + 2.f, -2.f);

    Tool *tool = Scene_createTool(scene, TOOL_HAZARD, &position, &aabb);
    if (!tool) goto ERROR_LABEL;

    // Création d'une limite à gauche du monde
    PE_Vec2_set(&position, 0.f, 0.f);
    PE_AABB_set(&aabb, -2.f, -4.f, 0.f, (float)lvlHeight + 5.f);

    tool = Scene_createTool(scene, TOOL_BOUND, &position, &aabb);
    if (!tool) goto ERROR_LABEL;

    // Création d'une limite à droite du monde
    PE_Vec2_set(&position, (float)lvlWidth, 0.f);
    PE_AABB_set(&aabb, 0.f, -4.f, 2.f, (float)lvlHeight + 5.f);

    tool = Scene_createTool(scene, TOOL_BOUND, &position, &aabb);
    if (!tool) goto ERROR_LABEL;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Scene_parseLevelBuffer()\n");
    return EXIT_FAILURE;
}

//-------------------------------------------------------------------------------------------------

Scene *Scene_new(RE_Renderer *renderer, FILE *levelFile, RE_Timer *time, float timeStep)
{
    Scene *scene = NULL;
    int lvlWidth, lvlHeight;
    char *levelBuffer = NULL;
    long size;
    int exitStatus;
    PE_Vec2 gravity;
    PE_AABB worldView;
    PE_AABB worldAABB;

    // Lecture du fichier de niveau
    levelBuffer = Scene_getLevelBuffer(levelFile, &size);
    if (!levelBuffer) goto ERROR_LABEL;

    exitStatus = Scene_getLevelDim(levelBuffer, size, &lvlWidth, &lvlHeight);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    // Allocation de la scène
    scene = (Scene *)calloc(1, sizeof(Scene));
    if (!scene) goto ERROR_LABEL;

    scene->m_timeStep = timeStep;
    scene->m_renderer = renderer;
    scene->m_time = time;
    scene->m_respawn = FALSE;

    scene->m_textures = GameTextures_new(renderer);
    if (!scene->m_textures) goto ERROR_LABEL;

    scene->m_animators = GameAnimators_new(scene);
    if (!scene->m_animators) goto ERROR_LABEL;

    PE_Vec2_set(&gravity, 0.f, -40.f);
    scene->m_world = PE_World_new(gravity, timeStep);
    if (!scene->m_world) goto ERROR_LABEL;

    scene->m_input = GameInput_new();
    if (!scene->m_input) goto ERROR_LABEL;

    scene->m_gizmos = Gizmos_new(scene);
    if (!scene->m_gizmos) goto ERROR_LABEL;

    scene->m_tilemap = Tilemap_new(scene, lvlWidth, lvlHeight);
    if (!scene->m_tilemap) goto ERROR_LABEL;

    scene->m_parallax = Parallax_new(scene);
    if (!scene->m_parallax) goto ERROR_LABEL;

    scene->m_manager = ObjectManager_new(scene);
    if (!scene->m_manager) goto ERROR_LABEL;

    // Initialisation de la scène
    exitStatus = Scene_parseLevelBuffer(scene, levelBuffer, size, lvlWidth, lvlHeight);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;
    free(levelBuffer);
    levelBuffer = NULL;

    exitStatus = Tilemap_initCollider(scene->m_tilemap);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    Tilemap_initTiles(scene->m_tilemap);
    scene->m_player = Scene_createPlayer(scene, &scene->m_startPos);
    if (!scene->m_player) goto ERROR_LABEL;

    PE_AABB_set(&worldView, 0.f, 0.f, 24.f * (81.f / 80.f), 13.5f * (81.f / 80.f));
    PE_AABB_set(&worldAABB, 0.f, 0.f, (float)lvlWidth, (float)lvlHeight);
    scene->m_camera = Scene_createCamera(scene, &worldView, &worldAABB);
    if (!scene->m_camera) goto ERROR_LABEL;

    // Pour initialiser la caméra
    exitStatus = ObjectManager_manageObjects(scene->m_manager);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    exitStatus = PE_World_fixedUpdate(scene->m_world);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return scene;

ERROR_LABEL:
    printf("ERROR - Scene_new()\n");
    free(levelBuffer);
    Scene_free(scene);
    return NULL;
}

void Scene_free(Scene *scene)
{
    if (!scene)
        return;

    ObjectManager_free(scene->m_manager);

    GameInput_free(scene->m_input);
    GameTextures_free(scene->m_textures);
    GameAnimators_free(scene->m_animators);
    Tilemap_free(scene->m_tilemap);
    Gizmos_free(scene->m_gizmos);
    Parallax_free(scene->m_parallax);

    // Destruction du moteur physique
    PE_World_free(scene->m_world);

    free(scene);
}

int Scene_update(Scene *scene)
{
    GameInput *input = scene->m_input;
    RE_Timer *time = scene->m_time;
    int exitStatus = EXIT_SUCCESS;

    GameInput_update(input);

    exitStatus = ObjectManager_manageObjects(scene->m_manager);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    if (scene->m_respawn)
    {
        exitStatus = ObjectManager_respawn(scene->m_manager);
        if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

        exitStatus = ObjectManager_manageObjects(scene->m_manager);
        if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

        scene->m_respawn = FALSE;
    }

    scene->m_accu += RE_Timer_getDelta(time);
    while (scene->m_accu >= scene->m_timeStep)
    {
        ObjectManager_fixedUpdate(scene->m_manager);
        PE_World_fixedUpdate(scene->m_world);
        scene->m_accu -= scene->m_timeStep;
    }
    scene->m_alpha = scene->m_accu / scene->m_timeStep;

    GameAnimators_update(scene->m_animators);
    ObjectManager_update(scene->m_manager);

    if (input->toggleGizmos)
        scene->m_showGizmos = !scene->m_showGizmos;

    if (input->quitPressed)
        return -1;
    else if (input->exitPressed || scene->m_gameOver || scene->m_finish)
        return 1;
    else
        return 0;

ERROR_LABEL:
    printf("ERROR - Scene_update()\n");
    return -1;
}

void Scene_render(Scene *scene)
{
    Parallax_renderBackground(scene->m_parallax);
    Tilemap_render(scene->m_tilemap);

    Camera *camera = scene->m_camera;
    GameObject **objects = Camera_getVisibleObjects(camera);
    int nbObjects = Camera_getNbVisibleObjects(camera);
    for (int i = 0; i < nbObjects; i++)
    {
        GameObject_render(objects[i]);
    }
    Parallax_renderForeground(scene->m_parallax);

    GameUI_render(scene, Player_getStats(scene->m_player));

    if (scene->m_showGizmos)
        Gizmos_render(scene->m_gizmos);
}

Block *Scene_createBlock(Scene *scene, int type, PE_Vec2 *position, PE_AABB *aabb)
{    
    Block *block = NULL;
    GameObject *object = NULL;

    block = Block_new(scene, type, position, aabb);
    if (!block) goto ERROR_LABEL;

    object = Block_getObject(block);
    int exitStatus = ObjectManager_addObject(scene->m_manager, object);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    exitStatus = Scene_startOject(scene, object);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return block;

ERROR_LABEL:
    printf("ERROR - Scene_createBlock()\n");
    GameObject_free(object);
    return NULL;
}

Tool *Scene_createTool(Scene *scene, int type, PE_Vec2 *position, PE_AABB *aabb)
{
    Tool *tool = NULL;
    GameObject *object = NULL;

    tool = Tool_new(scene, type, position, aabb);
    if (!tool) goto ERROR_LABEL;

    object = Tool_getObject(tool);
    int exitStatus = ObjectManager_addObject(scene->m_manager, object);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    exitStatus = Scene_startOject(scene, object);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return tool;

ERROR_LABEL:
    printf("ERROR - Scene_createTool()\n");
    GameObject_free(object);
    return NULL;
}

Player *Scene_createPlayer(Scene *scene, PE_Vec2 *position)
{
    Player *player = NULL;
    GameObject *object = NULL;

    player = Player_new(scene, position);
    if (!player) goto ERROR_LABEL;

    object = Player_getObject(player);
    int exitStatus = ObjectManager_addObject(scene->m_manager, object);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    exitStatus = Scene_startOject(scene, object);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return player;

ERROR_LABEL:
    printf("ERROR - Scene_createPlayer()\n");
    GameObject_free(object);
    return NULL;
}

Camera *Scene_createCamera(Scene *scene, PE_AABB *worldView, PE_AABB *worldAABB)
{
    Camera *camera = NULL;
    GameObject *object = NULL;

    camera = Camera_new(scene, worldView, worldAABB);
    if (!camera) goto ERROR_LABEL;

    object = Camera_getObject(camera);
    int exitStatus = ObjectManager_addObject(scene->m_manager, object);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    exitStatus = Scene_startOject(scene, object);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return camera;

ERROR_LABEL:
    printf("ERROR - Scene_createCamera()\n");
    GameObject_free(object);
    return NULL;
}

Collectable *Scene_createCollectable(Scene *scene, int type, PE_Vec2 *position)
{
    Collectable *collectable = NULL;
    GameObject *object = NULL;

    collectable = Collectable_new(scene, type, position);
    if (!collectable) goto ERROR_LABEL;

    object = Collectable_getObject(collectable);
    int exitStatus = ObjectManager_addObject(scene->m_manager, object);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    exitStatus = Scene_startOject(scene, object);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return collectable;

ERROR_LABEL:
    printf("ERROR - Scene_createCollectable()\n");
    GameObject_free(object);
    return NULL;
}

Enemy *Scene_createEnemy(Scene *scene, int type, PE_Vec2 *position)
{
    Enemy *enemy = NULL;
    GameObject *object = NULL;

    enemy = Enemy_new(scene, type, position);
    if (!enemy) goto ERROR_LABEL;

    object = Enemy_getObject(enemy);
    int exitStatus = ObjectManager_addObject(scene->m_manager, object);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    exitStatus = Scene_startOject(scene, object);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return enemy;

ERROR_LABEL:
    printf("ERROR - Scene_createEnemy()\n");
    GameObject_free(object);
    return NULL;
}
