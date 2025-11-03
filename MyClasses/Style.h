//
// Created by Hristo Dinkov on 3.11.2025 г..
//
#pragma once
#ifndef RAWENGINE_STYLE_H
#define RAWENGINE_STYLE_H
#include "imgui.h"

struct ImGuiStyle;

class Style {
public:

	void SetupImGuiStyle()
{
	// BlackDevil style by Naeemullah1 from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.5f;
	style.WindowPadding = ImVec2(10.0f, 10.0f);
	style.WindowRounding = 5.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(20.0f, 20.0f);
	style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 5.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 5.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(5.0f, 5.0f);
	style.FrameRounding = 5.0f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(8.0f, 5.0f);
	style.ItemInnerSpacing = ImVec2(5.0f, 5.0f);
	style.CellPadding = ImVec2(5.0f, 5.0f);
	style.IndentSpacing = 10.0f;
	style.ColumnsMinSpacing = 5.0f;
	style.ScrollbarSize = 10.0f;
	style.ScrollbarRounding = 5.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 0.0f;
	style.TabRounding = 10.0f;
	style.TabBorderSize = 0.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(0.78431374f, 0.78431374f, 0.78431374f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.43529412f, 0.40784314f, 0.3137255f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05882353f, 0.11764706f, 0.05882353f, 0.85f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.05882353f, 0.11764706f, 0.15686275f, 0.78431374f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.078431375f, 0.078431375f, 0.078431375f, 0.78431374f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.39215687f, 0.0f, 0.0f, 0.89f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.05882353f, 0.11764706f, 0.15686275f, 0.78431374f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.11764706f, 0.23529412f, 0.3529412f, 0.78431374f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.3529412f, 0.3529412f, 0.11764706f, 0.78431374f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.05882353f, 0.11764706f, 0.15686275f, 0.78431374f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.05882353f, 0.11764706f, 0.15686275f, 0.78431374f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.05882353f, 0.11764706f, 0.15686275f, 0.19607843f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.078431375f, 0.078431375f, 0.078431375f, 0.78431374f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05882353f, 0.11764706f, 0.15686275f, 0.78431374f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.11764706f, 0.3529412f, 0.23529412f, 0.78431374f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.11764706f, 0.3529412f, 0.3529412f, 0.78431374f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.11764706f, 0.5882353f, 0.23529412f, 0.78431374f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.11764706f, 0.5882353f, 0.23529412f, 0.78431374f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.11764706f, 0.3529412f, 0.23529412f, 0.78431374f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.11764706f, 0.5882353f, 0.23529412f, 0.78431374f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.3529412f, 0.3529412f, 0.11764706f, 0.78431374f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3529412f, 0.47058824f, 0.23529412f, 0.78431374f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.5882353f, 0.3529412f, 0.23529412f, 0.78431374f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.05882353f, 0.11764706f, 0.15686275f, 0.78431374f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.11764706f, 0.3529412f, 0.3529412f, 0.78431374f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.11764706f, 0.5882353f, 0.23529412f, 0.78431374f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.3529412f, 0.3529412f, 0.23529412f, 0.78431374f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.11764706f, 0.3529412f, 0.3529412f, 0.78431374f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.5882353f, 0.3529412f, 0.23529412f, 0.78431374f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.05882353f, 0.11764706f, 0.15686275f, 0.78431374f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.5882353f, 0.3529412f, 0.3529412f, 0.78431374f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.5882353f, 0.23529412f, 0.23529412f, 0.78431374f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.3529412f, 0.3529412f, 0.11764706f, 0.78431374f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.3529412f, 0.47058824f, 0.23529412f, 0.78431374f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.5882353f, 0.3529412f, 0.23529412f, 0.78431374f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.05882353f, 0.11764706f, 0.15686275f, 0.78431374f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.5882353f, 0.3529412f, 0.3529412f, 0.78431374f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39215687f, 0.78431374f, 0.39215687f, 0.78431374f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.42745098f, 0.34901962f, 0.78431374f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.0f, 0.3529412f, 0.39215687f, 0.78431374f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.19607843f, 0.5882353f, 0.5882353f, 0.78431374f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882353f, 0.1882353f, 0.2f, 0.78431374f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.30980393f, 0.30980393f, 0.34901962f, 0.78431374f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.22745098f, 0.22745098f, 0.24705882f, 0.78431374f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.78431374f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.06f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.39215687f, 0.3529412f, 0.39215687f, 0.39215687f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);
}
};
#endif //RAWENGINE_STYLE_H