#include "imgui_layer.h"

#include <imgui.h>
#include <rlImGui.h>

bool initImGui()
{
    rlImGuiSetup(true);

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImFont *font = io.Fonts->AddFontFromFileTTF(
        RESOURCES_PATH "fonts/Roboto-Regular.ttf",
        24.0f);

    if (!font)
    {
        return false;
    }

    io.FontDefault = font;
    rlImGuiReloadFonts();

    ImGuiStyle &style = ImGui::GetStyle();
    style.ScaleAllSizes(1.f);
    ImGui::StyleColorsClassic();

    return true;
}

void beginImGuiFrame()
{
    rlImGuiBegin();

    ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
    ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    ImGui::PopStyleColor(2);
}

void endImGuiFrame()
{
    rlImGuiEnd();
}

void shutdownImGui()
{
    rlImGuiShutdown();
}