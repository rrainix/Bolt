#pragma once
#include "Bolt/Collections/Vec2.hpp"
#include <bgfx/bgfx.h>


namespace Bolt {
	enum class Filter { Point, Bilinear, Trilinear, Anisotropic };
	enum class Wrap { Repeat, Clamp, Mirror, Border };

	inline uint32_t WrapU(Wrap w) {
		switch (w) {
		case Wrap::Clamp:  return BGFX_SAMPLER_U_CLAMP;
		case Wrap::Mirror: return BGFX_SAMPLER_U_MIRROR;
		case Wrap::Border: return BGFX_SAMPLER_U_BORDER;
		case Wrap::Repeat: default: return 0;
		}
	}
	inline uint32_t WrapV(Wrap w) {
		switch (w) {
		case Wrap::Clamp:  return BGFX_SAMPLER_V_CLAMP;
		case Wrap::Mirror: return BGFX_SAMPLER_V_MIRROR;
		case Wrap::Border: return BGFX_SAMPLER_V_BORDER;
		case Wrap::Repeat: default: return 0;
		}
	}
	inline uint32_t WrapW(Wrap w) {
		switch (w) {
		case Wrap::Clamp:  return BGFX_SAMPLER_W_CLAMP;
		case Wrap::Mirror: return BGFX_SAMPLER_W_MIRROR;
		case Wrap::Border: return BGFX_SAMPLER_W_BORDER;
		case Wrap::Repeat: default: return 0;
		}
	}

	class Texture2D {
	public:
		Texture2D() = default;
		Texture2D(const char* path, Filter filter = Filter::Point, Wrap u = Wrap::Clamp, Wrap v = Wrap::Clamp);

		bool IsValid() const;
		bgfx::TextureHandle Handle() const { return m_TextureHandle; }

		bgfx::TextureHandle Load(const char* path);

		static uint32_t MakeFlags(Filter filter, Wrap u = Wrap::Clamp, Wrap v = Wrap::Clamp);

		void Submit(uint8_t stage, bgfx::UniformHandle sampler) const;

		void SetSampler(Filter filter, Wrap u = Wrap::Clamp, Wrap v = Wrap::Clamp) {
			m_Flags = MakeFlags(filter, u, v);
		}

		Vec2 Size() const { return Vec2{ static_cast<float>(m_Width), static_cast<float>(m_Height) }; }
		Vec2 SizeNormalized() const { float max = std::max(static_cast<float>(m_Width), static_cast<float>(m_Width)); return Vec2{ static_cast<float>(m_Width) / max , static_cast<float>(m_Height) / max }; }
		float AspectRatio() const { return static_cast<float>(m_Width) / static_cast<float>(m_Height); }
		uint32_t GetFlags() const { return m_Flags; }


		void Destroy();

	private:
		bgfx::TextureHandle m_TextureHandle = BGFX_INVALID_HANDLE;
		uint32_t m_Flags = 0;
		uint16_t m_Width;
		uint16_t m_Height;
	};
}