#ifndef YUME_GL_PROGRAM_H
#define YUME_GL_PROGRAM_H

#include "shader.h"

struct GlProgram {
private:
    GLuint id{};
    //can't be public or someone's gonna overwrite it without calling glDeleteProgram and that's a memory leak on the GPU

    char countVShaders{};
    std::vector<Shader*> shaders;
public:
    GlProgram() : id(glCreateProgram()) {
        std::cout << "program nr.: " << id << " has been created." << std::endl;
    };

    /// <summary>
    ///Makse an already linked program with given shaders.
    ///Created program object is not responsible for managing given
    ///pointers, however it stores them and can be queried for them.
    /// </summary>
    GlProgram(Shader* vertexShader, Shader* fragmentShader) : id(glCreateProgram()) {

        std::cout << "program nr.: " << id << " has been created." << std::endl;

        if (vertexShader->sType() != GL_VERTEX_SHADER || fragmentShader->sType() != GL_FRAGMENT_SHADER)
            std::cout << "ERROR: INCORRECT ARGUMENTS FOR GlProgram CONSTRUCTOR " << std::endl;

        attachVShader(vertexShader);
        attachFShader(fragmentShader);

        linkProgram();
    }

    // FUNCTIONS

    /// <summary>
    ///To be used on a compleately untouched GlProgram created with the parameterless constuctor.
    ///Reads shader code from files, compiles it, REPLACES(NOT DELETEs) shader pointers with new objects, 
    ///attaches shaders to the program, links program, deletes pointers, that is shader objects.
    ///Mainly for simple useage.
    /// </summary>
    void makeProgramFromPaths(const std::string& vertexPath, const std::string& fragmentPath) {

        Shader vertexShader = generateShaderPath(GL_VERTEX_SHADER, vertexPath);
        Shader fragmentShader = generateShaderPath(GL_FRAGMENT_SHADER, fragmentPath);

        glAttachShader(id, vertexShader.getId());
        glAttachShader(id, fragmentShader.getId());

        linkProgram();
    }

    Shader* vShaderPointer(size_t index) const {
        if (index < 0 || index >= countVShaders) {
            std::cout << "WARNING: WRONG VSHADER RETURN INDEX " << std::endl;
            return nullptr;
        }

        return shaders[index];
    }
    Shader* fShaderPointer(size_t index) const {
        if (index >= shaders.size() || index < countVShaders) {
            std::cout << "WARNING: WRONG FSHADER RFETURN INDEX " << std::endl;
            return nullptr;
        }

        return shaders[index];
    }


    void attachVShader(Shader* vertexShader) {
        if (vertexShader->sType() != GL_VERTEX_SHADER) {
            std::cout << "ERROR: INCORRECT ARGUMENT FOR attachVShader " << std::endl;
            return;
        }

        shaders.insert(shaders.begin() + countVShaders, vertexShader);
        countVShaders++;
        glAttachShader(id, vertexShader->getId());
    }

    void attachFShader(Shader* fragmentShader) {
        if (fragmentShader->sType() != GL_FRAGMENT_SHADER) {
            std::cout << "ERROR: INCORRECT ARGUMENT FOR attachFShader " << std::endl;
            return;
        }

        shaders.push_back(fragmentShader);
        glAttachShader(id, fragmentShader->getId());
    }


    void detachVShader(Shader* vertexShader) {
        if (vertexShader->sType() != GL_VERTEX_SHADER) {
            std::cout << "ERROR: INCORRECT ARGUMENT FOR detachVShader " << std::endl;
            return;
        }

        short i = countVShaders - 1;

        while (i >= 0 && vertexShader != shaders[i]) {
            i--;
        }

        if (i == -1) {
            std::cout << "WARNING: WRONG VSHADER DETACHMENT POINTER " << std::endl;
            return;
        }

        detachVShader(i);
    }

