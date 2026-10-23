#include "VNRenderer.h"
#include <cmath>
#include <algorithm>

static std::wstring toWide(const std::string& str) {
    if (str.empty()) return std::wstring();
    int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstr(sizeNeeded, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstr[0], sizeNeeded);
    return wstr;
}

VNRenderer::VNRenderer(VNAssetManager* assetManager)
    : m_hwnd(NULL), m_width(1280), m_height(720), m_assetManager(assetManager) {
}

VNRenderer::~VNRenderer() {
}

bool VNRenderer::init(HWND hwnd, int width, int height) {
    m_hwnd = hwnd;
    m_width = width;
    m_height = height;

    resize(width, height);
    return true;
}

void VNRenderer::resize(int width, int height) {
    if (width <= 0 || height <= 0) return;
    m_width = width;
    m_height = height;

    m_graphics.reset();
    m_backBuffer = std::make_unique<Gdiplus::Bitmap>(m_width, m_height, PixelFormat32bppARGB);
    m_graphics = std::make_unique<Gdiplus::Graphics>(m_backBuffer.get());

    m_graphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    m_graphics->SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
    m_graphics->SetTextRenderingHint(Gdiplus::TextRenderingHintClearTypeGridFit);
}

void VNRenderer::beginFrame() {
    if (!m_graphics) return;
    m_graphics->Clear(Gdiplus::Color(255, 12, 16, 24));
}

void VNRenderer::endFrame(HDC hdc) {
    if (!m_backBuffer || !hdc) return;
    Gdiplus::Graphics screen(hdc);
    screen.DrawImage(m_backBuffer.get(), 0, 0, m_width, m_height);
}

void VNRenderer::drawBackground(Gdiplus::Bitmap* bg) {
    if (!m_graphics || !bg) return;
    m_graphics->DrawImage(bg, 0, 0, m_width, m_height);
}

void VNRenderer::drawPortrait(Gdiplus::Bitmap* portrait, int x, int y, int w, int h, float alpha) {
    if (!m_graphics || !portrait) return;

    if (alpha >= 0.99f) {
        m_graphics->DrawImage(portrait, x, y, w, h);
    } else {
        Gdiplus::ColorMatrix cm = {
            1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, alpha, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 1.0f
        };
        Gdiplus::ImageAttributes attr;
        attr.SetColorMatrix(&cm);
        m_graphics->DrawImage(portrait, Gdiplus::Rect(x, y, w, h), 0, 0,
                              portrait->GetWidth(), portrait->GetHeight(),
                              Gdiplus::UnitPixel, &attr);
    }
}

void VNRenderer::drawDialogueBox(int x, int y, int w, int h,
                                 const std::string& speaker,
                                 const std::string& displayedText,
                                 bool isComplete, float pulseTime) {
    if (!m_graphics) return;

    Gdiplus::Bitmap* boxImg = m_assetManager ? m_assetManager->getUI("dialogue_box") : nullptr;
    if (boxImg && boxImg->GetWidth() > 0) {
        m_graphics->DrawImage(boxImg, x, y, w, h);
    } else {
        // Fallback procedural box
        Gdiplus::SolidBrush bgBrush(Gdiplus::Color(230, 14, 18, 28));
        m_graphics->FillRectangle(&bgBrush, x, y, w, h);
        Gdiplus::Pen border(Gdiplus::Color(255, 215, 175, 75), 3.0f);
        m_graphics->DrawRectangle(&border, x, y, w, h);
    }

    // Nameplate if speaker is provided
    if (!speaker.empty()) {
        int npW = 280;
        int npH = 46;
        int npX = x + 30;
        int npY = y - 24;

        Gdiplus::Bitmap* npImg = m_assetManager ? m_assetManager->getUI("nameplate") : nullptr;
        if (npImg && npImg->GetWidth() > 0) {
            m_graphics->DrawImage(npImg, npX, npY, npW, npH);
        } else {
            Gdiplus::SolidBrush npBg(Gdiplus::Color(245, 28, 36, 52));
            m_graphics->FillRectangle(&npBg, npX, npY, npW, npH);
            Gdiplus::Pen npPen(Gdiplus::Color(255, 225, 185, 75), 2.0f);
            m_graphics->DrawRectangle(&npPen, npX, npY, npW, npH);
        }

        // Speaker Text
        drawText(speaker, npX + npW / 2, npY + 12, 17.0f,
                 Gdiplus::Color(255, 255, 235, 160), true,
                 Gdiplus::StringAlignmentCenter);
    }

    // Dialogue Text (wrapped)
    int textX = x + 45;
    int textY = y + 36;
    int textW = w - 90;
    int textH = h - 50;
    drawWrappedText(displayedText, textX, textY, textW, textH, 18.0f, Gdiplus::Color(255, 245, 248, 255));

    // Bouncing advance indicator when typing is complete
    if (isComplete) {
        int bounce = (int)(std::sin(pulseTime * 6.0f) * 4.0f);
        int indX = x + w - 50;
        int indY = y + h - 38 + bounce;

        Gdiplus::Bitmap* advImg = m_assetManager ? m_assetManager->getUI("advance_marker") : nullptr;
        if (advImg && advImg->GetWidth() > 0) {
            m_graphics->DrawImage(advImg, indX, indY, 26, 26);
        } else {
            Gdiplus::Point pts[3] = {
                Gdiplus::Point(indX, indY),
                Gdiplus::Point(indX + 20, indY),
                Gdiplus::Point(indX + 10, indY + 14)
            };
            Gdiplus::SolidBrush brush(Gdiplus::Color(255, 255, 215, 80));
            m_graphics->FillPolygon(&brush, pts, 3);
        }
    }
}

