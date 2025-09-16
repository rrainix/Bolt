#include "Bolt/btpch.hpp"
#include "Bolt/Graphics/Camera2D.hpp"
#include "Bolt/Collections/Viewport.hpp"

#include <bgfx/bgfx.h>
#include <bx/math.h>

namespace Bolt {
	Camera2D* Camera2D::s_Main = nullptr;
	std::shared_ptr<Viewport> Camera2D::m_Viewport;

	Camera2D* Camera2D::Main() {
		return s_Main;
	}

	void Camera2D::UpdateViewport() {
		m_ViewportWidth = Camera2D::m_Viewport->GetWidth();
		m_ViewportHeight = Camera2D::m_Viewport->GetHeight();
		UpdateProj();
	}

	void Camera2D::GetViewProjectionMatrix(float* outMatrix) const {
		bx::mtxMul(outMatrix, m_ViewMat, m_ProjMat);
	}


	void Camera2D::UpdateProj() {
		const float aspect = float(m_ViewportWidth) / float(m_ViewportHeight);
		const float halfH = m_OrthographicSize * m_Zoom;
		const float halfW = halfH * aspect;


		const float zNear = 0.0f;
		const float zFar = 100.0f;

		bx::mtxOrtho(
			m_ProjMat,
			-halfW, +halfW,
			-halfH, +halfH,
			zNear, zFar,
			0.0f,
			bgfx::getCaps()->homogeneousDepth
		);
	}

	void Camera2D::UpdateView() {
		float camModel[16];
		bx::mtxSRT(
			camModel,
			1.0f, 1.0f, 1.0f,
			0.0f, 0.0f, m_Transform->Rotation,
			m_Transform->Position.x, m_Transform->Position.y, 0.0f
		);
		bx::mtxInverse(m_ViewMat, camModel);
		UpdateViewportAABB();
	}

	void Camera2D::UpdateViewportAABB() {
		Vec2 worldViewport = WorldViewPort();
		m_WorldViewportAABB = AABB::Create(m_Transform->Position, worldViewport / 2.f);
	}

	void Camera2D::ApplyToView(uint16_t viewId) {
		bgfx::setViewTransform(viewId, m_ViewMat, m_ProjMat);
	}

	Vec2 Camera2D::WorldViewPort() const {
		float aspectRatio = static_cast<float>(m_ViewportWidth) / static_cast<float>(m_ViewportHeight);
		float worldHeight = 2.0f * (m_OrthographicSize / m_Zoom);
		float worldWidth = worldHeight * aspectRatio;

		return { worldWidth, worldHeight };
	}

	Vec2 Camera2D::ScreenToWorld(Vec2 pos) const
	{
		const float xNdc = (2.0f * pos.x / float(m_ViewportWidth)) - 1.0f;
		const float yNdc = 1.0f - (2.0f * pos.y / float(m_ViewportHeight));

		// Z für Welt-z = 0
		const bool hd = bgfx::getCaps()->homogeneousDepth;
		const float zNear = 0.0f, zFar = 100.0f;
		const float zNdc = hd ? (0.0f - zNear) / (zFar - zNear)
			: (2.0f * 0.0f - (zFar + zNear)) / (zFar - zNear);

		const float clip[4] = { xNdc, yNdc, zNdc, 1.0f };

		float vp[16], invVp[16];
		bx::mtxMul(vp, m_ViewMat, m_ProjMat);
		bx::mtxInverse(invVp, vp);

		float world4[4];
		bx::vec4MulMtx(world4, clip, invVp);

		const float invW = (world4[3] != 0.0f) ? 1.0f / world4[3] : 1.0f;
		return { world4[0] * invW, world4[1] * invW };
	}

	void Camera2D::Init(Transform2D& transform) {
		s_Main = this;
		m_Transform = &transform;
		bx::mtxIdentity(m_ViewMat);
		bx::mtxIdentity(m_ProjMat);
		UpdateProj();
		UpdateView();
	}

	void Camera2D::Destroy() {
		if (s_Main == this) {
			s_Main = nullptr;
		}
		m_Transform = nullptr;
	}
}