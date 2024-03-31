#include "pch.h"
#include "App.xaml.h"
#include "Window.xaml.h"
#include "Helpers/Win32Helper.h"
#include "Helpers/SettingsHelper.h"
#include <winrt/Microsoft.UI.Windowing.h>

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
        // OnLaunched and the constructor are executed at same time, so any initialization should be the responsibility of OnLaunched
    }

    void App::OnLaunched([[maybe_unused]] LaunchActivatedEventArgs const&)
    {
        // checking for the presence of other instances
        if (!Microsoft::Windows::AppLifecycle::AppInstance::FindOrRegisterForKey(L"YexuanXiao.Authenticator").IsCurrent())
        {
            Win32Helper::ActivateSingletonWindow(L"YexuanXiao.Authenticator");
            Win32Helper::Exit(Win32Helper::exit_code::instance_is_exist);
        }

        window_ = {};
        // add tag to activate the window from another process
        Win32Helper::DisableMultiInstanceWindow(window_, L"YexuanXiao.Authenticator");
        // make app display correct name in Task Manager
#ifdef _DEBUG
        window_.Title(L"Authenticator Dev");
#else
        window_.Title(L"Authenticator");
#endif //  _DEBUG
        auto titleBar{ window_.TitleBar() };
        auto appWindow{ window_.AppWindow() };
        // initialize appTitleBar_
        appTitleBar_ = appWindow.TitleBar();

        // Microsoft Fluent Icons
        // MIT Lisence
        // https://github.com/microsoft/fluentui-system-icons
        // PNG to ICO Converter
        // https://www.aconvert.com/cn/icon/png-to-ico/
        appWindow.SetIcon(L"Assets/Icon.ico");

#define Windows10 true
        // test Windows 10 branch on Windows 11
        if (Microsoft::UI::Windowing::AppWindowTitleBar::IsCustomizationSupported() && !Windows10) [[likely]]
#undef Windows10
        {
            // hide the titlebar of Window and display the titlebar of AppWindow
            appTitleBar_.ExtendsContentIntoTitleBar(true);
            // set UWP-style full height button
            appTitleBar_.PreferredHeightOption(Microsoft::UI::Windowing::TitleBarHeightOption::Tall);
            // todo
            appWindow.Changed({ this, &App::AppWindow_Changed });
            // notify AppTitleBar when the page is loaded and set the drag-and-drop area
            titleBar.Loaded({ this, &App::TitleBar_Loaded });
            // also do this when the page size changes
            titleBar.SizeChanged({ this, &App::TitleBar_SizeChanged });

            // read the stored theme
            SetTitleBarTheme(SettingsHelper::LoadTheme());
            // notify AppWindow when the theme changes
            titleBar.ActualThemeChanged({ this, &App::TitleBar_ThemeChanged });
        }
        else
        {
            // in the case that titlebar customization is not supported, fallback to WindowChrome
            window_.ExtendsContentIntoTitleBar(true);
            window_.SetTitleBar(window_.TitleBar());
        }

        window_.Activate();
    }

    void App::TitleBar_ThemeChanged(Microsoft::UI::Xaml::FrameworkElement const& sender, Windows::Foundation::IInspectable const&)
    {
        assert(appTitleBar_.ExtendsContentIntoTitleBar());
        SetTitleBarTheme(sender.ActualTheme());
    }

    void App::AppWindow_Changed(Microsoft::UI::Windowing::AppWindow sender, Microsoft::UI::Windowing::AppWindowChangedEventArgs const&)
    {
        // todo: https://learn.microsoft.com/zh-cn/windows/apps/develop/title-bar?tabs=wasdk#full-customization-example
    }

    void App::TitleBar_Loaded(Windows::Foundation::IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        assert(Microsoft::UI::Windowing::AppWindowTitleBar::IsCustomizationSupported());
        SetDragRegionForCustomTitleBar();
    }

    void App::TitleBar_SizeChanged(Windows::Foundation::IInspectable const&, Microsoft::UI::Xaml::SizeChangedEventArgs const&)
    {
        assert(Microsoft::UI::Windowing::AppWindowTitleBar::IsCustomizationSupported());
        assert(window_.AppWindow().TitleBar().ExtendsContentIntoTitleBar());
        SetDragRegionForCustomTitleBar();
    }

    void App::SetDragRegionForCustomTitleBar()
    {
        assert(Microsoft::UI::Windowing::AppWindowTitleBar::IsCustomizationSupported());
        auto appTitleBar{ window_.AppWindow().TitleBar() };
        assert(appTitleBar.ExtendsContentIntoTitleBar());
        auto scaleAdjustment{ Win32Helper::GetScaleAdjustment(window_) };
        auto titleBar{ window_.TitleBar() };
        auto rect{ Windows::Graphics::RectInt32{} };
        rect.X = static_cast<int32_t>((appTitleBar.LeftInset() + 48) * scaleAdjustment);
        rect.Y = 0;
        rect.Height = static_cast<int32_t>(48 * scaleAdjustment);
        rect.Width = static_cast<int32_t>(titleBar.ActualWidth() * scaleAdjustment - rect.X - appTitleBar.RightInset());
        appTitleBar.SetDragRectangles(winrt::array_view(&rect, &rect + 1));
    }

    void App::SetTitleBarTheme(Microsoft::UI::Xaml::ElementTheme theme)
    {
        using enum Microsoft::UI::Xaml::ElementTheme;
        using namespace Microsoft::UI;

        if (theme == Default) [[likely]]
        {
            appTitleBar_.BackgroundColor(nullptr);
            appTitleBar_.ButtonBackgroundColor(Colors::Transparent());
            appTitleBar_.ButtonForegroundColor(nullptr);
            appTitleBar_.ButtonInactiveBackgroundColor(nullptr);
            appTitleBar_.ButtonInactiveForegroundColor(nullptr);
            appTitleBar_.ButtonHoverBackgroundColor(nullptr);
            appTitleBar_.ButtonHoverForegroundColor(nullptr);
            appTitleBar_.ButtonPressedBackgroundColor(nullptr);
            appTitleBar_.ButtonPressedForegroundColor(nullptr);
        }
        else if (theme == Dark)
        {
            appTitleBar_.BackgroundColor(Colors::Transparent());
            appTitleBar_.ButtonBackgroundColor(Colors::Transparent());
            appTitleBar_.ButtonForegroundColor(Colors::White());
            appTitleBar_.ButtonInactiveBackgroundColor(Colors::Transparent());
            appTitleBar_.ButtonInactiveForegroundColor(ColorHelper::FromArgb(255, 96, 96, 96));
            appTitleBar_.ButtonHoverBackgroundColor(ColorHelper::FromArgb(255, 45, 45, 45));
            appTitleBar_.ButtonHoverForegroundColor(Colors::White());
            appTitleBar_.ButtonPressedBackgroundColor(ColorHelper::FromArgb(255, 41, 41, 41));
            appTitleBar_.ButtonPressedForegroundColor(ColorHelper::FromArgb(255, 167, 167, 167));
        }
        else if (theme == Light)
        {
            appTitleBar_.BackgroundColor(Colors::Transparent());
            appTitleBar_.ButtonBackgroundColor(Colors::Transparent());
            appTitleBar_.ButtonForegroundColor(Colors::Black());
            appTitleBar_.ButtonInactiveBackgroundColor(ColorHelper::FromArgb(255, 243, 243, 243));
            appTitleBar_.ButtonInactiveForegroundColor(ColorHelper::FromArgb(255, 144, 144, 144));
            appTitleBar_.ButtonHoverBackgroundColor(ColorHelper::FromArgb(255, 219, 219, 219));
            appTitleBar_.ButtonHoverForegroundColor(Colors::Black());
            appTitleBar_.ButtonPressedBackgroundColor(ColorHelper::FromArgb(255, 194, 194, 194));
            appTitleBar_.ButtonPressedForegroundColor(ColorHelper::FromArgb(255, 95, 95, 95));
        }
    }

    App::~App()
    {
        SettingsHelper::SetFirstUse();
    }
}
