// Im sorry for this mess but I was too lazy and just pasted my libraries from different projects together
// to ensure evrything works without checking which exact library I need

#include "help.h"
#include "wndhide.h"
#include "imgui/imgui.h"
#include "imgui/imconfig.h"
#include "imgui/imgui_internal.h"
#include "imgui/imstb_textedit.h"
#include "imgui/imstb_truetype.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/ImGui_FreeType.h"
#include <sstream>
#include <vector>
#include <string>
#include <string.h>
#include <fstream>
#include <winbase.h>
#include <tchar.h>
#include <WinInet.h>
#include <tchar.h>
#include <urlmon.h>
#include <comdef.h>
#include <tchar.h>
#include <shlobj.h>
#include <cstring>
#include <urlmon.h>
#include <wincrypt.h>
#include <ShlObj.h>
#include <filesystem>
#include <random>
#include <Windows.h>
#include <vector>
#include <mmsystem.h>
#include <winbase.h>
#include <tchar.h>
#include <direct.h>
#include <sstream>
#include <tlhelp32.h>
#include <iostream>
#include <WinUser.h>
#include <thread>
#include <chrono>
#include <ChString.h>
#include <future>
#include <Lmcons.h>
#include <wtsapi32.h>
#include <psapi.h> 
#pragma comment(lib,"Wininet.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "shell32.lib")

