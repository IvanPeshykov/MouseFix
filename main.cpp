#include <windows.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include "wintoastlib.h"

using namespace WinToastLib;

class WinToastHandler : public WinToastLib::IWinToastHandler
{
public:
    WinToastHandler() = default;
    void toastActivated() const override {}
    void toastActivated(int actionIndex) const override {}
    void toastDismissed(WinToastDismissalReason state) const override {}
    void toastFailed() const override {}
private:
};

// Is application enabled
bool enabled{ true };

void showNotification() {
    WinToastTemplate templ;
    templ = WinToastTemplate(WinToastTemplate::Text02);

    templ.setTextField(enabled ? L"Application is active" : L"Application is disabled", WinToastTemplate::FirstLine);
    templ.setTextField(enabled ? L"Press F12 to disable" : L"Press F12 to enable", WinToastTemplate::SecondLine);

    templ.setDuration(WinToastTemplate::Duration::Short);
    templ.setAudioOption(WinToastTemplate::AudioOption::Default);
    templ.setAudioPath(enabled ? WinToastTemplate::AudioSystemFile::Alarm3 : WinToastTemplate::AudioSystemFile::Call2);

    if (WinToast::instance()->showToast(templ, new WinToastHandler()) == -1L)
    {
       std::cout << "Failed to show toast!";
    }
}

std::time_t getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::system_clock::to_time_t(now);
}

HHOOK mouseHook;
long long timer{};

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {

    if (enabled && nCode >= 0) {
        if (wParam == WM_LBUTTONDOWN) {

            auto now = std::chrono::system_clock::now();

            auto duration = now.time_since_epoch();
            auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();


            auto prevTimer = timer;
            timer = milliseconds;


            if (timer - prevTimer < 100) return -1;
        }
    }
    return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}

HHOOK keyboardHook;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {

    if (nCode == HC_ACTION) {
        auto* kbdStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

        // Check for F12 key press
        if (kbdStruct->vkCode == VK_F12 && wParam == WM_KEYDOWN) {
            enabled = !enabled;
            showNotification();
        }
    }
    return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}


int main() {

    auto singleInstanceMutex = CreateMutex(NULL, TRUE, L"MouseFix");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        return 1;
    }

     // Initialize win toast
    if (!WinToast::isCompatible()) {
        std::cout << L"Error, your system in not supported!" << std::endl;
        return 1;
    }

    WinToast::WinToastError error;
    WinToast::instance()->setAppName(L"MouseFixer");
    const auto aumi = WinToast::configureAUMI
    (L"Ivan Corp", L"MouseFix", L"yeah", L"2025");
    WinToast::instance()->setAppUserModelId(aumi);

    if (!WinToast::instance()->initialize(&error)) {
        std::cout << L"Error, could not initialize the lib!" << std::endl;
        return 1;
    }

    // Show init notification
    showNotification();

    // Adding hook to left click and listening to it
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, 0);
    if (mouseHook == NULL) {
        std::cout << "Failed to install hook!" << std::endl;
        return 1;
    }

    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(mouseHook);
    ReleaseMutex(singleInstanceMutex);
    return 0;
}
