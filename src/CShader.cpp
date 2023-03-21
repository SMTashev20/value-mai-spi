#include "./CShader.hpp"
#include <cassert>

namespace Feather
{

    CShader::CShader()
        : m_Shader({}) {}

    CShader::CShader(const std::optional<std::string>& vsPath, const std::optional<std::string>& fsPath)
        : m_Shader({})
    {
        Load(vsPath, fsPath);
    }

    CShader::~CShader()
    {
        Unload();
    }

    template<typename T>
    void CShader::SetUniform(const T& data, const std::string& uniformName)
    {
    }

    template<typename T>
    void CShader::SetUniformArray(T* buffer, size_t bufferSize, const std::string& uniformName)
    {
    }

    void CShader::Bind()
    {
        if (!IsLoaded()) return;

        BeginShaderMode(m_Shader);
    }

    void CShader::Unbind()
    {
        if (!IsLoaded()) return;
        
        EndShaderMode();
    }

    void CShader::Load(const std::optional<std::string>& vsPath, const std::optional<std::string>& fsPath)
    {
        Unload();

        m_VertexPath = vsPath;
        m_FragmentPath = fsPath;

        const char *fsPathBuf = nullptr, *vsPathBuf = nullptr;
        if (m_VertexPath.has_value()) vsPathBuf = m_VertexPath->c_str();
        if (m_FragmentPath.has_value()) fsPathBuf = m_FragmentPath->c_str();

        m_Shader = LoadShader(vsPathBuf, fsPathBuf);
    }

    void CShader::Unload()
    {
        if (!IsLoaded()) return;
        UnloadShader(m_Shader);
        m_Shader = {};
    }

    int CShader::GetUniformLocation(const std::string& uniformName)
    {
        if (!IsLoaded()) assert(false && "Shader isn't loaded to fetch uniforms");

        if (m_UniformLocationMap.find(uniformName) == m_UniformLocationMap.end())
        {
            int loc = GetShaderLocation(m_Shader, uniformName.c_str());
            if (loc == -1) assert(false && "Uniform name doesn't exist");
            m_UniformLocationMap.insert(std::pair<std::string, int>(uniformName, loc));

            return loc;
        }
        else return m_UniformLocationMap.at(uniformName);
    }

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

    template<>
    void CShader::SetUniform<float>(const float& data, const std::string& uniformName)
    {
        if (!IsLoaded()) return;
        SetShaderValue(m_Shader, GetUniformLocation(uniformName), &data, SHADER_UNIFORM_FLOAT);
    }

    template<>
    void CShader::SetUniform<glm::vec2>(const glm::vec2& data, const std::string& uniformName)
    {
        if (!IsLoaded()) return;
        Vector2 raylibData{ data.x, data.y };
        SetShaderValue(m_Shader, GetUniformLocation(uniformName), &raylibData, SHADER_UNIFORM_VEC2);
    }

    template<>
    void CShader::SetUniform<glm::vec3>(const glm::vec3& data, const std::string& uniformName)
    {
        if (!IsLoaded()) return;
        Vector3 raylibData{ data.x, data.y, data.z };
        SetShaderValue(m_Shader, GetUniformLocation(uniformName), &raylibData, SHADER_UNIFORM_VEC3);
    }

    template<>
    void CShader::SetUniform<glm::vec4>(const glm::vec4& data, const std::string& uniformName)
    {
        if (!IsLoaded()) return;
        Vector4 raylibData{ data.x, data.y, data.z, data.w };
        SetShaderValue(m_Shader, GetUniformLocation(uniformName), &raylibData, SHADER_UNIFORM_VEC4);
    }

    template<>
    void CShader::SetUniform<int>(const int& data, const std::string& uniformName)
    {
        if (!IsLoaded()) return;
        SetShaderValue(m_Shader, GetUniformLocation(uniformName), &data, SHADER_UNIFORM_INT);
    }

    template<>
    void CShader::SetUniform<glm::ivec2>(const glm::ivec2& data, const std::string& uniformName)
    {
        if (!IsLoaded()) return;
        int rawData[2] = { data.x, data.y };
        SetShaderValue(m_Shader, GetUniformLocation(uniformName), &rawData, SHADER_UNIFORM_IVEC2);
    }

    template<>
    void CShader::SetUniform<glm::ivec3>(const glm::ivec3& data, const std::string& uniformName)
    {
        if (!IsLoaded()) return;
        int rawData[3] = { data.x, data.y, data.z };
        SetShaderValue(m_Shader, GetUniformLocation(uniformName), &rawData, SHADER_UNIFORM_IVEC3);
    }

    template<>
    void CShader::SetUniform<glm::ivec4>(const glm::ivec4& data, const std::string& uniformName)
    {
        if (!IsLoaded()) return;
        int rawData[4] = { data.x, data.y, data.z, data.w };
        SetShaderValue(m_Shader, GetUniformLocation(uniformName), &rawData, SHADER_UNIFORM_IVEC3);
    }

}