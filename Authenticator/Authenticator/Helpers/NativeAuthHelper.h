#pragma once
#include "pch.h"

namespace NativeAuthHelper
{
    /*
    * KeyCredentialManager is buggy, so these functions should not be used
    * see: https://github.com/microsoft/WindowsAppSDK/issues/4290
    winrt::Windows::Foundation::IAsyncOperation<bool> CheckCredentialsAvailable() noexcept;
    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Security::Credentials::KeyCredentialRetrievalResult> OpenCredential(bool create = true) noexcept;
    winrt::Windows::Foundation::IAsyncAction DeleteCredential() noexcept;
    winrt::Windows::Foundation::IAsyncAction SignPassword(winrt::hstring const& password) noexcept;
    */
    winrt::hstring DeriveSecureKey(winrt::hstring const& password) noexcept;
    void CreateCredential(winrt::hstring const& username, winrt::hstring const& password) noexcept;
    bool CheckPasswordMatch(winrt::hstring const& username, winrt::hstring const& password) noexcept;
    winrt::hstring GetKey(winrt::hstring const& username) noexcept;
    winrt::Windows::Foundation::Collections::IVector<winrt::hstring> RetrieveUserNames() noexcept;
    winrt::Windows::Storage::Streams::IBuffer EncryptAESGCM(winrt::Windows::Storage::Streams::IBuffer const& key, decltype(key) const& message, decltype(message) const& nonce) noexcept;
    winrt::Windows::Storage::Streams::IBuffer DecryptAESGCM(winrt::Windows::Storage::Streams::IBuffer const& key, decltype(key) const& message, decltype(message) const& nonce) noexcept;
}