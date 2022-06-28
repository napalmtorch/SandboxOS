#include <core/services/shell/winmgr.h>
#include <core/kernel.h>

namespace os
{
    namespace services
    {
        void window_manager::init()
        {
            windows = std::arraylist<std::gui::window*>();
            printf("%s Initialized window manager\n", DEBUG_OK);
        }

        void window_manager::update()
        {
            int highest_hover = -1;
            int hover = 0, moving = 0, resize = 0, i = 0;
            int moving_i = -1;

            for (i = 0; i < windows.length(); i++)
            {
                windows[i]->update();
                if (windows[i]->_closed) { unload(windows[i]); return; }

                if (windows[i]->_flags.hover) { hover++;  }
                if (windows[i]->_moving)      { moving++; if (moving_i < i) { moving_i = i; } }
                if (windows[i]->_resizing)    { resize++; }
            }

            for (i = windows.length() - 1; i >= 0; i--)
            {
                windows[i]->update();
                if (moving + resize > 0) { continue; }

                if (active_win != NULL)
                {
                    if (windows[i]->_flags.hover && !active_win->_flags.hover && windows[i] != active_win && std::is_msdown(std::mouse_btn::left) && !_msdown)
                    {
                        if (i > highest_hover) { highest_hover = i; _wintop = i; }
                        _msdown = true;
                    }
                }
                else
                {
                    if (windows[i]->_flags.hover && std::is_msdown(std::mouse_btn::left) && !_msdown)
                    {
                        if (i > highest_hover) { highest_hover = i; _wintop = i; }
                        _msdown = true;
                    }
                }
            }

            if (hover == 0 && moving == 0 && resize == 0 && std::is_msdown(std::mouse_btn::left) && !_msdown) { set_active(NULL); _msdown = true; }
            if (highest_hover >= 0) { set_active(windows[highest_hover]); }
            if (std::is_msup(std::mouse_btn::left)) { _msdown = false; }
        }

        void window_manager::draw()
        {
            for (size_t i = 0; i < windows.length(); i++)
            {
                if (windows[i]->state != std::gui::window_state::minimized) { windows[i]->render(); }
            }
        }

        void window_manager::load(std::gui::window* win)
        {
            if (win == NULL) { perror("Tried to load null window into manager"); return; }
            win->index = windows.length();
            windows.add(win);
            set_active(win);
            printf("%s Loaded window - %s\n", DEBUG_INFO, win->label());
        }

        void window_manager::unload(std::gui::window* win, bool disp)
        {
            if (win == NULL) { perror("Tried to unload null window from manager"); return; }
            for (size_t i = 0; i < windows.length(); i++)
            {
                if (windows[i] == win)
                {
                    windows.remove_at(i);
                    printf("%s Unloaded window - %s\n","" DEBUG_INFO, win->label());
                    if (disp) { win->dispose(); }
                    if (windows.length() > 0) { set_active(windows[windows.length() - 1]); }
                    else { set_active(NULL); }
                    return;
                }
            }
        }

        std::gui::window* window_manager::set_active(std::gui::window* win)
        {
            if (win == NULL)
            {
                active_win = NULL;
                return NULL;
            }

            std::arraylist<std::gui::window*> wins;
            for (size_t i = 0; i < windows.length(); i++)
            {
                if (windows[i] != win) { wins.add(windows[i]); windows[i]->flags()->active = false; }
            }
            wins.add(win);
            windows.dispose();
            windows = wins;
            win->flags()->active = true;
            active_win = win;
            printf("%s Set active window to 0x%8x\n", DEBUG_INFO, win);
            for (size_t i = 0; i < windows.length(); i++) { windows[i]->update(); windows[i]->draw(); windows[i]->render(); }
            return win;
        }
    }
}