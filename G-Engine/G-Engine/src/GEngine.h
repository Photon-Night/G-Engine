//
// Note:	this file is to be included in client applications ONLY
//			NEVER include this file anywhere in the engine codebase
//
#pragma once

#include "GEngine/Core/Application.h"
#include "GEngine/Core/Log.h"
#include "GEngine/Core/Input.h"
#include "GEngine/Core/Timestep.h"

#include "GEngine/Core/Events/Event.h"
#include "GEngine/Core/Events/ApplicationEvent.h"
#include "GEngine/Core/Events/KeyEvent.h"
#include "GEngine/Core/Events/MouseEvent.h"

#include "GEngine/Core/Math/AABB.h"
#include "GEngine/Core/Math/Ray.h"

#include "imgui/imgui.h"

// --- GEngine Render API ------------------------------
#include "GEngine/Renderer/Renderer.h"
#include "GEngine/Renderer/SceneRenderer.h"
#include "GEngine/Renderer/RenderPass.h"
#include "GEngine/Renderer/Framebuffer.h"
#include "GEngine/Renderer/Buffer.h"
#include "GEngine/Renderer/VertexArray.h"
#include "GEngine/Renderer/Texture.h"
#include "GEngine/Renderer/Shader.h"
#include "GEngine/Renderer/Mesh.h"
#include "GEngine/Renderer/Camera.h"
#include "GEngine/Renderer/Material.h"
// ---------------------------------------------------

// Scenes
#include "GEngine/Scene/Entity.h"
#include "GEngine/Scene/Scene.h"