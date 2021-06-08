#include "Enemy.h"
#include "Enemy_Nut.h"

#include "../../Scene.h"

#include <stdio.h>
#include <stdlib.h>

int Nut_init(Enemy *enemy)
{
    Scene *scene = GameObject_getScene(enemy->m_object);
    RE_Animator *animator = enemy->m_animator;
    GameTextures* textures = Scene_getTextures(scene);

    RE_TextureAnim* texAnim = RE_Animator_createTextureAnim(animator, textures->hazelnut, "Hazelnut");

    RE_TextureAnim_setCycleTime(texAnim, 0.2f);
    RE_Animator_playTextureAnim(animator, "Hazelnut");

    // TODO
    // Cette fonction est appelée lors de la création d'une noisette
    // Vous devez créer les animations de l'animateur d'une noisette

    return EXIT_SUCCESS;
}

int Nut_onStart(Enemy *enemy)
{
    Scene *scene = GameObject_getScene(enemy->m_object);
    PE_Vec2 *position = &enemy->m_startPos;

    // TODO
    // Cette fonction est appelée lorsque la noisette est ajoutée dans le jeu
    // (et donc dans le moteur physique)
    // Inspirez vous de la fonction Player_onStart()

    printf("Nut_onStart : Initialisation d'une noisette\n");

    return EXIT_SUCCESS;
}

int Nut_onRespawn(Enemy *enemy)
{
    // On supprime la noisette du moteur physique si elle y est toujours
    GameObject_removeBody(enemy->m_object);

    // On appelle Nut_onStart() pour recréer son corps dans le moteur physique avec les valeurs initiales
    int exitStatus = Nut_onStart(enemy);
    return exitStatus;
}

int Nut_fixedUpdate(Enemy *enemy)
{
    // TODO
    // Utilisez cette fonction pour modifier la physique d'une noisette.
    // Vous pouvez la mettre en mouvement quand elle est dans l'état NUT_STARTING.
    // Cet état signifie qu'elle a vu le joueur mais qu'elle ne tourne pas encore.

    printf("Nut_fixedUpdate : Mise à jour de la physique d'une noisette\n");

    return EXIT_SUCCESS;
}

void Nut_render(Enemy *enemy)
{
    // TODO
    // Dessinez la noisette.

    printf("Nut_render : Affichage d'une noisette\n");
}

void Nut_damage(Enemy *enemy)
{
    // TODO
    // Faire disparaître la noisette en cas de dommage.
    // Cette fonction peut être appelée par le joueur quand il entre en collision
    // avec un noisette et qu'il est situé au-dessus.
}
