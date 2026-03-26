#include <windows.h>
#include <shellapi.h>
#include <string>
#include <vector>
#include <fstream>

// --- 1. تعريف المعرفات (Identifiers) المفقودة ---
#define ID_AI_CHAT 101
#define ID_REPO    102
#define ID_PREVIEW 103
#define ID_BTN_LINK 104

// --- 2. وظيفة المعاينة وفتح الرابط (ExecuteWebBridge) ---
void ExecuteWebBridge() {
    // فتح السيرفر المحلي (افتراضياً على 8080)
    ShellExecuteW(NULL, L"open", L"http://localhost:8080", NULL, NULL, SW_SHOWNORMAL);
}

// --- 3. وظيفة السحب والإفلات (HandleFileDrop) ---
void HandleFileDrop(HDROP hDrop, HWND hList) {
    UINT fileCount = DragQueryFileW(hDrop, 0xFFFFFFFF, NULL, 0);
    for (UINT i = 0; i < fileCount; i++) {
        wchar_t filePath[MAX_PATH];
        DragQueryFileW(hDrop, i, filePath, MAX_PATH);
        SendMessageW(hList, LB_ADDSTRING, 0, (LPARAM)filePath);
    }
    DragFinish(hDrop);
}

// --- 4. وظيفة استخراج الموارد (ExtractResource) ---
void ExtractResource(int resId, std::wstring outPath) {
    HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(resId), RT_RCDATA);
    if (!hRes) return;
    HGLOBAL hData = LoadResource(NULL, hRes);
    DWORD size = SizeofResource(NULL, hRes);
    void* pData = LockResource(hData);
    HANDLE hFile = CreateFileW(outPath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD written;
        WriteFile(hFile, pData, size, &written, NULL);
        CloseHandle(hFile);
    }
}

// --- 5. واجهة البرنامج الرئيسية (Window Procedure) ---
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    static HWND hChat, hRepo, hPreview;
    switch (msg) {
       case WM_CREATE:
            // 1. عنوان المربع الأيسر (المستودع)
            CreateWindowW(L"Static", L"📦 مستودع المشروع", WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 0, 200, 20, hwnd, NULL, NULL, NULL);
            hRepo = CreateWindowW(L"ListBox", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL, 0, 20, 200, 480, hwnd, (HMENU)ID_REPO, NULL, NULL);

            // 2. عنوان المربع الأوسط (شات الذكاء الاصطناعي)
            CreateWindowW(L"Static", L"🤖 PURE-AI Chat", WS_CHILD | WS_VISIBLE | SS_CENTER, 205, 0, 400, 20, hwnd, NULL, NULL, NULL);
            hChat = CreateWindowW(L"Edit", L"اطلب من الذكاء الاصطناعي توليد أو تصحيح الأكواد...", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL, 205, 20, 400, 430, hwnd, (HMENU)ID_AI_CHAT, NULL, NULL);

            // 3. عنوان المربع الأيمن (المعاينة)
            CreateWindowW(L"Static", L"🖥️ معاينة المشروع", WS_CHILD | WS_VISIBLE | SS_CENTER, 610, 0, 300, 20, hwnd, NULL, NULL, NULL);
            hPreview = CreateWindowW(L"Static", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 610, 20, 300, 480, hwnd, (HMENU)ID_PREVIEW, NULL, NULL);

            // 4. الزر السفلي (رابط الويب)
            hBtnLink = CreateWindowW(L"Button", L"🌐 استخراج رابط ويب للمشروع", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 205, 460, 400, 40, hwnd, (HMENU)ID_BTN_LINK, NULL, NULL);
            
            DragAcceptFiles(hwnd, TRUE);
            break;
        case WM_COMMAND:
            if (LOWORD(wp) == ID_BTN_LINK) ExecuteWebBridge();
            break;
        case WM_DROPFILES:
            HandleFileDrop((HDROP)wp, hRepo);
            break;
        case WM_DESTROY: PostQuitMessage(0); break;
    }
    return DefWindowProc(hwnd, msg, wp, lp);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow) {
    WNDCLASSW wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"PURE_AI_CLASS";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassW(&wc);
    HWND hwnd = CreateWindowW(L"PURE_AI_CLASS", L"PURE-AI-Programmer v1.0", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 950, 550, NULL, NULL, hInst, NULL);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) { TranslateMessage(&msg); DispatchMessage(&msg); }
    return 0;
}
