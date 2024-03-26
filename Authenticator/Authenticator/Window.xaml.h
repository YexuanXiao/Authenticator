#pragma once

#include "Window.g.h"

namespace winrt::Authenticator::implementation
{
    struct Window : WindowT<Window>
    {
        Window()
        {
        }

        void InitializeComponent();
    };
}

namespace winrt::Authenticator::factory_implementation
{
    struct Window : WindowT<Window, implementation::Window>
    {
    };
}
