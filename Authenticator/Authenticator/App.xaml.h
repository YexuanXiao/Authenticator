#pragma once

#include "App.xaml.g.h"

namespace winrt::Authenticator::implementation
{
    struct App : AppT<App>
    {
        App();

        void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);

    private:
        winrt::Authenticator::Window window{ nullptr };
    };
}
