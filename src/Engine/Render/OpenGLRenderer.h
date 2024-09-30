#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H
#include <iostream>
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include "Renderer.h"
#include "../../../imgui/imgui_impl_sdl2.h"
#include "../../../imgui/imgui_impl_opengl3.h"

namespace Engine::Render {

    class OpenGLRenderer final : public Renderer<Common::ColorRGB>
    {
    protected:
        float renderScale = 1.0f;

        const char* vertexShaderSource = R"(precision mediump float;

attribute vec2 aPos;
varying vec2 TexCoord;

void main() {
    gl_Position = vec4(aPos.xy, 0.0, 1.0);
    TexCoord = (aPos.xy + 1.0) * 0.5;
}
)";

        const char* fragmentShaderSource = R"(precision mediump float;

varying vec2 TexCoord;
uniform sampler2D texture1;

void main() {
    gl_FragColor = texture2D(texture1, TexCoord);
}
)";

        SDL_Window* window = nullptr;
        GLuint vertexShader = 0;
        GLuint fragmentShader = 0;
        GLuint shaderProgram = 0;
        GLuint VBO = 0;
        GLuint EBO = 0;
        GLuint texture = 0;
        int textureWidth = 0;
        int textureHeight = 0;

        // Full-screen quad vertices
        const float quadVertices[8] = {
            -1.0f,  1.0f,
            -1.0f, -1.0f,
             1.0f, -1.0f,
             1.0f,  1.0f,
        };
        const unsigned int quadIndices[6] = {
            0, 1, 2,
            2, 3, 0
        };

        void updateTexture() const
        {
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGB,
                backBuffer->getWidth(),
                backBuffer->getHeight(),
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                backBuffer->getBuffer().data()
            );
        }

        void checkCompileErrors(const GLuint shader, const std::string &type)
        {
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

        void onInit() override
        {
            textureWidth = backBuffer->getWidth() * renderScale;
            textureHeight = backBuffer->getHeight() * renderScale;

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
            const SDL_GLContext context = SDL_GL_CreateContext(window);

            // Load OpenGL function pointers using Glad
            if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
                // Handle error
                SDL_GL_DeleteContext(context);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return;
            }

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

            // Set up VBO and EBO for the quad
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
            glEnableVertexAttribArray(0);

            // Create a texture in OpenGL
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);

            // Set texture parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            auto glsl_version = "#version 150";

            // IMGUI
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;
            ImGui_ImplSDL2_InitForOpenGL(window, context);
            ImGui_ImplOpenGL3_Init(glsl_version);
        }

        void onUpdate() override
        {
            updateTexture();

            // Use shader and bind texture
            glUseProgram(shaderProgram);

            // Draw the full-screen quad
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


            // start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();


            // a window is defined by Begin/End pair
            {
                static int counter = 0;
                // get the window size as a base for calculating widgets geometry
                int sdl_width = 0, sdl_height = 0, controls_width = 0;
                SDL_GetWindowSize(window, &sdl_width, &sdl_height);
                controls_width = sdl_width;
                // make controls widget width to be 1/3 of the main window width
                if ((controls_width /= 3) < 300) { controls_width = 300; }

                // position the controls widget in the top-right corner with some margin
                ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
                // here we set the calculated width and also make the height to be
                // be the height of the main window also with some margin
                ImGui::SetNextWindowSize(
                    ImVec2(static_cast<float>(controls_width), static_cast<float>(sdl_height - 20)),
                    ImGuiCond_Always
                    );
                // create a window and append into it
                ImGui::Begin("Controls", NULL, ImGuiWindowFlags_NoResize);

                ImGui::Dummy(ImVec2(0.0f, 1.0f));
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Platform");
                ImGui::Text("%s", SDL_GetPlatform());
                ImGui::Text("CPU cores: %d", SDL_GetCPUCount());
                ImGui::Text("RAM: %.2f GB", SDL_GetSystemRAM() / 1024.0f);

                // buttons and most other widgets return true when clicked/edited/activated
                if (ImGui::Button("Counter button"))
                {
                    std::cout << "counter button clicked\n";
                    counter++;
                }
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::End();
            }

            // rendering
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


            // Swap buffers.
            SDL_GL_SwapWindow(window);
        }

        void onDestroy() override
        {
            SDL_DestroyWindow(window);
        }

    public:
        static std::unique_ptr<OpenGLRenderer> create()
        {
            return std::make_unique<OpenGLRenderer>();
        }

        void setRenderScale(const float val)
        {
            if(val < 0.001f || val > 10.0f)
                return;

            renderScale = val;
        }
    };
}


#endif //OPENGLRENDERER_H
