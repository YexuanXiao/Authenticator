#pragma once
#define COM_NO_WINDOWS_H
#include <unknwn.h>
// Undefine WinBase.h GetCurrentTime macro to prevent
// conflict with Storyboard::GetCurrentTime
#undef GetCurrentTime

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.ApplicationModel.Activation.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Data.h>
#include <winrt/Microsoft.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Navigation.h>
#include <winrt/Microsoft.UI.Xaml.Shapes.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <wil/cppwinrt_helpers.h>

#include <winrt/Microsoft.Windows.AppLifecycle.h>

#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Security.Credentials.UI.h>

#include <winrt/Microsoft.UI.Windowing.h>

inline winrt::hstring operator""_hs(wchar_t const* begin, std::size_t const size) {
	return winrt::hstring{ begin, static_cast<winrt::hstring::size_type>(size) };
}

#if !defined(__cpp_size_t_suffix) || __cpp_size_t_suffix <= 202006L
// for C++ 23 std::size_t literal workaround
inline constexpr std::size_t operator""uz(unsigned long long const value) {
	if constexpr (sizeof(value) <= sizeof(std::size_t)) {
		return { value };
	}
	else {
		assert(value < SIZE_MAX);
		return { value };
	}
}
#endif
