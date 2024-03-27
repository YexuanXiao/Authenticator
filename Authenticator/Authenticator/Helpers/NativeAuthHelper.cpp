#include "pch.h"
#include "NativeAuthHelper.h"
#include "Win32Helper.h"

#include <winrt/Windows.Security.Credentials.h>
#include <winrt/Windows.Security.Cryptography.h>
#include <winrt/Windows.Security.Cryptography.Core.h>

namespace NativeAuthHelper
{
    /*
    winrt::Windows::Foundation::IAsyncOperation<bool> CheckCredentialsAvailable() noexcept
    {
        return winrt::Windows::Security::Credentials::KeyCredentialManager::IsSupportedAsync();
    }

    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Security::Credentials::KeyCredentialRetrievalResult> OpenCredential(bool create) noexcept
    {
        if (create)
            return winrt::Windows::Security::Credentials::KeyCredentialManager::RequestCreateAsync(L"YexuanXiao.Authenticator", winrt::Windows::Security::Credentials::KeyCredentialCreationOption::ReplaceExisting);
        else
            return winrt::Windows::Security::Credentials::KeyCredentialManager::OpenAsync(L"YexuanXiao.Authenticator");
    }

    winrt::Windows::Foundation::IAsyncAction DeleteCredential() noexcept
    {
        return winrt::Windows::Security::Credentials::KeyCredentialManager::DeleteAsync(L"YexuanXiao.Authenticator");
    }

    winrt::Windows::Foundation::IAsyncAction SignPassword(winrt::hstring const& password) noexcept
    {
        // todo
        auto result{ co_await OpenCredential() };

        if (auto status{ result.Status() }; status == decltype(status)::Success)
        {
            auto credential{ result.Credential() };
            auto pwd_buffer{ winrt::Windows::Security::Cryptography::CryptographicBuffer::ConvertStringToBinary(password, winrt::Windows::Security::Cryptography::BinaryStringEncoding::Utf16LE) };
            auto result = co_await credential.RequestSignAsync(pwd_buffer);
            auto signed_buffer = result.Result();
            auto signed_hex = winrt::Windows::Security::Cryptography::CryptographicBuffer::EncodeToHexString(signed_buffer);
        }
    }
    */

    // derive a secure key from the password
    // https://zh.wikipedia.org/wiki/PBKDF2
    winrt::hstring DeriveSecureKey(winrt::hstring const& password) noexcept
    {
        auto provider{ winrt::Windows::Security::Cryptography::Core::KeyDerivationAlgorithmProvider::OpenAlgorithm(winrt::Windows::Security::Cryptography::Core::KeyDerivationAlgorithmNames::Pbkdf2Sha512()) };
        auto salt{ winrt::Windows::Security::Cryptography::CryptographicBuffer::GenerateRandom(128) };
        auto parameters{ winrt::Windows::Security::Cryptography::Core::KeyDerivationParameters::BuildForPbkdf2(salt, 210'000) };
        auto buffer{ winrt::Windows::Security::Cryptography::CryptographicBuffer::ConvertStringToBinary(password, winrt::Windows::Security::Cryptography::BinaryStringEncoding::Utf16LE) };
        auto key{ provider.CreateKey(buffer) };
        auto derived{ winrt::Windows::Security::Cryptography::Core::CryptographicEngine::DeriveKeyMaterial(key, parameters, 256 / 8) }; // AES256

        return winrt::Windows::Security::Cryptography::CryptographicBuffer::EncodeToHexString(derived);
    }

    void CreateCredential(winrt::hstring const& username, winrt::hstring const& password) noexcept
    {
        winrt::Windows::Security::Credentials::PasswordVault vault{};
        winrt::Windows::Security::Credentials::PasswordCredential normal{ L"YexuanXiao.Authenticator", username, password };
        winrt::Windows::Security::Credentials::PasswordCredential key{ L"YexuanXiao.Authenticator.Crypto", username, DeriveSecureKey(password) };
        vault.Add(normal);
        vault.Add(key);
    }

    bool CheckPasswordMatch(winrt::hstring const& username, winrt::hstring const& password) noexcept
    try
    {
        winrt::Windows::Security::Credentials::PasswordVault vault{};
        auto expect{ vault.Retrieve(L"YexuanXiao.Authenticator", username) };

        if (expect.Password() == password)
            return true;

        return false;
    }
    catch (...)
    {
        return false;
    }

    winrt::hstring GetKey(winrt::hstring const& username) noexcept
    {
        winrt::Windows::Security::Credentials::PasswordVault vault{};
        auto key{ vault.Retrieve(L"YexuanXiao.Authenticator.Crypto", username) };

        return key.Password();
    }

    winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Security::Credentials::PasswordCredential> RetrieveAll() noexcept
    try
    {
        winrt::Windows::Security::Credentials::PasswordVault vault{};

        return vault.FindAllByResource(L"YexuanXiao.Authenticator");
    }
    catch (...)
    {
        return winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Security::Credentials::PasswordCredential>{};
    }

    winrt::Windows::Foundation::Collections::IVector<winrt::hstring> RetrieveUserNames() noexcept
    {
        auto result{ winrt::single_threaded_vector<winrt::hstring>() };

        for (auto const& i : RetrieveAll())
        {
            result.Append(i.UserName());
        }

        return result;
    }

    winrt::Windows::Storage::Streams::IBuffer EncryptAESGCM(winrt::Windows::Storage::Streams::IBuffer const& key, decltype(key) const& message, decltype(message) const& nonce) noexcept
    {
        auto alg{ winrt::Windows::Security::Cryptography::Core::SymmetricKeyAlgorithmProvider::OpenAlgorithm(winrt::Windows::Security::Cryptography::Core::SymmetricAlgorithmNames::AesGcm()) };
        auto symkey{ alg.CreateSymmetricKey(key) };

        return winrt::Windows::Security::Cryptography::Core::CryptographicEngine::Encrypt(symkey, message, nonce);
    }

    winrt::Windows::Storage::Streams::IBuffer DecryptAESGCM(winrt::Windows::Storage::Streams::IBuffer const& key, decltype(key) const& message, decltype(message) const& nonce) noexcept
    {
        auto alg{ winrt::Windows::Security::Cryptography::Core::SymmetricKeyAlgorithmProvider::OpenAlgorithm(winrt::Windows::Security::Cryptography::Core::SymmetricAlgorithmNames::AesGcm()) };
        auto symkey{ alg.CreateSymmetricKey(key) };

        return winrt::Windows::Security::Cryptography::Core::CryptographicEngine::Decrypt(symkey, message, nonce);
    }
}
