#include "VNEngine.h"
#include "SaveManager.h"
#include <windowsx.h>
#include <mmsystem.h>
#include <iostream>
#include <cmath>

static VNEngine* s_engineInstance = nullptr;

VNEngine::VNEngine()
    : m_hwnd(NULL), m_isRunning(false), m_windowWidth(1280), m_windowHeight(720),
      m_investigationMistakes(0), m_difficulty(1),
      m_currentScreen(VNScreen::MAIN_MENU), m_previousScreen(VNScreen::MAIN_MENU),
      m_currentSceneIndex(0), m_activeSuspectId("S02"), m_activeCharacterId("cruz"),
      m_activeExpression("neutral"), m_selectedEvidenceId(""), m_cluePin1(""), m_cluePin2(""),
      m_accusedSuspectId("S02"), m_accusedEvidenceId("E03"), m_accusedMotive("DEBT"),
      m_showDossierModal(false), m_toastMessage(""), m_toastTimer(0.0f),
      m_showObjection(false), m_objectionTimer(0.0f),
      m_mouseX(0), m_mouseY(0), m_globalTime(0.0f) {
    s_engineInstance = this;
    m_detective = std::make_shared<Detective>("D01", "Sherlock", "SCOT-7749", "Rookie Detective");
}

VNEngine::~VNEngine() {
    s_engineInstance = nullptr;
}

LRESULT CALLBACK VNEngine::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (!s_engineInstance) return DefWindowProc(hwnd, msg, wParam, lParam);

    switch (msg) {
        case WM_MOUSEMOVE:
            s_engineInstance->onMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;

        case WM_LBUTTONDOWN:
            s_engineInstance->onMouseDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 1);
            return 0;

        case WM_LBUTTONUP:
            s_engineInstance->onMouseUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 1);
            return 0;

        case WM_KEYDOWN:
            s_engineInstance->onKeyDown(wParam);
            return 0;

        case WM_TIMER:
            s_engineInstance->update(0.016f);
            InvalidateRect(hwnd, NULL, FALSE);
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            s_engineInstance->render(hdc);
            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_SIZE: {
            int w = LOWORD(lParam);
            int h = HIWORD(lParam);
            s_engineInstance->onResize(w, h);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

bool VNEngine::init(HINSTANCE hInstance, int nCmdShow) {
    if (!m_assetManager.initialize("assets")) {
        std::cerr << "[ERROR] Failed to initialize Asset Manager." << std::endl;
        return false;
    }

    WNDCLASSA wc = {};
    wc.lpfnWndProc = VNEngine::WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "CaseClosedVNClass";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClassA(&wc)) {
        std::cerr << "[ERROR] Window class registration failed." << std::endl;
        return false;
    }

    // Window dimensions for 1280x720 client area
    RECT wr = { 0, 0, m_windowWidth, m_windowHeight };
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
    int totalW = wr.right - wr.left;
    int totalH = wr.bottom - wr.top;

    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);
    int startX = (screenW - totalW) / 2;
    int startY = (screenH - totalH) / 2;

    m_hwnd = CreateWindowExA(
        0,
        "CaseClosedVNClass",
        "CASE CLOSED -- Detective Mystery Simulator (Visual Novel Edition)",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        startX, startY, totalW, totalH,
        NULL, NULL, hInstance, NULL
    );

    if (!m_hwnd) {
        std::cerr << "[ERROR] Failed to create game window." << std::endl;
        return false;
    }

    m_renderer = std::make_unique<VNRenderer>(&m_assetManager);
    m_renderer->init(m_hwnd, m_windowWidth, m_windowHeight);

    // Initialize with Case 1
    m_activeCase = std::make_unique<CaseFile>(1);

    setupMainMenu();

    ShowWindow(m_hwnd, nCmdShow);
    UpdateWindow(m_hwnd);

    // Set 60 FPS update timer (16 ms)
    SetTimer(m_hwnd, 1, 16, NULL);
    m_isRunning = true;
    return true;
}

