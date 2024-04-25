#pragma once
#include "RenderCommandQueue.h"
#include "RendererAPI.h"
#include "Mesh.h"
#include "RenderPass.h"

namespace GEngine
{
	class ShaderLibrary;

	
	class Renderer
	{
	public:
		typedef void(*RenderCommandFn)(void*);


		static void Clear();
		static void Clear(float r, float g, float b, float a = 1.0f);
		static void SetClearColor(float r, float g, float b, float a);

		static void DrawIndexed(uint32_t count, PrimitiveType type, bool depthTest = true);
		static void SetLineThickness(float thickness);
		static void ClearMagenta();

		static void Init();

		static const Scope<ShaderLibrary>& GetShaderLibrary();

		template<typename FuncT>
		static void Submit(FuncT&& func)
		{
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();

				
				pFunc->~FuncT();
				};
			auto storageBuffer = GetRenderCommandQueue().Allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}		

		static void WaitAndRender();

		static void BeginRenderPass(const Ref<RenderPass>& renderPass, bool clear = true);
		static void EndRenderPass();

		static void SubmitQuad(const Ref<MaterialInstance>& material, const glm::mat4& transform = glm::mat4(1.0f));
		static void SubmitFullscreenQuad(const Ref<MaterialInstance>& material);
		static void SubmitMesh(const Ref<Mesh>& mesh, const glm::mat4& transform, const Ref<MaterialInstance>& overrideMaterial = nullptr);

		static void DrawBoundingBox(const BoundingBox& boundingBox, const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawBoundingBox(const Ref<Mesh>& mesh, const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));
		
	private:
		static RenderCommandQueue& GetRenderCommandQueue();
	};
}