void VNRenderer::drawButton(int x, int y, int w, int h,
                            const std::string& label,
                            bool isHovered, bool isSelected,
                            bool isPrimary, bool isEnabled) {
    if (!m_graphics) return;

    Gdiplus::Color bgColor;
    Gdiplus::Color borderColor;
    Gdiplus::Color textColor;

    if (!isEnabled) {
        bgColor = Gdiplus::Color(200, 25, 28, 35);
        borderColor = Gdiplus::Color(255, 70, 75, 85);
        textColor = Gdiplus::Color(255, 110, 115, 125);
    } else if (isSelected) {
        bgColor = Gdiplus::Color(240, 185, 140, 40);
        borderColor = Gdiplus::Color(255, 255, 235, 120);
        textColor = Gdiplus::Color(255, 20, 25, 35);
    } else if (isHovered) {
        bgColor = isPrimary ? Gdiplus::Color(240, 45, 80, 140) : Gdiplus::Color(240, 40, 50, 72);
        borderColor = Gdiplus::Color(255, 255, 215, 90);
        textColor = Gdiplus::Color(255, 255, 245, 200);
    } else {
        bgColor = isPrimary ? Gdiplus::Color(230, 25, 55, 105) : Gdiplus::Color(230, 22, 28, 42);
        borderColor = isPrimary ? Gdiplus::Color(255, 180, 145, 60) : Gdiplus::Color(255, 120, 135, 160);
        textColor = Gdiplus::Color(255, 230, 235, 245);
    }

    Gdiplus::SolidBrush bgBrush(bgColor);
    m_graphics->FillRectangle(&bgBrush, x, y, w, h);

    Gdiplus::Pen borderPen(borderColor, isHovered ? 2.5f : 1.5f);
    m_graphics->DrawRectangle(&borderPen, x, y, w, h);

    // Accent line at bottom
    if (isEnabled && (isHovered || isSelected)) {
        Gdiplus::Pen accent(Gdiplus::Color(255, 255, 220, 100), 2.0f);
        m_graphics->DrawLine(&accent, x + 4, y + h - 2, x + w - 4, y + h - 2);
    }

    drawText(label, x + w / 2, y + (h - 18) / 2, 15.0f, textColor, isHovered || isSelected, Gdiplus::StringAlignmentCenter);
}

