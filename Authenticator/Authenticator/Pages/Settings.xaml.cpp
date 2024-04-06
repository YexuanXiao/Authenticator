#include "pch.h"
#include "Settings.xaml.h"
#if __has_include("Settings.g.cpp")
#include "Settings.g.cpp"
#endif

#include "Helpers/Win32Helper.h"
#include "Helpers/SettingsHelper.h"
#include <winrt/Windows.Globalization.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::Authenticator::implementation
{
    Windows::Foundation::IAsyncAction Settings::Page_Loading(Microsoft::UI::Xaml::FrameworkElement const&, Windows::Foundation::IInspectable const&)
    {
        if (co_await SettingsHelper::CheckWindowsHelloAvailable())
        {
            Hello().IsEnabled(true);

            if (SettingsHelper::CheckWindowsHelloEnabled())
                Hello().IsOn(true);
        }
        else
        {
            auto brush{ Microsoft::UI::Xaml::Application::Current().Resources().Lookup(winrt::box_value(L"SystemFillColorCriticalBackgroundBrush")) };
            HelloExpander().Background(brush.try_as<Microsoft::UI::Xaml::Media::Brush>());
            HelloDesc().Text(resource_.GetString(L"HelloUnsupported/Text"));
        }

        // Cannot binding handler in XAML because the event will be triggered by manually setting isOn,
        // causing the WindowsHello to be validated immediately

        Hello().Toggled({ this, &Settings::Hello_Toggle });
    }

    Windows::Foundation::IAsyncAction Settings::Hello_Toggle(Windows::Foundation::IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        if (Hello().IsOn())
        {
            if (co_await SettingsHelper::CheckWindowsHelloAvailable())
            {
                if (co_await SettingsHelper::RequestWindowsHello())
                {
                    SettingsHelper::WindowsHello(true);
                    co_return;
                }
            }
            else
            {
                auto brush{ Microsoft::UI::Xaml::Application::Current().Resources().Lookup(winrt::box_value(L"SystemFillColorCriticalBackgroundBrush")) };
                HelloExpander().Background(brush.try_as<Microsoft::UI::Xaml::Media::Brush>());
                HelloDesc().Text(resource_.GetString(L"HelloUnsupported/Text"));
            }
        }
        else
        {
            SettingsHelper::WindowsHello(false);
        }

        Hello().IsOn(false);
    }

    void Settings::InitializeComponent()
    {
        SettingsT::InitializeComponent();
        auto items{ Languages().Items() };

        // add default lang item
        {
            auto item{ Microsoft::UI::Xaml::Controls::MenuFlyoutItem{} };
            item.Text(resource_.GetString(L"Default/Content"));
            item.Tag(winrt::box_value(winrt::hstring{ L"default" }));
            item.Tapped(&Settings::Language_Selected);
            items.Append(item);
        }
        // add manifest langs
        for (auto bcp47 : Windows::Globalization::ApplicationLanguages::ManifestLanguages()) [[likely]]
        {
            auto lang{ Windows::Globalization::Language{ bcp47 } };
            auto item{ Microsoft::UI::Xaml::Controls::MenuFlyoutItem{} };
            item.Text(lang.NativeName());
            item.Tag(winrt::box_value(lang));
            item.Tapped(&Settings::Language_Selected);
            items.Append(item);
        }

        // first use
        if (!SettingsHelper::CheckFirstUse()) [[likely]]
            return;

        for (auto item : ContentGrid().Children()) [[likely]]
        {
            auto expander{ item.try_as<Microsoft::UI::Xaml::Controls::Expander>() };
            if (expander != nullptr)
                expander.IsExpanded(true);
        }
    }

    void Settings::Theme_Changed(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Controls::SelectionChangedEventArgs const&)
    {
        auto radio_button{ sender.try_as<Microsoft::UI::Xaml::Controls::RadioButtons>().SelectedItem().try_as<Microsoft::UI::Xaml::Controls::RadioButton>() };
        // if no item select, return
        if (radio_button == nullptr) [[likely]]
            return;

        auto tag{ winrt::unbox_value<winrt::hstring>(radio_button.Tag()) };
        auto theme{ Microsoft::UI::Xaml::ElementTheme::Default };

        if (tag == L"dark") [[likely]]
            theme = Microsoft::UI::Xaml::ElementTheme::Dark;
        else if (tag == L"light")
            theme = Microsoft::UI::Xaml::ElementTheme::Light;

        SettingsHelper::SetTheme(XamlRoot(), theme);
    }

    void Settings::Theme_Loaded(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        auto radioButtons{ sender.try_as<Microsoft::UI::Xaml::Controls::RadioButtons>() };
        auto pre{ SettingsHelper::LoadTheme() };
        radioButtons.SelectedIndex(static_cast<int32_t>(pre));
    }

    void Settings::Language_Selected(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Input::TappedRoutedEventArgs const&)
    {
        auto lang{ winrt::unbox_value<hstring>(sender.try_as<Microsoft::UI::Xaml::Controls::MenuFlyoutItem>().Tag()) };
        if (lang == L"default") [[unlikely]]
            Windows::Globalization::ApplicationLanguages::PrimaryLanguageOverride(winrt::hstring{});
        else
            // always persistence
            Windows::Globalization::ApplicationLanguages::PrimaryLanguageOverride(lang);
    }

    Windows::Foundation::IAsyncAction Settings::Reset_Tapped(Windows::Foundation::IInspectable const&, Microsoft::UI::Xaml::Input::TappedRoutedEventArgs const& args)
    {
        args.Handled(true);
        auto theme{ ActualTheme() };
        auto dialog{ Microsoft::UI::Xaml::Controls::ContentDialog{} };
        dialog.XamlRoot(XamlRoot());
        dialog.Title(winrt::box_value(resource_.GetString(L"Reset/Text") + L' ' + resource_.GetString(L"ApplicationData/Text")));
        dialog.CloseButtonText(common_.GetString(L"Cancel/Text"));
        dialog.DefaultButton(Microsoft::UI::Xaml::Controls::ContentDialogButton::Close);
        dialog.PrimaryButtonText(resource_.GetString(L"Reset/Text"));
        auto content{ Microsoft::UI::Xaml::Controls::Grid{} };
        auto rows{ content.RowDefinitions() };
        auto def1{ Microsoft::UI::Xaml::Controls::RowDefinition{} };
        def1.Height(Microsoft::UI::Xaml::GridLengthHelper::Auto());
        auto def2{ Microsoft::UI::Xaml::Controls::RowDefinition{} };
        def2.Height(Microsoft::UI::Xaml::GridLengthHelper::Auto());
        rows.Append(def1);
        rows.Append(def2);
        auto icon{ Microsoft::UI::Xaml::Controls::FontIcon{} };
        icon.Glyph(L"\uE7BA");
        icon.FontSize(50.);
        icon.Margin(Microsoft::UI::Xaml::ThicknessHelper::FromUniformLength(30.));
        icon.MaxWidth(300.);
        auto text{ Microsoft::UI::Xaml::Controls::TextBlock{} };
        text.Text(resource_.GetString(L"ResetWarning/Text"));
        text.TextAlignment(Microsoft::UI::Xaml::TextAlignment::Center);
        text.TextWrapping(Microsoft::UI::Xaml::TextWrapping::Wrap);
        text.HorizontalAlignment(Microsoft::UI::Xaml::HorizontalAlignment::Center);
        Microsoft::UI::Xaml::Controls::Grid::SetRow(icon, 0);
        Microsoft::UI::Xaml::Controls::Grid::SetRow(text, 1);
        auto children{ content.Children() };
        children.Append(icon);
        children.Append(text);
        dialog.Content(content);
        dialog.RequestedTheme(theme);
        auto result{ co_await dialog.ShowAsync() };

        if (result != Microsoft::UI::Xaml::Controls::ContentDialogResult::Primary)
            co_return;

        co_await SettingsHelper::RemoveAllData();

        Win32Helper::Exit(Win32Helper::exit_code::remove_data);
    }
}
