/* <editor-fold desc="MIT License">

Copyright(c) 2018 Robert Osfield

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */

#pragma once

#include <vsg/viewer/Window.h>
#include <vsg/ui/KeyEvent.h>

#include <xcb/xcb.h>

#include <iostream>

namespace vsgXcb
{

    class KeyboardMap : public vsg::Object
    {
    public:
        KeyboardMap();

        using KeycodeModifier = std::pair<uint16_t, uint16_t>;
        using KeycodeMap = std::map<KeycodeModifier, vsg::KeySymbol>;

        void add(uint16_t keycode, uint16_t modifier, vsg::KeySymbol key);

        void add(uint16_t keycode, std::initializer_list<std::pair<uint16_t, vsg::KeySymbol> > combinations);

        vsg::KeySymbol getKeySymbol(uint16_t keycode, uint16_t modifier);

    protected:
        KeycodeMap _keycodeMap;
        uint16_t   _modifierMask;
    };


    class Xcb_Surface : public vsg::Surface
    {
    public:
        Xcb_Surface(vsg::Instance* instance, xcb_connection_t* connection, xcb_window_t window, vsg::AllocationCallbacks* allocator = nullptr);
    };

    class Xcb_Window : public vsg::Window
    {
    public:

        Xcb_Window() = delete;
        Xcb_Window(const Xcb_Window&) = delete;
        Xcb_Window& operator = (const Xcb_Window&) = delete;

        static Result create(vsg::ref_ptr<vsg::Window::Traits> traits, bool debugLayer=false, bool apiDumpLayer=false, vsg::AllocationCallbacks* allocator=nullptr);

        bool valid() const override;

        bool pollEvents(vsg::Events& events) override;

        bool resized() const override;

        void resize() override;

    protected:
        Xcb_Window(vsg::ref_ptr<vsg::Window::Traits> traits, bool debugLayer, bool apiDumpLayer, vsg::AllocationCallbacks* allocator);

        ~Xcb_Window();

        xcb_connection_t* _connection = nullptr;
        xcb_screen_t* _screen = nullptr;
        xcb_window_t _window{};
        xcb_atom_t _wmProtocols{};
        xcb_atom_t _wmDeleteWindow{};

        bool _closeEventRecieved = false;
        bool _windowResized = false;

        xcb_timestamp_t _first_xcb_timestamp = 0;
        vsg::clock::time_point _first_xcb_time_point;

        vsg::ref_ptr<KeyboardMap> _keyboard;
    };


}

