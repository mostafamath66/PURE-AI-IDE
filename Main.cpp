#include <windows.h>
#include <shellapi.h>
#include <string>
#include <vector>

// --- 1. Constants and Identifiers ---
#define ID_AI_CHAT   101
#define ID_REPO      102
#define ID_PREVIEW   103
#define ID_BTN_LINK  104

// Global Handles
HWND hChat, hRepo, hPreview, hBtnLink;

// --- 2. Resource Extraction Logic ---
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

// --- 3. Drag and Drop Handler ---
void HandleFileDrop(HDROP hDrop, HWND hList) {
    UINT fileCount = DragQueryFileW(hDrop, 0xFFFFFFFF, NULL, 0);
    for (UINT i = 0; i < fileCount; i++) {
        wchar_t filePath[MAX_PATH];
        DragQueryFileW(hDrop, i, filePath, MAX_PATH);
        SendMessageW(hList, LB_ADDSTRING, 0, (LPARAM)filePath);
    }
    DragFinish(hDrop);
}

// --- 4. Main Window Procedure ---
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            // Left: Repository
            CreateWindowW(L"Static", L"Project Repository", WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 0, 200, 20, hwnd, NULL, NULL, NULL);
            hRepo = CreateWindowW(L"ListBox", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL, 0, 20, 200, 480, hwnd, (HMENU)ID_REPO, NULL, NULL);

            // Center: AI Chat
            CreateWindowW(L"Static", L"PURE-AI Programming Chat", WS_CHILD | WS_VISIBLE | SS_CENTER, 205, 0, 400, 20, hwnd, NULL, NULL, NULL);
            hChat = CreateWindowW(L"Edit", L"Type your coding request here...", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL, 205, 20, 400, 430, hwnd, (HMENU)ID_AI_CHAT, NULL, NULL);

            // Right: Preview
            CreateWindowW(L"Static", L"Project Preview", WS_CHILD | WS_VISIBLE | SS_CENTER, 610, 0, 300, 20, hwnd, NULL, NULL, NULL);
            hPreview = CreateWindowW(L"Static", L"Final Output Window", WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER, 610, 20, 300, 480, hwnd, (HMENU)ID_PREVIEW, NULL, NULL);

            // Bottom: Generate Link Button
            hBtnLink = CreateWindowW(L"Button", L"Generate Web Preview Link", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 205, 460, 400, 40, hwnd, (HMENU)ID_BTN_LINK, NULL, NULL);
            
            DragAcceptFiles(hwnd, TRUE);
            break;

        case WM_COMMAND:
            if (LOWORD(wp) == ID_BTN_LINK) {
                ShellExecuteW(NULL, L"open", L"http://localhost:8080", NULL, NULL, SW_SHOWNORMAL);
            }
            break;

        case WM_DROPFILES:
            HandleFileDrop((HDROP)wp, hRepo);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
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
    
    HWND hwnd = CreateWindowW(L"PURE_AI_CLASS", L"PURE-AI-Programmer v1.0 [Full Version]", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 950, 550, NULL, NULL, hInst, NULL);
    
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
