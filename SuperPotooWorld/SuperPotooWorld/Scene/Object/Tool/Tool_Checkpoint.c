#include "Tool.h"
#include "Tool_Checkpoint.h"

#include "../../Scene.h"

#include <stdio.h>
#include <stdlib.h>

int Checkpoint_onStart(Tool *tool)
{
    // TODO
    // Initialisez le checkpoint en créant un corps
    // Il faut ensuite définir un collider de type trigger pour détecter le joueur
    // L'activation du checkpoint se fait par un callback

    printf("Checkpoint_onStart : Initialisation du checkpoint\n");

    return EXIT_SUCCESS;
}

void Checkpoint_render(Tool *tool)
{
    // TODO
    // Dessinez le checkpoint

    printf("Checkpoint_render : Affichage du checkpoint\n");
}
