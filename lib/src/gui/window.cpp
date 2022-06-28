#include <gui/window.h>
#include <core/kernel.h>

namespace std
{
    namespace gui
    {
        window::window(int x, int y, int w, int h, char* label) : container(x, y, w, h, label, NULL)
        {
            _type = control_type::window;
            _tbar = new titlebar(this);
            state = window_state::normal;
            update();
            draw();
            render();
        }

        void window::dispose()
        {
            container::dispose();
            _tbar->dispose();
        }
        
        void window::update()
        {
            container::update();

            if (os::kernel::shell->winmgr.active_win != this) { this->_flags.active = false; }

            if (_tbar != NULL) { _tbar->update(); }
        }

        void window::draw()
        {
            container::draw();

            if (_tbar != NULL) { _tbar->draw(); }
        }

        void window::render()
        {
            if (_tbar != NULL) { _tbar->render(); }
            container::render();
        }
    }
}