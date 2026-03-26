#include <windows.h>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

// وظيفة مراقبة المستودع وإرسال التنبيه للـ AI
void WatchRepositoryChanges(const std::wstring& repoPath) {
    static auto lastWriteTime = fs::file_time_type::min();
    
    for (auto& p : fs::recursive_directory_iterator(repoPath)) {
        auto currentWriteTime = fs::last_write_time(p);
        
        if (currentWriteTime > lastWriteTime) {
            std::string fileName = p.path().filename().string();
            std::string extension = p.path().extension().string();

            // إرسال رسالة إلى شاشة الشات (المنتصف)
            std::string aiMessage = "لقد اكتشفت ملفاً جديداً: " + fileName;
            
            if (extension == ".dll") {
                aiMessage += ". هل تريد مني كتابة كود لربط هذه المكتبة بالمشروع؟";
            } else if (extension == ".txt") {
                aiMessage += ". هل هذا ملف إعدادات؟ يمكنني مساعدتك في برمجته.";
            }

            // تحديث واجهة الشات آلياً
            SetWindowTextA(GetDlgItem(hMainWnd, ID_AI_CHAT), aiMessage.c_str());
            lastWriteTime = currentWriteTime;
        }
    }
}