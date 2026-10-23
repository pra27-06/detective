#include "VNAssetManager.h"
#include <iostream>
#include <sstream>

static std::wstring toWideString(const std::string& str) {
    if (str.empty()) return std::wstring();
    int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstr(sizeNeeded, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstr[0], sizeNeeded);
    return wstr;
}

VNAssetManager::VNAssetManager()
    : m_gdiplusToken(0), m_initialized(false), m_baseDir("assets") {
}

VNAssetManager::~VNAssetManager() {
    shutdown();
}

bool VNAssetManager::initialize(const std::string& baseAssetDir) {
    if (m_initialized) return true;
    m_baseDir = baseAssetDir;

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::Status status = Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
    if (status != Gdiplus::Ok) {
        std::cerr << "[ERROR] GDI+ startup failed with status: " << status << std::endl;
        return false;
    }

    m_initialized = true;
    createFallbackPlaceholder();
    return true;
}

void VNAssetManager::shutdown() {
    if (!m_initialized) return;

    m_backgrounds.clear();
    m_portraits.clear();
    m_uiBitmaps.clear();
    m_placeholderBitmap.reset();

    Gdiplus::GdiplusShutdown(m_gdiplusToken);
    m_initialized = false;
}

void VNAssetManager::createFallbackPlaceholder() {
    m_placeholderBitmap = std::make_unique<Gdiplus::Bitmap>(200, 200, PixelFormat32bppARGB);
    Gdiplus::Graphics g(m_placeholderBitmap.get());
    Gdiplus::SolidBrush bg(Gdiplus::Color(255, 30, 40, 60));
    g.FillRectangle(&bg, 0, 0, 200, 200);

    Gdiplus::Pen border(Gdiplus::Color(255, 200, 160, 50), 3.0f);
    g.DrawRectangle(&border, 2, 2, 196, 196);

    Gdiplus::Font font(L"Segoe UI", 12.0f, Gdiplus::FontStyleBold);
    Gdiplus::SolidBrush textBrush(Gdiplus::Color(255, 240, 230, 200));
    Gdiplus::StringFormat format;
    format.SetAlignment(Gdiplus::StringAlignmentCenter);
    format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

    Gdiplus::RectF rect(0.0f, 0.0f, 200.0f, 200.0f);
    g.DrawString(L"[IMAGE PENDING]", -1, &font, rect, &format, &textBrush);
}

Gdiplus::Bitmap* VNAssetManager::loadBitmapFromFile(const std::string& path) {
    std::wstring wpath = toWideString(path);
    auto bmp = std::make_unique<Gdiplus::Bitmap>(wpath.c_str());
    if (bmp->GetLastStatus() == Gdiplus::Ok && bmp->GetWidth() > 0) {
        return bmp.release();
    }
    return nullptr;
}

Gdiplus::Bitmap* VNAssetManager::getBackground(const std::string& name) {
    if (!m_initialized) return nullptr;

    auto it = m_backgrounds.find(name);
    if (it != m_backgrounds.end()) {
        return it->second.get();
    }

    std::string path = m_baseDir + "/backgrounds/" + name;
    if (name.find(".png") == std::string::npos) {
        path += ".png";
    }

    Gdiplus::Bitmap* bmp = loadBitmapFromFile(path);
    if (bmp) {
        m_backgrounds[name] = std::unique_ptr<Gdiplus::Bitmap>(bmp);
        return bmp;
    }

    return m_placeholderBitmap.get();
}

Gdiplus::Bitmap* VNAssetManager::getPortrait(const std::string& charId, const std::string& expression) {
    if (!m_initialized) return nullptr;

    std::string key = charId + "_" + expression;
    auto it = m_portraits.find(key);
    if (it != m_portraits.end()) {
        return it->second.get();
    }

    // Try portraits/ first, then characters/
    std::string path1 = m_baseDir + "/portraits/" + key + ".png";
    std::string path2 = m_baseDir + "/characters/" + key + ".png";
    Gdiplus::Bitmap* bmp = loadBitmapFromFile(path1);
    if (!bmp) {
        bmp = loadBitmapFromFile(path2);
    }

    // If specific expression not found, fallback to neutral
    if (!bmp && expression != "neutral") {
        return getPortrait(charId, "neutral");
    }

    if (bmp) {
        m_portraits[key] = std::unique_ptr<Gdiplus::Bitmap>(bmp);
        return bmp;
    }

    return m_placeholderBitmap.get();
}

Gdiplus::Bitmap* VNAssetManager::getUI(const std::string& name) {
    if (!m_initialized) return nullptr;

    auto it = m_uiBitmaps.find(name);
    if (it != m_uiBitmaps.end()) {
        return it->second.get();
    }

    std::string path = m_baseDir + "/ui/" + name;
    if (name.find(".png") == std::string::npos) {
        path += ".png";
    }

    Gdiplus::Bitmap* bmp = loadBitmapFromFile(path);
    if (bmp) {
        m_uiBitmaps[name] = std::unique_ptr<Gdiplus::Bitmap>(bmp);
        return bmp;
    }

    return m_placeholderBitmap.get();
}

Gdiplus::Font* VNAssetManager::getDefaultFont(float size, bool bold) {
    static std::map<std::pair<int, bool>, std::unique_ptr<Gdiplus::Font>> s_fonts;
    int intSize = (int)(size * 10);
    auto key = std::make_pair(intSize, bold);
    if (s_fonts.find(key) == s_fonts.end()) {
        int style = bold ? Gdiplus::FontStyleBold : Gdiplus::FontStyleRegular;
        s_fonts[key] = std::make_unique<Gdiplus::Font>(L"Segoe UI", size, style, Gdiplus::UnitPixel);
    }
    return s_fonts[key].get();
}

Gdiplus::Font* VNAssetManager::getTitleFont(float size, bool bold) {
    static std::map<std::pair<int, bool>, std::unique_ptr<Gdiplus::Font>> s_titleFonts;
    int intSize = (int)(size * 10);
    auto key = std::make_pair(intSize, bold);
    if (s_titleFonts.find(key) == s_titleFonts.end()) {
        int style = bold ? Gdiplus::FontStyleBold : Gdiplus::FontStyleRegular;
        s_titleFonts[key] = std::make_unique<Gdiplus::Font>(L"Georgia", size, style, Gdiplus::UnitPixel);
    }
    return s_titleFonts[key].get();
}
