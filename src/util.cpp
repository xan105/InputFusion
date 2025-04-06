/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "util.h"

std::wstring Getenv(LPCWSTR name) {
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

void enableConsole() {

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