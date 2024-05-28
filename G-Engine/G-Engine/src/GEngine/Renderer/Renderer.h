#pragma once

#include "RenderCommandQueue.h"
#include "RenderPass.h"

#include "Mesh.h"

namespace GEngine {

	class ShaderLibrary;

	class Renderer
	{
	public:
		typedef void(*RenderCommandFn)(void*);

		static void Clear();
		static void Clear(float r, float g, float b, float a = 1.0f);
		static void SetClearColor(float r, float g, float b, float a);
		static void ClearMagenta();

		static void DrawIndexed(uint32_t count, PrimitiveType type, bool depthTest = true);

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

		static void SubmitFullscreenQuad(const Ref<MaterialInstance>& material);
		static void SubmitMesh(const Ref<Mesh>& mesh, const glm::mat4& transform, const Ref<MaterialInstance>& overrideMaterial = nullptr);
	private:
		static RenderCommandQueue& GetRenderCommandQueue();
	};

}
