#ifndef VN_RENDERER_H
#define VN_RENDERER_H

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <memory>
#include "VNAssetManager.h"

class VNRenderer {
private:
    HWND m_hwnd;
    int m_width;
    int m_height;

    std::unique_ptr<Gdiplus::Bitmap> m_backBuffer;
    std::unique_ptr<Gdiplus::Graphics> m_graphics;

    VNAssetManager* m_assetManager;

public:
    VNRenderer(VNAssetManager* assetManager);
    ~VNRenderer();

    bool init(HWND hwnd, int width, int height);
    void resize(int width, int height);

    void beginFrame();
    void endFrame(HDC hdc);

    Gdiplus::Graphics* getGraphics() { return m_graphics.get(); }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

    // Visual Novel Components
    void drawBackground(Gdiplus::Bitmap* bg);
    void drawPortrait(Gdiplus::Bitmap* portrait, int x, int y, int w, int h, float alpha = 1.0f);
    void drawDialogueBox(int x, int y, int w, int h,
                         const std::string& speaker,
                         const std::string& displayedText,
                         bool isComplete, float pulseTime);

    void drawButton(int x, int y, int w, int h,
                    const std::string& label,
                    bool isHovered, bool isSelected,
                    bool isPrimary = false, bool isEnabled = true);

    void drawHotspot(int x, int y, int radius,
                     const std::string& name,
                     bool isExamined, bool isHovered, float pulseTime);

    void drawEvidenceCard(int x, int y, int w, int h,
                          const std::string& id,
                          const std::string& name,
                          const std::string& category,
                          const std::string& relevance,
                          bool isHovered, bool isSelected);

    void drawHeaderBar(const std::string& caseTitle,
                       const std::string& detectiveInfo,
                       int score, int energy, int maxEnergy);

    void drawObjectionBanner(float progress);

    void drawText(const std::string& text, int x, int y, float size,
                  Gdiplus::Color color, bool bold = false,
                  Gdiplus::StringAlignment align = Gdiplus::StringAlignmentNear);

    void drawWrappedText(const std::string& text, int x, int y, int w, int h,
                         float size, Gdiplus::Color color, bool bold = false);
};

#endif // VN_RENDERER_H
