#pragma once


#include "InstanceHandler.h"
#include "windowHandler.h"
#include "DevicesHandler.h"
#include "QueuesHandler.h"
#include "CapabilitiesHandler.h"
#include "SwapchainHandler.h"
#include "RenderPassHandler.h"
#include "CommandsHandler.h"
#include "DescriptorsHandler.h"
#include "ShadersHandler.h"
#include "FramebuffersHandler.h"
#include "ModelsHandler.h"
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
extern SwapchainHandler* swapchainHandler;
extern RenderPassHandler* renderPassHandler;
extern CommandsHandler* commandsHandler;
extern DescriptorsHandler* descriptorsHandler;
extern ShadersHandler* shadersHandler;
extern FramebuffersHandler* framebuffersHandler;
extern ModelsHandler* modelsHandler;
extern CommandBuffersHandler* commandBuffersHandler;
extern SynchrosHandler* synchrosHandler;
extern DrawHandler* drawHandler;
extern InputHandler* inputHandler;
extern CameraHandler* cameraHandler;
