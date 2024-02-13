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

    // ����ͼ��
    image.Save(Path, Gdiplus::ImageFormatPNG);
    image.Destroy();
    return true;
}

std::string GetCurrentTimeAsString() {
    // ��ȡ��ǰʱ��
    time_t rawtime;
    struct tm timeinfo;
    char buffer[80];

    time(&rawtime);
    localtime_s(&timeinfo, &rawtime);

    // ��ʽ��ʱ��Ϊ�ַ���
    strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", &timeinfo);
    return std::string(buffer);
}

int main() {
    // ע���ȼ�
    RegisterHotkey(NULL, 1, MOD_CONTROL | MOD_SHIFT, 'A');

    // ��ͼ�����浽�ļ�
    int startX, startY, endX, endY;

    printf("����˵�1 X����:\n");
    scanf("%d", &startX);
    printf("����˵�1 Y����:\n");
    scanf("%d", &startY);
    printf("����˵�2 X����:\n");
    scanf("%d", &endX);
    printf("����˵�2 Y����:\n");
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
