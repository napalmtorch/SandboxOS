#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <arraylist.h>
#include <gui/container.h>

namespace std
{
    namespace gui
    {
        class titlebar;        

        class window : public container
        {
            friend class titlebar;
            
            protected:
                titlebar* _tbar;
                bool      _moving, _resizing;
            
            public:
                window(int x, int y, int w, int h, char* label);

            public:
                void dispose() override;
                void update() override;
                void draw() override;
                void render() override;
        };
    }
}