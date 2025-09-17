#include "Bolt/btpch.hpp"
#include "Bolt/Graphics/Renderer2D.hpp"

#include "Bolt/Components/Transform2D.hpp"
#include "Bolt/Components/SpriteRenderer.hpp"
#include "Bolt/Components/ParticleSystem2D.hpp"

#include "Bolt/Collections/Color.hpp"
#include "Bolt/Collections/Vec2.hpp"

#include "Bolt/Graphics/Camera2D.hpp"
#include "Bolt/Graphics/GizmoRenderer.hpp"
#include "Bolt/Graphics/TextureManager.hpp"
#include "Bolt/Graphics/Gizmos.hpp"

#include "Bolt/Scene/Scene.hpp"
#include "Bolt/Scene/SceneManager.hpp"

#include "Bolt/Utils/Timer.hpp"
#include "Bolt/Core/Time.hpp"

#include <bgfx/platform.h>
#include <bx/math.h>

namespace Bolt {
	bool Renderer2D::Initialize(const bgfx::PlatformData& pd, int w, int h, uint32_t resetFlags)
	{
		if (m_IsInitialized) return true;

		if (!m_RenderContext.Init(pd, w, h, resetFlags)) {
			std::cerr << "[Renderer2D] RenderContext init failed\n";
			return false;
		}

		if (!m_RenderResources.Init()) {
			std::cerr << "[Renderer2D] RenderResources init failed\n";
			return false;
		}

		if (!m_QuadMesh.Init(m_RenderResources.GetPosTexLayout())) {
			std::cerr << "[Renderer2D] QuadMesh init failed\n";
			return false;
		}

		InitInstanceLayout();

		GizmoRenderer::Initialize();
		TextureManager::Initialize();

		m_RenderContext.SetViewClear(ViewID::Main, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, Color{ .3f, .3f, .3f, 1.0f }.RGBA32());
		m_RenderContext.SetViewRect(ViewID::Main, 0, 0, static_cast<uint16_t>(w), static_cast<uint16_t>(h));

		m_InstancedShader = std::make_unique<Shader>(
			"Assets/Shaders_4/shaders/dx11/vs_instanced.bin",
			"Assets/Shaders_4/shaders/dx11/fs_instanced.bin"
		);

		if (!m_InstancedShader || !m_InstancedShader->IsValid()) {
			Logger::Warning("Failed to load instanced shader!");
			return false;
		}
		else {
			Logger::Message("Instanced shader loaded successfully");
		}

		m_Texture = std::make_unique<Texture2D>("Assets/Textures/liltracy_2.png", Filter::Point, Wrap::Repeat);
		if (!m_Texture->IsValid()) {
			std::cerr << "[Renderer2D] default texture invalid\n";
		}

		m_VertexBufferCPU.reserve(1024 * 4);
		m_IndexBufferCPU.reserve(1024 * 6);

		m_InstanceUsed = 0;
		m_InstanceCap = 0;
		if (bgfx::isValid(m_InstanceVB))
			bgfx::destroy(m_InstanceVB);
		EnsureInstanceCapacity(4096);                
		m_InstanceScratch.reserve(65536);

		m_IsInitialized = true;
		return true;
	}

	void Renderer2D::OnResize(int w, int h) {
		if (!m_IsInitialized) return;
		if (w <= 0 || h <= 0) return;
		if (Camera2D::Main() == nullptr) return;

		m_RenderContext.Reset(w, h);
		m_RenderContext.SetViewRect(ViewID::Main, 0, 0, static_cast<uint16_t>(w), static_cast<uint16_t>(h));
		Camera2D::Main()->UpdateViewport();
		GizmoRenderer::OnResize(w, h);
	}

	void Renderer2D::BeginFrame(uint16_t viewId) {
		if (!m_IsInitialized || Camera2D::Main() == nullptr) return;

		m_RenderContext.TouchView(ViewID::Main);
		Camera2D::Main()->ApplyToView(viewId);
		GizmoRenderer::BeginFrame();
	}

	void Renderer2D::RenderScenes() {
		RenderScene(SceneManager::GetActiveScene(), *Camera2D::Main());
	}

	inline float packRGBA8ToFloat(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		uint32_t u = (uint32_t)r | ((uint32_t)g << 8) | ((uint32_t)b << 16) | ((uint32_t)a << 24);
		float f; memcpy(&f, &u, 4); return f;
	}

