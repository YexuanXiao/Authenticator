#pragma once

#include "Detail.g.h"

namespace winrt::Authenticator::implementation
{
    struct Detail : DetailT<Detail>
    {
        Detail()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }
    };
}

namespace winrt::Authenticator::factory_implementation
{
    struct Detail : DetailT<Detail, implementation::Detail>
    {
    };
}
