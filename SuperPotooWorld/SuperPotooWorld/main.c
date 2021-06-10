#include "Settings.h"
#include "Menu/Menu.h"
#include "Scene/Scene.h"

#include <stdio.h>
#include <stdlib.h>

//#define FULLSCREEN
//#define NO_MENU
#define NO_MENU_PATH "../Assets/Levels/Level_01.txt"

#ifdef FULLSCREEN
#  define WINDOW_WIDTH  1920
#  define WINDOW_HEIGHT 1080
#else
#  define WINDOW_WIDTH  1280
#  define WINDOW_HEIGHT 720
#endif

#define RENDERER_WIDTH  1920
#define RENDERER_HEIGHT 1080

int main(int argc, char *argv[])
{
    RE_Window *window = NULL;
    RE_Renderer *renderer = NULL;
    RE_Timer *timer = NULL;
    Menu *menu = NULL;
    Scene *scene = NULL;
    FILE *levelFile = NULL;
    int exitStatus;
    float timeStep = 1.f / 50.f;
    Bool Loading = TRUE;
    exitStatus = RE_init();
    if (exitStatus == EXIT_FAILURE) goto ERROR_LABEL;

#ifdef FULLSCREEN
    window = RE_Window_new(u8"Super Potoo World", WINDOW_WIDTH, WINDOW_HEIGHT, RE_WINDOW_FULLSCREEN);
#else
    window = RE_Window_new(u8"Super Potoo World", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
#endif
    if (!window) goto ERROR_LABEL;

    renderer = RE_Window_getRenderer(window);

    RE_Renderer_setLogicalSize(renderer, RENDERER_WIDTH, RENDERER_HEIGHT);

    timer = RE_Timer_new();
    if (!timer) goto ERROR_LABEL;

    RE_Timer_start(timer);
    Bool quitGame = FALSE;
    while (!quitGame)
    {
        Bool quitLoop = FALSE;
        char *levelPath = NULL;
        float delay, accu;

    #ifdef NO_MENU
        // On evite l'écran du menu

        levelPath = NO_MENU_PATH;
        menu = NULL;
    #else
        // Chargement du menu

        menu = Menu_new(renderer, timer);
        if (!menu) goto ERROR_LABEL;
        RE_Timer* chrono = RE_Timer_new();
        RE_Timer_start(chrono);
        while (!quitLoop)
        {
            exitStatus = Menu_updateLoading(menu);
            RE_Timer_update(chrono);
            if (RE_Timer_getElapsed(chrono) > 2)
                Loading = FALSE;
            switch (exitStatus)
            {
            case -1:
                quitLoop = TRUE;
                quitGame = TRUE;
            case 1:
                if(!Loading)
                    quitLoop = TRUE;
                break;
            default:
                break;
            }
            if (quitLoop)
                break;
            RE_Renderer_clear(renderer);
            Menu_renderLoading(menu, Loading);
            RE_Renderer_update(renderer);
        }
        //Copie colle d'un assombrissement de pro, car c'est encore plus pro, mais avec un ECLAIRCISSEMENT de pro, et ouais
        RE_Timer_update(timer);
        delay = 0.5f;
        accu = 0.f;
        while (accu < delay)
        {
            RE_Renderer_clear(renderer);
            Menu_renderLoading(menu, Loading);
            RE_Renderer_fill(renderer, RE_Color_set(0, 0, 0, (int)(255 * accu / delay)));
            RE_Renderer_update(renderer);

            RE_Timer_update(timer);
            accu += RE_Timer_getDelta(timer);
        }
        RE_Renderer_clear(renderer);
        RE_Renderer_update(renderer);
        delay = 0.5f;
        accu = 0.f;
        while (accu < delay)
        {
            RE_Renderer_clear(renderer);
            Menu_render(menu);
            RE_Renderer_fill(renderer, RE_Color_set(0, 0, 0, (int)(255 - (255 * accu / delay))));
            RE_Renderer_update(renderer);

            RE_Timer_update(timer);
            accu += RE_Timer_getDelta(timer);
        }
        RE_Renderer_clear(renderer);
        RE_Renderer_update(renderer);
        //------------------------------------------------------------------------------------------
        // Boucle de rendu du menu
        quitLoop = FALSE;
        while (!quitLoop)
        {
            RE_Timer_update(timer);
            exitStatus = Menu_update(menu);

            switch (exitStatus)
            {
            case 1:
                quitLoop = TRUE;
                break;
            case -1:
                quitLoop = TRUE;
                quitGame = TRUE;
                break;
            default:
                break;
            }

            if (quitLoop)
                break;

            RE_Renderer_clear(renderer);
            Menu_render(menu);
            RE_Renderer_update(renderer);
        }

        if (quitGame)
            break;

        // Attention, il va faire tout noir
        // Assombrissement de l'écran, pour faire pro

        RE_Timer_update(timer);
        delay = 0.5f;
        accu = 0.f;
        while (accu < delay)
        {
            RE_Renderer_clear(renderer);
            Menu_render(menu);
            RE_Renderer_fill(renderer, RE_Color_set(0, 0, 0, (int)(255 * accu / delay)));
            RE_Renderer_update(renderer);

            RE_Timer_update(timer);
            accu += RE_Timer_getDelta(timer);
        }
        RE_Renderer_clear(renderer);
        RE_Renderer_update(renderer);

        // Libération du menu

        levelPath = Menu_getLevelPath(menu);
        if (!levelPath) goto ERROR_LABEL;

        Menu_free(menu);
        menu = NULL;
    #endif

        // Chargement du la scène

        levelFile = fopen(levelPath, "rb");
        if (!levelFile)
        {
            printf("ERROR - Can't open the level file\n");
            goto ERROR_LABEL;
        }

        scene = Scene_new(renderer, levelFile, timer, timeStep);
        if (!scene) goto ERROR_LABEL;

        fclose(levelFile);
        levelFile = NULL;

        //------------------------------------------------------------------------------------------
        // Boucle de rendu de la scène

        quitLoop = FALSE;
        while (!quitLoop)
        {
            RE_Timer_update(timer);
            exitStatus = Scene_update(scene);

            switch (exitStatus)
            {
            case 1:
                quitLoop = TRUE;
                break;
            case -1:
                quitLoop = TRUE;
                quitGame = TRUE;
                break;
            default:
                break;
            }

            if (quitLoop)
                break;

            RE_Renderer_clear(renderer);
            Scene_render(scene);
            RE_Renderer_update(renderer);
        }
        Scene_free(scene);
        scene = NULL;
    }

    // Libération de la mémoire
    Menu_free(menu);
    menu = NULL;
    Scene_free(scene);
    scene = NULL;

    RE_Window_free(window);
    window = NULL;
    RE_Timer_free(timer);
    timer = NULL;

    RE_quit();

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - main()\n");
    Menu_free(menu);
    Scene_free(scene);
    RE_Window_free(window);
    RE_Timer_free(timer);
    RE_quit();
    return EXIT_FAILURE;
}