	InstanceData32 Renderer2D::CreateInstanceData32(const Transform2D& t, const ColorOrderLayer& s)
	{
		InstanceData32 d{};
		const float c = cosf(t.Rotation);
		const float si = sinf(t.Rotation);

		d.RS0x = t.Scale.x * c;
		d.RS0y = -t.Scale.y * si;
		d.RS1x = t.Scale.x * si;
		d.RS1y = t.Scale.y * c;

		d.Tx = t.Position.x;
		d.Ty = t.Position.y;

		auto c255 = [](float v) { return (uint8_t)std::clamp(v * 255.f, 0.f, 255.f); };
		d.ColorPacked = packRGBA8ToFloat(
			c255(s.Color.r), c255(s.Color.g), c255(s.Color.b), c255(s.Color.a));
		d.Padding = 0.0f;

		return d;
	}

	void Renderer2D::EnsureInstanceCapacity(uint32_t needed)
	{
		if (needed <= m_InstanceCap && bgfx::isValid(m_InstanceVB)) return;

		uint32_t newCap = 1; while (newCap < needed) newCap <<= 1;

		if (bgfx::isValid(m_InstanceVB))
			bgfx::destroy(m_InstanceVB);

		const bgfx::Memory* mem = bgfx::alloc(newCap * sizeof(InstanceData32));
		std::memset(mem->data, 0, mem->size);

		m_InstanceVB = bgfx::createDynamicVertexBuffer(mem, m_InstanceLayout32);
		m_InstanceCap = newCap;
		m_InstanceUsed = 0;
	}

	void Renderer2D::UploadAndBindInstances(const InstanceData32* data, uint32_t count)
	{
		EnsureInstanceCapacity(m_InstanceUsed + count);
		const uint32_t start = m_InstanceUsed;

		const bgfx::Memory* mem = bgfx::copy(data, count * (uint32_t)sizeof(InstanceData32));
		bgfx::update(m_InstanceVB, start, mem);
		bgfx::setInstanceDataBuffer(m_InstanceVB, start, count);

		m_InstanceUsed += count;
	}

	void Renderer2D::InitInstanceLayout()
	{
		m_InstanceLayout32.begin()
			.add(bgfx::Attrib::TexCoord7, 4, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord6, 4, bgfx::AttribType::Float)
			.end();
	}

	void Renderer2D::RebuildStaticBatches(Scene& scene)
	{
		if (!m_StaticDirty) return;
		m_StaticDirty = false;

		Logger::Message("[Renderer2D] Rebuilding static batches...");


		for (auto& [k, b] : m_StaticBatches) {
			if (bgfx::isValid(b.instVB)) bgfx::destroy(b.instVB);
		}
		m_StaticBatches.clear();
		m_StaticSorted.clear();

		auto& registry = scene.GetRegistry();
		auto sprites = registry.view<Transform2D, SpriteRenderer, StaticTag>();


		std::unordered_map<StaticBatchKey, std::vector<InstanceData32>, StaticBatchKeyHash> buckets;

		for (auto [e, tr, sr] : sprites.each())
		{
			const Texture2D& tex = TextureManager::GetTexture(sr.Sprite);
			const bgfx::TextureHandle th = bgfx::isValid(tex.Handle())
				? tex.Handle()
				: m_RenderResources.GetDefaultTexture();
			const uint32_t flags = (uint32_t)tex.GetFlags();

			StaticBatchKey key{ (uint8_t)sr.SortingLayer, (int16_t)sr.SortingOrder, th, flags };
			auto& vec = buckets[key];
			vec.emplace_back(CreateInstanceData32(tr, ColorOrderLayer{ sr.Color, sr.SortingOrder, sr.SortingLayer }));
		}

		for (auto& [key, data] : buckets)
		{
			if (data.empty()) continue;

			const uint32_t count = (uint32_t)data.size();
			const bgfx::Memory* mem = bgfx::copy(data.data(), count * (uint32_t)sizeof(InstanceData32));

			bgfx::DynamicVertexBufferHandle vb = bgfx::createDynamicVertexBuffer(mem, m_InstanceLayout32);

			StaticBatch batch;
			batch.key = key;
			batch.instVB = vb;
			batch.count = count;

			Vec2 mn{ FLT_MAX,  FLT_MAX }, mx{ -FLT_MAX, -FLT_MAX };
			for (const auto& d : data) {
				mn.x = std::min(mn.x, d.Tx); mn.y = std::min(mn.y, d.Ty);
				mx.x = std::max(mx.x, d.Tx); mx.y = std::max(mx.y, d.Ty);
			}
			batch.aabb = AABB(mn, mx);

			m_StaticBatches.emplace(key, batch);
		}


		m_StaticSorted.reserve(m_StaticBatches.size());
		for (auto& [k, b] : m_StaticBatches) m_StaticSorted.push_back(&b);
		std::sort(m_StaticSorted.begin(), m_StaticSorted.end(),
			[](const StaticBatch* a, const StaticBatch* b) {
				if (a->key.layer != b->key.layer)   return a->key.layer < b->key.layer;
				if (a->key.order != b->key.order)   return a->key.order < b->key.order;
				if (a->key.sampler != b->key.sampler) return a->key.sampler < b->key.sampler;
				return a->key.tex.idx < b->key.tex.idx;
			});
	}


