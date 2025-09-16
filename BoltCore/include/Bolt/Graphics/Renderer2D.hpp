#pragma once
#include "Bolt/Graphics/Texture2D.hpp"
#include "Bolt/Graphics/RenderContext.hpp"
#include "Bolt/Graphics/RenderResources.hpp"
#include "Bolt/Graphics/QuadMesh.hpp"
#include "Bolt/Graphics/Shader.hpp"

#include "Bolt/Collections/Vec2.hpp"
#include "Bolt/Collections/Color.hpp"
#include "Bolt/Collections/AABB.hpp"

#include <bgfx/bgfx.h>
#include <memory>
#include <vector>
namespace Bolt {

	struct Transform2D;
	struct SpriteRenderer;
	class Scene;
	struct Color;
	class Camera2D;

	struct InstanceData {
		float Row0[2];
		float Row0Pad[2];
		float Row1[2];
		float Row1Pad[2];
		float Translation[2];
		float TranslationPad[2];
		float Color[4];
	};

	struct ColorOrderLayer {
		Color Color;
		short SortingOrder;
		uint8_t SortingLayer;
	};

	struct InstanceData32
	{
		float RS0x, RS0y;
		float RS1x, RS1y;

		float Tx, Ty;
		float ColorPacked; 
		float Padding;
	};

	struct Item {
		bgfx::TextureHandle TextureHandle;
		uint32_t SamplerFlags;
		int16_t  SortingOrder;
		uint8_t  SortingLayer;
		InstanceData32 Instance;
	};


	struct StaticBatchKey {
		uint8_t  layer;
		int16_t  order;
		bgfx::TextureHandle tex;
		uint32_t sampler;
		bool operator==(const StaticBatchKey& o) const noexcept {
			return layer == o.layer && order == o.order &&
				tex.idx == o.tex.idx && sampler == o.sampler;
		}
	};
	struct StaticBatchKeyHash {
		size_t operator()(const StaticBatchKey& k) const noexcept {
			return (size_t)k.layer ^ ((size_t)(uint16_t)k.order << 8) ^ ((size_t)k.tex.idx << 16) ^ ((size_t)k.sampler << 24);
		}
	};

	struct StaticBatch {
		StaticBatchKey key;
		bgfx::DynamicVertexBufferHandle instVB = BGFX_INVALID_HANDLE; 
		uint32_t count = 0;
		AABB aabb;
	};

	class Renderer2D {

	public:
		bool Initialize(const bgfx::PlatformData& pd, int w, int h, uint32_t resetFlags = BGFX_RESET_VSYNC | BGFX_RESET_MSAA_X4);
		void OnResize(int w, int h);
		bool DrawQuad(uint16_t viewId, const Transform2D& tr, const SpriteRenderer& sp);
		void RenderScenes();
		void RenderScene(Scene& scene, Camera2D& mainCamera);
		void BeginFrame(uint16_t viewId = 0);
		void EndFrame();
		void Shutdown();

		inline  void EnsureInstanceCapacity(uint32_t needed);
		inline void UploadAndBindInstances(const InstanceData32* data, uint32_t count);
		void RebuildStaticBatches(Scene& scene);
		void ShouldRebuildStaticBatches() { m_StaticDirty = true; }

	private:
		bool m_IsInitialized = false;

		RenderContext m_RenderContext;
		RenderResources m_RenderResources;
		QuadMesh m_QuadMesh;

		bgfx::VertexLayout m_InstanceLayout;
		std::vector<Item> s_Items;

		bgfx::DynamicVertexBufferHandle m_InstanceVB{ BGFX_INVALID_HANDLE };
		uint32_t                       m_InstanceCap{ 0 };
		uint32_t                       m_InstanceUsed{ 0 };
		bgfx::VertexLayout             m_InstanceLayout32;
		std::vector<InstanceData32>    m_InstanceScratch;

		std::unordered_map<StaticBatchKey, StaticBatch, StaticBatchKeyHash> m_StaticBatches;

		std::vector<StaticBatch*> m_StaticSorted;
		bool m_StaticDirty = true;

		std::vector<VertexPosTexColor> m_VertexBufferCPU;
		std::vector<uint16_t> m_IndexBufferCPU;
		bgfx::TextureHandle m_CurrentTexture = BGFX_INVALID_HANDLE;

		std::unique_ptr<Shader> m_DefaultShader;
		std::unique_ptr<Shader> m_InstancedShader;
		std::unique_ptr<Texture2D> m_Texture;

		size_t m_VertexCount = 0;
		size_t m_IndexCount = 0;
		size_t m_MaxVerticesPerBatch = 65536;
		bool m_BatchingEnabled = true;

		void InitInstanceLayout();
		InstanceData CreateInstanceData(const Transform2D& transform, const Color& color);
		InstanceData32 CreateInstanceData32(const Transform2D& transform, const ColorOrderLayer& spriteRenderer);
	};
}