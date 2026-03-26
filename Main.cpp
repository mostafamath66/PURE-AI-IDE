#include <windows.h>
#include <string>
#include "AI_Logic.h"
#include "Security.h"

// تعريف معرفات الواجهة
#define ID_AI_CHAT 101
#define ID_REPO 102
#define ID_PREVIEW 103
#define ID_BTN_LINK 104

HWND hChat, hRepo, hPreview, hBtnLink;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            // 1. إنشاء المستودع (يسار)
            hRepo = CreateWindowExW(WS_EX_ACCEPTFILES, L"ListBox", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 0, 0, 200, 500, hwnd, (HMENU)ID_REPO, NULL, NULL);
            // 2. إنشاء شات الـ AI (منتصف)
            hChat = CreateWindowExW(0, L"Edit", L"اطلب ميزة جديدة هنا...", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE, 205, 0, 400, 450, hwnd, (HMENU)ID_AI_CHAT, NULL, NULL);
            // 3. إنشاء المعاينة (يمين)
            hPreview = CreateWindowExW(0, L"Static", L"المعاينة النهائية", WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER, 610, 0, 300, 500, hwnd, (HMENU)ID_PREVIEW, NULL, NULL);
            // 4. زر استخراج الرابط
            hBtnLink = CreateWindowExW(0, L"Button", L"استخراج رابط ويب", WS_CHILD | WS_VISIBLE, 205, 460, 400, 30, hwnd, (HMENU)ID_BTN_LINK, NULL, NULL);
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == ID_BTN_LINK) {
                // استدعاء محرك الويب وفتح المتصفح
                ExecuteWebBridge();
            }
            break;

        case WM_DROPFILES:
            // محرك السحب والإفلات الذكي
            HandleFileDrop((HDROP)wParam, hRepo);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}