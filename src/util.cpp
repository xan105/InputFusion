/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "util.h"
#include <sstream>
#include <iomanip>

namespace util {

    void EnableConsole()
    {
        if (!AllocConsole()) return;

        // Open the active console screen buffer explicitly
        HANDLE hOut = CreateFileW(
            L"CONOUT$",
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            0,
            NULL
        );

        if (hOut == INVALID_HANDLE_VALUE) return;

        // Explicitly configure the console output handle
        DWORD mode = 0;
        if (!GetConsoleMode(hOut, &mode)) return;

        mode |= ENABLE_PROCESSED_OUTPUT;
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        mode |= DISABLE_NEWLINE_AUTO_RETURN;

        SetConsoleMode(hOut, mode);

        // Redirect CRT streams to the console
        FILE* dummy = NULL;
        freopen_s(&dummy, "CONOUT$", "w", stdout);
        freopen_s(&dummy, "CONOUT$", "w", stderr);
        freopen_s(&dummy, "CONIN$", "r", stdin);

        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);

        // Configure the process standard handles too
        SetStdHandle(STD_OUTPUT_HANDLE, hOut);
        SetStdHandle(STD_ERROR_HANDLE, hOut);

        HWND hwnd = GetConsoleWindow();
        if (hwnd)
        {
            ShowWindow(hwnd, SW_SHOW);
            SetWindowPos(
                hwnd, HWND_TOPMOST,
                0, 0, 0, 0,
                SWP_NOMOVE | SWP_NOSIZE
            );
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

    std::wstring GetLastErrorMessage() {
        DWORD code = GetLastError();
        LPWSTR buffer = nullptr;
        size_t size = FormatMessageW(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            code,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPWSTR)&buffer,
            0,
            NULL
        );
        std::wstring message(buffer, size);
        LocalFree(buffer);
        return L"Error " + std::to_wstring(code) + L": " + message;
    }

    std::wstring GetModuleFileNameWString(HMODULE hMod) {
        WCHAR buffer[MAX_PATH] = {};
        DWORD length = GetModuleFileNameW(hMod, buffer, MAX_PATH);
        if (length == 0 || length >= MAX_PATH) {
            return {};
        }
        return std::wstring(buffer, length);
    }

    const std::wstring& GetModuleDirectory() {
        static const std::wstring dir = []() -> std::wstring {
            HMODULE hMod = nullptr;

            if (!GetModuleHandleExW(
                GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                reinterpret_cast<LPCWSTR>(&GetModuleDirectory),
                &hMod))
            {
                return {};
            }

            std::wstring path = GetModuleFileNameWString(hMod);
            const auto pos = path.find_last_of(L"\\/");
            if (pos != std::wstring::npos) {
                path.resize(pos + 1); // Keep trailing slash
            }

            return path;
            }();

        return dir;
    }

    const std::wstring& GetCurrentProcessName() {
        static const std::wstring name = []() -> std::wstring {

            std::wstring path = GetModuleFileNameWString(nullptr);
            const auto pos = path.find_last_of(L"\\");
            if (pos != std::wstring::npos) {
                path = path.substr(pos + 1);
            }

            return path;
            }();

        return name;
    }

    const std::wstring& GetCurrentWorkingDir() {
        static const std::wstring dir = []() -> std::wstring {
            WCHAR buffer[MAX_PATH] = {};

            DWORD length = GetCurrentDirectoryW(MAX_PATH, buffer);
            if (length == 0 || length >= MAX_PATH) {
                return {};
            }

            std::wstring path(buffer, length);
            if (path.back() != L'\\') {
                path += L'\\'; // Add trailing slash
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

    std::wstring StringToUtf16(const std::string& str)
    {
        if (str.empty()) {
            return {};
        }

        const int size = MultiByteToWideChar(
            CP_UTF8, 
            MB_ERR_INVALID_CHARS,
            str.data(), 
            static_cast<int>(str.size()),
            nullptr, 
            0
        );

        if (size == 0) {
            return {};
        }

        std::wstring result(static_cast<size_t>(size), L'\0');

        if (MultiByteToWideChar(
            CP_UTF8, 
            MB_ERR_INVALID_CHARS,
            str.data(), 
            static_cast<int>(str.size()),
            result.data(), 
            size) != size)
        {
            return {};
        }

        return result;
    }
}