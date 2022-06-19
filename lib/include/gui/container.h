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
        class container : public control
        {
            public:
                gfx::image framebuffer;

            private:
                ivec2d_t            _oldsz;
                arraylist<control*> _ctrls;

            public:
                container(int x, int y, int w, int h, char* label, container* parent = NULL);

            public:
                void dispose() override;
                void update()  override;
                void draw()    override;
                void render()  override;

            public:
                bool add_ctrl(control* ctrl);
                bool remove_ctrl(control* ctrl);
                bool contains_ctrl(control* ctrl);

            public:
                arraylist<control*>* controls();
        };
    }
}