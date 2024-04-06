#pragma once
#include "pch.h"

namespace SettingsHelper
{
    winrt::Microsoft::UI::Xaml::ElementTheme LoadTheme() noexcept;
    // for settings page use
    void SetTheme(winrt::Microsoft::UI::Xaml::XamlRoot element, winrt::Microsoft::UI::Xaml::ElementTheme theme) noexcept;
    // for init
    void SetTheme(winrt::Microsoft::UI::Xaml::FrameworkElement& element, winrt::Microsoft::UI::Xaml::ElementTheme theme) noexcept;
    bool CheckFirstUse() noexcept;
    void SetFirstUse() noexcept;
    winrt::Windows::Foundation::IAsyncAction RemoveAllData() noexcept;
    winrt::Windows::Foundation::IAsyncOperation<bool> RequestWindowsHello() noexcept;
    // check whether Windows Hello has been enabled on the device and test it once
    // don't call it in the Window.Activate event, this will result in no other way
    // to stop Windows Hello from taking up the device than reboot
    winrt::Windows::Foundation::IAsyncOperation<bool> CheckWindowsHelloAvailable() noexcept;
    void WindowsHello(bool) noexcept;
    bool CheckWindowsHelloEnabled() noexcept;
}