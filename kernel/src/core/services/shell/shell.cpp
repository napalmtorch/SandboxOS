#include <core/services/shell/shell.h>
#include <core/services/terminal.h>
#include <core/kernel.h>

namespace os
{
    namespace services
    {
        std::gui::window* win;
        std::gui::button* btn;

        void shell_host::init()
        {
            std::gui::default_styles::init();

            framebuffer = std::gfx::image(hal::devices::vbe->modeinfo().width, hal::devices::vbe->modeinfo().height);
            if (sys::asset_manager::fetch_image(sys::image_id::default_bg) != NULL)
            {
                if (!sys::asset_manager::fetch_image(sys::image_id::default_bg)->size().equals(framebuffer.size())) { sys::asset_manager::fetch_image(sys::image_id::default_bg)->resize(framebuffer.size()); }
            }

            taskbar = new shell_taskbar(this);

            winmgr.init();

            win = new std::gui::window(320, 240, 256, 192, "TESTING");
            btn = new std::gui::button(32, 32, 92, 22, "hello", win);
            win->add_ctrl(btn);
            winmgr.load(win);

            winmgr.load(new os::services::terminal_host(128, 128, 80, 25, 0xFFFFFFFF, 0xFF000000, *sys::asset_manager::fetch_font(sys::font_id::thin_8x12)));
            

            printf("%s Initialized shell instance\n", DEBUG_OK);
        }

        void shell_host::dispose()
        {
            framebuffer.dispose();
        }

        void shell_host::start()
        {
            std::arraylist<char*> args;
            args.add((char*)this);

            thread = threading::thread_create("shell_host", STACKSZ_LARGE, this->main, args.length(), args.ptr());
            threading::scheduler::load(thread);
            threading::scheduler::start(thread);
            printf("%s Started shell host service\n", DEBUG_OK);
        }

        void shell_host::update()
        {
            
        }

        void shell_host::draw()
        {
            if (sys::asset_manager::fetch_image(sys::image_id::default_bg)== NULL) { framebuffer.clear(std::color32::dark_cyan); }
            else { framebuffer.copy(0, 0, sys::asset_manager::fetch_image(sys::image_id::default_bg)); }
            sys::debug::draw_overlay(&framebuffer);
            
            winmgr.update();
            winmgr.draw();

            taskbar->update();
            taskbar->render();

            framebuffer.copy(std::mspos(), std::color32::magenta, sys::asset_manager::fetch_image(sys::image_id::ms_default));

            hal::devices::vbe->copy(0, 0, framebuffer.size().x, framebuffer.size().y, framebuffer.data().ptr());
        }

        int shell_host::main(int argc, char** argv)
        {
            shell_host* shell = (shell_host*)argv[0];
            
            lock();
            printf("%s Entered shell main\n", DEBUG_INFO);
            shell->init();
            unlock();

            uint32_t now = 0, last = 0, frames = 0;
            uint32_t sw_now = 0, sw_last = 0, sw_timer = 0;
            char temp[32];
            char fpsstr[32];
            while (true)
            {                
                lock();
                threading::thread_monitor();     
                shell->update();

                now = std::timenow().second;
                if (now != last)
                { 
                    shell->_fps = frames;
                    frames      = 0;
                    last        = now;

                    memset(temp, 0, sizeof(temp));
                    memset(fpsstr, 0, sizeof(fpsstr));
                    ltoa(shell->_fps, temp, 10);
                    strcat(fpsstr, temp);
                    strcat(fpsstr, " FPS");
                }

                sw_now = hal::pit::millis();
                if (sw_now != sw_last) { sw_last = sw_now; sw_timer++; }           
                if (sw_timer == 3 || sw_timer == 6) { unlock(); threading::scheduler::yield(); }
                if (sw_timer >= 15)
                {
                    lock();
                    frames++;
                    sw_timer = 0;                
                    shell->draw();
                }

                unlock();
            }

            return 0;
        }

        uint32_t shell_host::fps() { return _fps; }
    }
}