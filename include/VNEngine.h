#ifndef VN_ENGINE_H
#define VN_ENGINE_H

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <memory>
#include <string>
#include <vector>
#include "VNAssetManager.h"
#include "VNRenderer.h"
#include "DialogueSystem.h"
#include "CaseFile.h"
#include "Detective.h"
#include "Notebook.h"

enum class VNScreen {
    MAIN_MENU,
    CASE_SELECT,
    BRIEFING,
    INVESTIGATION,
    INTERROGATION,
    CONTRADICTION,
    CLUE_BOARD,
    DEDUCTION_MATRIX,
    NOTEBOOK,
    ACCUSATION,
    VERDICT,
    HOW_TO_PLAY,
    CREDITS,
    ARCHIVES
};

struct ClickableRegion {
    int x, y, w, h;
    std::string id;
    std::string label;
    bool enabled;
    std::function<void()> onClick;

    ClickableRegion(int x_, int y_, int w_, int h_, const std::string& id_,
                    const std::string& lbl = "", std::function<void()> cb = nullptr, bool en = true)
        : x(x_), y(y_), w(w_), h(h_), id(id_), label(lbl), enabled(en), onClick(cb) {}

    bool contains(int px, int py) const {
        return enabled && px >= x && px <= x + w && py >= y && py <= y + h;
    }
};

class VNEngine {
private:
    HWND m_hwnd;
    bool m_isRunning;
    int m_windowWidth;
    int m_windowHeight;

    VNAssetManager m_assetManager;
    std::unique_ptr<VNRenderer> m_renderer;
    DialogueSystem m_dialogue;

    // OOP Domain Logic State
    std::shared_ptr<Detective> m_detective;
    std::unique_ptr<CaseFile> m_activeCase;
    Notebook m_notebook;
    int m_investigationMistakes;
    int m_difficulty;
    AccusationResult m_lastVerdict;

    // Navigation & Screen State
    VNScreen m_currentScreen;
    VNScreen m_previousScreen;
    size_t m_currentSceneIndex;
    std::string m_activeSuspectId;
    std::string m_activeCharacterId;
    std::string m_activeExpression;

    // Interactive selections
    std::string m_selectedEvidenceId;
    std::string m_cluePin1;
    std::string m_cluePin2;
    std::string m_accusedSuspectId;
    std::string m_accusedEvidenceId;
    std::string m_accusedMotive;

    // Modal & Toast feedback
    bool m_showDossierModal;
    Evidence m_dossierEvidence;
    std::string m_toastMessage;
    float m_toastTimer;

    // Objection animation state
    bool m_showObjection;
    float m_objectionTimer;

    // Input state
    int m_mouseX;
    int m_mouseY;
    std::vector<ClickableRegion> m_clickables;
    float m_globalTime;

    // Core Setup & Screen Setup
    void setupMainMenu();
    void setupCaseSelect();
    void setupBriefingScript();
    void setupInvestigationScreen();
    void setupInterrogationScreen();
    void setupContradictionScreen();
    void setupClueBoardScreen();
    void setupDeductionScreen();
    void setupNotebookScreen();
    void setupAccusationScreen();
    void setupVerdictScreen();
    void setupHowToPlayScreen();
    void setupCreditsScreen();
    void setupArchivesScreen();

    // Event Handlers
    void onHotspotClicked(size_t hotspotIndex);
    void onSuspectSelected(const std::string& suspectId);
    void onTopicQuestion(const std::string& topicKey);
    void onContradictionSubmitted(const std::string& evidenceId);
    void onCluePinClicked(const std::string& evidenceId);
    void onSubmitAccusation();
    void onSaveGame();
    void onLoadGame();

    void showToast(const std::string& msg, float duration = 3.0f);

public:
    VNEngine();
    ~VNEngine();

    bool init(HINSTANCE hInstance, int nCmdShow);
    void run();

    void update(float dt);
    void render(HDC hdc);

    void onMouseMove(int x, int y);
    void onMouseDown(int x, int y, int button);
    void onMouseUp(int x, int y, int button);
    void onKeyDown(WPARAM key);
    void onResize(int w, int h);

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

#endif // VN_ENGINE_H
