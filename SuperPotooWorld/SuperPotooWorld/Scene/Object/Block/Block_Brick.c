#include "Block.h"
#include "Block_Brick.h"

#include "../../Scene.h"

#include <stdio.h>
#include <stdlib.h>

int Brick_onStart(Block *block)
{
    // TODO
    // Inspirez-vous de la fonction Ground_onStart()

    printf("Brick_onStart : Initialisation d'un bloc cassable\n");

    return EXIT_SUCCESS;
}

int Brick_onRespawn(Block *block)
{
    // On supprime le bloc du moteur physique si il y est toujours
    GameObject_removeBody(block->m_object);

    // On appelle Brick_onStart() pour recréer son corps dans le moteur physique avec les valeurs initiales
    int exitStatus = Brick_onStart(block);
    return exitStatus;
}

void Brick_render(Block *block)
{
    // TODO
    // Dessinez le bloc cassable.

    printf("Brick_render : Affichage d'un bloc cassable\n");
}

void Brick_hit(Block *block)
{
    // TODO
    // Cette fonction doit être appelée par le joueur
    // quand il entre en collision avec un bloc cassable.
    // Modifiez le bonus en conséquence.
}
