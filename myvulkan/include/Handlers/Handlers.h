#pragma once


#include "InstanceHandler.h"
#include "windowHandler.h"
#include "DevicesHandler.h"
#include "QueuesHandler.h"
#include "CapabilitiesHandler.h"
#include "Presentation/Presentation.h"
#include "RenderPassHandler.h"
#include "CommandsHandler.h"
#include "DescriptorsHandler.h"
#include "ShadersHandler.h"
#include "ModelsHandler.h"
#include "LightsHandler.h"
#include "CommandBuffersHandler.h"
#include "SynchrosHandler.h"
#include "DrawHandler.h"
#include "InputHandler.h"
#include "CameraHandler.h"

extern InstanceHandler* instanceHandler;
extern WindowHandler* windowHandler;
extern DevicesHandler* devicesHandler;
extern QueuesHandler* queuesHandler;
extern CapabilitiesHandler* capabilitiesHandler;
extern Presentation* presentation;
extern RenderPassHandler* renderPassHandler;
extern CommandsHandler* commandsHandler;
extern DescriptorsHandler* descriptorsHandler;
extern ShadersHandler* shadersHandler;
extern ModelsHandler* modelsHandler;
extern LightsHandler* lightsHandler;
extern CommandBuffersHandler* commandBuffersHandler;
extern SynchrosHandler* synchrosHandler;
extern DrawHandler* drawHandler;
extern InputHandler* inputHandler;
extern CameraHandler* cameraHandler;
