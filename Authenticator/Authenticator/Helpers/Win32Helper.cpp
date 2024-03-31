#include "pch.h"
#include "Win32Helper.h"
#include <Microsoft.UI.Xaml.Window.h>
#include <shlobj.h>
#include <shobjidl_core.h>

#define SECURITY_WIN32
#include <Security.h>
#include <secext.h>

namespace Win32Helper
{
    void Exit(exit_code code) noexcept
    {
        static_assert(std::is_same_v<UINT, std::underlying_type_t<exit_code>>);
        ::ExitProcess(std::to_underlying(code));
    }

    void ActivateSingletonWindow(winrt::hstring const& appname) noexcept
    {
        // enumerate all direct child windows of desktop
        for (auto previous{ ::FindWindowExW(nullptr, nullptr, nullptr, nullptr) };
             // return null if at the end
             previous != nullptr;
             // pass the previous as the 2nd argument to get next window_handle
             previous = ::FindWindowExW(nullptr, previous, nullptr, nullptr)) [[likely]]
        {
            // check if window_ has the specified property
            // set in MainWindow constructor
            if (::GetPropW(previous, appname.data())) [[unlikely]]
            {
                // show and restore window_
                ::ShowWindow(previous, SW_RESTORE);
                // activate, set foreground and get forcus
                if (::SetForegroundWindow(previous))
                    Exit(exit_code::instance_is_exist);
            }
        }
    }

    HWND GetHandleFromWindow(winrt::Microsoft::UI::Xaml::Window const& window) noexcept
    {
        assert(window);
        HWND handle{};
        window.try_as<IWindowNative>()->get_WindowHandle(&handle);

        return handle;
    }

    double GetScaleAdjustment(winrt::Microsoft::UI::Xaml::Window const& window) noexcept
    {
        auto dpi_x{ ::GetDpiForWindow(GetHandleFromWindow(window)) };
        auto scale_factor_percent{ (dpi_x * 100 + (96 >> 1)) / 96 };

        return scale_factor_percent / 100.;
    }

    void DisableMultiInstanceWindow(winrt::Microsoft::UI::Xaml::Window const& window, winrt::hstring const& appname) noexcept
    {
        auto window_handle{ GetHandleFromWindow(window) };
        ::SetPropW(window_handle, appname.data(), window_handle);
    }

    void RegistCoreWindow(winrt::Windows::Foundation::IInspectable const& object) noexcept
    {
        object.try_as<IInitializeWithWindow>()->Initialize(::GetActiveWindow());
    }

    void OpenExplorer(winrt::hstring const& path) noexcept
    {
#pragma comment(lib, "Shell32.lib")
        SHELLEXECUTEINFOW info{ .cbSize = sizeof(info), .lpFile = L"explorer.exe", .lpParameters = path.c_str(), .nShow = SW_SHOWNORMAL };
        ::ShellExecuteExW(&info);
    }

    winrt::hstring GetUserName() noexcept
    {
        ::ULONG size;
        ::GetUserNameExW(::NameUserPrincipal, nullptr, &size);
        std::wstring buffer{};
        buffer.resize(size);
        ::GetUserNameExW(::NameUserPrincipal, buffer.data(), &size);

        return { buffer.data(), static_cast<winrt::hstring::size_type>(size) };
    }
}
