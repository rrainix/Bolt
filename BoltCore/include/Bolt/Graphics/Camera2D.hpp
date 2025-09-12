#pragma once
#include "Bolt/Components/Transform2D.hpp"
#include "Bolt/Collections/AABB.hpp"


namespace Bolt {
	class Viewport;

	class Camera2D {
	public:
		Camera2D() = default;
 
		static Camera2D* Main();

		void UpdateViewport();

		void SetOrthographicSize(float size) { m_OrthographicSize = (size > 0 ? size : 0.001f); UpdateProj(); }
		void AddOrthographicSize(float ds) { SetOrthographicSize(m_OrthographicSize + ds); }
		float GetOrthographicSize() const { return m_OrthographicSize; }

		void SetPosition(Vec2 p) { m_Transform->Position = p; UpdateView(); }
		void AddPosition(Vec2 p) { SetPosition(p + m_Transform->Position); }
		Vec2 GetPosition() const { return m_Transform->Position; }

		void SetRotation(float rad) { m_Transform->Rotation = rad; UpdateView(); }
		float GetRotation() const { return m_Transform->Rotation; }

		void SetZoom(float z) { m_Zoom = z; UpdateProj(); }
		float GetZoom() const { return m_Zoom; }


		AABB GetViewportAABB() const { return m_WorldViewportAABB; }
		Vec2 WorldViewPort() const;
		Vec2 ScreenToWorld(Vec2 pos) const;

		void GetViewProjectionMatrix(float* outMatrix) const;
		const float* GetViewMatrix() const { return m_ViewMat; }
		const float* GetProjectionMatrix() const { return m_ProjMat; }
		void ApplyToView(uint16_t viewId);

		float ViewportWidth() const { return static_cast<float>(m_ViewportWidth); }
		float ViewportHeight() const { return static_cast<float>(m_ViewportHeight); }

		bool IsValid() const { return m_Transform; }
	private:
		void UpdateProj();
		void UpdateView();
		void UpdateViewportAABB();

		void Init(Transform2D& transform);
		void Destroy();

		Transform2D* m_Transform = nullptr;
		static Camera2D* s_Main;
		float m_Zoom{ 1.0f };
		float m_OrthographicSize{ 5.0f };
		int   m_ViewportWidth{ 1 }, m_ViewportHeight{ 1 };

		float m_ViewMat[16]{};
		float m_ProjMat[16]{};
		AABB m_WorldViewportAABB;
		static std::shared_ptr<Viewport> m_Viewport;

		friend class Scene;
		friend class Application;
	};
}