    void detachFShader(Shader* fragmentShader) {
        if (fragmentShader->sType() != GL_FRAGMENT_SHADER) {
            std::cout << "ERROR: INCORRECT ARGUMENT FOR detachFShader " << std::endl;
            return;
        }

        short i = shaders.size();

        while (i >= countVShaders && fragmentShader != shaders[i]) {
            i--;
        }

        if (i == countVShaders - 1) {
            std::cout << "WARNING: WRONG FSHADER DETACHMENT POINTER " << std::endl;
            return;
        }

        detachFShader(i);
    }


    void detachVShader(size_t index) {
        if (index < 0 || index >= countVShaders) {
            std::cout << "WARNING: WRONG VSHADER DETACHMENT INDEX " << std::endl;
            return;
        }

        glDetachShader(id, shaders[index]->getId());
        shaders.erase(shaders.begin() + index);
        countVShaders--;
    }

    void detachFShader(size_t index) {
        if (index >= shaders.size() || index < countVShaders) {
            std::cout << "WARNING: WRONG FSHADER DETACHMENT INDEX " << std::endl;
            return;
        }

        glDetachShader(id, shaders[index]->getId());
        shaders.erase(shaders.begin() + index);
    }

    void linkProgram() {
        glLinkProgram(id);
        checkLinkingErrors();
    }

    GLint getId() const {
        return id;
    }

    void use() const {
        glUseProgram(id);
    }

    GLint getUniformLocation(const std::string& name) {
        return glGetUniformLocation(id, name.c_str());
    }


    void setBool(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
    }
    void setBool(GLint location, bool value) const {
        glUniform1i(location, (int)value);
    }


    void setInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }
    void setInt(GLint location, int value) const {
        glUniform1i(location, value);
    }


    void setFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }
    void setFloat(GLint location, float value) const {
        glUniform1f(location, value);
    }


    void setVec2(const std::string& name, const glm::vec2& value) const {
        glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string& name, float x, float y) const {
        glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
    }

    void setVec2(GLint location, const glm::vec2& value) const {
        glUniform2fv(location, 1, &value[0]);
    }
    void setVec2(GLint location, float x, float y) const {
        glUniform2f(location, x, y);
    }


    void setVec3(const std::string& name, const glm::vec3& value) const {
        glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string& name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
    }

    void setVec3(GLint location, const glm::vec3& value) const {
        glUniform3fv(location, 1, &value[0]);
    }
    void setVec3(GLint location, float x, float y, float z) const {
        glUniform3f(location, x, y, z);
    }


    void setVec4(const std::string& name, const glm::vec4& value) const {
        glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string& name, float x, float y, float z, float w) const {
        glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
    }

    void setVec4(GLint location, const glm::vec4& value) const {
        glUniform4fv(location, 1, &value[0]);
    }
    void setVec4(GLint location, float x, float y, float z, float w) const {
        glUniform4f(location, x, y, z, w);
    }


    void setMat2(const std::string& name, const glm::mat2& mat) const {
        glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat2(GLint location, const glm::mat2& mat) const {
        glUniformMatrix2fv(location, 1, GL_FALSE, &mat[0][0]);
    }


    void setMat3(const std::string& name, const glm::mat3& mat) const {
        glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat3(GLint location, const glm::mat3& mat) const {
        glUniformMatrix3fv(location, 1, GL_FALSE, &mat[0][0]);
    }


    void setMat4(const std::string& name, const glm::mat4& mat) const {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat4(GLint location, const glm::mat4& mat) const {
        glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
    }


    ~GlProgram() {
        glDeleteProgram(id);
        std::cout << "program nr.: " << id << " has been deleted." << std::endl;
    }

private:

    void checkLinkingErrors() {
        GLint success;
        GLchar infoLog[1024];

        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(id, 1024, nullptr, infoLog);
            std::cerr << "ERROR: PROGRAM LINKING ERROR " << std::endl
                << infoLog << success << std::endl << " -- --------------------------------------------------- -- " << std::endl;
        }
    }
};

#endif