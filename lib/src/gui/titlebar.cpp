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
            btn_close->events()->on_click = btn_close_click;

            btn_max = new button(0, 0, 16, 16, "", parent);
            btn_max->events()->on_click = btn_max_click;

            btn_min = new button(0, 0, 16, 16, "", parent);
            btn_min->events()->on_click = btn_min_click;

            _info.style = parent->_info.style;

            update();
            draw();
            render();
        }

        void titlebar::dispose()
        {
            control::dispose();
            btn_close->dispose();
            btn_max->dispose();
            btn_min->dispose();
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

            if (_ow != _info.bounds.x || _oh != _info.bounds.y)
            {
                _ow = _info.bounds.x;
                _oh = _info.bounds.y;
                ((window*)_info.parent)->update();
                ((window*)_info.parent)->draw();
                ((window*)_info.parent)->render();
            }

            handle_movement();
        }

        void titlebar::draw()
        {
            control::draw();

            uint32_t bg = _info.style->color(color_index::top);
            if (!((window*)_info.parent)->_flags.active) { bg =_info.style->color(color_index::top_inactive); }
            _info.parent->framebuffer.rect_filled(_info.bounds, bg);
            
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
            if (btn_close != NULL) { _info.parent->framebuffer.copy(btn_close->bounds()->x + 4, btn_close->bounds()->y + 3, irect_t(0, 0, 8, 8), color32::magenta, os::sys::asset_manager::fetch_image(os::sys::image_id::icons_tbar)); }
            if (btn_max != NULL)   { _info.parent->framebuffer.copy(btn_max->bounds()->x + 3, btn_max->bounds()->y + 3, irect_t(9, 0, 9, 9), color32::magenta, os::sys::asset_manager::fetch_image(os::sys::image_id::icons_tbar)); }
            if (btn_min != NULL)   { _info.parent->framebuffer.copy(btn_min->bounds()->x + 4, btn_min->bounds()->y + 3, irect_t(18, 0, 9, 9), color32::magenta, os::sys::asset_manager::fetch_image(os::sys::image_id::icons_tbar)); }
            control::render();
        }

        void titlebar::handle_movement()
        {
            if (_info.parent->flags()->active && os::kernel::shell->winmgr.active_win == _info.parent)
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

        void titlebar::btn_close_click(control* sender, void* arg)
        {
            titlebar* tbar = (titlebar*)sender;
            ((window*)tbar->_info.parent)->_closed = true;
        }

        void titlebar::btn_max_click(control* sender, void* arg)
        {
            titlebar* tbar = (titlebar*)sender;
            window* win = (window*)tbar->_info.parent;
            if (win->state == window_state::normal)
            {
                win->state = window_state::maximized;
                win->_old_bounds  = irect_t(win->_info.bounds.x, win->_info.bounds.y, win->_info.bounds.w, win->_info.bounds.h);
                win->_info.bounds = irect_t(0, 0, os::hal::devices::vbe->modeinfo().width, os::hal::devices::vbe->modeinfo().height);
            }
            else if (win->state == window_state::maximized)
            {
                win->state = window_state::normal;
                win->_info.bounds = win->_old_bounds;
            }
            win->update();
            win->draw();
            win->render();
        }

        void titlebar::btn_min_click(control* sender, void* arg)
        {
            titlebar* tbar = (titlebar*)sender;
            window* win = (window*)tbar->_info.parent;
            win->state = window_state::minimized;
        }
    }
}