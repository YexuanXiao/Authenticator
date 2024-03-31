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
        void Navigation_ItemInvoked(Microsoft::UI::Xaml::Controls::NavigationView const&, Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs const&);
        void Navigation_BackRequested(Microsoft::UI::Xaml::Controls::NavigationView const&, Microsoft::UI::Xaml::Controls::NavigationViewBackRequestedEventArgs const&);
    };
}

namespace winrt::Authenticator::factory_implementation
{
    struct Window : WindowT<Window, implementation::Window>
    {
    };
}
