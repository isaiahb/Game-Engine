//
//  shader.cpp
//  Game Engine
//
//  Created by isaiah ballah on 2016-03-19.
//  Copyright © 2016 isaiah ballah. All rights reserved.
//

#include "shader.hpp"

using namespace glm;
namespace ballah { namespace graphics {
    GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path, const char *geo_file_path = nullptr);

    Shader::Shader(const char* vertPath, const char* fragPath) {
        m_ShaderID = LoadShaders(vertPath, fragPath);
    }
	Shader::Shader(const char* vertPath, const char* fragPath, const char *geoPath) {
		m_ShaderID = LoadShaders(vertPath, fragPath, geoPath);
	}
	
    Shader::~Shader()
    {
        glDeleteProgram(m_ShaderID);
    }
    
    
    GLuint Shader::load()
    {
        GLuint program = glCreateProgram();
        GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
        
        std::string vertSourceString = FileUtils::read_file(m_VertPath);
        std::string fragSourceString = FileUtils::read_file(m_FragPath);
        
        const char* vertSource = vertSourceString.c_str();
        const char* fragSource = fragSourceString.c_str();
        
        glShaderSource(vertex, 1, &vertSource, NULL);
        glCompileShader(vertex);
        
        GLint result;
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            GLint length;
            glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &length);
            std::vector<char> error(length);
            glGetShaderInfoLog(vertex, length, &length, &error[0]);
            std::cout << "Failed to compile vertex shader!" << std::endl << &error[0] << std::endl;
            glDeleteShader(vertex);
            return 0;
        }
        
        glShaderSource(fragment, 1, &fragSource, NULL);
        glCompileShader(fragment);
        
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            GLint length;
            glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &length);
            std::vector<char> error(length);
            glGetShaderInfoLog(fragment, length, &length, &error[0]);
            std::cout << "Failed to compile fragment shader!" << std::endl << &error[0] << std::endl;
            glDeleteShader(fragment);
            return 0;
        }
        
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        
        glLinkProgram(program);
        glValidateProgram(program);
        
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        
        return program;
    }
    
    GLint Shader::getUniformLocation(const GLchar* name)
    {
        return glGetUniformLocation(m_ShaderID, name);
    }
    
    void Shader::setUniform1f(const GLchar* name, float value)
    {
        glUniform1f(getUniformLocation(name), value);
    }
    
    void Shader::setUniform1i(const GLchar* name, int value)
    {
        glUniform1i(getUniformLocation(name), value);
    }
    
    void Shader::setUniform2f(const GLchar* name, const vec2& vector)
    {
        glUniform2f(getUniformLocation(name), vector.x, vector.y);
    }
    
    void Shader::setUniform3f(const GLchar* name, const vec3& vector)
    {
        glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z);
    }
    
    void Shader::setUniform4f(const GLchar* name, const vec4& vector)
    {
        glUniform4f(getUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
    }
    
    void Shader::setUniformMat4(const GLchar* name, const mat4& matrix)
    {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
    }
    
    void Shader::enable() const
    {
        glUseProgram(m_ShaderID);
    }
    
    void Shader::disable() const
    {
        glUseProgram(0);
    }
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    // wombo combo  //
    //////////////////
    GLuint LoadShaders(const char *vertex_file_path, const char *fragment_file_path, const char *geometry_file_path){
        
        // Create the shaders
        GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		GLuint GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
        
        // Read the Vertex Shader code from the file
        std::string VertexShaderCode;
        std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
        if(VertexShaderStream.is_open()){
            std::string Line = "";
            while(getline(VertexShaderStream, Line))
                VertexShaderCode += "\n" + Line;
            VertexShaderStream.close();
        }else{
            printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
            getchar();
            return 0;
        }
        
        // Read the Fragment Shader code from the file
        std::string FragmentShaderCode;
        std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
        if(FragmentShaderStream.is_open()){
            std::string Line = "";
            while(getline(FragmentShaderStream, Line))
                FragmentShaderCode += "\n" + Line;
            FragmentShaderStream.close();
		} else{
			printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", fragment_file_path);
			getchar();
			return 0;
		}
	
		
        GLint Result = GL_FALSE;
        int InfoLogLength;
		
        // Compile Vertex Shader
        printf("Compiling shader : %s\n", vertex_file_path);
        char const * VertexSourcePointer = VertexShaderCode.c_str();
        glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
        glCompileShader(VertexShaderID);
        
        // Check Vertex Shader
        glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ){
            std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
            glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
            printf("%s\n", &VertexShaderErrorMessage[0]);
        }
        
		
        // Compile Fragment Shader
        printf("Compiling shader : %s\n", fragment_file_path);
        char const * FragmentSourcePointer = FragmentShaderCode.c_str();
        glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
        glCompileShader(FragmentShaderID);
        
        // Check Fragment Shader
        glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ){
            std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
            glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
            printf("%s\n", &FragmentShaderErrorMessage[0]);
        }
		
		//if geomtry shader then load it from file, compile and check it.
		if (geometry_file_path != nullptr) {
			// Read the Geometry Shader code from the file
			std::string GeometryShaderCode;
			std::ifstream GeometryShaderStream(geometry_file_path, std::ios::in);
			if(GeometryShaderStream.is_open()){
				std::string Line = "";
				while(getline(GeometryShaderStream, Line))
					GeometryShaderCode += "\n" + Line;
				GeometryShaderStream.close();
			} else {
				printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", geometry_file_path);
				getchar();
				return 0;
			}
			
			// Compile Geometry Shader
			printf("Compiling shader : %s\n", geometry_file_path);
			char const * GeometrySourcePointer = GeometryShaderCode.c_str();
			glShaderSource(GeometryShaderID, 1, &GeometrySourcePointer , NULL);
			glCompileShader(GeometryShaderID);
			
			// Check Geometry Shader
			glGetShaderiv(GeometryShaderID, GL_COMPILE_STATUS, &Result);
			glGetShaderiv(GeometryShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
			if ( InfoLogLength > 0 ){
				std::vector<char> GeometryShaderErrorMessage(InfoLogLength+1);
				glGetShaderInfoLog(GeometryShaderID, InfoLogLength, NULL, &GeometryShaderErrorMessage[0]);
				printf("%s\n", &GeometryShaderErrorMessage[0]);
			}
			
		}
		
		
        // Link the program
        printf("Linking program\n");
        GLuint ProgramID = glCreateProgram();
        glAttachShader(ProgramID, VertexShaderID);
        glAttachShader(ProgramID, FragmentShaderID);
		if(geometry_file_path != nullptr) {
			glAttachShader(ProgramID, GeometryShaderID);
		}
        glLinkProgram(ProgramID);
        
        // Check the program
        glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
        glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ){
            std::vector<char> ProgramErrorMessage(InfoLogLength+1);
            glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            printf("%s\n", &ProgramErrorMessage[0]);
        }
		
        glDetachShader(ProgramID, VertexShaderID);
        glDetachShader(ProgramID, FragmentShaderID);
		
        glDeleteShader(VertexShaderID);
        glDeleteShader(FragmentShaderID);
		
		//if geometry shader detach and delete it
		if (geometry_file_path != nullptr) {
			glDetachShader(ProgramID, GeometryShaderID);
			glDeleteShader(GeometryShaderID);
		}
		
        
        return ProgramID;
    }

} }