void VNEngine::run() {
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void VNEngine::onResize(int w, int h) {
    if (w > 0 && h > 0) {
        m_windowWidth = w;
        m_windowHeight = h;
        if (m_renderer) {
            m_renderer->resize(w, h);
        }
    }
}

void VNEngine::onMouseMove(int x, int y) {
    m_mouseX = x;
    m_mouseY = y;
}

void VNEngine::onMouseDown(int x, int y, int button) {
    (void)button;

    // Toast dismissal
    if (m_toastTimer > 0.0f) {
        m_toastTimer = 0.0f;
    }

    // Modal dismissal
    if (m_showDossierModal) {
        m_showDossierModal = false;
        return;
    }

    // Check clickables from top to bottom
    for (const auto& cr : m_clickables) {
        if (cr.contains(x, y)) {
            if (cr.onClick) {
                cr.onClick();
                return;
            }
        }
    }

    // If dialogue is active, advance or finish typing
    if (!m_dialogue.isEnded()) {
        m_dialogue.advance();
    }
}

void VNEngine::onMouseUp(int x, int y, int button) {
    (void)x; (void)y; (void)button;
}

void VNEngine::onKeyDown(WPARAM key) {
    if (key == VK_SPACE || key == VK_RETURN) {
        if (m_showDossierModal) {
            m_showDossierModal = false;
            return;
        }
        if (!m_dialogue.isEnded()) {
            m_dialogue.advance();
        }
    } else if (key == VK_ESCAPE) {
        if (m_showDossierModal) {
            m_showDossierModal = false;
        } else if (m_currentScreen != VNScreen::MAIN_MENU) {
            setupMainMenu();
        }
    }
}

void VNEngine::update(float dt) {
    m_globalTime += dt;

    if (m_toastTimer > 0.0f) {
        m_toastTimer -= dt;
    }

    if (m_showObjection) {
        m_objectionTimer += dt;
        if (m_objectionTimer >= 2.0f) {
            m_showObjection = false;
            m_objectionTimer = 0.0f;
        }
    }

    m_dialogue.update(dt);
}

void VNEngine::showToast(const std::string& msg, float duration) {
    m_toastMessage = msg;
    m_toastTimer = duration;
}

// -------------------------------------------------------------
// SCREEN SETUPS
// -------------------------------------------------------------

void VNEngine::setupMainMenu() {
    m_currentScreen = VNScreen::MAIN_MENU;
    m_clickables.clear();
    m_dialogue.clear();

    int startY = 320;
    int btnW = 340;
    int btnH = 46;
    int gap = 16;
    int btnX = (m_windowWidth - btnW) / 2;

    m_clickables.emplace_back(btnX, startY, btnW, btnH, "new_game", "New Criminal Investigation", [this]() {
        setupCaseSelect();
    });

    m_clickables.emplace_back(btnX, startY + (btnH + gap), btnW, btnH, "continue", "Continue Saved Case", [this]() {
        onLoadGame();
    });

    m_clickables.emplace_back(btnX, startY + (btnH + gap) * 2, btnW, btnH, "archives", "Case Files & Archives", [this]() {
        setupArchivesScreen();
    });

    m_clickables.emplace_back(btnX, startY + (btnH + gap) * 3, btnW, btnH, "how_to_play", "How to Play (Detective Manual)", [this]() {
        setupHowToPlayScreen();
    });

    m_clickables.emplace_back(btnX, startY + (btnH + gap) * 4, btnW, btnH, "credits", "Credits & OOPs Architecture", [this]() {
        setupCreditsScreen();
    });

    m_clickables.emplace_back(btnX, startY + (btnH + gap) * 5, btnW, btnH, "exit", "Exit to Desktop", [this]() {
        PostQuitMessage(0);
    });
}

void VNEngine::setupCaseSelect() {
    m_currentScreen = VNScreen::CASE_SELECT;
    m_clickables.clear();
    m_dialogue.clear();

    int cardW = 360;
    int cardH = 260;
    int startX = 60;
    int gapX = 40;
    int cardY = 220;

    // Case 1
    m_clickables.emplace_back(startX, cardY, cardW, cardH, "case_1", "Case #1: The Midnight Museum Heist", [this]() {
        m_activeCase = std::make_unique<CaseFile>(1);
        m_detective = std::make_shared<Detective>("D01", "Sherlock", "SCOT-7749", "Rookie Detective");
        m_currentSceneIndex = 0;
        setupBriefingScript();
    });

    // Case 2
    m_clickables.emplace_back(startX + cardW + gapX, cardY, cardW, cardH, "case_2", "Case #2: The Last Train Mystery", [this]() {
        m_activeCase = std::make_unique<CaseFile>(2);
        m_detective = std::make_shared<Detective>("D01", "Sherlock", "SCOT-7749", "Senior Detective");
        m_currentSceneIndex = 0;
        setupBriefingScript();
    });

    // Case 3
    m_clickables.emplace_back(startX + (cardW + gapX) * 2, cardY, cardW, cardH, "case_3", "Case #3: The Locked Room Incident", [this]() {
        m_activeCase = std::make_unique<CaseFile>(3);
        m_detective = std::make_shared<Detective>("D01", "Sherlock", "SCOT-7749", "Chief Inspector");
        m_currentSceneIndex = 0;
        setupBriefingScript();
    });

    // Back button
    m_clickables.emplace_back(60, m_windowHeight - 80, 160, 42, "back_menu", "Back to Menu", [this]() {
        setupMainMenu();
    });
}

void VNEngine::setupBriefingScript() {
    m_currentScreen = VNScreen::BRIEFING;
    m_clickables.clear();
    m_dialogue.clear();

    std::vector<DialogueEntry> script = {
        DialogueEntry("b1", "Detective Sherlock", "detective", "thinking",
                      "Scotland Yard Dispatch has called us in on urgent duty. An audacious crime has just unfolded in London.",
                      "briefing_room"),
        DialogueEntry("b2", "Detective Sherlock", "detective", "neutral",
                      m_activeCase->getTitle() + ": " + m_activeCase->getTagline(),
                      "briefing_room"),
        DialogueEntry("b3", "Incident Report", "detective", "neutral",
                      m_activeCase->getBriefing(),
                      "briefing_room"),
        DialogueEntry("b4", "Detective Sherlock", "detective", "pointing",
                      "Our primary objectives are clear: Explore all crime scenes, discover decisive forensic evidence, expose suspect contradictions, and deliver justice to the Magistrate!",
                      "briefing_room", "", [this]() {
                          // Callback when dialogue ends -> transition to investigation hub
                      })
    };

    m_dialogue.setEntries(script);
    m_dialogue.start();

    // Skip / Enter button
    m_clickables.emplace_back(m_windowWidth - 220, m_windowHeight - 75, 180, 42, "begin_investigation", "Begin Search >>", [this]() {
        setupInvestigationScreen();
    });
}

void VNEngine::setupInvestigationScreen() {
    m_currentScreen = VNScreen::INVESTIGATION;
    m_clickables.clear();
    m_dialogue.clear();

    // Scene Navigation Tabs (Top)
    auto& scenes = m_activeCase->getScenes();
    int tabW = 240;
    int tabH = 36;
    int tabX = 20;
    int tabY = 62;

    for (size_t i = 0; i < scenes.size(); ++i) {
        m_clickables.emplace_back(tabX + (int)i * (tabW + 10), tabY, tabW, tabH,
                                  "scene_tab_" + std::to_string(i),
                                  scenes[i].getName(), [this, i]() {
            m_currentSceneIndex = i;
            setupInvestigationScreen();
        });
    }

    // Interactive Hotspots in current scene
    if (m_currentSceneIndex < scenes.size()) {
        auto& curScene = scenes[m_currentSceneIndex];
        const auto& hotspots = curScene.getHotspots();

        // Preset visual coordinates for hotspots in the 3 scenes
        std::vector<std::pair<int, int>> coords;
        if (m_currentSceneIndex == 0) { // Vault
            coords = { {640, 420}, {480, 560}, {840, 470} };
        } else if (m_currentSceneIndex == 1) { // Substation
            coords = { {960, 320}, {620, 630}, {310, 350} };
        } else { // Locker room
            coords = { {260, 330}, {640, 550}, {820, 330} };
        }

        for (size_t h = 0; h < hotspots.size() && h < coords.size(); ++h) {
            int hx = coords[h].first;
            int hy = coords[h].second;
            int r = 26;
            m_clickables.emplace_back(hx - r, hy - r, r * 2, r * 2,
                                      "hotspot_" + std::to_string(h),
                                      hotspots[h].name, [this, h]() {
                onHotspotClicked(h);
            });
        }
    }

    // Top Right Action Buttons
    int actW = 140;
    int actH = 34;
    int actGap = 8;
    int actY = 62;
    int actStartX = m_windowWidth - (actW + actGap) * 4 - 20;

    m_clickables.emplace_back(actStartX, actY, actW, actH, "goto_interrogate", "Interrogate", [this]() {
        setupInterrogationScreen();
    });

    m_clickables.emplace_back(actStartX + (actW + actGap), actY, actW, actH, "goto_clueboard", "Clue Board", [this]() {
        setupClueBoardScreen();
    });

    m_clickables.emplace_back(actStartX + (actW + actGap) * 2, actY, actW, actH, "goto_notebook", "Notebook", [this]() {
        setupNotebookScreen();
    });

    m_clickables.emplace_back(actStartX + (actW + actGap) * 3, actY, actW, actH, "goto_accuse", "Accuse!", [this]() {
        setupAccusationScreen();
    });

    // Bottom Action Bar
    int btmY = m_windowHeight - 52;
    m_clickables.emplace_back(20, btmY, 130, 36, "hint", "Request Hint", [this]() {
        if (m_detective->getEnergy() >= 10 || m_difficulty == 1) {
            showToast("Hint: Compare Marcus Cruz's claimed patrol time with the Electronic Access Log (E03). Can a guard be in two places at once?", 6.0f);
        } else {
            showToast("Not enough energy for a hint!", 3.0f);
        }
    });

    m_clickables.emplace_back(160, btmY, 130, 36, "save", "Save Case", [this]() {
        onSaveGame();
    });

    m_clickables.emplace_back(m_windowWidth - 160, btmY, 140, 36, "menu_return", "Main Menu", [this]() {
        setupMainMenu();
    });
}

void VNEngine::onHotspotClicked(size_t hotspotIndex) {
    auto& scenes = m_activeCase->getScenes();
    if (m_currentSceneIndex >= scenes.size()) return;

    auto& scene = scenes[m_currentSceneIndex];
    if (hotspotIndex >= scene.getHotspotCount()) return;

    std::string evId = scene.examineHotspot(hotspotIndex);
    if (!evId.empty()) {
        bool isNew = m_activeCase->markEvidenceDiscovered(evId);
        m_activeCase->getClueBoard().addPinnedEvidence(evId);

        Evidence* ev = m_activeCase->getEvidenceById(evId);
        if (isNew && ev) {
            PlaySoundA("SystemAsterisk", NULL, SND_ALIAS | SND_ASYNC);
            m_dossierEvidence = *ev;
            m_showDossierModal = true;
            m_detective->addScore(25);
            m_detective->spendEnergy(10);
            m_notebook.addLead("Discovered " + ev->getId() + ": " + ev->getName() + " at " + scene.getName());
            m_activeCase->updateObjectives();
        } else {
            showToast("Already documented: " + (ev ? ev->getName() : evId), 3.5f);
        }
    } else {
        showToast("Inspected: " + scene.getHotspots()[hotspotIndex].inspectDescription, 4.0f);
    }
}

void VNEngine::setupInterrogationScreen() {
    m_currentScreen = VNScreen::INTERROGATION;
    m_clickables.clear();
    m_dialogue.clear();

    // Suspect Selector Sidebar on left
    const auto& characters = m_activeCase->getCharacters();
    int sideW = 250;
    int cardH = 58;
    int startY = 80;
    int gapY = 10;

    for (size_t i = 0; i < characters.size(); ++i) {
        std::string cid = characters[i]->getId();
        std::string name = characters[i]->getName();
        std::string role = characters[i]->getRole();

        m_clickables.emplace_back(20, startY + (int)i * (cardH + gapY), sideW, cardH,
                                  "suspect_" + cid, name, [this, cid]() {
            onSuspectSelected(cid);
        });
    }

    // Interrogation Actions (Center-Bottom)
    auto suspect = m_activeCase->getSuspectById(m_activeSuspectId);
    auto witness = m_activeCase->getWitnessById(m_activeSuspectId);

    int actX = 300;
    int actY = m_windowHeight - 240;
    int actW = 280;
    int actH = 42;
    int actGap = 12;

    if (suspect) {
        // Option 1: Alibi
        m_clickables.emplace_back(actX, actY, actW, actH, "ask_alibi", "1. Ask about Alibi", [this, suspect]() {
            m_activeExpression = suspect->isAlibiBroken() ? "broken" : "neutral";
            DialogueEntry de("alibi", suspect->getName(), m_activeCharacterId, m_activeExpression,
                             suspect->getAlibi(), "interrogation_room");
            m_dialogue.setEntries({ de });
            m_dialogue.start();
        });

        // Option 2: Motive
        m_clickables.emplace_back(actX + actW + actGap, actY, actW, actH, "ask_motive", "2. Inquire about Motive", [this, suspect]() {
            m_activeExpression = "nervous";
            DialogueEntry de("motive", suspect->getName(), m_activeCharacterId, m_activeExpression,
                             suspect->getMotive(), "interrogation_room");
            m_dialogue.setEntries({ de });
            m_dialogue.start();
        });

        // Option 3: Conditional Clue Dialogue (e.g. E04 Solvent Rag)
        auto discovered = m_activeCase->getDiscoveredEvidenceIds();
        auto conditionalTopics = suspect->getUnlockedConditionalTopics(discovered);
        if (!conditionalTopics.empty()) {
            std::string prompt = conditionalTopics[0].promptLabel;
            std::string response = conditionalTopics[0].response;
            m_clickables.emplace_back(actX, actY + actH + actGap, actW, actH, "ask_conditional",
                                      "[CLUE] " + prompt, [this, suspect, response]() {
                m_activeExpression = "nervous";
                DialogueEntry de("cond", suspect->getName(), m_activeCharacterId, m_activeExpression,
                                 response, "interrogation_room");
                m_dialogue.setEntries({ de });
                m_dialogue.start();
            });
        }

        // Option 4: Confront with Contradiction!
        m_clickables.emplace_back(actX + actW + actGap, actY + actH + actGap, actW, actH, "confront",
                                  "3. CONFRONT CONTRADICTION!", [this]() {
            setupContradictionScreen();
        }, true);

    } else if (witness) {
        m_clickables.emplace_back(actX, actY, actW, actH, "sworn_testimony", "Request Sworn Testimony", [this, witness]() {
            m_activeExpression = "earnest";
            DialogueEntry de("witness_test", witness->getName(), m_activeCharacterId, m_activeExpression,
                             witness->giveTestimony(), "interrogation_room");
            m_dialogue.setEntries({ de });
            m_dialogue.start();
            m_notebook.addLead("Witness " + witness->getName() + " testified: " + witness->giveTestimony());
        });
    }

    // Return to Investigation
    m_clickables.emplace_back(20, m_windowHeight - 56, 180, 38, "back_investigate", "<< Return to Scene", [this]() {
        setupInvestigationScreen();
    });
}

void VNEngine::onSuspectSelected(const std::string& suspectId) {
    m_activeSuspectId = suspectId;
    int caseNum = m_activeCase ? m_activeCase->getCaseNumber() : 1;

    if (caseNum == 1) {
        if (suspectId == "S01") m_activeCharacterId = "vance";
        else if (suspectId == "S02") m_activeCharacterId = "cruz";
        else if (suspectId == "S03") m_activeCharacterId = "elena";
        else if (suspectId == "S04") m_activeCharacterId = "sterling";
        else if (suspectId == "W01") m_activeCharacterId = "bert";
    } else if (caseNum == 2) {
        if (suspectId == "S01") m_activeCharacterId = "cruz";       // Julian Gray (culprit: nervous/broken)
        else if (suspectId == "S02") m_activeCharacterId = "elena"; // Nora Vance
        else if (suspectId == "S03") m_activeCharacterId = "vance"; // Dr. Finch
        else if (suspectId == "S04") m_activeCharacterId = "sterling"; // Chef Hayes
        else if (suspectId == "W01") m_activeCharacterId = "bert";  // Conductor Morris
    } else { // Case 3
        if (suspectId == "S01") m_activeCharacterId = "cruz";       // Cornelius Thorne (culprit: fidgety/broken)
        else if (suspectId == "S02") m_activeCharacterId = "elena"; // Miriam Black
        else if (suspectId == "S03") m_activeCharacterId = "vance"; // Silas Vance
        else if (suspectId == "S04") m_activeCharacterId = "sterling"; // Evelyn Cross
        else if (suspectId == "W01") m_activeCharacterId = "bert";  // Constable Higgins
    }
    m_activeExpression = "neutral";

    setupInterrogationScreen();

    // Play greeting
    auto ch = m_activeCase->getCharacterById(suspectId);
    if (ch) {
        DialogueEntry de("greet", ch->getName(), m_activeCharacterId, m_activeExpression,
                         ch->getGreeting(), "interrogation_room");
        m_dialogue.setEntries({ de });
        m_dialogue.start();
    }
}

void VNEngine::setupContradictionScreen() {
    m_currentScreen = VNScreen::CONTRADICTION;
    m_clickables.clear();
    m_dialogue.clear();

    auto suspect = m_activeCase->getSuspectById(m_activeSuspectId);
    if (!suspect) {
        setupInterrogationScreen();
        return;
    }

    // Grid of Discovered Evidence Cards
    auto discovered = m_activeCase->getDiscoveredEvidence();
    int cardW = 210;
    int cardH = 90;
    int startX = 60;
    int startY = 280;
    int cols = 4;
    int gap = 20;

    for (size_t i = 0; i < discovered.size(); ++i) {
        int col = (int)i % cols;
        int row = (int)i / cols;
        int cx = startX + col * (cardW + gap);
        int cy = startY + row * (cardH + gap);

        std::string eid = discovered[i].getId();
        m_clickables.emplace_back(cx, cy, cardW, cardH, "present_" + eid, discovered[i].getName(), [this, eid]() {
            onContradictionSubmitted(eid);
        });
    }

    // Cancel Button
    m_clickables.emplace_back(60, m_windowHeight - 60, 180, 40, "cancel_contradiction", "Cancel Confrontation", [this]() {
        setupInterrogationScreen();
    });
}

void VNEngine::onContradictionSubmitted(const std::string& evidenceId) {
    auto suspect = m_activeCase->getSuspectById(m_activeSuspectId);
    if (!suspect) return;

    std::string statement, resolution;
    bool success = suspect->testContradiction(evidenceId, statement, resolution);

    if (success) {
        // Dramatic Objection Animation & SFX!
        PlaySoundA("SystemExclamation", NULL, SND_ALIAS | SND_ASYNC);
        m_showObjection = true;
        m_objectionTimer = 0.0f;
        m_activeExpression = "broken";
        m_detective->addScore(50);
        m_notebook.addLead(suspect->getName() + "'s statement shattered with " + evidenceId);
        m_activeCase->updateObjectives();

        DialogueEntry de("broken_confess", suspect->getName(), m_activeCharacterId, "broken",
                         resolution, "interrogation_room", "", [this]() {
            setupInterrogationScreen();
        });
        m_dialogue.setEntries({ de });
        m_dialogue.start();
        showToast("CONTRADICTION EXPOSED! Suspect's alibi shattered! (+50 Points)", 4.0f);
    } else {
        PlaySoundA("SystemHand", NULL, SND_ALIAS | SND_ASYNC);
        m_detective->deductScore(15);
        m_investigationMistakes++;
        m_activeExpression = "angry";

        DialogueEntry de("failed_contra", suspect->getName(), m_activeCharacterId, "angry",
                         "That proves nothing against me, Detective! Check your facts before making reckless claims.",
                         "interrogation_room", "", [this]() {
            setupInterrogationScreen();
        });
        m_dialogue.setEntries({ de });
        m_dialogue.start();
        showToast("Contradiction failed! Score Penalty: -15 Points", 3.5f);
    }
}

void VNEngine::setupClueBoardScreen() {
    m_currentScreen = VNScreen::CLUE_BOARD;
    m_clickables.clear();
    m_dialogue.clear();

    auto discovered = m_activeCase->getDiscoveredEvidence();
    int cardW = 200;
    int cardH = 95;
    int startX = 60;
    int startY = 130;
    int cols = 4;
    int gap = 20;

    for (size_t i = 0; i < discovered.size(); ++i) {
        int col = (int)i % cols;
        int row = (int)i / cols;
        int cx = startX + col * (cardW + gap);
        int cy = startY + row * (cardH + gap);

        std::string eid = discovered[i].getId();
        m_clickables.emplace_back(cx, cy, cardW, cardH, "clue_" + eid, discovered[i].getName(), [this, eid]() {
            onCluePinClicked(eid);
        });
    }

    // Connect button
    m_clickables.emplace_back(m_windowWidth / 2 - 120, m_windowHeight - 65, 240, 42, "connect_pins", "Connect Pinned Clues", [this]() {
        if (!m_cluePin1.empty() && !m_cluePin2.empty()) {
            std::string title, insight;
            int res = m_activeCase->getClueBoard().connectClues(m_cluePin1, m_cluePin2, title, insight);
            if (res == 1) {
                PlaySoundA("SystemAsterisk", NULL, SND_ALIAS | SND_ASYNC);
                m_detective->addScore(35);
                m_notebook.addLead("Deduction [" + title + "]: " + insight);
                m_activeCase->updateObjectives();
                showToast("DEDUCTION BREAKTHROUGH: " + title, 5.0f);
            } else if (res == 0) {
                showToast("These clues are already connected.", 3.0f);
            } else {
                showToast("No logical connection between these clues.", 3.0f);
            }
            m_cluePin1.clear();
            m_cluePin2.clear();
            setupClueBoardScreen();
        } else {
            showToast("Select two clues to connect.", 2.5f);
        }
    });

    // Return button
    m_clickables.emplace_back(60, m_windowHeight - 65, 160, 42, "back_inv", "<< Return", [this]() {
        setupInvestigationScreen();
    });
}

void VNEngine::onCluePinClicked(const std::string& evidenceId) {
    if (m_cluePin1.empty()) {
        m_cluePin1 = evidenceId;
        showToast("First clue selected: " + evidenceId + ". Now click second clue.", 3.0f);
    } else if (m_cluePin2.empty() && evidenceId != m_cluePin1) {
        m_cluePin2 = evidenceId;
        showToast("Second clue selected: " + evidenceId + ". Click 'Connect Pinned Clues'!", 3.0f);
    } else {
        m_cluePin1 = evidenceId;
        m_cluePin2.clear();
    }
}

void VNEngine::setupNotebookScreen() {
    m_currentScreen = VNScreen::NOTEBOOK;
    m_clickables.clear();
    m_dialogue.clear();

    m_clickables.emplace_back(60, m_windowHeight - 65, 160, 42, "back_inv", "<< Return", [this]() {
        setupInvestigationScreen();
    });
}

void VNEngine::setupAccusationScreen() {
    m_currentScreen = VNScreen::ACCUSATION;
    m_clickables.clear();
    m_dialogue.clear();

    // Step 1: Suspect Selection
    const auto& characters = m_activeCase->getCharacters();
    int startY = 160;
    int btnW = 260;
    int btnH = 40;
    int gap = 10;

    for (size_t i = 0; i < characters.size(); ++i) {
        std::string cid = characters[i]->getId();
        std::string name = characters[i]->getName();
        m_clickables.emplace_back(60, startY + (int)i * (btnH + gap), btnW, btnH, "accuse_suspect_" + cid, name, [this, cid]() {
            m_accusedSuspectId = cid;
            setupAccusationScreen();
        });
    }

    // Step 2: Evidence Selection
    auto discovered = m_activeCase->getDiscoveredEvidence();
    int evX = 360;
    int evY = 160;
    int evW = 280;

    for (size_t i = 0; i < discovered.size() && i < 6; ++i) {
        std::string eid = discovered[i].getId();
        std::string name = discovered[i].getName();
        m_clickables.emplace_back(evX, evY + (int)i * (btnH + gap), evW, btnH, "accuse_ev_" + eid, eid + ": " + name, [this, eid]() {
            m_accusedEvidenceId = eid;
            setupAccusationScreen();
        });
    }

    // Step 3: Motive Selection
    std::vector<std::pair<std::string, std::string>> motives = {
        {"DEBT", "1. Financial Debt / Blackmail"},
        {"EMBEZZLEMENT", "2. Embezzlement Cover-Up"},
        {"PATENT", "3. Disinheritance / Patent Theft"},
        {"VENDETTA", "4. Jealousy / Personal Vendetta"}
    };
    int motX = 680;
    int motY = 160;
    for (size_t i = 0; i < motives.size(); ++i) {
        std::string mkey = motives[i].first;
        std::string mlbl = motives[i].second;
        m_clickables.emplace_back(motX, motY + (int)i * (btnH + gap), btnW, btnH, "accuse_mot_" + mkey, mlbl, [this, mkey]() {
            m_accusedMotive = mkey;
            setupAccusationScreen();
        });
    }

    // Submit Accusation Button
    m_clickables.emplace_back(m_windowWidth / 2 - 140, m_windowHeight - 80, 280, 48, "submit_indictment", "SUBMIT FORMAL INDICTMENT", [this]() {
        onSubmitAccusation();
    });

    // Cancel Button
    m_clickables.emplace_back(60, m_windowHeight - 75, 160, 42, "cancel_accuse", "<< Back", [this]() {
        setupInvestigationScreen();
    });
}

void VNEngine::onSubmitAccusation() {
    m_lastVerdict = m_activeCase->evaluateAccusation(m_accusedSuspectId, m_accusedEvidenceId, m_accusedMotive);
    PlaySoundA(m_lastVerdict.isCorrectCulprit ? "SystemAsterisk" : "SystemHand", NULL, SND_ALIAS | SND_ASYNC);
    setupVerdictScreen();
}

void VNEngine::setupVerdictScreen() {
    m_currentScreen = VNScreen::VERDICT;
    m_clickables.clear();
    m_dialogue.clear();

    m_clickables.emplace_back(m_windowWidth / 2 - 120, m_windowHeight - 70, 240, 44, "verdict_main_menu", "Return to Main Menu", [this]() {
        setupMainMenu();
    });
}

void VNEngine::setupHowToPlayScreen() {
    m_currentScreen = VNScreen::HOW_TO_PLAY;
    m_clickables.clear();

    m_clickables.emplace_back(60, m_windowHeight - 65, 160, 42, "back_menu", "<< Back to Menu", [this]() {
        setupMainMenu();
    });
}

void VNEngine::setupCreditsScreen() {
    m_currentScreen = VNScreen::CREDITS;
    m_clickables.clear();

    m_clickables.emplace_back(60, m_windowHeight - 65, 160, 42, "back_menu", "<< Back to Menu", [this]() {
        setupMainMenu();
    });
}

void VNEngine::setupArchivesScreen() {
    m_currentScreen = VNScreen::ARCHIVES;
    m_clickables.clear();

    m_clickables.emplace_back(60, m_windowHeight - 65, 160, 42, "back_menu", "<< Back to Menu", [this]() {
        setupMainMenu();
    });
}

void VNEngine::onSaveGame() {
    bool ok = SaveManager::saveGame("saves/case_save.dat", m_difficulty, *m_detective, *m_activeCase, m_notebook);
    if (ok) {
        showToast("Investigation successfully saved to saves/case_save.dat!", 3.5f);
    } else {
        showToast("Failed to save investigation.", 3.5f);
    }
}

void VNEngine::onLoadGame() {
    GameSaveState state;
    bool ok = SaveManager::loadGame("saves/case_save.dat", state);
    if (ok) {
        m_difficulty = state.difficulty;
        std::string rankTitle = "Senior Investigator";
        if (m_difficulty == 1) rankTitle = "Junior Detective";
        else if (m_difficulty == 3) rankTitle = "Chief Superintendent";

        m_detective = std::make_shared<Detective>("DET_01", state.detectiveName, "SY-7492", rankTitle);
        m_detective->setScore(state.detectiveScore);
        m_detective->setEnergy(state.detectiveEnergy);
        m_detective->setHintsUsed(state.hintsUsed);

        m_activeCase = std::make_unique<CaseFile>(state.caseNumber);
        m_notebook.clear();
        m_investigationMistakes = 0;

        // Restore evidence
        for (const auto& evId : state.discoveredEvidenceIds) {
            m_activeCase->markEvidenceDiscovered(evId);
        }

        // Restore broken alibis
        for (const auto& sId : state.brokenAlibiSuspectIds) {
            auto susp = m_activeCase->getSuspectById(sId);
            if (susp) susp->setAlibiBroken(true);
        }

        // Restore examined hotspots
        for (const auto& hsPair : state.examinedHotspots) {
            size_t colon = hsPair.find(':');
            if (colon != std::string::npos) {
                std::string scId = hsPair.substr(0, colon);
                int idx = std::stoi(hsPair.substr(colon + 1));
                CrimeScene* sc = m_activeCase->getSceneById(scId);
                if (sc) sc->setHotspotExamined(idx, true);
            }
        }

        // Restore deductions
        for (int idx : state.unlockedDeductionIndices) {
            m_activeCase->getClueBoard().unlockDeductionByIndex(idx);
        }

        // Restore notebook
        m_notebook.loadNotes(state.customNotes);
        m_notebook.loadLeads(state.recordedLeads);

        m_activeCase->updateObjectives();
        m_currentSceneIndex = 0;

        showToast("Investigation resumed for Detective " + m_detective->getName() + "!", 3.5f);
        setupInvestigationScreen();
    } else {
        showToast("No valid save file found at saves/case_save.dat", 3.5f);
    }
}

// -------------------------------------------------------------
// RENDER DISPATCH
// -------------------------------------------------------------

void VNEngine::render(HDC hdc) {
    if (!m_renderer) return;

    m_renderer->beginFrame();

    switch (m_currentScreen) {
        case VNScreen::MAIN_MENU: {
            Gdiplus::Bitmap* bg = m_assetManager.getBackground("main_menu");
            m_renderer->drawBackground(bg);

            // Title Banner
            m_renderer->drawText("CASE CLOSED", m_windowWidth / 2, 110, 52.0f,
                                 Gdiplus::Color(255, 255, 225, 130), true, Gdiplus::StringAlignmentCenter);
            m_renderer->drawText("Anime Visual Novel Detective Mystery Simulator", m_windowWidth / 2, 180, 20.0f,
                                 Gdiplus::Color(255, 180, 200, 230), false, Gdiplus::StringAlignmentCenter);
            m_renderer->drawText("Standard C++17 Standard Library Architecture", m_windowWidth / 2, 215, 14.0f,
                                 Gdiplus::Color(255, 140, 160, 190), false, Gdiplus::StringAlignmentCenter);

            // Buttons
            for (const auto& cr : m_clickables) {
                bool isHovered = cr.contains(m_mouseX, m_mouseY);
                m_renderer->drawButton(cr.x, cr.y, cr.w, cr.h, cr.label, isHovered, false, cr.id == "new_game");
            }
            break;
        }

        case VNScreen::CASE_SELECT: {
            Gdiplus::Bitmap* bg = m_assetManager.getBackground("main_menu");
            m_renderer->drawBackground(bg);

            m_renderer->drawText("SELECT CRIMINAL CASE DOSSIER", m_windowWidth / 2, 70, 32.0f,
                                 Gdiplus::Color(255, 255, 220, 100), true, Gdiplus::StringAlignmentCenter);

            // Case Cards
            for (const auto& cr : m_clickables) {
                bool isHovered = cr.contains(m_mouseX, m_mouseY);
                if (cr.id.find("case_") != std::string::npos) {
                    m_renderer->drawButton(cr.x, cr.y, cr.w, cr.h, cr.label, isHovered, false, true);
                } else {
                    m_renderer->drawButton(cr.x, cr.y, cr.w, cr.h, cr.label, isHovered, false);
                }
            }
            break;
        }

        case VNScreen::BRIEFING: {
            Gdiplus::Bitmap* bg = m_assetManager.getBackground("briefing_room");
            m_renderer->drawBackground(bg);

            // Detective Sherlock portrait on right
            Gdiplus::Bitmap* portrait = m_assetManager.getPortrait("detective", m_dialogue.getCurrentEntry() ? m_dialogue.getCurrentEntry()->expression : "neutral");
            m_renderer->drawPortrait(portrait, m_windowWidth - 480, m_windowHeight - 650, 420, 600);

            // Dialogue Box at bottom
            if (m_dialogue.getCurrentEntry()) {
                m_renderer->drawDialogueBox(40, m_windowHeight - 220, m_windowWidth - 80, 180,
                                            m_dialogue.getCurrentEntry()->speaker,
                                            m_dialogue.getDisplayedText(),
                                            m_dialogue.isComplete(), m_globalTime);
            }

            for (const auto& cr : m_clickables) {
                bool isHovered = cr.contains(m_mouseX, m_mouseY);
                m_renderer->drawButton(cr.x, cr.y, cr.w, cr.h, cr.label, isHovered, false, true);
            }
            break;
        }

        case VNScreen::INVESTIGATION: {
            std::string bgName = m_currentSceneIndex == 0 ? "scene_vault" : (m_currentSceneIndex == 1 ? "scene_substation" : "scene_locker");
            Gdiplus::Bitmap* bg = m_assetManager.getBackground(bgName);
            m_renderer->drawBackground(bg);

            // Top Header Bar
            m_renderer->drawHeaderBar(m_activeCase->getTitle(),
                                      "Detective: " + m_detective->getName() + " (" + m_detective->getRole() + ")",
                                      m_detective->getScore(), m_detective->getEnergy(), 100);

            // Draw Scene Tabs & Action Buttons
            for (const auto& cr : m_clickables) {
                bool isHovered = cr.contains(m_mouseX, m_mouseY);
                if (cr.id.find("scene_tab_") != std::string::npos) {
                    bool isSelected = cr.id == ("scene_tab_" + std::to_string(m_currentSceneIndex));
                    m_renderer->drawButton(cr.x, cr.y, cr.w, cr.h, cr.label, isHovered, isSelected);
                } else if (cr.id.find("hotspot_") != std::string::npos) {
                    size_t hIdx = std::stoi(cr.id.substr(8));
                    bool isExamined = m_activeCase->getScenes()[m_currentSceneIndex].isHotspotExamined(hIdx);
                    m_renderer->drawHotspot(cr.x + cr.w / 2, cr.y + cr.h / 2, cr.w / 2, cr.label, isExamined, isHovered, m_globalTime);
                } else {
                    m_renderer->drawButton(cr.x, cr.y, cr.w, cr.h, cr.label, isHovered, false, cr.id == "goto_accuse");
                }
            }

            // Evidence Dossier Modal if an item was just discovered
            if (m_showDossierModal) {
                int modW = 560;
                int modH = 340;
                int modX = (m_windowWidth - modW) / 2;
                int modY = (m_windowHeight - modH) / 2;

                Gdiplus::SolidBrush dimBrush(Gdiplus::Color(180, 0, 0, 0));
                m_renderer->getGraphics()->FillRectangle(&dimBrush, 0, 0, m_windowWidth, m_windowHeight);

                Gdiplus::SolidBrush modBg(Gdiplus::Color(250, 18, 24, 36));
                m_renderer->getGraphics()->FillRectangle(&modBg, modX, modY, modW, modH);
                Gdiplus::Pen modPen(Gdiplus::Color(255, 235, 185, 75), 3.0f);
                m_renderer->getGraphics()->DrawRectangle(&modPen, modX, modY, modW, modH);

                m_renderer->drawText("[NEW EVIDENCE SECURED]", modX + modW / 2, modY + 20, 20.0f,
                                     Gdiplus::Color(255, 255, 220, 90), true, Gdiplus::StringAlignmentCenter);
                m_renderer->drawText(m_dossierEvidence.getId() + " - " + m_dossierEvidence.getName(),
                                     modX + modW / 2, modY + 60, 22.0f, Gdiplus::Color(255, 255, 255, 255), true, Gdiplus::StringAlignmentCenter);

                std::string catRel = "Category: " + m_dossierEvidence.getCategoryName() + " | Relevance: " + m_dossierEvidence.getRelevanceName();
                m_renderer->drawText(catRel, modX + modW / 2, modY + 95, 13.0f, Gdiplus::Color(255, 180, 200, 220), false, Gdiplus::StringAlignmentCenter);

                m_renderer->drawWrappedText(m_dossierEvidence.getDescription(), modX + 40, modY + 130, modW - 80, 120,
                                            15.0f, Gdiplus::Color(255, 240, 245, 255));

                m_renderer->drawText(">> Click anywhere or press Space to continue...", modX + modW / 2, modY + modH - 35, 13.0f,
                                     Gdiplus::Color(255, 255, 215, 80), false, Gdiplus::StringAlignmentCenter);
            }
            break;
        }

        case VNScreen::INTERROGATION: {
            Gdiplus::Bitmap* bg = m_assetManager.getBackground("interrogation_room");
            m_renderer->drawBackground(bg);

            // Large Suspect Portrait on Right
            Gdiplus::Bitmap* portrait = m_assetManager.getPortrait(m_activeCharacterId, m_activeExpression);
            m_renderer->drawPortrait(portrait, m_windowWidth - 520, m_windowHeight - 650, 460, 650);

            // Header Bar
            m_renderer->drawHeaderBar("INTERROGATION & WITNESS INQUIRY",
                                      "Interrogating: " + m_activeSuspectId,
                                      m_detective->getScore(), m_detective->getEnergy(), 100);

            // Dialogue Box
            if (m_dialogue.getCurrentEntry()) {
                m_renderer->drawDialogueBox(290, m_windowHeight - 200, m_windowWidth - 320, 160,
                                            m_dialogue.getCurrentEntry()->speaker,
                                            m_dialogue.getDisplayedText(),
                                            m_dialogue.isComplete(), m_globalTime);
            }

            // Buttons
            for (const auto& cr : m_clickables) {
                bool isHovered = cr.contains(m_mouseX, m_mouseY);
                bool isSelected = cr.id == ("suspect_" + m_activeSuspectId);
                m_renderer->drawButton(cr.x, cr.y, cr.w, cr.h, cr.label, isHovered, isSelected, cr.id == "confront");
            }
            break;
        }

        case VNScreen::CONTRADICTION: {
            Gdiplus::Bitmap* bg = m_assetManager.getBackground("interrogation_room");
            m_renderer->drawBackground(bg);

            // Suspect portrait
            Gdiplus::Bitmap* portrait = m_assetManager.getPortrait(m_activeCharacterId, m_activeExpression);
            m_renderer->drawPortrait(portrait, m_windowWidth - 420, m_windowHeight - 550, 360, 520);

            // Title
            m_renderer->drawText("CONFRONT SUSPECT WITH EVIDENCE", 60, 70, 28.0f,
                                 Gdiplus::Color(255, 255, 220, 90), true);

            auto suspect = m_activeCase->getSuspectById(m_activeSuspectId);
            if (suspect) {
                std::string claim = "Suspect Claim: \"" + suspect->getAlibi() + "\"";
                m_renderer->drawWrappedText(claim, 60, 120, m_windowWidth - 450, 80, 16.0f, Gdiplus::Color(255, 240, 245, 255));
            }

            m_renderer->drawText("Select the evidence card that shatters their statement:", 60, 235, 16.0f,
                                 Gdiplus::Color(255, 200, 220, 245), true);

            for (const auto& cr : m_clickables) {
                bool isHovered = cr.contains(m_mouseX, m_mouseY);
                if (cr.id.find("present_") != std::string::npos) {
                    std::string eid = cr.id.substr(8);
                    Evidence* ev = m_activeCase->getEvidenceById(eid);
                    if (ev) {
                        m_renderer->drawEvidenceCard(cr.x, cr.y, cr.w, cr.h, ev->getId(), ev->getName(),
                                                     ev->getCategoryName(), ev->getRelevanceName(), isHovered, false);
                    }
                } else {
                    m_renderer->drawButton(cr.x, cr.y, cr.w, cr.h, cr.label, isHovered, false);
                }
            }

            // Objection Banner if triggered
            if (m_showObjection) {
                m_renderer->drawObjectionBanner(m_objectionTimer);
            }
            break;
        }

        case VNScreen::CLUE_BOARD: {
            Gdiplus::Bitmap* bg = m_assetManager.getBackground("clueboard_bg");
            m_renderer->drawBackground(bg);

            // Draw Red String connections
            const auto& connections = m_activeCase->getClueBoard().getDeductions();
            for (const auto& ded : connections) {
                if (ded.discovered) {
                    // Draw red thread between card 1 and card 2
                    Gdiplus::Pen thread(Gdiplus::Color(255, 210, 30, 30), 3.0f);
                    m_renderer->getGraphics()->DrawLine(&thread, 200, 220, 600, 220);
                }
            }

            // Cards
            for (const auto& cr : m_clickables) {
                bool isHovered = cr.contains(m_mouseX, m_mouseY);
                if (cr.id.find("clue_") != std::string::npos) {
                    std::string eid = cr.id.substr(5);
                    bool isPinned = (eid == m_cluePin1 || eid == m_cluePin2);
                    Evidence* ev = m_activeCase->getEvidenceById(eid);
                    if (ev) {
                        m_renderer->drawEvidenceCard(cr.x, cr.y, cr.w, cr.h, ev->getId(), ev->getName(),
                                                     ev->getCategoryName(), ev->getRelevanceName(), isHovered, isPinned);
                    }
                } else {
                    m_renderer->drawButton(cr.x, cr.y, cr.w, cr.h, cr.label, isHovered, false, cr.id == "connect_pins");
                }
            }
            break;
        }

        case VNScreen::ACCUSATION: {
            Gdiplus::Bitmap* bg = m_assetManager.getBackground("courtroom");
            m_renderer->drawBackground(bg);

            m_renderer->drawText("MAGISTRATE'S BENCH -- CRIMINAL INDICTMENT", m_windowWidth / 2, 60, 32.0f,
                                 Gdiplus::Color(255, 255, 225, 120), true, Gdiplus::StringAlignmentCenter);

            m_renderer->drawText("Step 1: Choose Culprit", 60, 125, 16.0f, Gdiplus::Color(255, 255, 215, 80), true);
            m_renderer->drawText("Step 2: Decisive Evidence", 360, 125, 16.0f, Gdiplus::Color(255, 255, 215, 80), true);
            m_renderer->drawText("Step 3: Criminal Motive", 680, 125, 16.0f, Gdiplus::Color(255, 255, 215, 80), true);

            for (const auto& cr : m_clickables) {
                bool isHovered = cr.contains(m_mouseX, m_mouseY);
                bool isSelected = false;
                if (cr.id.find("accuse_suspect_") != std::string::npos) {
                    isSelected = cr.id == ("accuse_suspect_" + m_accusedSuspectId);
                } else if (cr.id.find("accuse_ev_") != std::string::npos) {
                    isSelected = cr.id == ("accuse_ev_" + m_accusedEvidenceId);
                } else if (cr.id.find("accuse_mot_") != std::string::npos) {
                    isSelected = cr.id == ("accuse_mot_" + m_accusedMotive);
                }
                m_renderer->drawButton(cr.x, cr.y, cr.w, cr.h, cr.label, isHovered, isSelected, cr.id == "submit_indictment");
            }
            break;
        }

        case VNScreen::VERDICT: {
            Gdiplus::Bitmap* bg = m_assetManager.getBackground("courtroom");
            m_renderer->drawBackground(bg);

            std::string title = m_lastVerdict.isCorrectCulprit ? "MASTER DETECTIVE -- CASE CLOSED!" : "ACCUSATION COLLAPSED";
            Gdiplus::Color tColor = m_lastVerdict.isCorrectCulprit ? Gdiplus::Color(255, 255, 215, 80) : Gdiplus::Color(255, 220, 50, 50);
            m_renderer->drawText(title, m_windowWidth / 2, 70, 36.0f, tColor, true, Gdiplus::StringAlignmentCenter);

            std::string rankStr = "Final Score: " + std::to_string(m_detective->getScore()) + " | Performance Rank: " +
                                  (m_lastVerdict.isCorrectCulprit ? "Rank S - Master Sleuth" : "Rank C - Insufficient Proof");
            m_renderer->drawText(rankStr, m_windowWidth / 2, 125, 18.0f, Gdiplus::Color(255, 240, 245, 255), true, Gdiplus::StringAlignmentCenter);

            // Report Details Box
            int boxW = 1000;
            int boxH = 430;
            int boxX = (m_windowWidth - boxW) / 2;
            int boxY = 160;

            Gdiplus::SolidBrush repBg(Gdiplus::Color(235, 16, 20, 30));
            m_renderer->getGraphics()->FillRectangle(&repBg, boxX, boxY, boxW, boxH);
            Gdiplus::Pen repPen(Gdiplus::Color(255, 180, 140, 60), 2.0f);
            m_renderer->getGraphics()->DrawRectangle(&repPen, boxX, boxY, boxW, boxH);

            m_renderer->drawWrappedText(m_lastVerdict.outcomeDescription, boxX + 30, boxY + 25, boxW - 60, 120, 16.0f,
                                        Gdiplus::Color(255, 250, 245, 230));

            // Suspect eliminations report
            int elimY = boxY + 160;
            m_renderer->drawText("--- Suspect Elimination Report (Why Innocents Were Cleared) ---", boxX + 30, elimY, 14.0f,
                                 Gdiplus::Color(255, 255, 215, 80), true);
            for (size_t el = 0; el < m_lastVerdict.suspectEliminations.size() && el < 3; ++el) {
                const auto& item = m_lastVerdict.suspectEliminations[el];
                std::string line = "[" + item.suspectId + "] " + item.suspectName + " -> " + item.reason;
                m_renderer->drawText(line, boxX + 30, elimY + 25 + (int)el * 22, 12.0f, Gdiplus::Color(255, 200, 215, 235));
            }

            for (const auto& cr : m_clickables) {
                bool isHovered = cr.contains(m_mouseX, m_mouseY);
                m_renderer->drawButton(cr.x, cr.y, cr.w, cr.h, cr.label, isHovered, false, true);
            }
            break;
        }

        case VNScreen::NOTEBOOK: {
            Gdiplus::Bitmap* bg = m_assetManager.getBackground("briefing_room");
            m_renderer->drawBackground(bg);

            m_renderer->drawText("DETECTIVE'S FIELD NOTEBOOK", m_windowWidth / 2, 70, 32.0f,
                                 Gdiplus::Color(255, 255, 220, 90), true, Gdiplus::StringAlignmentCenter);

            int boxW = 1000;
            int boxH = 480;
            int boxX = (m_windowWidth - boxW) / 2;
            int boxY = 120;

            Gdiplus::SolidBrush nbBg(Gdiplus::Color(240, 22, 28, 40));
            m_renderer->getGraphics()->FillRectangle(&nbBg, boxX, boxY, boxW, boxH);
            Gdiplus::Pen nbPen(Gdiplus::Color(255, 190, 150, 65), 2.0f);
            m_renderer->getGraphics()->DrawRectangle(&nbPen, boxX, boxY, boxW, boxH);

            m_renderer->drawText("Official Case Leads & Discovered Facts:", boxX + 40, boxY + 30, 18.0f,
                                 Gdiplus::Color(255, 255, 220, 80), true);

            const auto& leads = m_notebook.getLeads();
            for (size_t ld = 0; ld < leads.size() && ld < 12; ++ld) {
                m_renderer->drawText("[*] " + leads[ld], boxX + 40, boxY + 70 + (int)ld * 26, 14.0f,
                                     Gdiplus::Color(255, 230, 240, 250));
            }

            for (const auto& cr : m_clickables) {
                bool isHovered = cr.contains(m_mouseX, m_mouseY);
                m_renderer->drawButton(cr.x, cr.y, cr.w, cr.h, cr.label, isHovered, false);
            }
            break;
        }

        case VNScreen::HOW_TO_PLAY: {
            Gdiplus::Bitmap* bg = m_assetManager.getBackground("main_menu");
            m_renderer->drawBackground(bg);

            m_renderer->drawText("HOW TO PLAY -- DETECTIVE MANUAL", m_windowWidth / 2, 70, 32.0f,
                                 Gdiplus::Color(255, 255, 220, 90), true, Gdiplus::StringAlignmentCenter);

            int boxW = 960;
            int boxH = 480;
            int boxX = (m_windowWidth - boxW) / 2;
            int boxY = 120;

            Gdiplus::SolidBrush bgBox(Gdiplus::Color(235, 14, 18, 26));
            m_renderer->getGraphics()->FillRectangle(&bgBox, boxX, boxY, boxW, boxH);
            Gdiplus::Pen pen(Gdiplus::Color(255, 180, 140, 60), 2.0f);
            m_renderer->getGraphics()->DrawRectangle(&pen, boxX, boxY, boxW, boxH);

            std::vector<std::string> rules = {
                "1. INVESTIGATION: Click scene tabs to explore hotspots. Examine points of interest to acquire forensic clues.",
                "2. INTERROGATION: Question suspects on alibis and motives. Discovering clues unlocks targeted dialogue options.",
                "3. CONTRADICTIONS: When a suspect makes a false claim, select 'Confront Contradiction' and present evidence.",
                "4. CLUE BOARD: Connect pairs of related clues to unlock analytical deductions.",
                "5. ACCUSATION: At the Magistrate's Bench, name the true culprit, present decisive proof, and establish motive!"
            };

            for (size_t r = 0; r < rules.size(); ++r) {
                m_renderer->drawWrappedText(rules[r], boxX + 40, boxY + 40 + (int)r * 75, boxW - 80, 65, 15.0f,
                                            Gdiplus::Color(255, 235, 240, 250));
            }

            for (const auto& cr : m_clickables) {
                bool isHovered = cr.contains(m_mouseX, m_mouseY);
                m_renderer->drawButton(cr.x, cr.y, cr.w, cr.h, cr.label, isHovered, false);
            }
            break;
        }

        case VNScreen::CREDITS: {
            Gdiplus::Bitmap* bg = m_assetManager.getBackground("main_menu");
            m_renderer->drawBackground(bg);

            m_renderer->drawText("PROJECT CREDITS & OOPS ARCHITECTURE", m_windowWidth / 2, 70, 32.0f,
                                 Gdiplus::Color(255, 255, 220, 90), true, Gdiplus::StringAlignmentCenter);

            int boxW = 960;
            int boxH = 480;
            int boxX = (m_windowWidth - boxW) / 2;
            int boxY = 120;

            Gdiplus::SolidBrush bgBox(Gdiplus::Color(235, 14, 18, 26));
            m_renderer->getGraphics()->FillRectangle(&bgBox, boxX, boxY, boxW, boxH);
            Gdiplus::Pen pen(Gdiplus::Color(255, 180, 140, 60), 2.0f);
            m_renderer->getGraphics()->DrawRectangle(&pen, boxX, boxY, boxW, boxH);

            std::vector<std::string> credits = {
                "Language: Standard C++17 (C++ Standard Library & Win32 GDI+ Graphics)",
                "Encapsulation: Private member state in Character, CaseFile, Evidence, and VNEngine.",
                "Abstraction: Abstract base Character class defining pure virtual interfaces.",
                "Inheritance: Detective, Suspect, and Witness classes inheriting from Character.",
                "Runtime Polymorphism: Virtual dispatch across std::vector<std::shared_ptr<Character>> collections.",
                "Composition: CaseFile contains CrimeScenes, Evidence, Characters, and ClueBoard.",
                "Crafted for Academic Capstone Demonstration in Object-Oriented Software Design."
            };

            for (size_t c = 0; c < credits.size(); ++c) {
                m_renderer->drawText("[*] " + credits[c], boxX + 40, boxY + 45 + (int)c * 50, 15.0f,
                                     Gdiplus::Color(255, 230, 240, 255));
            }

            for (const auto& cr : m_clickables) {
                bool isHovered = cr.contains(m_mouseX, m_mouseY);
                m_renderer->drawButton(cr.x, cr.y, cr.w, cr.h, cr.label, isHovered, false);
            }
            break;
        }

        case VNScreen::ARCHIVES: {
            Gdiplus::Bitmap* bg = m_assetManager.getBackground("main_menu");
            m_renderer->drawBackground(bg);

            m_renderer->drawText("SCOTLAND YARD CRIMINAL ARCHIVES", m_windowWidth / 2, 70, 32.0f,
                                 Gdiplus::Color(255, 255, 220, 90), true, Gdiplus::StringAlignmentCenter);

            int boxW = 960;
            int boxH = 480;
            int boxX = (m_windowWidth - boxW) / 2;
            int boxY = 120;

            Gdiplus::SolidBrush bgBox(Gdiplus::Color(235, 14, 18, 26));
            m_renderer->getGraphics()->FillRectangle(&bgBox, boxX, boxY, boxW, boxH);
            Gdiplus::Pen pen(Gdiplus::Color(255, 180, 140, 60), 2.0f);
            m_renderer->getGraphics()->DrawRectangle(&pen, boxX, boxY, boxW, boxH);

            std::vector<std::pair<std::string, std::string>> cases = {
                { "CASE #1: THE MIDNIGHT MUSEUM HEIST",
                  "A 45-carat sapphire disappears during a 4-minute blackout. Access logs and bootprints contradict initial claims." },
                { "CASE #2: THE LAST TRAIN MYSTERY",
                  "Tycoon Victor Sterling is poisoned in his private compartment. A missing cufflink and tea delivery hold the key." },
                { "CASE #3: THE LOCKED ROOM INCIDENT",
                  "Master clockmaker Thaddeus Thorne shot behind a door bolted from the inside. Mechanical physics reveal the trap." }
            };

            for (size_t i = 0; i < cases.size(); ++i) {
                int cy = boxY + 35 + (int)i * 125;
                m_renderer->drawText(cases[i].first, boxX + 40, cy, 17.0f, Gdiplus::Color(255, 255, 215, 80), true);
                m_renderer->drawWrappedText(cases[i].second, boxX + 40, cy + 30, boxW - 80, 60, 14.0f,
                                            Gdiplus::Color(255, 220, 230, 245));
            }

            for (const auto& cr : m_clickables) {
                bool isHovered = cr.contains(m_mouseX, m_mouseY);
                m_renderer->drawButton(cr.x, cr.y, cr.w, cr.h, cr.label, isHovered, false);
            }
            break;
        }

        default:
            break;
    }

    // Render Toast Message if active
    if (m_toastTimer > 0.0f && !m_toastMessage.empty()) {
        int toastW = 860;
        int toastH = 50;
        int toastX = (m_windowWidth - toastW) / 2;
        int toastY = 80;

        Gdiplus::SolidBrush toastBg(Gdiplus::Color(245, 20, 28, 42));
        m_renderer->getGraphics()->FillRectangle(&toastBg, toastX, toastY, toastW, toastH);
        Gdiplus::Pen toastPen(Gdiplus::Color(255, 255, 220, 90), 2.0f);
        m_renderer->getGraphics()->DrawRectangle(&toastPen, toastX, toastY, toastW, toastH);

        m_renderer->drawText(m_toastMessage, toastX + toastW / 2, toastY + 14, 15.0f,
                             Gdiplus::Color(255, 255, 255, 240), true, Gdiplus::StringAlignmentCenter);
    }

    m_renderer->endFrame(hdc);
}
