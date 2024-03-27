#include "pch.h"
#include "App.xaml.h"
#include "Window.xaml.h"
#include "Helpers/Win32Helper.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::Authenticator::implementation
{
    App::App()
    {
#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
        UnhandledException([](IInspectable const&, UnhandledExceptionEventArgs const& e) {
            if (IsDebuggerPresent())
            {
                auto errorMessage = e.Message();
                __debugbreak();
            }
        });
#endif

        if (!Microsoft::Windows::AppLifecycle::AppInstance::FindOrRegisterForKey(L"YexuanXiao.Authenticator").IsCurrent())
            Win32Helper::ActivateSingletonWindow(L"YexuanXiao.Authenticator");
    }

    void App::OnLaunched([[maybe_unused]] LaunchActivatedEventArgs const&)
    {
        window = Authenticator::Window{};
        Win32Helper::DisableMultiInstanceWindow(window, L"YexuanXiao.Authenticator");
        window.Activate();
    }
}