	void Renderer2D::RenderScene(Scene& scene, Camera2D& mainCamera)
	{
		const float dt = Time::GetDeltaTime();
		auto& registry = scene.GetRegistry();

		
		bgfx::setViewTransform((uint16_t)ViewID::Main,
			mainCamera.GetViewMatrix(),
			mainCamera.GetProjectionMatrix());

		const AABB camAABB = mainCamera.GetViewportAABB();

		RebuildStaticBatches(scene);


		auto spritesView = registry.view<Transform2D, SpriteRenderer>(entt::exclude<DisabledTag, StaticTag>);
		auto particleView = registry.view<Transform2D, ParticleSystem2D>(entt::exclude<DisabledTag>);

		s_Items.clear();
		s_Items.reserve(spritesView.size_hint() + particleView.size_hint());

		const size_t batchCount = m_StaticBatches.size();


		size_t staticSpriteCount = 0;
		for (const auto& [key, batch] : m_StaticBatches)
			staticSpriteCount += batch.count;


		for (auto [e, tr, sr] : spritesView.each())
		{
			if (!AABB::Intersects(AABB::FromTransform(tr), camAABB))
				continue;

			const Texture2D& texObj = TextureManager::GetTexture(sr.Sprite);
			const bgfx::TextureHandle th = bgfx::isValid(texObj.Handle())
				? texObj.Handle()
				: m_RenderResources.GetDefaultTexture();
			const uint32_t flags = (uint32_t)texObj.GetFlags();

			auto& it = s_Items.emplace_back();
			it.TextureHandle = th;
			it.SamplerFlags = flags;
			it.SortingOrder = sr.SortingOrder;
			it.SortingLayer = sr.SortingLayer;
			it.Instance = CreateInstanceData32(
				tr, ColorOrderLayer{ sr.Color, sr.SortingOrder, sr.SortingLayer });
		}

		for (auto [e, tr, ps] : particleView.each())
		{
			ps.Update(dt);

			const Texture2D& tex = TextureManager::GetTexture(ps.GetTextureHandle());
			const bgfx::TextureHandle th = tex.Handle();
			const uint32_t flags = (uint32_t)tex.GetFlags();

			const auto& particles = ps.GetParticles();
			s_Items.reserve(s_Items.size() + particles.size());

			const int16_t sortOrder = ps.RenderingSettings.SortingOrder;
			const uint8_t sortLayer = ps.RenderingSettings.SortingLayer;

			if (ps.EmissionSettings.EmissionSpace == Space::Local)
			{
				for (const Particle& p : particles)
				{
					auto& it = s_Items.emplace_back();
					it.TextureHandle = th;
					it.SamplerFlags = flags;
					it.SortingOrder = sortOrder;
					it.SortingLayer = sortLayer;
					it.Instance = CreateInstanceData32(
						p.Transform + tr,
						ColorOrderLayer{ p.Color, sortOrder, sortLayer });
				}
			}
			else
			{
				for (const Particle& p : particles)
				{
					auto& it = s_Items.emplace_back();
					it.TextureHandle = th;
					it.SamplerFlags = flags;
					it.SortingOrder = sortOrder;
					it.SortingLayer = sortLayer;
					it.Instance = CreateInstanceData32(
						p.Transform,
						ColorOrderLayer{ p.Color, sortOrder, sortLayer });
				}
			}
		}

		Logger::Message("static batches: " + std::to_string(batchCount)
			+ ", static sprites: " + std::to_string(staticSpriteCount) + " proccessed sprites: " + std::to_string(spritesView.size_hint()) + " rendered sprites: " + std::to_string(s_Items.size()));


		if (m_StaticSorted.empty() && s_Items.empty())
			return;

		std::stable_sort(s_Items.begin(), s_Items.end(),
			[](const Item& a, const Item& b)
			{
				if (a.SortingLayer != b.SortingLayer)   return a.SortingLayer < b.SortingLayer;
				if (a.SortingOrder != b.SortingOrder)   return a.SortingOrder < b.SortingOrder;
				if (a.SamplerFlags != b.SamplerFlags)   return a.SamplerFlags < b.SamplerFlags;
				return a.TextureHandle.idx < b.TextureHandle.idx;
			});

		const uint64_t state = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_ALPHA;


		m_InstanceUsed = 0;
		EnsureInstanceCapacity((uint32_t)s_Items.size());


		auto keyOfItem = [](const Item& it)
			{
				return std::tuple<uint8_t, int16_t, uint32_t, uint16_t>(
					it.SortingLayer, it.SortingOrder, it.SamplerFlags, it.TextureHandle.idx);
			};
		auto keyOfBatch = [](const StaticBatch* b)
			{
				return std::tuple<uint8_t, int16_t, uint32_t, uint16_t>(
					b->key.layer, b->key.order, b->key.sampler, b->key.tex.idx);
			};

		size_t s = 0;
		size_t d = 0;
		constexpr uint32_t kMaxPerSubmit = 1'000'000u;

		while (s < m_StaticSorted.size() || d < s_Items.size())
		{
			bool takeStatic = false;

			if (s < m_StaticSorted.size() && d < s_Items.size())
				takeStatic = (keyOfBatch(m_StaticSorted[s]) <= keyOfItem(s_Items[d]));
			else if (s < m_StaticSorted.size())
				takeStatic = true;

			if (takeStatic)
			{
				const StaticBatch* sb = m_StaticSorted[s++];
				if (!AABB::Intersects(sb->aabb, camAABB))
					continue;

				bgfx::setVertexBuffer(0, m_QuadMesh.VB());
				bgfx::setIndexBuffer(m_QuadMesh.IB());
				bgfx::setInstanceDataBuffer(sb->instVB, 0, sb->count);
				bgfx::setTexture(0, m_RenderResources.GetSamplerUniform(), sb->key.tex, sb->key.sampler);
				bgfx::setState(state);
				m_InstancedShader->Submit((uint16_t)ViewID::Main);
			}
			else
			{
				const auto layer = s_Items[d].SortingLayer;
				const auto order = s_Items[d].SortingOrder;
				const auto flags = s_Items[d].SamplerFlags;
				const auto tex = s_Items[d].TextureHandle;

				size_t start = d;
				while (d < s_Items.size())
				{
					const Item& it = s_Items[d];
					if (it.SortingLayer != layer || it.SortingOrder != order || it.SamplerFlags != flags || it.TextureHandle.idx != tex.idx)
						break;
					++d;
				}
				const size_t count = d - start;

				size_t off = 0;
				while (off < count)
				{
					const uint32_t batch = (uint32_t)std::min<size_t>(kMaxPerSubmit, count - off);

					if (m_InstanceScratch.size() < batch)
						m_InstanceScratch.resize(batch);
					for (uint32_t k = 0; k < batch; ++k)
						m_InstanceScratch[k] = s_Items[start + off + k].Instance;

					bgfx::setVertexBuffer(0, m_QuadMesh.VB());
					bgfx::setIndexBuffer(m_QuadMesh.IB());
					UploadAndBindInstances(m_InstanceScratch.data(), batch);

					bgfx::setTexture(0, m_RenderResources.GetSamplerUniform(), tex, flags);
					bgfx::setState(state);
					m_InstancedShader->Submit((uint16_t)ViewID::Main);

					off += batch;
				}
			}
		}
	}



	void Renderer2D::EndFrame() {
		if (!m_IsInitialized || Camera2D::Main() == nullptr) return;

		try {
			RenderScenes();
			GizmoRenderer::EndFrame();
			bgfx::frame();
		}
		catch (std::exception e) {
			Logger::Error(e.what());
		}
	}

	void Renderer2D::Shutdown()
	{
		if (!m_IsInitialized) return;

		for (auto& [k, b] : m_StaticBatches) {
			if (bgfx::isValid(b.instVB)) bgfx::destroy(b.instVB);
		}
		m_StaticBatches.clear();
		m_StaticSorted.clear();

		if (bgfx::isValid(m_InstanceVB))
			bgfx::destroy(m_InstanceVB);
		m_InstanceVB = BGFX_INVALID_HANDLE;
		m_InstanceCap = 0;
		m_InstanceUsed = 0;

		TextureManager::Shutdown();
		GizmoRenderer::Shutdown();

		if (m_InstancedShader) m_InstancedShader.reset();
		if (m_Texture)         m_Texture.reset();

		m_QuadMesh.Shutdown();
		m_RenderResources.Shutdown();
		m_RenderContext.Shutdown();

		m_IsInitialized = false;
	}
}