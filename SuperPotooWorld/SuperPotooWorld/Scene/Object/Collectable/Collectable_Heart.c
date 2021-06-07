#include "Collectable.h"
#include "Collectable_Heart.h"

#include "../../Scene.h"

#include <stdio.h>
#include <stdlib.h>

int Heart_onStart(Collectable *collectable)
{
    Scene *scene = GameObject_getScene(collectable->m_object);
    PE_Vec2 *position = &collectable->m_startPos;

    // TODO
    // Cette fonction est appelée lorsque le coeur est ajoutée dans le jeu
    // (et donc dans le moteur physique)
    // Inspirez vous de la fonction Player_onStart()

    printf("Heart_onStart : Initialisation d'un coeur\n");

    return EXIT_SUCCESS;
}

int Heart_onRespawn(Collectable *collectable)
{
    // TODO
    // Supprimez le coeur si le joueur meurt
    // (et ne supprimez pas seulement son corps physique)

    printf("Heart_onRespawn : Réinitialisation d'un coeur\n");

    return EXIT_SUCCESS;
}

void Heart_render(Collectable *collectable)
{
    // TODO
    // Dessinez le coeur.

    printf("Heart_render : Affichage d'un coeur\n");
}
