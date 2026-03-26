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
            hRepo = CreateWindowW(L"ListBox", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 0, 0, 200, 500, hwnd, (HMENU)ID_REPO, NULL, NULL);
            hChat = CreateWindowW(L"Edit", L"PURE-AI: اطلب تطوير ميزتك هنا...", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE, 205, 0, 400, 450, hwnd, (HMENU)ID_AI_CHAT, NULL, NULL);
            hPreview = CreateWindowW(L"Static", L"Preview Area", WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER, 610, 0, 300, 500, hwnd, (HMENU)ID_PREVIEW, NULL, NULL);
            CreateWindowW(L"Button", L"استخراج رابط ويب", WS_CHILD | WS_VISIBLE, 205, 460, 400, 30, hwnd, (HMENU)ID_BTN_LINK, NULL, NULL);
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
