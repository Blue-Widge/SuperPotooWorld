#include "Tool.h"
#include "Tool_Finish.h"

#include "../../Scene.h"

#include <stdio.h>
#include <stdlib.h>

int Finish_onStart(Tool *tool)
{
    // TODO
    // Initialisez la zone de fin en cr�ant un corps
    // Il faut ensuite d�finir un collider de type trigger pour d�tecter le joueur
    // L'activation de la fin du niveau se fait par un callback
    // Utilisez ensuite la fonction Scene_finish()

    printf("Finish_onStart : Fin du niveau\n");

    return EXIT_SUCCESS;
}
