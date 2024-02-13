#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <iostream>
#include <string>
#include <ctime>
#include <atlimage.h>

void RegisterHotkey(HWND hwnd, int id, UINT modifiers, UINT vk) {
    if (!RegisterHotKey(hwnd, id, modifiers, vk)) {
        std::cerr << "Failed to register hotkey." << std::endl;
    }
}

void UnregisterHotkey(HWND hwnd, int id) {
    UnregisterHotKey(hwnd, id);
}

bool CaptureScreenPng(int x1,int y1,int x2,int y2 ,CString Path) {
    HDC hdcSrc = ::GetDC(NULL);
    int nBitPerPixel = GetDeviceCaps(hdcSrc, BITSPIXEL);
    int nWidth = abs(x2 - x1);
    int nHeight = abs(y2 - y1);
    CImage image;
    image.Create(nWidth, nHeight, nBitPerPixel);
    BitBlt(image.GetDC(), 0, 0, nWidth, nHeight, hdcSrc, x1, y1, SRCCOPY);

    ::ReleaseDC(NULL, hdcSrc);
    image.ReleaseDC();

    // 保存图像
    image.Save(Path, Gdiplus::ImageFormatPNG);
    image.Destroy();
    return true;
}

std::string GetCurrentTimeAsString() {
    // 获取当前时间
    time_t rawtime;
    struct tm timeinfo;
    char buffer[80];

    time(&rawtime);
    localtime_s(&timeinfo, &rawtime);

    // 格式化时间为字符串
    strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", &timeinfo);
    return std::string(buffer);
}

int main() {
    // 注册热键
    RegisterHotkey(NULL, 1, MOD_CONTROL | MOD_SHIFT, 'A');

    // 截图并保存到文件
    int startX, startY, endX, endY;

    printf("输入端点1 X坐标:\n");
    scanf("%d", &startX);
    printf("输入端点1 Y坐标:\n");
    scanf("%d", &startY);
    printf("输入端点2 X坐标:\n");
    scanf("%d", &endX);
    printf("输入端点2 Y坐标:\n");
    scanf("%d", &endY);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        int count;
        if (msg.wParam == 1) {
            std::string time = GetCurrentTimeAsString();
            CRect rect(startX, startY, endX, endY);
            CString str = time.c_str();
            str.Append(L".png");
            CaptureScreenPng(startX,startY,endX,endY,str);
            std::wcout << L"Screenshot saved as " << time.c_str() << ".png" << std::endl;
        }
    }
        
    return 0;
}
