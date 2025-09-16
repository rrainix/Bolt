#include "Bolt/btpch.hpp"
#include "Bolt/Graphics/Texture2D.hpp"

#include <bx/file.h>
#include <bimg/decode.h>
#include <bx/allocator.h>
#include <bx/bx.h>
#include <bx/error.h>

namespace Bolt {
	static void releaseTextureCallback(void* _data, void* _userData)
	{
		bimg::ImageContainer* image = static_cast<bimg::ImageContainer*>(_userData);
		bimg::imageFree(image);
	}

	Texture2D::Texture2D(const char* path, Filter filter, Wrap u, Wrap v) {
		m_TextureHandle = Load(path);
		m_Flags = MakeFlags(filter, u, v);
	}

	void Texture2D::Submit(uint8_t stage, bgfx::UniformHandle sampler) const {
		bgfx::setTexture(stage, sampler, m_TextureHandle, m_Flags);
	}

	void Texture2D::Destroy() {
		if (bgfx::isValid(m_TextureHandle)) {
			bgfx::destroy(m_TextureHandle);
			m_TextureHandle = BGFX_INVALID_HANDLE;
			m_Width = 0;
			m_Height = 0;
		}
	}

	bool Texture2D::IsValid() const { return bgfx::isValid(m_TextureHandle); }

	uint32_t Texture2D::MakeFlags(Filter filter, Wrap u, Wrap v)
	{
		uint32_t f = 0;
		f |= WrapU(u) | WrapV(v);

		switch (filter)
		{
		case Filter::Point:
			f |= BGFX_SAMPLER_POINT;
			break;
		case Filter::Bilinear:
			f |= BGFX_SAMPLER_MIP_POINT;
			break;
		case Filter::Trilinear:
			break;
		case Filter::Anisotropic:
			f |= BGFX_SAMPLER_MIN_ANISOTROPIC | BGFX_SAMPLER_MAG_ANISOTROPIC;
			break;
		}
		return f;
	}

	bgfx::TextureHandle Texture2D::Load(const char* path)
	{
		bx::FileReader reader;
		if (!bx::open(&reader, path))
		{
			Logger::Error("[LoadTexture] Can't open: " + std::string(path) + "\n");
			return BGFX_INVALID_HANDLE;
		}

		const uint32_t size = (uint32_t)bx::getSize(&reader);
		const bgfx::Memory* fileMem = bgfx::alloc(size);
		bx::read(&reader, fileMem->data, size, nullptr);
		bx::close(&reader);

		static bx::DefaultAllocator s_allocator;
		bimg::ImageContainer* image = bimg::imageParse(
			&s_allocator,
			fileMem->data,
			fileMem->size
		);
		if (!image)
		{
			Logger::Error("[LoadTexture] Decode failed: " + std::string(path) + "\n");
			return BGFX_INVALID_HANDLE;
		}

		const bool   hasMips = image->m_numMips > 1;
		m_Width = uint16_t(image->m_width);
		m_Height = uint16_t(image->m_height);

		const uint16_t numMips = uint16_t(image->m_numMips);
		const bgfx::TextureFormat::Enum fmt =
			bgfx::TextureFormat::Enum(image->m_format);

		const bgfx::Memory* texMem = bgfx::makeRef(
			image->m_data,
			image->m_size,
			releaseTextureCallback,
			image
		);

		return bgfx::createTexture2D(
			m_Width,
			m_Height,
			hasMips,
			numMips,
			fmt,
			BGFX_SAMPLER_UVW_CLAMP | BGFX_SAMPLER_MIN_ANISOTROPIC,
			texMem
		);
	}
}