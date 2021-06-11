#include "MenuInput.h"

#include <stdio.h>
#include <stdlib.h>

MenuInput *MenuInput_new()
{
    MenuInput *input = NULL;

    input = (MenuInput *)calloc(1, sizeof(MenuInput));
    if (!input) goto ERROR_LABEL;

    return input;

ERROR_LABEL:
    printf("ERROR - MenuInput_new()\n");
    MenuInput_free(input);
    return NULL;
}

void MenuInput_free(MenuInput *input)
{
    if (!input)
        return;

    free(input);
}

void MenuInput_update(MenuInput *input)
{
    SDL_Event evt;
    SDL_Scancode scanCode;

    input->quitPressed = FALSE;
    input->rightClick = FALSE;
    input->buttonPressed = FALSE;

    while (SDL_PollEvent(&evt))
    {
        switch (evt.type)
        {
        case SDL_QUIT:
            input->quitPressed = TRUE;
            break;

        case SDL_MOUSEMOTION:
            input->mousePos.x = (float)evt.motion.x;
            input->mousePos.y = (float)evt.motion.y;
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (evt.button.button == SDL_BUTTON_LEFT)
            {
                input->rightClick = TRUE;
                input->buttonPressed = TRUE;
            }
            break;

        case SDL_KEYDOWN:
            scanCode = evt.key.keysym.scancode;
            input->buttonPressed = TRUE;
            if (evt.key.repeat)
                break;

            // Touche pour quiter le jeu
            if (scanCode == SDL_SCANCODE_ESCAPE)
            {
                input->quitPressed = TRUE;
            }
            break;

        case SDL_KEYUP:
            scanCode = evt.key.keysym.scancode;
            break;

        default:
            break;
        }
    }
}
