# MouseFix: Double-Click Issue Resolver

MouseFix is a lightweight Windows application designed to address the common issue of double-clicking in mice like the Logitech g series. The program works by intercepting mouse and keyboard inputs, implementing a simple yet effective fix for unintended double-clicks, and providing the user with control over enabling or disabling the feature via a hotkey.

---

## Features

### 🔧 Mouse Double-Click Fix
- Detects rapid consecutive left mouse button clicks and prevents them from being registered as separate clicks.
- Configurable to toggle functionality on/off.

### 🔔 Notifications
- Uses [WinToastLib](https://github.com/mohabouje/WinToast) to display toast notifications to indicate the application's state (enabled/disabled).
- Audio cues accompany the notifications for enhanced feedback.

### 🖱️ Hotkey for Toggle
- Press `F12` to toggle the application on or off, providing flexibility to the user.

---

## How It Works

1. **Hooks into Mouse and Keyboard Events**:
   - The application installs low-level hooks for mouse and keyboard input to intercept and process events.

2. **Filters Out Double Clicks**:
   - Left mouse button clicks occurring in rapid succession (within 100ms) are ignored to prevent double-clicking issues.

3. **Displays Notifications**:
   - Provides feedback to the user with toast notifications when toggling the application's state.

4. **Single Instance Mutex**:
   - Ensures only one instance of the application runs at a time.
