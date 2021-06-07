#include "Block.h"
#include "Block_Bonus.h"

#include "../../Scene.h"

#include <stdio.h>
#include <stdlib.h>

int Bonus_onStart(Block *block)
{
    // TODO
    // Inspirez-vous de la fonction Ground_onStart()

    printf("Bonus_onStart : Initialisation d'un bonus\n");

    return EXIT_SUCCESS;
}

int Bonus_onRespawn(Block *block)
{
    // TODO
    // Réinitialisez le bonus.

    printf("Bonus_onRespawn : Réinitialisation d'un bloc bonus\n");

    return EXIT_SUCCESS;
}

void Bonus_render(Block *block)
{
    // TODO
    // Dessinez le bloc bonus.

    printf("Bonus_render : Affichage d'un bloc bonus\n");
}

void Bonus_hit(Block *block)
{
    // TODO
    // Cette fonction doit être appelée par le joueur
    // quand il entre en collision avec un bonus.
    // Modifiez le bonus en conséquence.
}
