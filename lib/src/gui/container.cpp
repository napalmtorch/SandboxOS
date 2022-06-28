#include <gui/container.h>
#include <core/kernel.h>

namespace std
{
    namespace gui
    {
        container::container(int x, int y, int w, int h, char* label, container* parent) : control(x, y, w, h, label, control_type::container, parent)
        {
            framebuffer = gfx::image(w, h);
            _oldsz = ivec2d_t(w, h);
            _ctrls = arraylist<control*>();
            update();
            draw();
            render();
        }

        void container::dispose()
        {
            control::dispose();
            framebuffer.dispose();
            for (size_t i = 0; i < _ctrls.length(); i++) { _ctrls[i]->dispose(); }
            _ctrls.dispose();
        }

        void container::update()
        {
            control::update();

            if (_info.bounds.w != _oldsz.x || _info.bounds.h != _oldsz.y)
            {
                _oldsz.x = _info.bounds.w;
                _oldsz.y = _info.bounds.h;
                framebuffer.dispose();
                framebuffer = gfx::image(_info.bounds.w, _info.bounds.h);
                draw();
                render();
            }

            for (size_t i = 0; i < _ctrls.length(); i++) { _ctrls[i]->update(); }
        }

        void container::draw()
        {
            control::draw();

            framebuffer.clear(_info.style->color(color_index::bg));
            
            if (_info.style->bordersz() > 0) { framebuffer.rect(0, 0, _info.bounds.w, _info.bounds.h, _info.style->bordersz(), _info.style->color(color_index::border)); }

            for (size_t i = 0; i < _ctrls.length(); i++) { _ctrls[i]->draw(); }
        }

        void container::render()
        {
            if (_info.parent != NULL)
            {
                return;   
            }   

            os::kernel::shell->framebuffer.copy(_info.bounds.x, _info.bounds.y, &framebuffer);
            control::render();

            for (size_t i = 0; i < _ctrls.length(); i++) { _ctrls[i]->render(); }
        }

        bool container::add_ctrl(control* ctrl)
        {
            if (ctrl == NULL) { return false; }
            _ctrls.add(ctrl);
            draw();
            return true;
        }

        bool container::remove_ctrl(control* ctrl)
        {
            if (ctrl == NULL) { return false; }
            for (size_t i = 0; i < _ctrls.length(); i++)
            {
                if (_ctrls[i] == ctrl) { _ctrls.remove_at(i); draw(); return true; }
            }
            return false;
        }

        bool container::contains_ctrl(control* ctrl)
        {
            if (ctrl == NULL) { return false; }
            for (size_t i = 0; i < _ctrls.length(); i++)
            {
                if (_ctrls[i] == ctrl) { return true; }
            }
            return false;
        }

        arraylist<control*>* container::controls() { return &_ctrls; }
    }
}