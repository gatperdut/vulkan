#pragma once


#include "InstanceHandler.h"
#include "WindowHandler.h"
#include "DevicesHandler.h"
#include "QueuesHandler.h"
#include "CapabilitiesHandler.h"
#include "SwapchainHandler.h"
#include "RenderPassHandler.h"
#include "CommandsHandler.h"
#include "DescriptorsHandler.h"
#include "UniformsHandler.h"
#include "TexturesHandler.h"
#include "PipelinesHandler.h"
#include "ShadersHandler.h"


extern InstanceHandler* instanceHandler;

extern WindowHandler* windowHandler;

extern DevicesHandler* devicesHandler;

extern QueuesHandler* queuesHandler;

extern CapabilitiesHandler* capabilitiesHandler;

extern SwapchainHandler* swapchainHandler;

extern RenderPassHandler* renderPassHandler;

extern CommandsHandler* commandsHandler;

extern DescriptorsHandler* descriptorsHandler;

extern UniformsHandler* uniformsHandler;

extern TexturesHandler* texturesHandler;

extern PipelinesHandler* pipelinesHandler;

extern ShadersHandler* shadersHandler;
