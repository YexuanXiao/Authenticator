#include "pch.h"
#include "Win32Helper.h"
#include <winrt/Windows.Storage.h>

#include <winrt/Windows.Security.Credentials.UI.h>

namespace SettingsHelper
{
    constexpr std::wstring_view Theme_Key = L"Theme";
    constexpr std::wstring_view First_Key = L"First";
    constexpr std::wstring_view Hello_Key = L"Hello";
    constexpr std::wstring_view Language_Key = L"Language";

    winrt::Windows::Foundation::Collections::IPropertySet GetApplicationSettings() noexcept
    {
        return winrt::Windows::Storage::ApplicationData::Current().LocalSettings().Values();
    }

    winrt::Windows::Storage::StorageFolder GetDataFolder() noexcept
    {
        return winrt::Windows::Storage::ApplicationData::Current().LocalFolder();
    }

    void StoreTheme(winrt::Microsoft::UI::Xaml::ElementTheme theme) noexcept
    {
        auto settings{ GetApplicationSettings() };
        auto value{ static_cast<int32_t>(theme) };
        settings.Insert(Theme_Key.data(), winrt::box_value(value));
    }

    winrt::Microsoft::UI::Xaml::ElementTheme LoadTheme() noexcept
    {
        auto settings{ GetApplicationSettings() };
        auto value{ winrt::unbox_value_or<int32_t>(settings.Lookup(Theme_Key.data()), 0) };
        return winrt::Microsoft::UI::Xaml::ElementTheme{ value };
    }

    void SetTheme(winrt::Microsoft::UI::Xaml::XamlRoot element, winrt::Microsoft::UI::Xaml::ElementTheme theme) noexcept
    {
        element.Content().try_as<winrt::Microsoft::UI::Xaml::FrameworkElement>().RequestedTheme(theme);
        StoreTheme(theme);
    }

    void SetTheme(winrt::Microsoft::UI::Xaml::FrameworkElement& element, winrt::Microsoft::UI::Xaml::ElementTheme theme) noexcept
    {
        element.RequestedTheme(theme);
    }

    bool CheckFirstUse() noexcept
    {
        auto settings{ GetApplicationSettings() };
#ifndef _DEBUG
        return !settings.HasKey(impl_::First_Key.data());
#else
        return true;
#endif
    }

    void SetFirstUse() noexcept
    {
        auto settings{ GetApplicationSettings() };
        settings.Insert(First_Key.data(), winrt::box_value(true));
    }

    winrt::Windows::Foundation::IAsyncAction RemoveAllData() noexcept
    {
        return winrt::Windows::Storage::ApplicationData::Current().ClearAsync();
    }

    winrt::Windows::Foundation::IAsyncOperation<bool> RequestWindowsHello() noexcept
    {
        auto flag = co_await winrt::Windows::Security::Credentials::UI::UserConsentVerifier::RequestVerificationAsync(L"请求进行 Windows Hello 验证");

        co_return flag == decltype(flag)::Verified;
    }

    winrt::Windows::Foundation::IAsyncOperation<bool> CheckWindowsHelloAvailable() noexcept
    {
        auto flag = co_await winrt::Windows::Security::Credentials::UI::UserConsentVerifier::CheckAvailabilityAsync();

        co_return flag == decltype(flag)::Available;
    }

    void WindowsHello(bool enable)
    {
        auto settings{ GetApplicationSettings() };

        if (enable)
            settings.Insert(Hello_Key.data(), winrt::box_value(true));
        else
            settings.Insert(Hello_Key.data(), winrt::box_value(false));
    }

    bool CheckWindowsHelloEnabled() noexcept
    {
        auto settings{ GetApplicationSettings() };
        auto value{ winrt::unbox_value_or<bool>(settings.Lookup(Hello_Key.data()), false) };

        return value;
    }
}