#ifndef VN_ASSET_MANAGER_H
#define VN_ASSET_MANAGER_H

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <map>
#include <memory>

class VNAssetManager {
private:
    ULONG_PTR m_gdiplusToken;
    bool m_initialized;
    std::string m_baseDir;

    // Bitmap Caches
    std::map<std::string, std::unique_ptr<Gdiplus::Bitmap>> m_backgrounds;
    std::map<std::string, std::unique_ptr<Gdiplus::Bitmap>> m_portraits;
    std::map<std::string, std::unique_ptr<Gdiplus::Bitmap>> m_uiBitmaps;

    // Fallback procedural placeholder
    std::unique_ptr<Gdiplus::Bitmap> m_placeholderBitmap;

    Gdiplus::Bitmap* loadBitmapFromFile(const std::string& path);
    void createFallbackPlaceholder();

public:
    VNAssetManager();
    ~VNAssetManager();

    bool initialize(const std::string& baseAssetDir = "assets");
    void shutdown();

    Gdiplus::Bitmap* getBackground(const std::string& name);
    Gdiplus::Bitmap* getPortrait(const std::string& charId, const std::string& expression);
    Gdiplus::Bitmap* getUI(const std::string& name);

    Gdiplus::Font* getDefaultFont(float size, bool bold = false);
    Gdiplus::Font* getTitleFont(float size, bool bold = true);
};

#endif // VN_ASSET_MANAGER_H
