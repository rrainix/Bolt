#include "Bolt/btpch.hpp"
#include "Bolt/Graphics/GizmoRenderer.hpp"
#include "Bolt/Graphics/Renderer2D.hpp"
#include "Bolt/Graphics/Camera2D.hpp"
#include "Bolt/Graphics/Gizmos.hpp"

#include <bx/math.h>
#include <bgfx/bgfx.h>

namespace Bolt {
	// Static member definitions
	bool GizmoRenderer::m_IsInitialized = false;
	std::unique_ptr<Shader> GizmoRenderer::m_GizmoShader;
	bgfx::VertexLayout GizmoRenderer::m_GizmoLayout;
	std::vector<PosColorVertex> GizmoRenderer::m_GizmoVertices;
	std::vector<uint16_t> GizmoRenderer::m_GizmoIndices;
	uint16_t GizmoRenderer::m_GizmoViewId = (uint16_t)ViewID::Gizmos;

	bool GizmoRenderer::Initialize() {
		if (m_IsInitialized) return true;

		// Setup gizmo vertex layout
		m_GizmoLayout.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.end();

		m_GizmoShader = std::make_unique<Shader>(
			"Assets/Shaders_3/compiled_shaders/vs_gizmoDx.bin",
			"Assets/Shaders_3/compiled_shaders/fs_gizmoDx.bin"
		);

		if (!m_GizmoShader) {
			Logger::Error("[GizmosRenderSystem] Failed to load gizmo shader\n");
			return false;
		}

		// Reserve space for vertices and indices
		m_GizmoVertices.reserve(4096);
		m_GizmoIndices.reserve(8192);

		// Setup gizmo view (no clear, renders on top)
		bgfx::setViewClear(m_GizmoViewId, BGFX_CLEAR_NONE);
		bgfx::setViewRect(m_GizmoViewId, 0, 0, bgfx::BackbufferRatio::Equal);
		bgfx::setViewMode(m_GizmoViewId, bgfx::ViewMode::Sequential);

		m_IsInitialized = true;
		return true;
	}

	void GizmoRenderer::Shutdown() {
		if (!m_IsInitialized) return;

		m_GizmoShader.reset();
		m_GizmoVertices.clear();
		m_GizmoIndices.clear();

		m_IsInitialized = false;
	}

	void GizmoRenderer::OnResize(int w, int h) {
		if (!m_IsInitialized || w <= 0 || h <= 0) return;

		// Update gizmo view viewport
		bgfx::setViewRect(m_GizmoViewId, 0, 0, bgfx::BackbufferRatio::Equal);
	}

	void GizmoRenderer::BeginFrame(uint16_t viewId) {
		if (!m_IsInitialized) return;

		// Clear buffers for new frame
		m_GizmoVertices.clear();
		m_GizmoIndices.clear();

		Camera2D::Main()->ApplyToView(m_GizmoViewId);
	}

	void GizmoRenderer::Render() {
		if (!m_IsInitialized || !Gizmos::s_IsEnabled) return;

		// Process all boxes
		for (const auto& box : Gizmos::s_Boxes) {
			uint32_t color = box.Color.ABGR32();
			uint16_t baseIndex = (uint16_t)m_GizmoVertices.size();

			// Calculate rotated corners
			Vec2 corners[4] = {
				Vec2(-box.HalfExtents.x, -box.HalfExtents.y),
				Vec2(box.HalfExtents.x, -box.HalfExtents.y),
				Vec2(box.HalfExtents.x,  box.HalfExtents.y),
				Vec2(-box.HalfExtents.x,  box.HalfExtents.y)
			};

			// Apply rotation and translation
			for (int i = 0; i < 4; ++i) {
				Vec2 rotated = Bolt::Rotated(corners[i],box.Radiant); //May wrong
				Vec2 final = box.Center + rotated;
				m_GizmoVertices.push_back({ final.x, final.y, 0.0f, color });
			}

			// Add line loop indices
			m_GizmoIndices.push_back(baseIndex + 0);
			m_GizmoIndices.push_back(baseIndex + 1);
			m_GizmoIndices.push_back(baseIndex + 1);
			m_GizmoIndices.push_back(baseIndex + 2);
			m_GizmoIndices.push_back(baseIndex + 2);
			m_GizmoIndices.push_back(baseIndex + 3);
			m_GizmoIndices.push_back(baseIndex + 3);
			m_GizmoIndices.push_back(baseIndex + 0);
		}

		// Process all lines
		for (const auto& line : Gizmos::s_Lines) {
			uint32_t color =line.Color.ABGR32();
			uint16_t baseIndex = (uint16_t)m_GizmoVertices.size();

			m_GizmoVertices.push_back({ line.Start.x, line.Start.y, 0.0f, color });
			m_GizmoVertices.push_back({ line.End.x, line.End.y, 0.0f, color });

			m_GizmoIndices.push_back(baseIndex);
			m_GizmoIndices.push_back(baseIndex + 1);
		}

		// Process all circles
		for (const auto& circle : Gizmos::s_Circles) {
			uint32_t color = circle.Color.ABGR32();
			uint16_t baseIndex = (uint16_t)m_GizmoVertices.size();

			// Generate circle vertices
			float angleStep = 2.0f * Pi<float>() / circle.Segments;
			for (int i = 0; i < circle.Segments; ++i) {
				float angle = i * angleStep;
				float x = circle.Center.x + circle.Radius * Cos(angle);
				float y = circle.Center.y + circle.Radius * Sin(angle);
				m_GizmoVertices.push_back({ x, y, 0.0f, color });
			}

			// Add line loop indices
			for (int i = 0; i < circle.Segments; ++i) {
				m_GizmoIndices.push_back(baseIndex + i);
				m_GizmoIndices.push_back(baseIndex + ((i + 1) % circle.Segments));
			}
		}

		FlushGizmos();
	    Gizmos::Clear();
	}

	void GizmoRenderer::FlushGizmos() {
		if (!m_IsInitialized || m_GizmoVertices.empty() || !m_GizmoShader) return;

		// Create transient buffers
		bgfx::TransientVertexBuffer tvb;
		bgfx::TransientIndexBuffer tib;

		// Allocate buffers
		bgfx::allocTransientVertexBuffer(&tvb, (uint32_t)m_GizmoVertices.size(), m_GizmoLayout);
		bgfx::allocTransientIndexBuffer(&tib, (uint32_t)m_GizmoIndices.size());

		// Copy data
		bx::memCopy(tvb.data, m_GizmoVertices.data(), m_GizmoVertices.size() * sizeof(PosColorVertex));
		bx::memCopy(tib.data, m_GizmoIndices.data(), m_GizmoIndices.size() * sizeof(uint16_t));

		// Set identity transform
		float identityMtx[16];
		bx::mtxIdentity(identityMtx);
		bgfx::setTransform(identityMtx);

		// Set buffers
		bgfx::setVertexBuffer(0, &tvb);
		bgfx::setIndexBuffer(&tib);

		// State for line rendering (no depth test, renders on top)
		const uint64_t state =
			BGFX_STATE_WRITE_RGB |
			BGFX_STATE_WRITE_A |
			BGFX_STATE_BLEND_ALPHA |
			BGFX_STATE_PT_LINES |
			BGFX_STATE_LINEAA |
			BGFX_STATE_MSAA;

		bgfx::setState(state);
		m_GizmoShader->Submit(m_GizmoViewId);
	}

	void GizmoRenderer::EndFrame() {
		Render();
	}
}