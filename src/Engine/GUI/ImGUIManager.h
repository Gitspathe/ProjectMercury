#ifndef IMGUIMANAGER_H
#define IMGUIMANAGER_H

#if DEV_MODE && CLIENT
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"
#include "../Core/Subsystem.h"
#include "../../Engine.h"

namespace Engine::GUI
{
    class ImGUIManager final : public Core::Subsystem, public Core::ISubsystemSDLEventReceiver
    {
    protected:
        bool onInit() override
        {
            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            return true;
        }

        void onEarlyUpdate(float deltaTime) override
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();
        }

        void onRenderGame() override
        {
            static int counter = 0;

            ImGui::ShowDemoWindow();

            // // get the window size as a base for calculating widgets geometry
            // int sdl_width = game.lock()->getScreen().getWidth();
            // int sdl_height = game.lock()->getScreen().getHeight();
            // int controls_width = sdl_width;
            //
            // // make controls widget width to be 1/3 of the main window width
            // if ((controls_width /= 3) < 300) { controls_width = 300; }
            //
            // // position the controls widget in the top-right corner with some margin
            // ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
            //
            // // here we set the calculated width and also make the height to be
            // // be the height of the main window also with some margin
            // ImGui::SetNextWindowSize(
            //     ImVec2(static_cast<float>(controls_width), static_cast<float>(sdl_height - 20)),
            //     ImGuiCond_Always
            // );
            //
            // // create a window and append into it
            // ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_NoResize);
            //
            // ImGui::Dummy(ImVec2(0.0f, 1.0f));
            // ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Platform");
            // ImGui::Text("%s", SDL_GetPlatform());
            // ImGui::Text("CPU cores: %d", SDL_GetCPUCount());
            // ImGui::Text("RAM: %.2f GB", SDL_GetSystemRAM() / 1024.0f);
            //
            // // buttons and most other widgets return true when clicked/edited/activated
            // if (ImGui::Button("Counter button")){
            //     std::cout << "counter button clicked\n";
            //     counter++;
            // }
            // ImGui::SameLine();
            // ImGui::Text("counter = %d", counter);
            //
            // ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
    public:
        std::string getName() override
        {
            return "ImGUIManager";
        }

        uint32_t getOrder() override
        {
            return Core::SubsystemExecOrder::IMGUI;
        }

        void handleEvent(SDL_Event &ev) override
        {
            ImGui_ImplSDL2_ProcessEvent(&ev);
        }
    };
}

#endif
#endif //IMGUIMANAGER_H
