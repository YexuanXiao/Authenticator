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
}
