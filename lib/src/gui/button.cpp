#include <gui/button.h>
#include <core/kernel.h>

namespace std
{
    namespace gui
    {
        button::button(int x, int y, int w, int h, char* label, container* parent) : control(x, y, w, h, label, control_type::button, parent)
        {
            update();
            draw();
            render();
        }

        void button::dispose()
        {
            control::dispose();
        }

        void button::update()
        {
            control::update();
        }

        void button::draw()
        {
            control::draw();

            gfx::image* fb = &os::kernel::shell->framebuffer;
            if (_info.parent != NULL) { fb = &_info.parent->framebuffer; }

            uint32_t bg = _info.style->color(color_index::bg), bord = _info.style->color(color_index::border), fg = _info.style->color(color_index::txt);
            if (_flags.hover && !_flags.down) { bg = _info.style->color(color_index::bg_hover); bord = _info.style->color(color_index::border_hover); fg = _info.style->color(color_index::txt_hover); }
            if (_flags.down) { bg = _info.style->color(color_index::bg_down); bord = _info.style->color(color_index::border_down); fg = _info.style->color(color_index::txt_down); }

            fb->rect_filled(_info.bounds, bg);

            if (_info.style->bordersz() > 0) { fb->rect(_info.bounds, _info.style->bordersz(), bord); }

            if (_info.label != NULL)
            {
                int xx = (_info.bounds.w / 2) - ((_info.style->font().width() * strlen(_info.label)) / 2);
                int yy = (_info.bounds.h / 2) - (_info.style->font().height() / 2);
                fb->putstr(_info.bounds.x + xx, _info.bounds.y + yy, _info.label, _info.style->font(), fg, (uint32_t)std::color32::transparent);
            }
        }

        void button::render()
        {
            control::render();
        }
    }
}