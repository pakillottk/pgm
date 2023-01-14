#pragma once

#include <memory>
#include <string_view>

namespace PGM { namespace Platform {

    /**
    * Creates a new window handle which abstracts
    * from the concrete platform windowing 
    */
    class Window
    {
    public:  
        enum FLAGS
        {
            bDefault = 0
            // TODO
        };

        Window(const std::string_view &title, unsigned w = 800, unsigned h = 600, FLAGS flags = bDefault);
        ~Window();

        /**
        * Runs the window's main loop 
        */
        void show();

    private:
        struct window_impl_t;
        std::unique_ptr<window_impl_t> m_Impl;
    };

}}