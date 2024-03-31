#pragma once

#include <pch.h>

// Win32Helper is responsible for all Win32 API interactions
namespace Win32Helper {
    enum class exit_code : unsigned int
    {
        normal,
        instance_is_exist,
    };
    void Exit(exit_code) noexcept;
    /// <summary>
    /// make app only has one instance
    /// call at entry point
    /// </summary>
    void ActivateSingletonWindow(winrt::hstring const&) noexcept;
    /// <summary>
    /// make app only has one instance
    /// call at Window initialized
    /// </summary>
    void DisableMultiInstanceWindow(winrt::Microsoft::UI::Xaml::Window const&, winrt::hstring const&) noexcept;
    double GetScaleAdjustment(winrt::Microsoft::UI::Xaml::Window const&) noexcept;
    void RegistCoreWindow(winrt::Windows::Foundation::IInspectable const&) noexcept;
    void OpenExplorer(winrt::hstring const&) noexcept;
    winrt::hstring GetUserName() noexcept;
}
