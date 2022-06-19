#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <arraylist.h>
#include <gui/control.h>

namespace std
{
    namespace gui
    {
        class button : public control
        {
            public:
                button(int x, int y, int w, int h, char* label, container* parent = NULL);

            public:
                void dispose() override;
                void update()  override;
                void draw()    override;
                void render()  override;
        };
    }
}