void VNRenderer::drawHotspot(int x, int y, int radius,
                             const std::string& name,
                             bool isExamined, bool isHovered, float pulseTime) {
    if (!m_graphics) return;

    float pulse = (std::sin(pulseTime * 5.0f) + 1.0f) * 0.5f; // 0.0 to 1.0
    int currentRadius = isHovered ? radius + 4 : radius;

    // Outer glow
    int glowAlpha = (int)(60 + pulse * 60);
    Gdiplus::Color glowColor = isExamined ? Gdiplus::Color(glowAlpha, 60, 180, 100) : Gdiplus::Color(glowAlpha, 255, 215, 60);
    Gdiplus::Pen glowPen(glowColor, 4.0f);
    m_graphics->DrawEllipse(&glowPen, x - currentRadius - 4, y - currentRadius - 4, (currentRadius + 4) * 2, (currentRadius + 4) * 2);

    // Center fill
    Gdiplus::Color fillColor = isExamined ? Gdiplus::Color(140, 30, 100, 50) : Gdiplus::Color(160, 180, 140, 30);
    Gdiplus::SolidBrush fillBrush(fillColor);
    m_graphics->FillEllipse(&fillBrush, x - currentRadius, y - currentRadius, currentRadius * 2, currentRadius * 2);

    // Center icon
    Gdiplus::Color coreColor = isExamined ? Gdiplus::Color(255, 80, 220, 120) : Gdiplus::Color(255, 255, 225, 90);
    Gdiplus::Pen corePen(coreColor, 2.5f);
    m_graphics->DrawEllipse(&corePen, x - currentRadius, y - currentRadius, currentRadius * 2, currentRadius * 2);

    // Crosshairs
    m_graphics->DrawLine(&corePen, x, y - currentRadius + 3, x, y + currentRadius - 3);
    m_graphics->DrawLine(&corePen, x - currentRadius + 3, y, x + currentRadius - 3, y);

    // Hover tooltip banner
    if (isHovered) {
        std::string badgeText = name + (isExamined ? " [EXAMINED]" : " [UNEXAMINED]");
        int badgeW = 240;
        int badgeH = 34;
        int badgeX = x - badgeW / 2;
        int badgeY = y - currentRadius - badgeH - 8;

        Gdiplus::SolidBrush badgeBg(Gdiplus::Color(240, 15, 20, 32));
        m_graphics->FillRectangle(&badgeBg, badgeX, badgeY, badgeW, badgeH);
        Gdiplus::Pen badgeBorder(coreColor, 2.0f);
        m_graphics->DrawRectangle(&badgeBorder, badgeX, badgeY, badgeW, badgeH);

        drawText(badgeText, badgeX + badgeW / 2, badgeY + 8, 13.0f,
                 Gdiplus::Color(255, 255, 240, 200), true, Gdiplus::StringAlignmentCenter);
    }
}

void VNRenderer::drawEvidenceCard(int x, int y, int w, int h,
                                  const std::string& id,
                                  const std::string& name,
                                  const std::string& category,
                                  const std::string& relevance,
                                  bool isHovered, bool isSelected) {
    if (!m_graphics) return;

    Gdiplus::Color bgColor = isSelected ? Gdiplus::Color(245, 38, 48, 70)
                                        : (isHovered ? Gdiplus::Color(240, 28, 35, 52)
                                                     : Gdiplus::Color(230, 18, 22, 34));
    Gdiplus::Color borderColor = isSelected ? Gdiplus::Color(255, 255, 220, 80)
                                            : (isHovered ? Gdiplus::Color(255, 215, 175, 70)
                                                         : Gdiplus::Color(255, 80, 95, 120));

    Gdiplus::SolidBrush bgBrush(bgColor);
    m_graphics->FillRectangle(&bgBrush, x, y, w, h);

    Gdiplus::Pen border(borderColor, isSelected ? 3.0f : (isHovered ? 2.0f : 1.5f));
    m_graphics->DrawRectangle(&border, x, y, w, h);

    // ID Tag
    Gdiplus::SolidBrush tagBg(Gdiplus::Color(255, 190, 150, 45));
    m_graphics->FillRectangle(&tagBg, x + 8, y + 8, 48, 24);
    drawText(id, x + 32, y + 12, 13.0f, Gdiplus::Color(255, 15, 18, 24), true, Gdiplus::StringAlignmentCenter);

    // Category Badge
    drawText(category, x + 62, y + 13, 11.0f, Gdiplus::Color(255, 160, 175, 200), false);

    // Name
    drawText(name, x + 10, y + 40, 14.0f, Gdiplus::Color(255, 245, 248, 255), true);

    // Relevance Tag
    Gdiplus::Color relColor = Gdiplus::Color(255, 120, 200, 140);
    if (relevance.find("KEY") != std::string::npos || relevance.find("CRITICAL") != std::string::npos) {
        relColor = Gdiplus::Color(255, 245, 170, 40);
    } else if (relevance.find("RED HERRING") != std::string::npos || relevance.find("QUESTIONABLE") != std::string::npos) {
        relColor = Gdiplus::Color(255, 220, 90, 80);
    }
    drawText(relevance, x + 10, y + h - 22, 10.0f, relColor, true);
}

