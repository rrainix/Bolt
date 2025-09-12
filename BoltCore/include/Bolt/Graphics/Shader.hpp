#pragma once
#include <string>
#include <bgfx/bgfx.h>

namespace Bolt {
	class Shader {
	public:
        Shader(const std::string& vsPath, const std::string& fsPath);
        ~Shader();


        void Submit(uint16_t view) const;

        bgfx::ProgramHandle GetHandle() const { return m_Program; }
        bool IsValid() const { return m_IsValid && bgfx::isValid(m_Program); }

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;
        Shader(Shader&&) noexcept;
        Shader& operator=(Shader&&) noexcept;

    private:
        bgfx::ProgramHandle m_Program{ BGFX_INVALID_HANDLE };
        bool m_IsValid{ false };
	};
}