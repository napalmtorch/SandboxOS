#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <gui/control.h>

namespace std
{
    namespace gui
    {
        class container : public control
        {
            public:
                gfx::image framebuffer;

            public:
                container(int x, int y, int w, int h, char* label, container* parent = NULL);

            public:
                void dispose() override;
                void update()  override;
                void draw()    override;
                void render()  override;
        };
    }
}