#ifndef _UITEXTURES_H_
#define _UITEXTURES_H_

#include "../Settings.h"

typedef struct UITextures_s
{
    RE_Texture* Lives;
    RE_Texture* Hearts;
    RE_Texture* digits;
    RE_Texture* Fireflies;
    RE_Texture* LivesFire;
    RE_Texture* LivesScream;

} UITextures;

UITextures* UITextures_new(RE_Renderer* renderer);
void UITextures_free(UITextures* textures);

#endif
