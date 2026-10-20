/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "util.h"

namespace util {

    void EnableConsole() {

        if (AllocConsole()) {
            HWND consoleWindow = GetConsoleWindow();

            if (consoleWindow) {
                // Set the console window to be layered
                LONG style = GetWindowLong(consoleWindow, GWL_EXSTYLE);
                SetWindowLong(consoleWindow, GWL_EXSTYLE, style | WS_EX_LAYERED);
                SetLayeredWindowAttributes(consoleWindow, 0, 225, LWA_COLORKEY);

                // Show the console window
                ShowWindow(consoleWindow, SW_SHOW);
            }

            // Redirect stdio to the console
            FILE* dummy;
            freopen_s(&dummy, "CONOUT$", "w", stdout);
            freopen_s(&dummy, "CONIN$", "r", stdin);
            freopen_s(&dummy, "CONOUT$", "w", stderr);
        }
    }
    
    std::wstring GetEnvVar(LPCWSTR name) {
        std::wstring buffer(65535, L'\0');
        DWORD size = GetEnvironmentVariableW(name, &buffer[0], static_cast<DWORD>(buffer.size()));
        if (size) {
            buffer.resize(size);
            return buffer;
        }
        else {
            return L"";
        }
    }

    const std::wstring& GetModuleDirectory() {
        static const std::wstring dir = []() -> std::wstring {
            HMODULE hModule = nullptr;

            if (!GetModuleHandleExW(
                GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                reinterpret_cast<LPCWSTR>(&GetModuleDirectory),
                &hModule))
            {
                return L"";
            }

            WCHAR buffer[MAX_PATH] = {};
            DWORD length = GetModuleFileNameW(hModule, buffer, MAX_PATH);

            if (length == 0 || length >= MAX_PATH) {
                return L"";
            }

            std::wstring path(buffer, length);
            const auto pos = path.find_last_of(L"\\/");

            if (pos != std::wstring::npos) {
                path.resize(pos + 1); // Keep trailing slash
            }

            return path;
            }();

        return dir;
    }

    // Converts a GUID to a std::string
    std::string GUIDToString(REFIID riid) {
        std::ostringstream oss;
        oss << std::hex << std::setfill('0')
            << std::setw(8) << riid.Data1 << '-'
            << std::setw(4) << riid.Data2 << '-'
            << std::setw(4) << riid.Data3 << '-';

        for (int i = 0; i < 2; ++i)
            oss << std::setw(2) << static_cast<int>(riid.Data4[i]);

        oss << '-';
        for (int i = 2; i < 8; ++i)
            oss << std::setw(2) << static_cast<int>(riid.Data4[i]);

        return oss.str();
    }

    std::wstring GetModuleFileNameWString(HMODULE hMod) {
        WCHAR szPath[MAX_PATH] = {};
        DWORD length = GetModuleFileNameW(hMod, szPath, MAX_PATH);
        if (length == 0 || length >= MAX_PATH) {
            return {};
        }
        return std::wstring(szPath, length);
    }

    std::string WStringToUtf8(const std::wstring& wstr)
    {
        if (wstr.empty()) {
            return {};
        }

        if (wstr.size() > static_cast<size_t>((std::numeric_limits<int>::max)())) {
            return {};
        }

        const int len = static_cast<int>(wstr.size());

        const int size = WideCharToMultiByte(
            CP_UTF8, 
            WC_ERR_INVALID_CHARS,
            wstr.data(), 
            len, 
            nullptr, 
            0, 
            nullptr, 
            nullptr
        );

        if (size == 0) {
            return {};
        }

        std::string result(static_cast<size_t>(size), '\0');

        if (WideCharToMultiByte(
            CP_UTF8,
            WC_ERR_INVALID_CHARS,
            wstr.data(),
            len,
            result.data(),
            size,
            nullptr,
            nullptr) != size)
        {
            return {};
        }

        return result;
    }
}