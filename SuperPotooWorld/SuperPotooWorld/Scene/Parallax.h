#ifndef _PARALLAX_H_
#define _PARALLAX_H_

#include "../Settings.h"

typedef struct Scene_s Scene;

typedef struct Parallax_s
{
    Scene *m_scene;
    int m_width;
    int m_height;
} Parallax;

Parallax *Parallax_new(Scene *scene);
void Parallax_free(Parallax *parallax);

void Parallax_renderBackground(Parallax *parallax);
void Parallax_renderForeground(Parallax* parallax);

#endif