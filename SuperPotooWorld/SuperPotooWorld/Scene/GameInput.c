#include "GameInput.h"

#include <stdio.h>
#include <stdlib.h>

GameInput *GameInput_new()
{
    GameInput *input = NULL;

    input = (GameInput *)calloc(1, sizeof(GameInput));
    if (!input) goto ERROR_LABEL;

    return input;

ERROR_LABEL:
    printf("ERROR - GameInput_new()\n");
    GameInput_free(input);
    return NULL;
}

void GameInput_free(GameInput *input)
{
    if (!input)
        return;

    free(input);
}

void GameInput_update(GameInput *input)
{
    SDL_Event evt;
    SDL_Scancode scanCode;

    input->quitPressed = FALSE;
    input->exitPressed = FALSE;
    input->jumpPressed = FALSE;
    input->toggleGizmos = FALSE;
    input->shootPressed = FALSE;

    while (SDL_PollEvent(&evt))
    {
        switch (evt.type)
        {
        case SDL_QUIT:
            input->quitPressed = TRUE;
            break;

        case SDL_KEYDOWN:
            scanCode = evt.key.keysym.scancode;

            if (evt.key.repeat)
                break;

            // Touche pour quitter le jeu
            if (scanCode == SDL_SCANCODE_ESCAPE)
            {
                input->quitPressed = TRUE;
            }

            // Touche retour vers le menu
            if (scanCode == SDL_SCANCODE_BACKSPACE)
            {
                input->exitPressed = TRUE;
            }

            // Deplacement à droite
            if (scanCode == SDL_SCANCODE_RIGHT)
            {
                input->hAxis = 1.f;
            }

            // Deplacement à gauche
            if (scanCode == SDL_SCANCODE_LEFT)
            {
                input->hAxis = -1.f;
            }

            // Saut
            if ((scanCode == SDL_SCANCODE_SPACE) || (scanCode == SDL_SCANCODE_UP))
            {
                input->jumpDown = TRUE;
                input->jumpPressed = TRUE;
            }

            // Gizmos
            if (scanCode == SDL_SCANCODE_G)
            {
                input->toggleGizmos = TRUE;
            }

            if (scanCode == SDL_SCANCODE_F)
            {
                input->shootPressed = TRUE;
            }

            if (scanCode == SDL_SCANCODE_S)
            {
                input->skiddingPressed = TRUE;
            }
            break;
        case SDL_KEYUP:
            scanCode = evt.key.keysym.scancode;

            // Deplacement à droite
            if (scanCode == SDL_SCANCODE_RIGHT)
            {
                if (input->hAxis > 0.f)
                    input->hAxis = 0.f;
            }

            // Deplacement à gauche
            if (scanCode == SDL_SCANCODE_LEFT)
            {
                if (input->hAxis < 0.f)
                    input->hAxis = 0.f;
            }

            // Saut
            if ((scanCode == SDL_SCANCODE_SPACE) || (scanCode == SDL_SCANCODE_UP))
            {
                input->jumpDown = FALSE;
            }

            if (scanCode == SDL_SCANCODE_F)
            {
                input->shootPressed = FALSE;
            }

            if (scanCode == SDL_SCANCODE_S)
            {
                input->skiddingPressed = FALSE;
            }
            break;
        default:
            break;
        }
    }
}
