#pragma once

#include <Windows.h>

class Form
{
public:
    Form(int width, int height);
    ~Form();

    bool HandleEvents();
    void Quit();

    void SetImage(HBITMAP bitmap);
    void Render(const void* bits, size_t size);

private:
    HWND      m_window;
    HINSTANCE m_instance;
    HBITMAP   m_bitmap;

    void Blit(HDC hdc);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