using namespace std;
#define _WIN32_WINNT 0x0500

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, LOADER_BRAND, NULL };
    RegisterClassEx(&wc);
    main_hwnd = CreateWindow(wc.lpszClassName, LOADER_BRAND, WS_POPUP, 0, 0, 5, 5, NULL, NULL, wc.hInstance, NULL);

    if (!CreateDeviceD3D(main_hwnd)) {
        CleanupDeviceD3D();
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ShowWindow(main_hwnd, SW_HIDE);
    UpdateWindow(main_hwnd);

    // defining variables
    bool hidden = false;
    string name;

    // getting username
    char username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    GetUserName((TCHAR*)username, &username_len);
    string user = username;
    const char* userdisplay = username;
    DWORD pid = 0;
    // initializing hwnd of target window
    HWND windowhandle = nullptr;

    ImGui::CreateContext(); // creates imgui context

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // imgui style (half broken sry)
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        void Theme(); {
            ImVec4* colors = ImGui::GetStyle().Colors;
            colors[ImGuiCol_Text] = ImVec4(0.77f, 0.76f, 0.76f, 1.00f);
            colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
            colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.09f, 0.88f);
            colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
            colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
            colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.18f, 0.18f, 0.54f);
            colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.15f, 0.15f, 0.40f);
            colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.26f, 0.26f, 0.67f);
            colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
            colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.29f, 0.48f, 1.00f);
            colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
            colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
            colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
            colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
            colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
            colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
            colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.80f, 0.06f, 1.00f);
            colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
            colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_Button] = ImVec4(0.11f, 0.11f, 0.11f, 0.88f);
            colors[ImGuiCol_ButtonHovered] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
            colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
            colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
            colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
            colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
            colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
            colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
            colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
            colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
            colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
            colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.35f, 0.58f, 0.86f);
            colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
            colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.41f, 0.68f, 1.00f);
            colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
            colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
            colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
            colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
            colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
            colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
            colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
            colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
            colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
            colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
            colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
            colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
            colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
            colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
            colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

            style.WindowPadding = ImVec2(8.00f, 8.00f);
            style.FramePadding = ImVec2(0.90f, 0.90f);
            style.CellPadding = ImVec2(4.00f, 2.00f);
            style.ItemSpacing = ImVec2(8.00f, 4.00f);
            style.ItemInnerSpacing = ImVec2(4.00f, 4.00f);
            style.TouchExtraPadding = ImVec2(0.00f, 3.00f);
            style.IndentSpacing = 21.00f;
            style.ScrollbarSize = 14.00f;
            style.GrabMinSize = 10.00f;
            style.WindowBorderSize = 0.00f;
            style.ChildBorderSize = 0.00f;
            style.PopupBorderSize = 0.00f;
            style.FrameBorderSize = 0.00f;
            style.TabBorderSize = 0.00f;
            style.WindowRounding = 12.00f;
            style.ChildRounding = 12.00f;
            style.FrameRounding = 12.00f;
            style.PopupRounding = 12.00f;
            style.ScrollbarRounding = 9.00f;
            style.GrabRounding = 12.00f;
            style.LogSliderDeadzone = 4.00f;
            style.TabRounding = 12.00f;
            style.WindowTitleAlign = ImVec2(0.00f, 0.50f);
            style.WindowMenuButtonPosition = 0;
            style.ColorButtonPosition = 1;
            style.ButtonTextAlign = ImVec2(0.50f, 0.50f);
            style.SelectableTextAlign = ImVec2(0.00f, 0.00f);
            style.DisplaySafeAreaPadding = ImVec2(3.00f, 3.00f);
        }
    }

    ImGui_ImplWin32_Init(main_hwnd); // initialize the imgui window
    ImGui_ImplDX9_Init(g_pd3dDevice);

    // the sloppy way of adding different types of font sizes... (i didnt include them in the source code so you will have to have pdd.ttf in the same directory as the .exe)
    ImFont* def = io.Fonts->AddFontFromFileTTF("pdd.ttf", 17.0f);
    ImFont* xe = io.Fonts->AddFontFromFileTTF("pdd.ttf", 24.0f);
    ImFont* smol = io.Fonts->AddFontFromFileTTF("pdd.ttf", 13.0f);
    ImFont* plus = io.Fonts->AddFontFromFileTTF("pdd.ttf", 19.0f);


    DWORD window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus;


    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }


        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {

            ImVec2 position;
            ImDrawList* draw;
            static ImDrawData drawData = ImDrawData();

            ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT));
            ImGui::SetNextWindowBgAlpha(1.0f);

            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 4);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 1);


            ImGui::Begin(LOADER_BRAND, NULL, ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
            {
                position = ImGui::GetWindowPos();
                draw = ImGui::GetWindowDrawList();
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
                ImGui::PushFont(def);
                ImGui::Text(("  WindowHide GUI | Version 1                 "));
                ImGui::SameLine();
                if (ImGui::Button((" + "))) {

                    startup();
                }
                ImGui::SameLine();
                if (ImGui::Button(("  EXIT "))) {

                    return(0);
                }
                //welcome taubenrgb
                ImGui::Separator();
                ImGui::PopFont();
                ImGui::PushFont(xe);
                ImGui::TextColored(ImColor(180, 12, 7), "                      a");
                ImGui::SameLine();
                ImGui::TextColored(ImColor(180, 12, 7), "n");
                ImGui::SameLine();
                ImGui::TextColored(ImColor(180, 12, 7), "t");
                ImGui::SameLine();
                ImGui::TextColored(ImColor(180, 12, 7), "i");
                ImGui::SameLine();
                ImGui::TextColored(ImColor(180, 12, 7), "SS");
                //ImGui::SameLine();
                //ImGui::TextColored(ImColor(180, 12, 7), "S");
                

                ImGui::PopFont();
                ImGui::Separator();
                //Aimbot
                ImGui::TextColored(ImColor(199, 196, 196), ("Welcome, "));
                ImGui::SameLine();
                ImGui::TextColored(ImColor(199, 196, 196), (userdisplay));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
                ImGui::InputText("Window Title", &name);



                if (ImGui::Checkbox((" Hide from ScreenCapture"), &hide)) {
                    
                    SetWindowDisplayAffinity(myself, WDA_EXCLUDEFROMCAPTURE);

                    const char* antiwnd = name.c_str();
                    windowhandle = FindWindowA(0, antiwnd);
                    SetWindowText(windowhandle, "Microsoft Word");
                    GetWindowThreadProcessId(windowhandle, &pid);
                    if (!pid)
                    {
                        getchar();
                        return 1;
                    }
                    bool status = wndhide::hide_window(pid, windowhandle);
                    hidden = true;
                    getchar();


                    if (!hide) {
                        hide = true;

                    }
                }
                ImGui::TextColored(ImColor(199, 196, 196), (" "));
                ImGui::PushFont(smol);
                ImGui::TextColored(ImColor(199, 196, 196), "PID: %u", pid);
                ImGui::TextColored(ImColor(199, 196, 196), "HWND: %u", windowhandle);
                ImGui::TextColored(ImColor(199, 196, 196), "Base Status: %u", hide);
                ImGui::PopFont();
                ImGui::Separator();
                ImGui::TextColored(ImColor(199, 196, 196), ("Window is: "));

                if (hidden) {
                    ImGui::SameLine();
                    ImGui::TextColored(ImColor(0, 200, 10), "Hidden");
                }
                if (!hidden) {
                    ImGui::SameLine();
                    ImGui::TextColored(ImColor(200, 0, 10), "Visible");
                }




                ImGui::PushFont(def);
                ImGui::TextColored(ImColor(199, 196, 196), ("Created by"));
                ImGui::SameLine();


                ImGui::TextColored(ImColor(27, 116, 211), ("taubensuppe#0283"));

                if (ImGui::Button("Help")) {
                    MessageBoxA(0, "1. Hover your cursor over one of the programs on the taskbar\n2. Note the name of the window\n3. enter the name in the InputBox and press Hide from ScreenCapture\n3. after 2 seconds, the window will only be visible on the monitor, but not on screen recordings or screenshots\n \nTip: To hide more than one window, you can press the ""+"" to the left of EXIT!", "Tutorial", MB_ICONQUESTION);
                }

                ImGui::PopFont();




                //draw->AddLine(ImVec2{ position.x + 0,position.y + 428 }, ImVec2{ position.x + 360,position.y + 428 }, ImColor(90, 90, 90), 4.0f);
                ImGui::PushFont(def);

            }
            ImGui::End();

        }

        ImGui::EndFrame();

        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }


        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);


        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
            ResetDevice();
        }
        if (!loader_active) {
            msg.message = WM_QUIT;
        }
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    DestroyWindow(main_hwnd);
    UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
