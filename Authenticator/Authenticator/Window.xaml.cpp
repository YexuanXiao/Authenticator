#include "pch.h"
#include "Window.xaml.h"
#if __has_include("Window.g.cpp")
#include "Window.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::Authenticator::implementation
{
    void Window::InitializeComponent()
    {
        WindowT::InitializeComponent();
    }

    void Window::Navigation_BackRequested(Microsoft::UI::Xaml::Controls::NavigationView const&, Microsoft::UI::Xaml::Controls::NavigationViewBackRequestedEventArgs const&)
    {
        RootFrame().GoBack();
    }

    void Window::Navigation_ItemInvoked(Microsoft::UI::Xaml::Controls::NavigationView const&, Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs const& args)
    {
        if (args.IsSettingsInvoked())
            RootFrame().Navigate(winrt::xaml_typename<Authenticator::Settings>());
    }
}
