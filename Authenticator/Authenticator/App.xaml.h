#pragma once

// Give XamlTypeInfo.g.cpp a chance to use the pch.h
#ifndef WINRT_CONSUME_MODULE
#include "pch.h"
#endif

#include "App.xaml.g.h"

namespace winrt::Authenticator::implementation
{
    struct App : AppT<App>
    {
        App();
        void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);
        ~App();

    private:
        Authenticator::Window window_{ nullptr };
        Microsoft::UI::Windowing::AppWindowTitleBar appTitleBar_{ nullptr };

        void SetDragRegionForCustomTitleBar();
        void TitleBar_Loaded(Windows::Foundation::IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&);
        void AppWindow_Changed(Microsoft::UI::Windowing::AppWindow, Microsoft::UI::Windowing::AppWindowChangedEventArgs const&);
        void TitleBar_SizeChanged(Windows::Foundation::IInspectable const&, Microsoft::UI::Xaml::SizeChangedEventArgs const&);
        void TitleBar_ThemeChanged(Microsoft::UI::Xaml::FrameworkElement const&, Windows::Foundation::IInspectable const&);
        void SetTitleBarTheme(Microsoft::UI::Xaml::ElementTheme theme);
    };
}
