#ifndef _TOOL_CHECKPOINT_H_
#define _TOOL_CHECKPOINT_H_

#include "../../../Settings.h"

typedef struct Tool_s Tool;

typedef enum CheckpointState_e
{
    CHECKPOINT_OFF,
    CHECKPOINT_ON,
} CheckpointState;

int Checkpoint_onStart(Tool *tool);
void Checkpoint_render(Tool *tool);

#endif