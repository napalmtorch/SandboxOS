#include <core/services/shell/taskbar.h>
#include <core/kernel.h>

namespace os
{
    namespace services
    {
        shell_taskbar::shell_taskbar(shell_host* host) : container(0, 0, 128, 22, "", NULL)
        {
            this->host = host;
            this->_info.style = (std::gui::visual_style*)tmalloc(sizeof(std::gui::visual_style), ALLOCTYPE_OBJ);
            memcpy(this->_info.style, std::gui::default_styles::CONTAINER, sizeof(std::gui::visual_style));
            this->_info.style->set_bordersz(0);

            this->btn_menu = new std::gui::button(0, 0, 100, 19, "Activities", this);
            this->btn_menu->info()->tooltip_txt = (char*)tmalloc(256, ALLOCTYPE_STRING);
            strcat(this->btn_menu->info()->tooltip_txt, "Start applications and manage system");
            this->btn_menu->flags()->tooltip    = true;
            this->btn_menu->info()->style = new std::gui::visual_style();
            memcpy(this->btn_menu->info()->style, std::gui::default_styles::CONTAINER, sizeof(std::gui::visual_style));
            this->btn_menu->info()->style->set_color(std::gui::color_index::bg, 0xFF200020);
            this->add_ctrl(this->btn_menu);

            _wincnt = 11111;
            _wincnt_last = 1111111;
            _winlast = NULL;

            update();
            draw();
            render();
        }

        void shell_taskbar::dispose()
        {
            if (this->btn_menu->info()->tooltip_txt != NULL) { free(this->btn_menu->info()->tooltip_txt); }
            container::dispose();
        }

        void shell_taskbar::update()
        {
            container::update();
            _info.bounds = std::irect_t(0, host->framebuffer.size().y - 22, host->framebuffer.size().x, 22);

            btn_menu->bounds()->x = 2;
            btn_menu->bounds()->y = 2;

            std::time_t tm = std::timenow();
            _now = tm.second;
            if (_now != _last)
            {
                _last = _now;
                memset(timestr, 0, sizeof(timestr));
                std::timestr(tm, timestr, std::time_format::standard, true);
                draw();
            }

            _wincnt = kernel::shell->winmgr.windows.length();
            if (_wincnt != _wincnt_last)
            {
                _wincnt_last = _wincnt;
                for (size_t i = 0; i < taskbtns.length(); i++) { taskbtns[i]->dispose(); free(taskbtns[i]); }
                taskbtns.clear();
                for (size_t i = 0; i < kernel::shell->winmgr.windows.length(); i++) { taskbtns.add(new std::gui::button(0, 0, 0, 0, kernel::shell->winmgr.windows[i]->label(), this)); }
                update();
                draw();
                render();
            }

            if (_winlast != kernel::shell->winmgr.active_win)
            {
                _winlast = kernel::shell->winmgr.active_win;
                for (size_t i = 0; i < taskbtns.length(); i++) { taskbtns[i]->dispose(); free(taskbtns[i]); }
                taskbtns.clear();
                for (size_t i = 0; i < kernel::shell->winmgr.windows.length(); i++) { taskbtns.add(new std::gui::button(0, 0, 0, 0, kernel::shell->winmgr.windows[i]->label(), this)); }
                update();
                draw();
                render();
            }

            int xx = btn_menu->bounds()->x + btn_menu->bounds()->w + 12, yy = btn_menu->bounds()->y;
            for (size_t i = 0; i < taskbtns.length(); i++) 
            { 
                taskbtns[i]->info()->bounds = std::irect_t(xx, yy, 128, btn_menu->bounds()->h);
                taskbtns[i]->update();
                xx += taskbtns[i]->info()->bounds.w + 4;
            }
        }

        void shell_taskbar::draw()
        {
            container::draw();
            framebuffer.rect_filled(0, 0, _info.bounds.w, 1, _info.style->color(std::gui::color_index::border_down));
            framebuffer.rect_filled(btn_menu->bounds()->x + btn_menu->bounds()->w + 6, btn_menu->bounds()->y + 1, 1, btn_menu->bounds()->h - 2, _info.style->color(std::gui::color_index::border_down));

            for (size_t i = 0; i < taskbtns.length(); i++) { taskbtns[i]->draw(); }

            int tx = _info.bounds.w - (strlen(timestr) * _info.style->font().width()) - 8;
            int ty = (_info.bounds.h / 2) - (_info.style->font().height() / 2);
            framebuffer.putstr(tx, ty, timestr, _info.style->font(), _info.style->color(std::gui::color_index::txt), (uint32_t)std::color32::transparent);

            framebuffer.copy(tx - 20, 3, std::irect_t(32, 0, 16, 16), std::color32::magenta, sys::asset_manager::fetch_image(sys::image_id::icons_tray));
            framebuffer.copy(tx - 40, 3, std::irect_t(0, 0, 16, 16), std::color32::magenta, sys::asset_manager::fetch_image(sys::image_id::icons_tray));
            framebuffer.copy(tx - 60, 3, std::irect_t(48, 0, 16, 16), std::color32::magenta, sys::asset_manager::fetch_image(sys::image_id::icons_tray));
        }

        void shell_taskbar::render()
        {
            container::render();
            for (size_t i = 0; i < taskbtns.length(); i++) { taskbtns[i]->render(); }
        }
    }
}