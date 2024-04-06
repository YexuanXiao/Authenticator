#pragma once

#include "Settings.g.h"
// BUG: https://github.com/microsoft/WindowsAppSDK/issues/4312
// #include <winrt/Microsoft.Windows.ApplicationModel.Resources.h>
#include <winrt/Windows.ApplicationModel.Resources.h>

namespace winrt::Authenticator::implementation
{
    struct Settings: SettingsT<Settings>
    {
        Settings()
        {
        }

        void InitializeComponent();

        void Theme_Loaded(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);
        void Theme_Changed(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::Controls::SelectionChangedEventArgs const&);
        winrt::Windows::Foundation::IAsyncAction Reset_Tapped(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::Input::TappedRoutedEventArgs const&);

    private:
        static void Language_Selected(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::Input::TappedRoutedEventArgs const&);
        // winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader resource_{ L"Settings" };
        winrt::Windows::ApplicationModel::Resources::ResourceLoader resource_{ L"Settings" };
    };
}

namespace winrt::Authenticator::factory_implementation
{
    struct Settings: SettingsT<Settings, implementation::Settings>
    {
    };
}
