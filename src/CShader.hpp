#pragma once

#include <raylib.h>
#include <string>
#include <memory>
#include <unordered_map>
#include <optional>
#include <glm/glm.hpp>

enum class UniformType {
    SHADER_UNIFORM_FLOAT = 0,       // Shader uniform type: float
    SHADER_UNIFORM_VEC2,            // Shader uniform type: vec2 (2 float)
    SHADER_UNIFORM_VEC3,            // Shader uniform type: vec3 (3 float)
    SHADER_UNIFORM_VEC4,            // Shader uniform type: vec4 (4 float)
    SHADER_UNIFORM_INT,             // Shader uniform type: int
    SHADER_UNIFORM_IVEC2,           // Shader uniform type: ivec2 (2 int)
    SHADER_UNIFORM_IVEC3,           // Shader uniform type: ivec3 (3 int)
    SHADER_UNIFORM_IVEC4,           // Shader uniform type: ivec4 (4 int)
    SHADER_UNIFORM_SAMPLER2D        // Shader uniform type: sampler2d
};

namespace Feather
{

    class CShader
    {
    public:
        CShader();
        CShader(const std::optional<std::string>& vsPath, const std::optional<std::string>& fsPath);

        virtual ~CShader();

        template<typename T>
        void SetUniform(const T& data, const std::string& uniformName);

        template<typename T>
        void SetUniformArray(T* buffer, size_t bufferSize, const std::string& uniformName);

        void Bind();
        void Unbind();

        void Load(const std::optional<std::string>& vsPath, const std::optional<std::string>& fsPath);
        void Unload();

        const std::string& GetVertexPath() const { return m_VertexPath.value_or(""); };
        const std::string& GetFragmentPath() const { return m_FragmentPath.value_or(""); };

        bool IsLoaded() const { return m_Shader.id != 0; };

        const Shader& GetRaylib() { return m_Shader; };

    private:
        int GetUniformLocation(const std::string& uniformName);

    private:
        Shader m_Shader;
        std::optional<std::string> m_VertexPath, m_FragmentPath;
        std::unordered_map<std::string, int> m_UniformLocationMap;
    };

}
