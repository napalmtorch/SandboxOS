#include <gui/control.h>
#include <core/kernel.h>

namespace std
{
    namespace gui
    {
        control::control(int x, int y, int w, int h, char* label, control_type type, container* parent)
        {
            memset(&_info,   0, sizeof(control_info_t));
            memset(&_flags,  0, sizeof(control_flags_t));
            memset(&_events, 0, sizeof(control_events_t));
            _type = type;
            
            _info.parent = parent;
            _info.bounds = irect_t(x, y, w, h);
            _info.style  = default_styles::CONTAINER;
            set_label(label);

            _flags.active  = true;
            _flags.enabled = true;
            _flags.visible = true;
        }

        void control::dispose()
        {
            free(_info.label);
        }

        void control::update()
        {
            update_events();
        }

        void control::update_events()
        {
            irect_t bnds = screen_bounds();
            if (bnds.contains(mspos()))
            {
                if (!_flags.hover)
                {
                    _flags.hover = true;
                    draw();
                    if (_events.on_ms_enter != NULL) { _events.on_ms_enter(this, NULL); }
                }
                
                if (_events.on_ms_hover != NULL) { _events.on_ms_hover(this, NULL); }

                if (is_msdown(mouse_btn::left)) 
                { 
                    _flags.active = true; 
                    if (!_flags.clicked)
                    {
                        _flags.down      = true;
                        _flags.clicked   = true;
                        _flags.unclicked = false;
                        draw();
                        if (_events.on_ms_down != NULL) { _events.on_ms_down(this, NULL); }
                    }
                }

                if (is_msup(mouse_btn::left) && !_flags.unclicked)
                {
                    _flags.down = false;
                    if (_flags.clicked)
                    {
                        draw();
                        if (_events.on_click != NULL) { _events.on_click(this, NULL); }
                        if (_events.on_ms_up != NULL) { _events.on_ms_up(this, NULL); }
                        _flags.unclicked = true;
                        _flags.clicked   = false;
                    }
                }
            }
            else
            {
                if (_flags.hover)
                {
                    _flags.hover = false;
                    draw();
                    if (_events.on_ms_leave != NULL) { _events.on_ms_leave(this, NULL); }
                }
                _flags.down      = false;
                _flags.clicked   = false;
                _flags.unclicked = false;
                if (is_msdown(mouse_btn::left) && _type != control_type::window) { _flags.active = false; }
            }
        }

        void control::draw()
        {

        }

        void control::render()
        {
            if (_flags.tooltip && _info.tooltip_txt != NULL && _flags.hover)
            {
                std::gfx::image img = os::kernel::shell->framebuffer;

                ivec2d_t txt_size = ivec2d_t(_info.style->font().width() * strlen(_info.tooltip_txt), _info.style->font().height());
                int xx = std::mspos().x, yy = std::mspos().y;
                if (yy + txt_size.y + 8 >= os::hal::devices::vbe->modeinfo().height) { yy -= txt_size.y + 8; }
                while (xx + txt_size.x + 8 >= os::hal::devices::vbe->modeinfo().width) { xx--; }

                img.rect_filled(xx, yy, txt_size.x + 8, txt_size.y + 8, _info.style->color(std::gui::color_index::bg));
                img.rect(xx, yy, txt_size.x + 8, txt_size.y + 8, 1, _info.style->color(std::gui::color_index::border));
                img.putstr(xx + 4, yy + 4, _info.tooltip_txt, _info.style->font(), _info.style->color(std::gui::color_index::txt), (uint32_t)std::color32::transparent);
            }
        }

        void control::set_label(char* label)
        {
            if (label == NULL || strlen(label) == 0) { return; }
            if (_info.label != NULL) { free(_info.label); }
            _info.label = (char*)tmalloc(strlen(label) + 1, ALLOCTYPE_STRING);
            strcpy(_info.label, label);
        }

        char* control::label()              { return _info.label; }
        container* control::parent()        { return _info.parent; }
        irect_t* control::bounds()          { return &_info.bounds; }
        control_info_t* control::info()     { return &_info; }
        control_flags_t* control::flags()   { return &_flags; }
        control_events_t* control::events() { return &_events; }
        control_type      control::type()   { return _type; }

        irect_t control::screen_bounds()
        {
            irect_t bnds = _info.bounds;
            container* p = _info.parent;
            while (true)
            {
                if (p == NULL) { break; }
                bnds.x += _info.parent->_info.bounds.x;
                bnds.y += _info.parent->_info.bounds.y;
                p = p->_info.parent;
            }
            return bnds;
        }
    }
}