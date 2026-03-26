#include <windows.h>
#include <wincrypt.h> // مكتبة الويندوز الأصلية للتشفير (خفيفة جداً)

// وظيفة تشفير ملفات المستودع بأسلوب AES-256
bool EncryptDevRoom(const std::wstring& filePath, const std::wstring& password) {
    // استراتيجية 7z: استخدام مكتبة التشفير المدمجة في الويندوز لتقليل الحجم
    HCRYPTPROV hProv;
    HCRYPTHASH hHash;
    HCRYPTKEY hKey;

    if (CryptAcquireContext(&hProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
        CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash);
        CryptHashData(hHash, (BYTE*)password.c_str(), password.length() * sizeof(wchar_t), 0);
        CryptDeriveKey(hProv, CALG_AES_256, hHash, 0, &hKey);
        
        // هنا يتم تشفير محتوى الملف وحفظه بصيغة .7z_secure
        // (تم تبسيط الكود لسهولة القراءة)
        
        CryptDestroyKey(hKey);
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return true;
    }
    return false;
}