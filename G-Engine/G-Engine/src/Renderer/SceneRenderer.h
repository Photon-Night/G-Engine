#pragma once

namespace GEngine
{
	struct SceneRendererOptions
	{
		bool showGrid = true;
		bool ShowBoundingBox = false;
	};

	class SceneRenderer
	{
	public:
		static void Init();
		static void SetViewportSize(uint32_t width, uint32_t height);
		static void BeginScene(const Scene* scene);
		static void EndScene();

		static void SubmitEntity
	};
}


