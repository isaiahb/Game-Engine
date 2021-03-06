#pragma once

#include <iostream>
#include <vector>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "fileutils.hpp"

using namespace glm;
namespace ballah { namespace graphics {
    
    class Shader
    {
    private:
        GLuint m_ShaderID;
        const char* m_VertPath;
        const char* m_FragPath;
    public:
        Shader(const char* vertPath, const char* fragPath);
		Shader(const char* vertPath, const char* fragPath, const char* geoPath);

        ~Shader();
        
        void setUniform1f(const GLchar* name, float value);
        void setUniform1i(const GLchar* name, int value);
        void setUniform2f(const GLchar* name, const vec2& vector);
        void setUniform3f(const GLchar* name, const vec3& vector);
        void setUniform4f(const GLchar* name, const vec4& vector);
        void setUniformMat4(const GLchar* name, const mat4& matrix);
        
        void enable() const;
        void disable() const;
    private:
        GLuint load();
        GLint getUniformLocation(const GLchar* name);
    };
    
} }