#include <iostream>
#include <vector>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>
#include "OpenGLRenderer.h"
#include "Screen.h"

namespace Render
{
    const char* vertexShaderSource = R"(precision mediump float;

attribute vec2 aPos;  // "attribute" is used instead of "in"
varying vec2 TexCoord;  // "varying" is used instead of "out"

void main() {
    gl_Position = vec4(aPos.xy, 0.0, 1.0);
    TexCoord = (aPos.xy + 1.0) * 0.5;  // Convert [-1,1] range to [0,1] for texture coordinates
}
)";

    const char* fragmentShaderSource = R"(precision mediump float;

varying vec2 TexCoord;  // "varying" is used to receive interpolated data
uniform sampler2D texture1;

void main() {
    gl_FragColor = texture2D(texture1, TexCoord);  // Use gl_FragColor instead of user-defined outputs
}
)";

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint shaderProgram;
    GLuint VAO, VBO, EBO;
    std::vector<Color3> textureData;
    GLuint texture;
    int textureWidth, textureHeight;

    // Full-screen quad vertices
    float quadVertices[] = {
        // positions
        -1.0f,  1.0f,
        -1.0f, -1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f,
    };
    unsigned int quadIndices[] = {
        0, 1, 2,
        2, 3, 0
    };

    SDL_Window* window;

    void OpenGLRenderer::setRenderScale(float val)
    {
        if(val < 0.001f || val > 10.0f)
            return;

        renderScale = val;
    }

    void OpenGLRenderer::assignData(std::vector<Color3>& data, Screen* screen)
    {
        data = screen->getBuffer();
    }

    void checkCompileErrors(GLuint shader, std::string type) {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n";
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n";
            }
        }
    }

    void OpenGLRenderer::on_init()
    {
        textureWidth = screen->getWidth() * renderScale;
        textureHeight = screen->getHeight() * renderScale;

        // Set GL attributes.
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetSwapInterval(0);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        // Create window and OpenGL context.
        window = SDL_CreateWindow(
            "Project Mercury",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            textureWidth,
            textureHeight,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
        );
        SDL_GL_CreateContext(window);

        // Build and compile shaders
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);
        checkCompileErrors(vertexShader, "VERTEX");

        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);
        checkCompileErrors(fragmentShader, "FRAGMENT");

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        checkCompileErrors(shaderProgram, "PROGRAM");

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Set up VAO, VBO, and EBO for the quad
        glGenBuffers(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindBuffer(GL_VERTEX_ARRAY, VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Generate a procedural texture.
        textureData = std::vector<Color3>(screen->getWidth() * screen->getHeight() * 3); // RGB format

        // Create a texture in OpenGL
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    void OpenGLRenderer::on_update()
    {
        for(int i = 0; i < 4096; i++) {
            int w = screen->getWidth();
            int h = screen->getHeight();
            screen->drawRect(
                RectF(rand() % w, rand() % h, rand() % w, rand() % h),
                Color3(rand() % 255, rand() % 255, rand() % 255)
            );
        }

        assignData(textureData, screen);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen->getWidth(), screen->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, textureData.data());

        // Use shader and bind texture
        glUseProgram(shaderProgram);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Draw the full-screen quad
        glBindBuffer(GL_VERTEX_ARRAY, VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap buffers.
        SDL_GL_SwapWindow(window);
    }

    void OpenGLRenderer::on_destroy()
    {
        SDL_DestroyWindow(window);
    }

}
