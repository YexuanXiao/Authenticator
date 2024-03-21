#pragma once

#include "Window.g.h"

namespace winrt::Authenticator::implementation
{
    struct Window : WindowT<Window>
    {
        Window()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }
    };
}

namespace winrt::Authenticator::factory_implementation
{
    struct Window : WindowT<Window, implementation::Window>
    {
    };
}