void VNRenderer::drawHeaderBar(const std::string& caseTitle,
                               const std::string& detectiveInfo,
                               int score, int energy, int maxEnergy) {
    if (!m_graphics) return;

    int barH = 54;
    Gdiplus::SolidBrush barBg(Gdiplus::Color(235, 12, 16, 24));
    m_graphics->FillRectangle(&barBg, 0, 0, m_width, barH);

    Gdiplus::Pen bottomLine(Gdiplus::Color(255, 185, 145, 55), 2.0f);
    m_graphics->DrawLine(&bottomLine, 0, barH, m_width, barH);

    // Case Title
    drawText(caseTitle, 20, 8, 17.0f, Gdiplus::Color(255, 255, 225, 130), true);
    drawText(detectiveInfo, 20, 30, 12.0f, Gdiplus::Color(255, 160, 175, 200));

    // Score on right
    std::string scoreStr = "Score: " + std::to_string(score);
    drawText(scoreStr, m_width - 150, 8, 16.0f, Gdiplus::Color(255, 255, 215, 80), true, Gdiplus::StringAlignmentFar);

    // Energy Bar
    int barW = 160;
    int energyX = m_width - barW - 30;
    int energyY = 32;
    int energyH = 14;

    Gdiplus::SolidBrush energyBg(Gdiplus::Color(255, 30, 35, 45));
    m_graphics->FillRectangle(&energyBg, energyX, energyY, barW, energyH);

    float ratio = maxEnergy > 0 ? (float)energy / (float)maxEnergy : 0.0f;
    ratio = std::max(0.0f, std::min(1.0f, ratio));
    int fillW = (int)(barW * ratio);

    Gdiplus::Color fillCol = ratio > 0.5f ? Gdiplus::Color(255, 45, 180, 85)
                                          : (ratio > 0.25f ? Gdiplus::Color(255, 230, 165, 35)
                                                           : Gdiplus::Color(255, 215, 50, 45));
    Gdiplus::SolidBrush energyFill(fillCol);
    m_graphics->FillRectangle(&energyFill, energyX, energyY, fillW, energyH);

    Gdiplus::Pen energyBorder(Gdiplus::Color(255, 120, 135, 155), 1.0f);
    m_graphics->DrawRectangle(&energyBorder, energyX, energyY, barW, energyH);

    std::string energyLabel = "Energy " + std::to_string(energy) + "/" + std::to_string(maxEnergy);
    drawText(energyLabel, energyX - 10, energyY, 11.0f, Gdiplus::Color(255, 200, 210, 225), false, Gdiplus::StringAlignmentFar);
}

void VNRenderer::drawObjectionBanner(float progress) {
    if (!m_graphics || progress <= 0.0f) return;

    Gdiplus::Bitmap* banner = m_assetManager ? m_assetManager->getUI("objection_banner") : nullptr;
    int bannerW = 800;
    int bannerH = 180;
    int targetX = (m_width - bannerW) / 2;
    int targetY = (m_height - bannerH) / 2 - 60;

    // Slide-in animation from left
    float t = std::min(1.0f, progress * 2.0f);
    float ease = 1.0f - std::pow(1.0f - t, 3.0f); // cubic ease out
    int currentX = (int)(-bannerW + (targetX + bannerW) * ease);

    if (banner && banner->GetWidth() > 0) {
        m_graphics->DrawImage(banner, currentX, targetY, bannerW, bannerH);
    } else {
        Gdiplus::SolidBrush bannerBg(Gdiplus::Color(245, 180, 25, 35));
        m_graphics->FillRectangle(&bannerBg, currentX, targetY, bannerW, bannerH);
        Gdiplus::Pen bannerBorder(Gdiplus::Color(255, 255, 220, 90), 4.0f);
        m_graphics->DrawRectangle(&bannerBorder, currentX, targetY, bannerW, bannerH);

        drawText("CONTRADICTION EXPOSED!", currentX + bannerW / 2, targetY + 60, 36.0f,
                 Gdiplus::Color(255, 255, 245, 210), true, Gdiplus::StringAlignmentCenter);
    }
}

void VNRenderer::drawText(const std::string& text, int x, int y, float size,
                          Gdiplus::Color color, bool bold,
                          Gdiplus::StringAlignment align) {
    if (!m_graphics || text.empty() || !m_assetManager) return;

    std::wstring wtext = toWide(text);
    Gdiplus::Font* font = m_assetManager->getDefaultFont(size, bold);
    Gdiplus::SolidBrush brush(color);

    Gdiplus::StringFormat format;
    format.SetAlignment(align);
    format.SetLineAlignment(Gdiplus::StringAlignmentNear);

    Gdiplus::PointF point((float)x, (float)y);
    m_graphics->DrawString(wtext.c_str(), -1, font, point, &format, &brush);
}

void VNRenderer::drawWrappedText(const std::string& text, int x, int y, int w, int h,
                                 float size, Gdiplus::Color color, bool bold) {
    if (!m_graphics || text.empty() || !m_assetManager) return;

    std::wstring wtext = toWide(text);
    Gdiplus::Font* font = m_assetManager->getDefaultFont(size, bold);
    Gdiplus::SolidBrush brush(color);

    Gdiplus::RectF rect((float)x, (float)y, (float)w, (float)h);
    Gdiplus::StringFormat format;
    format.SetAlignment(Gdiplus::StringAlignmentNear);
    format.SetLineAlignment(Gdiplus::StringAlignmentNear);

    m_graphics->DrawString(wtext.c_str(), -1, font, rect, &format, &brush);
}
