#ifndef _GIZMOS_H_
#define _GIZMOS_H_

#include "../Settings.h"

typedef struct Scene_s Scene;

typedef struct Gizmos_s
{
    Scene *m_scene;

    struct GizmosColor {
        RE_Color staticBody;
        RE_Color dynamicBody;
        RE_Color velocity;
        RE_Color position;
        RE_Color sleeping;
    } m_color;
} Gizmos;

Gizmos *Gizmos_new(Scene *scene);
void Gizmos_free(Gizmos *gizmos);
void Gizmos_render(Gizmos *gizmos);

#endif
