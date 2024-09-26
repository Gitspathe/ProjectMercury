#include "OpenGLRenderer.h"
#include <iostream>
#include <vector>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>
#include "Screen.h"
#include "../Common/Color.h";

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

    // Generates a procedural bitmap (checkerboard pattern in this case)
    void generateCheckerboardTexture(std::vector<unsigned char>& data, int width, int height) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int i = (y * width + x) * 3;
                if ((x / 16 % 2 == 0 && y / 16 % 2 == 0) || (x / 16 % 2 == 1 && y / 16 % 2 == 1)) {
                    data[i] = 255;  // R
                    data[i + 1] = 255; // G
                    data[i + 2] = 255; // B
                } else {
                    data[i] = 0;   // R
                    data[i + 1] = 0;   // G
                    data[i + 2] = 0;   // B
                }
            }
        }
    }

    void assignData(std::vector<unsigned char>& data, Screen* screen)
    {
        for (int y = 0; y < screen->getHeight(); ++y) {
            for (int x = 0; x < screen->getWidth(); ++x) {
                const Common::Color col = screen->get(x, y);
                const int i = (y * screen->getWidth() + x) * 3;
                data[i] = col.r;     // R
                data[i + 1] = col.g; // G
                data[i + 2] = col.b; // B
            }
        }
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

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint shaderProgram;
    GLuint VAO, VBO, EBO;
    std::vector<unsigned char> textureData;
    GLuint texture;
    int textureWidth = 1024;
    int textureHeight = 512;

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

    void OpenGLRenderer::on_init()
    {
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
            1024,
            512,
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

        // Generate a procedural texture (e.g., checkerboard pattern)
        textureData = std::vector<unsigned char>(textureWidth * textureHeight * 3); // RGB format
        generateCheckerboardTexture(textureData, textureWidth, textureHeight);

        // Create a texture in OpenGL
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Upload the texture data to OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData.data());
    }

    int x = 0;
    void OpenGLRenderer::on_update()
    {
        screen->set(rand() % 1025, rand() % 516, Common::Color::getBlue());
        assignData(textureData, screen);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData.data());
        x++;

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Use shader and bind texture
        glUseProgram(shaderProgram);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Draw the full-screen quad
        glBindBuffer(GL_VERTEX_ARRAY, VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap buffers and poll events
        SDL_GL_SwapWindow(window);
    }

    void OpenGLRenderer::on_destroy()
    {
        SDL_DestroyWindow(window);
    }

}
