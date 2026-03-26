// SelfInstall.h
void InjectIntoTarget(std::wstring targetProgramPath) {
    // 1. الحصول على مسار المجلد للبرنامج المستهدف
    std::wstring targetDir = GetDirectory(targetProgramPath);
    
    // 2. استخراج المحركات الأساسية (7z.dll & TCC) من موارد البرنامج الحالية
    // وزرعها صامتاً في مجلد البرنامج المستهدف
    ExtractResource(ZIP_DLL, targetDir + L"\\core_engine.dll");
    ExtractResource(TCC_BIN, targetDir + L"\\compiler.exe");

    // 3. إنشاء ملف "الارتباط الذكي" (Launcher)
    // هذا الملف يجعل البرنامج المستهدف يتعرف على الـ AI Terminal الخاص بنا
    std::ofstream loader(targetDir + L"\\ai_addon.txt");
    loader << "AI_INTEGRATION_ACTIVE=TRUE\nPORT=8080";
    loader.close();

    ShowTaskbarNotification(L"تثبيت ناجح", L"تم دمج ميزات الذاء الاصطناعي داخل البرنامج المستهدف بنجاح.");
}