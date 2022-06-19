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
            update();
            draw();
            render();
        }

        void shell_taskbar::dispose()
        {
            container::dispose();
        }

        void shell_taskbar::update()
        {
            container::update();
            _info.bounds = std::irect_t(0, host->framebuffer.size().y - 22, host->framebuffer.size().x, 22);

            std::time_t tm = std::timenow();
            _now = tm.second;
            if (_now != _last)
            {
                _last = _now;
                memset(timestr, 0, sizeof(timestr));
                std::timestr(tm, timestr, std::time_format::standard, true);
                draw();
            }
        }

        void shell_taskbar::draw()
        {
            container::draw();
            framebuffer.rect_filled(0, 0, _info.bounds.w, 1, _info.style->color(std::gui::color_index::border_down));

            int tx = _info.bounds.w - (strlen(timestr) * _info.style->font().width()) - 8;
            int ty = (_info.bounds.h / 2) - (_info.style->font().height() / 2);
            framebuffer.putstr(tx, ty, timestr, _info.style->font(), _info.style->color(std::gui::color_index::txt), (uint32_t)std::color32::transparent);
        }

        void shell_taskbar::render()
        {
            container::render();
        }
    }
}