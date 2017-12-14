#include "stdafx.h"
#include "Form.h"

#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

#include <intrin.h>
#include "BitField.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    BitField map;
    Form form(64, 32);

    uint32_t idx = 0;
    while (form.HandleEvents())
    {
        uint32_t i = idx % 2048;

        if (map.IsSet(i))
            map.Clear(i);
        else
            map.Set(i);

        idx++;

        form.Render(map.data(), map.size());
        Sleep(10);
    }

    return 0;
}
