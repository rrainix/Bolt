#include "Bolt/btpch.hpp"
#include "Bolt/Graphics/Shader.hpp"
#include "Bolt/Debug/Logger.hpp"

namespace Bolt {
    static const bgfx::ShaderHandle loadShader(const std::string& path) {
        FILE* f = fopen(path.c_str(), "rb");
        if (!f) {
            Logger::Error("[Shader] Path (" + path + ") not found at");
            return BGFX_INVALID_HANDLE; 
        }
        fseek(f, 0, SEEK_END);
        size_t size = ftell(f);
        fseek(f, 0, SEEK_SET);
        uint8_t* data = new uint8_t[size];
        fread(data, 1, size, f);
        fclose(f);
        const bgfx::Memory* mem = bgfx::copy(data, uint32_t(size));
        delete[] data;
        return bgfx::createShader(mem);
    }

    Shader::Shader(const std::string& vsPath, const std::string& fsPath) {
        bgfx::ShaderHandle vsh = loadShader(vsPath);
        if (!bgfx::isValid(vsh)) {
            Logger::Error("[Shader] Failed to load vertex shader: " + vsPath);
            return;
        }

        bgfx::ShaderHandle fsh = loadShader(fsPath);
        if (!bgfx::isValid(fsh)) {
            Logger::Error("[Shader] Failed to load fragment shader: " + fsPath);
            return;
        }
        m_Program = bgfx::createProgram(vsh, fsh, true);
        if (!bgfx::isValid(m_Program)) {
            Logger::Error("[Shader] createProgram failed: " + vsPath + " + " + fsPath);
            return;
        }
        m_IsValid = true;
    }

    Shader::~Shader() {
        if (m_IsValid && bgfx::isValid(m_Program)) {
            bgfx::destroy(m_Program);
        }
    }

    Shader::Shader(Shader&& o) noexcept {
        m_Program = o.m_Program; m_IsValid = o.m_IsValid;
        o.m_Program = BGFX_INVALID_HANDLE; o.m_IsValid = false;
    }

    Shader& Shader::operator=(Shader&& o) noexcept {
        if (this != &o) {
            if (m_IsValid && bgfx::isValid(m_Program)) bgfx::destroy(m_Program);
            m_Program = o.m_Program; m_IsValid = o.m_IsValid;
            o.m_Program = BGFX_INVALID_HANDLE; o.m_IsValid = false;
        }
        return *this;
    }

    void Shader::Submit(uint16_t view) const {
        if (IsValid()) {
            bgfx::submit(view, m_Program);
        }
    }
}