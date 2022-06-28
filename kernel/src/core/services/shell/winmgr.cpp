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
            for (size_t i = 0; i < windows.length(); i++)
            {
                windows[i]->update();
                if (windows[i]->_closed) { unload(windows[i]); }
            }
        }

        void window_manager::draw()
        {
            for (size_t i = 0; i < windows.length(); i++)
            {
                windows[i]->render();
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

        void window_manager::unload(std::gui::window* win)
        {
            if (win == NULL) { perror("Tried to unload null window from manager"); return; }
            for (size_t i = 0; i < windows.length(); i++)
            {
                if (windows[i] == win)
                {
                    windows.remove_at(i);
                    printf("%s Unloaded window - %s\n","" DEBUG_INFO, win->label());
                    win->dispose();
                    return;
                }
            }
        }

        std::gui::window* window_manager::set_active(std::gui::window* win)
        {
            if (win == NULL) { return NULL; }

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
            return win;
        }
    }
}