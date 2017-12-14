#include "stdafx.h"
#include "Form.h"
#include <Windows.h>
#include <assert.h>

static const PWSTR s_windowClassName = L"Form Window";


Form::Form(int width, int height)
    : m_bitmap(nullptr)
{
    m_instance = GetModuleHandle(nullptr);

    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_instance;
    wcex.hIcon = NULL;
    wcex.hCursor = nullptr;
    wcex.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = s_windowClassName;
    wcex.hIconSm = NULL;
    RegisterClassExW(&wcex);

    DWORD dwStyle = WS_OVERLAPPED | WS_THICKFRAME | WS_SYSMENU;
    DWORD dwExStyle = 0;

    RECT rc = { 0, 0, width*4, height*4 };
    AdjustWindowRect(&rc, dwStyle, FALSE);

    m_window = CreateWindowEx(
        dwExStyle,
        s_windowClassName,
        _T("Form"),
        dwStyle,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rc.right - rc.left,
        rc.bottom - rc.top,
        nullptr,
        nullptr,
        m_instance,
        this);

    SetWindowLongPtrW(m_window, GWLP_USERDATA, LONG_PTR(this));
    ShowWindow(m_window, SW_SHOW);
    UpdateWindow(m_window);
    SetFocus(m_window);

    m_bitmap = CreateBitmap(width, height, 1, 1, nullptr);
    assert(m_bitmap != nullptr);
}

Form::~Form()
{
    DeleteObject(m_bitmap);
    UnregisterClassW(s_windowClassName, m_instance);
}

void Form::SetImage(HBITMAP bitmap)
{
    m_bitmap = bitmap;
    PostMessage(m_window, WM_PAINT, 0, 0);
}

bool Form::HandleEvents()
{
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            return false;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

void Form::Quit()
{
    PostQuitMessage(0);
}

void Form::Render(const void* bits, size_t size)
{
    if (m_bitmap != nullptr)
    {
        SetBitmapBits(m_bitmap, (DWORD)size, bits);

        HDC hdc = GetDC(m_window);
        Blit(GetDC(m_window));
    }
}

void Form::Blit(HDC hdc)
{
    if (m_bitmap != nullptr)
    {
        BITMAP bm;
        GetObject(m_bitmap, sizeof(bm), &bm);

        RECT rc;
        GetClientRect(m_window, &rc);

        HDC hdcMem = CreateCompatibleDC(hdc);
        SelectObject(hdcMem, m_bitmap);
        StretchBlt(hdc, 0, 0, rc.right, rc.bottom, hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
        DeleteDC(hdcMem);
    }
}

LRESULT CALLBACK Form::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Form* form = reinterpret_cast<Form*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
    if (form != nullptr)
    {
        switch (message)
        {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_PAINT:
            if (form->m_bitmap != nullptr)
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hWnd, &ps);
                form->Blit(hdc);
                EndPaint(hWnd, &ps);
                return 0;
            }
        }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}