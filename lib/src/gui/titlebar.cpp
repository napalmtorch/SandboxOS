#include <gui/titlebar.h>
#include <core/kernel.h>

namespace std
{
    namespace gui
    {
        bool _win_click;
        int  _win_mx, _win_my;

        titlebar::titlebar(window* parent) : control(0, 0, 128, 22, "titlebar", control_type::titlebar, parent)
        {
            btn_close = new button(0, 0, 16, 16, "", parent);
            btn_max   = new button(0, 0, 16, 16, "", parent);
            btn_min   = new button(0, 0, 16, 16, "", parent);
            update();
            draw();
            render();
        }

        void titlebar::dispose()
        {
            control::dispose();
        }

        void titlebar::update()
        {
            control::update();
            _info.bounds = irect_t(1, 1, _info.parent->bounds()->w - 2, 22);
            int xx = _info.bounds.w - 19;

            if (btn_close != NULL)
            {
                btn_close->bounds()->x = xx;
                btn_close->bounds()->y = 4;
                btn_close->update();
                xx -= 18;
            }

            if (btn_max != NULL)
            {
                btn_max->bounds()->x = xx;
                btn_max->bounds()->y = 4;
                btn_max->update();
                xx -= 18;
            }

            if (btn_min != NULL)
            {
                btn_min->bounds()->x = xx;
                btn_min->bounds()->y = 4;
                btn_min->update();
                xx -= 18;
            }

            handle_movement();
        }

        void titlebar::draw()
        {
            control::draw();

            _info.parent->framebuffer.rect_filled(_info.bounds, _info.style->color(color_index::top));
            
            if (_info.parent->label() != NULL)
            {
                int yy = (_info.bounds.h / 2) - (_info.style->font().height() / 2);
                _info.parent->framebuffer.putstr(_info.bounds.x + 3, _info.bounds.y + yy, _info.parent->label(), _info.style->font(), _info.style->color(color_index::toptxt), (uint32_t)color32::transparent);
            }

            if (btn_close != NULL) { btn_close->draw(); }
            if (btn_max   != NULL) { btn_max->draw(); }
            if (btn_min   != NULL) { btn_min->draw(); }
        }

        void titlebar::render()
        {
            control::render();

            if (btn_close != NULL) { _info.parent->framebuffer.copy(btn_close->bounds()->x + 4, btn_close->bounds()->y + 3, irect_t(0, 0, 8, 8), color32::magenta, &os::sys::assets::tbar_icons); }
            if (btn_max != NULL)   { _info.parent->framebuffer.copy(btn_max->bounds()->x + 3, btn_max->bounds()->y + 3, irect_t(9, 0, 9, 9), color32::magenta, &os::sys::assets::tbar_icons); }
            if (btn_min != NULL)   { _info.parent->framebuffer.copy(btn_min->bounds()->x + 4, btn_min->bounds()->y + 3, irect_t(18, 0, 9, 9), color32::magenta, &os::sys::assets::tbar_icons); }
        }

        void titlebar::handle_movement()
        {
            if (_info.parent->flags()->active)
            {
                if (screen_bounds().contains(mspos()))
                {
                    // this if statement will need to be fixed when button selection is added(control box)
                    if (is_msdown(mouse_btn::left) && !btn_close->flags()->hover && !btn_max->flags()->hover && !btn_min->flags()->hover)
                    {
                        if (!_win_click)
                        {
                            _win_mx = mspos().x - _info.parent->bounds()->x;
                            _win_my = mspos().y - _info.parent->bounds()->y;
                            _win_click = true;
                        }
                        ((window*)_info.parent)->_moving = true;
                    }
                }

                if (((window*)_info.parent)->_moving)
                {
                    _info.parent->bounds()->x = mspos().x - _win_mx;
                    _info.parent->bounds()->y = mspos().y - _win_my;
                    if (_info.parent->bounds()->y < 0) { _info.parent->bounds()->y = 0; }
                }
            }

            if (is_msup(mouse_btn::left))
            {
                ((window*)_info.parent)->_moving   = false;
                ((window*)_info.parent)->_resizing = false;
                _win_click = false;
            }
        }
    }
}