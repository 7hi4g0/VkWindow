#ifndef UNICODE
#define UNICODE
#endif

#define VK_USE_PLATFORM_WIN32_KHR

#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <vulkan/vulkan.h>
#include <vulkan/vk_sdk_platform.h>

VkInstance InitVulkan() {
    VkResult err;
    uint32_t extension_count;

    err = vkEnumerateInstanceExtensionProperties(NULL, &extension_count, NULL);
    assert(!err);

    if (extension_count > 0) {
        VkExtensionProperties *extensions = (VkExtensionProperties *) malloc(sizeof(VkExtensionProperties) * extension_count);

        err = vkEnumerateInstanceExtensionProperties(NULL, &extension_count, extensions);
        assert(!err);

        char output[255];
        for (uint32_t i = 0; i < extension_count; i++) {
            sprintf(output, "%s\n", extensions[i].extensionName);
            OutputDebugStringA(output);
        }
    }

    const char *extensions[2] = {VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME};

    VkApplicationInfo app = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = NULL,
        .pApplicationName = "My Game",
        .applicationVersion = 0,
        .pEngineName = "Ny Game Engine",
        .engineVersion = 0,
        .apiVersion = VK_API_VERSION_1_2,
    };
    VkInstanceCreateInfo vulkan_info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = NULL,
        .pApplicationInfo = &app,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = NULL,
        .enabledExtensionCount = 2,
        .ppEnabledExtensionNames = extensions,
    };

    VkInstance vulkan_inst = {};

    err = vkCreateInstance(&vulkan_info, NULL, &vulkan_inst);
    assert(!err);

    return vulkan_inst;
}

void CleanupVulkan(VkInstance vulkan_inst) {
    vkDestroyInstance(vulkan_inst, NULL);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"My Game Window Class";

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"My Game",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL);

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    VkInstance vulkan_inst = InitVulkan();

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    CleanupVulkan(vulkan_inst);

    return 0;
}