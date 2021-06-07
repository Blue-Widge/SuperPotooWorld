#ifndef _TOOL_H_
#define _TOOL_H_

#include "../../../Settings.h"
#include "../GameObject.h"

typedef struct Scene_s Scene;

typedef enum ToolType_e
{
    TOOL_CHECKPOINT,
    TOOL_HAZARD,
    TOOL_BOUND,
    TOOL_FINISH
} ToolType;

typedef struct Tool_s
{
    GameObject *m_object;
    int m_type;
    int m_state;

    PE_Vec2 m_startPos;
    PE_AABB m_startAABB;
} Tool;

Tool *Tool_new(Scene *scene, int type, PE_Vec2 *position, PE_AABB *aabb);

INLINE GameObject *Tool_getObject(Tool *tool)
{
    return tool->m_object;
}

#endif