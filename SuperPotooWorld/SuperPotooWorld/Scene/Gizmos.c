#include "Gizmos.h"
#include "Scene.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//-------------------------------------------------------------------------------------------------
//  Fonctions auxiliaires

void Gizmos_drawAABB(Gizmos *gizmos, PE_AABB *aabb, RE_Color color)
{
    Rect rect;
    float lowerX, lowerY, upperX, upperY;
    Camera *camera = Scene_getCamera(gizmos->m_scene);
    RE_Renderer *renderer = Scene_getRenderer(gizmos->m_scene);

    Camera_worldToView(camera, &aabb->lower, &lowerX, &lowerY);
    Camera_worldToView(camera, &aabb->upper, &upperX, &upperY);

    rect.x = lowerX;
    rect.y = lowerY;
    rect.w = upperX - lowerX;
    rect.h = upperY - lowerY;

    RE_Renderer_drawRect(renderer, &rect, color);
}

void Gizmos_fillAABB(Gizmos *gizmos, PE_AABB *aabb, RE_Color color)
{
    Rect rect;
    float lowerX, lowerY, upperX, upperY;
    Camera *camera = Scene_getCamera(gizmos->m_scene);
    RE_Renderer *renderer = Scene_getRenderer(gizmos->m_scene);

    Camera_worldToView(camera, &aabb->lower, &lowerX, &lowerY);
    Camera_worldToView(camera, &aabb->upper, &upperX, &upperY);

    rect.x = lowerX;
    rect.y = lowerY;
    rect.w = upperX - lowerX;
    rect.h = upperY - lowerY;

    RE_Renderer_fillRect(renderer, &rect, color);
}

void Gizmos_renderBody(Gizmos *gizmos, PE_Body *body)
{
    RE_Color color;
    Camera *camera = Scene_getCamera(gizmos->m_scene);
    RE_Renderer *renderer = Scene_getRenderer(gizmos->m_scene);
    PE_Vec2 position, velocity;
    float x, y, x0, y0, x1, y1;
    float alpha = Scene_getAlpha(gizmos->m_scene);

    PE_Body_getInterpolation(body, alpha, &position);
    PE_Body_getVelocity(body, &velocity);

    color = gizmos->m_color.position;
    Camera_worldToView(camera, &position, &x, &y);
    RE_Renderer_drawPoint(renderer, (int)x, (int)y, color);

    if (fabsf(velocity.x) + fabsf(velocity.y) > 0.1f)
    {
        position.y += 0.5f;
        Camera_worldToView(camera, &position, &x0, &y0);

        PE_Vec2_multiplyBy(&velocity, 0.25f);
        PE_Vec2_addTo(&velocity, &position);

        color = gizmos->m_color.velocity;
        Camera_worldToView(camera, &velocity, &x1, &y1);
        RE_Renderer_drawLine(renderer, (int)x0, (int)y0, (int)x1, (int)y1, color);
    }
}

void Gizmos_renderCollider(Gizmos *gizmos, PE_Collider *collider)
{
    PE_AABB aabb;
    PE_Vec2 position;
    RE_Color color;
    float alpha = Scene_getAlpha(gizmos->m_scene);

    PE_Body *body = PE_Collider_getBody(collider);
    PE_Shape *shape = PE_Collider_getShape(collider);

    PE_Body_getInterpolation(body, alpha, &position);
    PE_Shape_getAABB(shape, &position, &aabb);

    color = gizmos->m_color.dynamicBody;

    if (PE_Body_getType(body) == PE_STATIC_BODY)
    {
        color = gizmos->m_color.staticBody;
    }
    else if (PE_Body_isSleeping(body))
    {
        color = gizmos->m_color.sleeping;
    }

    if (!PE_Collider_isTrigger(collider))
    {
        color.a = 128;
        Gizmos_fillAABB(gizmos, &aabb, color);
    }
    color.a = 255;
    Gizmos_drawAABB(gizmos, &aabb, color);
}

//-------------------------------------------------------------------------------------------------
//  Fonctions principales

Gizmos *Gizmos_new(Scene *scene)
{
    Gizmos *gizmos = NULL;

    gizmos = (Gizmos *)calloc(1, sizeof(Gizmos));
    if (!gizmos) goto ERROR_LABEL;

    gizmos->m_scene = scene;
    gizmos->m_color.position = RE_Color_set(255, 255, 255, 255);
    gizmos->m_color.velocity = RE_Color_set(255, 255, 255, 255);
    gizmos->m_color.sleeping = RE_Color_set(40, 100, 255, 255);
    gizmos->m_color.staticBody = RE_Color_set(255, 150, 0, 255);
    gizmos->m_color.dynamicBody = RE_Color_set(255, 64, 64, 255);

    return gizmos;

ERROR_LABEL:
    printf("ERROR - Gizmos_new()\n");
    return NULL;
}

void Gizmos_free(Gizmos *gizmos)
{
    if (!gizmos)
        return;

    free(gizmos);
}

void Gizmos_render(Gizmos *gizmos)
{
    PE_World *world = Scene_getWorld(gizmos->m_scene);

    PE_BodyIterator bodyIt;
    PE_World_getBodyIterator(world, &bodyIt);
    while (PE_BodyIterator_moveNext(&bodyIt))
    {
        PE_Body *body = PE_BodyIterator_current(&bodyIt);

        PE_ColliderIterator colliderIt;
        PE_Body_getColliderIterator(body, &colliderIt);
        while (PE_ColliderIterator_moveNext(&colliderIt))
        {
            PE_Collider *collider = PE_ColliderIterator_current(&colliderIt);
            Gizmos_renderCollider(gizmos, collider);
        }

        Gizmos_renderBody(gizmos, body);
    }
}

