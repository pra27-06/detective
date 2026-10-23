#include "VNEngine.h"
#include <windows.h>
#include <iostream>
#include <exception>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)hPrevInstance;
    (void)lpCmdLine;
    try {
        VNEngine vnEngine;
        if (!vnEngine.init(hInstance, nCmdShow)) {
            MessageBoxA(NULL, "Failed to initialize Case Closed Graphical Visual Novel Engine.", "Initialization Error", MB_ICONERROR | MB_OK);
            return 1;
        }
        vnEngine.run();
    } catch (const std::exception& ex) {
        MessageBoxA(NULL, ex.what(), "Fatal Visual Novel Error", MB_ICONERROR | MB_OK);
        return 1;
    } catch (...) {
        MessageBoxA(NULL, "An unknown fatal runtime error occurred in the Graphical Visual Novel.", "Fatal Error", MB_ICONERROR | MB_OK);
        return 1;
    }
    return 0;
}

// Fallback entry point in case compiler targets console subsystem
int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOW);
}
