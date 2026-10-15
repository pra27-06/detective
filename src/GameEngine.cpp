#include "GameEngine.h"
#include "TerminalUI.h"
#include "SaveManager.h"
#include <iostream>
#include <vector>

GameEngine::GameEngine()
    : m_detective(nullptr), m_activeCase(nullptr),
      m_difficulty(GameDifficulty::ROOKIE), m_isRunning(true),
      m_caseSolved(false), m_investigationMistakes(0) {
    UI::initTerminal();
}

std::string GameEngine::getDifficultyName() const {
    switch (m_difficulty) {
        case GameDifficulty::ROOKIE:          return "Rookie Detective (Guided)";
        case GameDifficulty::SENIOR:          return "Senior Detective (Standard)";
        case GameDifficulty::CHIEF_INSPECTOR: return "Chief Inspector (Hardcore)";
        default:                              return "Standard";
    }
}

void GameEngine::run() {
    while (m_isRunning) {
        showMainMenu();
    }
    UI::clearScreen();
    std::cout << UI::Color::BRIGHT_CYAN << "\nThank you for playing CASE CLOSED. Goodbye, Detective!\n\n" << UI::Color::RESET;
}

void GameEngine::toggleColorMode() {
    bool current = UI::isColorEnabled();
    UI::setColorEnabled(!current);
    UI::clearScreen();
    if (!current) {
        UI::printSuccess("ANSI color formatting ENABLED.");
    } else {
        std::cout << "\n[INFO] Plain-text (No-Color) mode ENABLED for standard terminals.\n";
    }
    UI::pause();
}

void GameEngine::showMainMenu() {
    UI::clearScreen();
    UI::printBanner();

    std::string colorStatus = UI::isColorEnabled() ? "ANSI Colors (ON)" : "Plain Text (OFF)";

    std::cout << UI::Color::BOLD << UI::Color::WHITE << "  MAIN MENU:\n" << UI::Color::RESET;
    std::cout << "  [1] Start New Investigation\n";
    std::cout << "  [2] Continue Saved Case\n";
    std::cout << "  [3] Case Files & Mystery Archives\n";
    std::cout << "  [4] How to Play & Investigation Guide\n";
    std::cout << "  [5] Credits & OOPs Architecture\n";
    std::cout << "  [6] Toggle Visual Mode (Current: " << colorStatus << ")\n";
    std::cout << "  [7] Exit to Desktop\n\n";

    int choice = UI::promptInt("Select an option", 1, 7);

    switch (choice) {
        case 1: startNewGame(); break;
        case 2: continueGame(); break;
        case 3: browseCaseFiles(); break;
        case 4: showHowToPlay(); break;
        case 5: showCredits(); break;
        case 6: toggleColorMode(); break;
        case 7: m_isRunning = false; break;
    }
}

void GameEngine::startNewGame() {
    UI::clearScreen();
    UI::printHeader("NEW INVESTIGATION: DETECTIVE ENROLLMENT");

    std::string name = UI::promptString("Enter your Detective's Name");
    if (name.empty()) name = "Sherlock";

    std::cout << "\n" << UI::Color::CYAN << "Select Difficulty Level:" << UI::Color::RESET << "\n";
    std::cout << "  [1] Rookie Detective   (Forgiving energy, hints available, low penalties)\n";
    std::cout << "  [2] Senior Detective   (Standard rules, hints cost deduction points)\n";
    std::cout << "  [3] Chief Inspector    (Hardcore, strict penalties, no free hints)\n\n";
    int diffChoice = UI::promptInt("Choose difficulty", 1, 3);
    m_difficulty = static_cast<GameDifficulty>(diffChoice);

    std::cout << "\n" << UI::Color::CYAN << "Select Criminal Case File:" << UI::Color::RESET << "\n";
    std::cout << "  [1] Case #1: The Midnight Museum Heist (Theft / Sensor Bypass)\n";
    std::cout << "  [2] Case #2: The Last Train Mystery    (Poisoning / Alibi Deception)\n";
    std::cout << "  [3] Case #3: The Locked Room Incident  (Mechanical Clockwork Trap)\n\n";
    int caseChoice = UI::promptInt("Select case to open", 1, 3);

    std::string rankTitle = "Inspector";
    if (m_difficulty == GameDifficulty::ROOKIE) rankTitle = "Junior Detective";
    else if (m_difficulty == GameDifficulty::CHIEF_INSPECTOR) rankTitle = "Chief Superintendent";

    m_detective = std::make_shared<Detective>("DET_01", name, "SY-7492", rankTitle);
    m_activeCase = std::make_unique<CaseFile>(caseChoice);
    m_notebook.clear();
    m_caseSolved = false;
    m_investigationMistakes = 0;

    UI::clearScreen();
    UI::printSuccess("Case file dispatched to " + m_detective->getRole() + " " + m_detective->getName() + "!");
    m_activeCase->displayBriefingScreen(getDifficultyName());
    UI::pause();

    runCaseLoop();
}

void GameEngine::continueGame() {
    UI::clearScreen();
    UI::printHeader("CONTINUE INVESTIGATION");

    std::string savePath = SaveManager::getDefaultSavePath();
    if (!SaveManager::hasSaveFile(savePath)) {
        UI::printError("No save file detected at: " + savePath);
        UI::printInfo("Start a New Game to begin a case.");
        UI::pause();
        return;
    }

    GameSaveState state;
    if (!SaveManager::loadGame(savePath, state)) {
        UI::printError("Failed to parse save file. File may be corrupted or invalid.");
        UI::pause();
        return;
    }

    m_difficulty = static_cast<GameDifficulty>(state.difficulty);
    std::string rankTitle = "Inspector";
    if (m_difficulty == GameDifficulty::ROOKIE) rankTitle = "Junior Detective";
    else if (m_difficulty == GameDifficulty::CHIEF_INSPECTOR) rankTitle = "Chief Superintendent";

    m_detective = std::make_shared<Detective>("DET_01", state.detectiveName, "SY-7492", rankTitle);
    m_detective->setScore(state.detectiveScore);
    m_detective->setEnergy(state.detectiveEnergy);
    m_detective->setHintsUsed(state.hintsUsed);

    m_activeCase = std::make_unique<CaseFile>(state.caseNumber);
    m_notebook.clear();
    m_caseSolved = false;
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

    UI::printSuccess("Investigation data successfully restored for Detective " + m_detective->getName() + "!");
    std::cout << UI::Color::CYAN << "Resuming: " << m_activeCase->getTitle() << UI::Color::RESET << "\n";
    UI::pause();

    runCaseLoop();
}

void GameEngine::browseCaseFiles() {
    UI::clearScreen();
    UI::printHeader("SCOTLAND YARD CASE ARCHIVES");

    std::cout << UI::Color::BOLD << "CASE FILE #1: THE MIDNIGHT MUSEUM HEIST" << UI::Color::RESET << "\n";
    std::cout << "  Category: Grand Larceny / Security Tampering\n";
    std::cout << "  Synopsis: The 45-carat Star of Alexandria vanished from a sealed glass vault during\n"
              << "            a 4-minute blackout. Access logs and bootprints contradict initial statements.\n";
    UI::printDivider();

    std::cout << UI::Color::BOLD << "CASE FILE #2: THE LAST TRAIN MYSTERY" << UI::Color::RESET << "\n";
    std::cout << "  Category: Homicide / Fast-Acting Poisoning\n";
    std::cout << "  Synopsis: Tycoon Victor Sterling was poisoned in his first-class compartment\n"
              << "            before reaching Glenridge. A missing cufflink and a tea delivery hold the key.\n";
    UI::printDivider();

    std::cout << UI::Color::BOLD << "CASE FILE #3: THE LOCKED ROOM INCIDENT" << UI::Color::RESET << "\n";
    std::cout << "  Category: Homicide Staged as Suicide / Mechanical Apparatus\n";
    std::cout << "  Synopsis: Master clockmaker Thaddeus Thorne was shot behind a door bolted from\n"
              << "            the inside. The secret lies in clockwork escapements and piano wire physics.\n";
    UI::printDivider();

    UI::pause();
}

void GameEngine::showHowToPlay() {
    UI::clearScreen();
    UI::printHeader("HOW TO PLAY — INVESTIGATOR'S MANUAL");

    std::vector<std::string> lines;
    lines.push_back("1. EXPLORING CRIME SCENES:");
    lines.push_back("   Visit crime scenes to examine specific hotspots. Examining objects reveals");
    lines.push_back("   crucial evidence, forensic findings, and eyewitness leads.");
    lines.push_back("");
    lines.push_back("2. INTERROGATION & BREAKING ALIBIS:");
    lines.push_back("   Question suspects and witnesses. When a suspect makes a false statement,");
    lines.push_back("   select 'Present Evidence' with the corresponding Evidence ID (e.g., E03).");
    lines.push_back("   Presenting correct proof breaks their alibi and forces admissions!");
    lines.push_back("");
    lines.push_back("3. CONDITIONAL DIALOGUE (EVIDENCE-TRIGGERED):");
    lines.push_back("   Discovering specific evidence unlocks targeted interrogation topics");
    lines.push_back("   allowing you to press suspects on exact physical inconsistencies.");
    lines.push_back("");
    lines.push_back("4. THE CLUE BOARD (CONNECTING THREADS):");
    lines.push_back("   Open your Clue Board to link two related clues (e.g. E01 and E03).");
    lines.push_back("   Finding logical connections unlocks critical deduction breakthroughs.");
    lines.push_back("");
    lines.push_back("5. DEDUCTION MATRIX:");
    lines.push_back("   Review confirmed physical facts, synthesized theories, and exposed contradictions.");
    lines.push_back("");
    lines.push_back("6. THE ACCUSATION & COURT VERDICT:");
    lines.push_back("   When confident, submit your final accusation. You must name the true culprit,");
    lines.push_back("   present decisive evidence, and explain the criminal motive.");
    UI::printBox("DETECTIVE MANUAL", lines, 74);

    UI::pause();
}

void GameEngine::showCredits() {
    UI::clearScreen();
    UI::printHeader("PROJECT CREDITS & ARCHITECTURE");

    std::vector<std::string> lines;
    lines.push_back("PROJECT:      CASE CLOSED — Detective Mystery Simulator");
    lines.push_back("LANGUAGE:     C++17 (Standard Library Only)");
    lines.push_back("COMPILER:     MinGW g++ / Cross-Platform C++17");
    lines.push_back("");
    lines.push_back("OBJECT-ORIENTED PROGRAMMING CONCEPTS DEMONSTRATED:");
    lines.push_back("  - Encapsulation: Private state in Character, Evidence, CaseFile, GameEngine.");
    lines.push_back("  - Abstraction: Abstract base Character class with pure virtual interface.");
    lines.push_back("  - Inheritance: Detective, Suspect, Witness classes inheriting Character.");
    lines.push_back("  - Runtime Polymorphism: Virtual dispatch via shared_ptr<Character> collections.");
    lines.push_back("  - Composition: CaseFile contains Scenes, Characters, Evidence, and ClueBoard.");
    lines.push_back("  - Modern C++: STL containers (vector, map, set), RAII, smart pointers.");
    lines.push_back("");
    lines.push_back("Crafted for Academic Excellence in Computer Science & Engineering.");
    UI::printBox("CREDITS", lines, 74);

    UI::pause();
}

void GameEngine::displayHubHeader() const {
    UI::printDoubleDivider(76);
    m_activeCase->displayCaseHeader();
    m_activeCase->displayProgress();

    std::cout << UI::Color::WHITE << "Detective: " << UI::Color::BOLD << m_detective->getName() << UI::Color::RESET
              << " (" << m_detective->getRole() << ") | "
              << "Score: " << UI::Color::BRIGHT_YELLOW << m_detective->getScore() << UI::Color::RESET << " | "
              << "Energy: " << UI::Color::BRIGHT_GREEN << m_detective->getEnergy() << "/100" << UI::Color::RESET << "\n";
    UI::printDivider('-', 76);
}

void GameEngine::runCaseLoop() {
    while (!m_caseSolved && m_isRunning) {
        UI::clearScreen();
        displayHubHeader();

        std::cout << UI::Color::BOLD << UI::Color::CYAN << "INVESTIGATION ACTIONS:\n" << UI::Color::RESET;
        std::cout << "  [1] Investigate Crime Scenes\n";
        std::cout << "  [2] Interrogate Suspects & Question Witnesses\n";
        std::cout << "  [3] Examine Clue Board (Link Evidence Pairs)\n";
        std::cout << "  [4] View Deduction Matrix (Facts, Theories, Contradictions)\n";
        std::cout << "  [5] Review Discovered Evidence Inventory\n";
        std::cout << "  [6] Review Detective Notebook & Add Field Notes\n";
        std::cout << "  [7] Review Interrogation Transcripts\n";
        std::cout << "  [8] Request Investigative Hint\n";
        std::cout << "  [9] Submit Final Accusation to Magistrate\n";
        std::cout << "  [10] Save Current Investigation\n";
        std::cout << "  [11] Suspend Case & Return to Main Menu\n\n";

        int choice = UI::promptInt("Choose an action", 1, 11);

        switch (choice) {
            case 1: handleInvestigation(); break;
            case 2: handleInterrogation(); break;
            case 3: handleClueBoard(); break;
            case 4: handleDeductionMatrix(); break;
            case 5: {
                UI::clearScreen();
                m_activeCase->displayDiscoveredEvidenceList();
                auto disc = m_activeCase->getDiscoveredEvidence();
                if (!disc.empty()) {
                    std::string evId = UI::promptString("Enter Evidence ID to view full card (or 'back')", true);
                    evId = UI::toUpper(evId);
                    Evidence* ev = m_activeCase->getEvidenceById(evId);
                    if (ev && ev->isDiscovered()) {
                        ev->displayCard();
                    } else if (evId != "BACK" && !evId.empty()) {
                        UI::printWarning("Evidence ID not found in current inventory.");
                    }
                }
                UI::pause();
                break;
            }
            case 6: handleNotebook(); break;
            case 7: handleTranscripts(); break;
            case 8: handleHintRequest(); break;
            case 9: handleAccusation(); break;
            case 10: handleSaveGame(); break;
            case 11: {
                if (UI::promptConfirm("Are you sure you want to return to main menu? Unsaved progress will be lost.")) {
                    return;
                }
                break;
            }
        }
    }
}

void GameEngine::handleInvestigation() {
    UI::clearScreen();
    UI::printHeader("CRIME SCENE INVESTIGATION");

    auto& scenes = m_activeCase->getScenes();
    std::cout << "Available Scenes:\n";
    for (size_t i = 0; i < scenes.size(); ++i) {
        std::cout << "  [" << (i + 1) << "] " << scenes[i].getName() << "\n";
    }
    std::cout << "  [" << (scenes.size() + 1) << "] Return to Hub\n\n";

    int sceneChoice = UI::promptInt("Select a location to visit", 1, static_cast<int>(scenes.size() + 1));
    if (sceneChoice == static_cast<int>(scenes.size() + 1)) return;

    CrimeScene& scene = scenes[sceneChoice - 1];

    while (true) {
        UI::clearScreen();
        scene.displaySceneOverview();

        std::cout << "\nActions:\n";
        std::cout << "  [1-" << scene.getHotspotCount() << "] Examine Point of Interest\n";
        std::cout << "  [" << (scene.getHotspotCount() + 1) << "] Leave Location\n\n";

        int hsChoice = UI::promptInt("Select target", 1, static_cast<int>(scene.getHotspotCount() + 1));
        if (hsChoice == static_cast<int>(scene.getHotspotCount() + 1)) break;

        size_t hsIdx = hsChoice - 1;
        int energyCost = (m_difficulty == GameDifficulty::CHIEF_INSPECTOR) ? 15 : 10;
        if (!m_detective->spendEnergy(energyCost)) {
            UI::printWarning("You are fatigued! Rest or drink coffee before performing intensive physical searches.");
            m_detective->restoreEnergy(25);
            UI::pause();
            continue;
        }

        std::string foundEvId = scene.examineHotspot(hsIdx);
        const auto& hs = scene.getHotspots()[hsIdx];

        std::cout << "\n" << UI::Color::CYAN << "Examining " << hs.name << "..." << UI::Color::RESET << "\n";
        std::cout << UI::wrapText(hs.inspectDescription, 74) << "\n\n";

        if (!foundEvId.empty()) {
            if (m_activeCase->markEvidenceDiscovered(foundEvId)) {
                Evidence* ev = m_activeCase->getEvidenceById(foundEvId);
                UI::printSuccess("NEW EVIDENCE SECURED: " + foundEvId + " - " + (ev ? ev->getName() : ""));
                m_detective->addScore(25);
                m_notebook.addLead("Discovered " + foundEvId + ": " + (ev ? ev->getName() : "") + " at " + scene.getName());
                if (ev) ev->displayCard();
            } else {
                UI::printInfo("You have already documented all evidentiary clues from this point.");
            }
        } else {
            UI::printInfo("No fresh physical evidence was recovered here, but the observation is noted.");
        }

        UI::pause();
    }
}

void GameEngine::handleInterrogation() {
    UI::clearScreen();
    UI::printHeader("INTERROGATION ROOM & WITNESS INQUIRY");

    const auto& characters = m_activeCase->getCharacters();
    std::cout << "Persons of Interest:\n";
    for (size_t i = 0; i < characters.size(); ++i) {
        std::cout << "  [" << (i + 1) << "] [" << characters[i]->getId() << "] "
                  << characters[i]->getName() << " (" << characters[i]->getRole() << ")\n";
    }
    std::cout << "  [" << (characters.size() + 1) << "] Return to Hub\n\n";

    int charChoice = UI::promptInt("Select person to question", 1, static_cast<int>(characters.size() + 1));
    if (charChoice == static_cast<int>(characters.size() + 1)) return;

    auto target = characters[charChoice - 1];
    auto suspect = std::dynamic_pointer_cast<Suspect>(target);
    auto witness = std::dynamic_pointer_cast<Witness>(target);

    while (true) {
        UI::clearScreen();
        // Polymorphic profile display!
        target->displayProfile();

        std::vector<std::string> discoveredIds = m_activeCase->getDiscoveredEvidenceIds();
        std::vector<ConditionalTopic> unlockedCondTopics;
        if (suspect) {
            unlockedCondTopics = suspect->getUnlockedConditionalTopics(discoveredIds);
        }

        std::cout << "\n" << UI::Color::BOLD << "Questioning Options:\n" << UI::Color::RESET;
        std::cout << "  [1] Ask about Alibi & Whereabouts\n";
        std::cout << "  [2] Inquire about Motive & Suspicions\n";
        std::cout << "  [3] Explore Specific Dialogue Topic\n";

        int nextOptionNum = 4;
        int condTopicStartIdx = -1;
        if (!unlockedCondTopics.empty()) {
            condTopicStartIdx = nextOptionNum;
            for (size_t i = 0; i < unlockedCondTopics.size(); ++i) {
                std::cout << "  [" << (nextOptionNum++) << "] " << UI::Color::BRIGHT_CYAN 
                          << "[CLUE UNLOCKED] " << unlockedCondTopics[i].promptLabel << UI::Color::RESET << "\n";
            }
        }

        int confrontOption = -1;
        int swornTestimonyOption = -1;
        if (suspect) {
            confrontOption = nextOptionNum++;
            std::cout << "  [" << confrontOption << "] " << UI::Color::BRIGHT_YELLOW 
                      << "CONFRONT WITH CONTRADICTION (Present Evidence)" << UI::Color::RESET << "\n";
        } else if (witness) {
            swornTestimonyOption = nextOptionNum++;
            std::cout << "  [" << swornTestimonyOption << "] Request Complete Sworn Testimony\n";
        }

        int concludeOption = nextOptionNum;
        std::cout << "  [" << concludeOption << "] Conclude Interview\n\n";

        int qChoice = UI::promptInt("Choose dialogue action", 1, concludeOption);
        if (qChoice == concludeOption) break;

        if (qChoice == 1) {
            std::string resp = target->interact("alibi");
            std::vector<std::string> lines;
            lines.push_back(target->getName() + " responds:");
            lines.push_back("\"" + resp + "\"");
            UI::printBox("STATEMENT: ALIBI", lines, 74);
            m_notebook.addLead(target->getName() + " Alibi: " + resp);
            UI::pause();
        } else if (qChoice == 2) {
            std::string resp = target->interact("motive");
            std::vector<std::string> lines;
            lines.push_back(target->getName() + " responds:");
            lines.push_back("\"" + resp + "\"");
            UI::printBox("STATEMENT: MOTIVE / INTENT", lines, 74);
            UI::pause();
        } else if (qChoice == 3) {
            std::vector<std::string> topics;
            if (suspect) topics = suspect->getAvailableTopics();
            else if (witness) topics = witness->getAvailableTopics();

            std::cout << "\nAvailable inquiry topics:\n";
            for (size_t i = 0; i < topics.size(); ++i) {
                std::cout << "  [" << (i + 1) << "] " << topics[i] << "\n";
            }
            std::cout << "  [" << (topics.size() + 1) << "] Cancel\n\n";

            int tChoice = UI::promptInt("Select topic", 1, static_cast<int>(topics.size() + 1));
            if (tChoice <= static_cast<int>(topics.size())) {
                std::string chosenTopic = topics[tChoice - 1];
                std::string resp = target->interact(chosenTopic);
                std::vector<std::string> lines;
                lines.push_back(target->getName() + " regarding '" + chosenTopic + "':");
                lines.push_back("\"" + resp + "\"");
                UI::printBox("TOPIC RESPONSE", lines, 74);
                UI::pause();
            }
        } else if (condTopicStartIdx != -1 && qChoice >= condTopicStartIdx && qChoice < (condTopicStartIdx + static_cast<int>(unlockedCondTopics.size()))) {
            size_t idx = qChoice - condTopicStartIdx;
            const auto& ct = unlockedCondTopics[idx];
            if (suspect) suspect->recordInterview("Pressed on " + ct.promptLabel, ct.response);
            std::vector<std::string> lines;
            lines.push_back(target->getName() + " response to " + ct.promptLabel + ":");
            lines.push_back("\"" + ct.response + "\"");
            UI::printBox("SPECIAL INQUIRY RESPONSE", lines, 74);
            m_notebook.addLead(target->getName() + ": " + ct.response);
            UI::pause();
        } else if (qChoice == confrontOption && suspect) {
            UI::clearScreen();
            UI::printSubHeader("CONFRONTING " + suspect->getName() + " WITH EVIDENCE");
            m_activeCase->displayDiscoveredEvidenceList();

            std::string evId = UI::promptString("Enter Evidence ID that contradicts this suspect's claims (or 'cancel')");
            evId = UI::toUpper(evId);

            if (evId == "CANCEL" || evId.empty()) {
                continue;
            }

            std::string statementQuote, resolutionText;
            if (suspect->testContradiction(evId, statementQuote, resolutionText)) {
                UI::printSuccess("CONTRADICTION EXPOSED! The suspect's alibi shatters!");
                m_detective->addScore(50);

                std::vector<std::string> lines;
                lines.push_back("Disproven Statement:");
                lines.push_back("\"" + statementQuote + "\"");
                lines.push_back("");
                lines.push_back("Suspect Admission / Resolution:");
                lines.push_back(resolutionText);
                UI::printBox("CONTRADICTION SHATTERED", lines, 74);

                m_notebook.addLead(suspect->getName() + "'s statement shattered with " + evId);
                m_activeCase->updateObjectives();
            } else {
                int penalty = (m_difficulty == GameDifficulty::CHIEF_INSPECTOR) ? 30 : 15;
                m_detective->deductScore(penalty);
                m_investigationMistakes++;
                UI::printError("Contradiction failed! " + suspect->getName() + 
                              " scoffs: 'That proves nothing against me, Officer. Check your facts.'");
                UI::printWarning("Score penalty incurred: -" + std::to_string(penalty) + " points.");
            }
            UI::pause();
        } else if (qChoice == swornTestimonyOption && witness) {
            std::string test = witness->giveTestimony();
            std::vector<std::string> lines;
            lines.push_back("Sworn Testimony of " + witness->getName() + ":");
            lines.push_back("\"" + test + "\"");
            UI::printBox("EYEWITNESS TESTIMONY RECORD", lines, 74);
            m_notebook.addLead("Witness " + witness->getName() + " stated: " + test);

            std::string clueId = witness->getKeyClueEvidenceId();
            if (!clueId.empty() && m_activeCase->markEvidenceDiscovered(clueId)) {
                Evidence* ev = m_activeCase->getEvidenceById(clueId);
                UI::printSuccess("Witness statement logged as official clue: [" + clueId + "] " + (ev ? ev->getName() : ""));
            }
            UI::pause();
        }
    }
}

void GameEngine::handleClueBoard() {
    while (true) {
        UI::clearScreen();
        m_activeCase->getClueBoard().displayBoard();

        std::cout << "\nActions:\n";
        std::cout << "  [1] Connect Two Clues (Deduction Matrix)\n";
        std::cout << "  [2] View All Discovered Evidence\n";
        std::cout << "  [3] Return to Hub\n\n";

        int choice = UI::promptInt("Select action", 1, 3);
        if (choice == 3) break;

        if (choice == 1) {
            UI::clearScreen();
            m_activeCase->displayDiscoveredEvidenceList();

            std::cout << UI::Color::CYAN << "Select two Evidence IDs to connect with investigative thread (e.g. E01 and E03):\n" << UI::Color::RESET;
            std::string id1 = UI::toUpper(UI::promptString("First Evidence ID (or 'cancel')"));
            if (id1 == "CANCEL") continue;

            std::string id2 = UI::toUpper(UI::promptString("Second Evidence ID (or 'cancel')"));
            if (id2 == "CANCEL") continue;

            if (id1 == id2) {
                UI::printWarning("You must select two distinct pieces of evidence to form a connection.");
                UI::pause();
                continue;
            }

            std::string title, insight;
            int res = m_activeCase->getClueBoard().connectClues(id1, id2, title, insight);

            if (res == 1) {
                UI::printSuccess("DEDUCTIVE BREAKTHROUGH: " + title);
                std::vector<std::string> lines;
                lines.push_back("Clue Pair: " + id1 + " <---> " + id2);
                lines.push_back("");
                lines.push_back("Analytical Deduction:");
                lines.push_back(insight);
                UI::printBox("CASE DEDUCTION UNLOCKED", lines, 74);
                m_detective->addScore(35);
                m_notebook.addLead("Deduction [" + title + "]: " + insight);
                m_activeCase->updateObjectives();
            } else if (res == 0) {
                UI::printInfo("This link has already been established: " + title);
            } else {
                UI::printWarning("No logical deduction forms between " + id1 + " and " + id2 + ". Keep investigating.");
                m_detective->deductScore(5);
            }
            UI::pause();
        } else if (choice == 2) {
            UI::clearScreen();
            m_activeCase->displayDiscoveredEvidenceList();
            UI::pause();
        }
    }
}

void GameEngine::handleDeductionMatrix() {
    UI::clearScreen();
    m_activeCase->displayDeductionMatrix();
    UI::pause();
}

void GameEngine::handleNotebook() {
    while (true) {
        UI::clearScreen();
        m_notebook.displayNotebook();

        std::cout << "\nActions:\n";
        std::cout << "  [1] Write a New Field Note\n";
        std::cout << "  [2] Return to Hub\n\n";

        int choice = UI::promptInt("Select option", 1, 2);
        if (choice == 2) break;

        std::string note = UI::promptString("Enter your note (max 100 characters)");
        if (!note.empty()) {
            m_notebook.addNote(note);
            UI::printSuccess("Field note entered in leather notebook.");
            UI::pause();
        }
    }
}

void GameEngine::handleTranscripts() {
    UI::clearScreen();
    UI::printHeader("ARCHIVED INTERROGATION TRANSCRIPTS");

    const auto& characters = m_activeCase->getCharacters();
    std::cout << "Select Person of Interest to view transcript:\n";
    for (size_t i = 0; i < characters.size(); ++i) {
        std::cout << "  [" << (i + 1) << "] " << characters[i]->getName() << "\n";
    }
    std::cout << "  [" << (characters.size() + 1) << "] Return to Hub\n\n";

    int choice = UI::promptInt("Choose person", 1, static_cast<int>(characters.size() + 1));
    if (choice == static_cast<int>(characters.size() + 1)) return;

    auto target = characters[choice - 1];
    auto susp = std::dynamic_pointer_cast<Suspect>(target);
    if (susp) {
        UI::clearScreen();
        susp->displayTranscript();
    } else {
        UI::printInfo("Witness interviews are recorded directly in the official case leads.");
    }
    UI::pause();
}

void GameEngine::handleHintRequest() {
    UI::clearScreen();
    UI::printHeader("CONSULT SENIOR DETECTIVE (INVESTIGATIVE HINT)");

    if (m_difficulty == GameDifficulty::CHIEF_INSPECTOR) {
        UI::printWarning("Chief Inspector difficulty: Department policy forbids senior consultation.");
        UI::pause();
        return;
    }

    int cost = (m_difficulty == GameDifficulty::ROOKIE) ? 0 : 20;
    std::cout << "Requesting a hint will cost: " << UI::Color::BOLD 
              << cost << " Deduction Points." << UI::Color::RESET << "\n";

    if (!UI::promptConfirm("Do you wish to proceed with consultation?")) {
        return;
    }

    m_detective->incrementHints();
    m_detective->deductScore(cost);

    std::string hintText = "";
    int caseNum = m_activeCase->getCaseNumber();
    if (caseNum == 1) {
        if (!m_activeCase->getEvidenceById("E03")->isDiscovered()) {
            hintText = "Check the Security Terminal Server in the Basement. Electronic card timestamps reveal travel impossibilities.";
        } else {
            hintText = "Compare the Guard's claimed North Wing patrol time with the Electronic Access Log (E03). Can a person be in two places at once?";
        }
    } else if (caseNum == 2) {
        if (!m_activeCase->getEvidenceById("E02")->isDiscovered()) {
            hintText = "Inspect the sliding door runner track in Compartment 4-A. The killer dropped something during their rushed escape.";
        } else {
            hintText = "Conductor Morris noticed distinctive gold eagle cufflinks at 11:40 PM. Check which passenger is missing an eagle cufflink!";
        }
    } else if (caseNum == 3) {
        if (!m_activeCase->getEvidenceById("E02")->isDiscovered()) {
            hintText = "Examine the suicide note on Thaddeus's desk carefully. Look at the type of ink used in the signature.";
        } else {
            hintText = "The friction marks on the door bolt (E01) and the tension reel in the grandfather clock (E03) explain how the door was locked after the killer left.";
        }
    }

    UI::printHint(hintText);
    m_notebook.addNote("Senior Hint: " + hintText);
    UI::pause();
}

void GameEngine::handleAccusation() {
    UI::clearScreen();
    UI::printHeader("FINAL MAGISTRATE INDICTMENT");

    std::cout << UI::Color::BRIGHT_RED << UI::Color::BOLD
              << "WARNING: You are about to summon the High Magistrate and make a formal criminal accusation.\n"
              << "A premature or incorrect accusation will collapse the prosecution!\n\n" << UI::Color::RESET;

    if (!UI::promptConfirm("Are you prepared to submit your accusation now?")) {
        return;
    }

    UI::clearScreen();
    m_activeCase->displaySuspectsList();

    std::string accusedId = UI::toUpper(UI::promptString("Step 1: Enter Suspect ID of the Culprit (e.g., S01, S02)"));
    auto susp = m_activeCase->getSuspectById(accusedId);
    if (!susp) {
        UI::printError("Invalid Suspect ID. Indictment aborted.");
        UI::pause();
        return;
    }

    UI::clearScreen();
    m_activeCase->displayDiscoveredEvidenceList();

    std::string evId = UI::toUpper(UI::promptString("Step 2: Enter the Decisive Evidence ID establishing guilt"));
    Evidence* ev = m_activeCase->getEvidenceById(evId);
    if (!ev || !ev->isDiscovered()) {
        UI::printError("Evidence ID is invalid or not in your discovered inventory.");
        UI::pause();
        return;
    }

    std::cout << "\n" << UI::Color::CYAN << "Step 3: Select the Prime Criminal Motive:\n" << UI::Color::RESET;
    std::cout << "  [1] Financial Debt / Blackmail\n";
    std::cout << "  [2] Embezzlement Cover-Up\n";
    std::cout << "  [3] Disinheritance / Patent Theft\n";
    std::cout << "  [4] Jealousy / Personal Vendetta\n\n";

    int motChoice = UI::promptInt("Select motive category", 1, 4);
    std::string motiveWord = "";
    if (motChoice == 1) motiveWord = "DEBT";
    else if (motChoice == 2) motiveWord = "EMBEZZLEMENT";
    else if (motChoice == 3) motiveWord = "INHERITANCE";
    else motiveWord = "VENDETTA";

    AccusationResult result = m_activeCase->evaluateAccusation(accusedId, evId, motiveWord);
    showFinalReport(result);
    m_caseSolved = true;
}

void GameEngine::showFinalReport(const AccusationResult& result) {
    UI::clearScreen();
    UI::printHeader("MAGISTRATE'S BENCH - CASE ADJUDICATION");

    std::cout << UI::Color::BOLD << (result.isCorrectCulprit ? UI::Color::BRIGHT_GREEN : UI::Color::BRIGHT_RED)
              << "VERDICT: " << result.verdictTitle << UI::Color::RESET << "\n\n";

    std::vector<std::string> reportLines;
    reportLines.push_back("ADJUDICATION SUMMARY:");
    reportLines.push_back(result.outcomeDescription);
    reportLines.push_back("");
    for (const auto& line : result.reportDetails) {
        reportLines.push_back(line);
    }
    reportLines.push_back("");
    reportLines.push_back("--- CHRONOLOGICAL EVENT RECONSTRUCTION ---");
    for (const auto& ev : result.chronologicalTimeline) {
        reportLines.push_back("  [" + ev.time + "] " + ev.description);
    }
    reportLines.push_back("");
    reportLines.push_back("--- RESOLVED SUSPECT CONTRADICTIONS ---");
    if (result.resolvedContradictions.empty()) {
        reportLines.push_back("  (No suspect contradictions were formally exposed during this run.)");
    } else {
        for (const auto& cont : result.resolvedContradictions) {
            reportLines.push_back("  [*] " + cont);
        }
    }
    reportLines.push_back("");
    reportLines.push_back("--- SUSPECT ELIMINATION REPORT (WHY INNOCENTS WERE CLEARED) ---");
    for (const auto& elim : result.suspectEliminations) {
        reportLines.push_back("  [" + elim.suspectId + "] " + elim.suspectName + " -> " + elim.reason);
    }
    UI::printBox("OFFICIAL CRIMINAL CASE REPORT", reportLines, 74);

    // Score calculations
    m_detective->addScore(result.scoreAwarded);
    int finalScore = m_detective->getScore();
    std::string rank = calculateRank(finalScore);

    std::cout << "\n" << UI::Color::BOLD << UI::Color::WHITE << "FINAL DETECTIVE PERFORMANCE RATING:\n" << UI::Color::RESET;
    std::cout << "  Final Deduction Score: " << UI::Color::BRIGHT_YELLOW << finalScore << UI::Color::RESET << "\n";
    std::cout << "  Investigation Mistakes: " << m_investigationMistakes << "\n";
    std::cout << "  Hints Utilized:        " << m_detective->getHintsUsed() << "\n";
    std::cout << "  Department Rank:       " << UI::Color::BOLD << UI::Color::BRIGHT_CYAN << rank << UI::Color::RESET << "\n\n";

    UI::pause();
}

std::string GameEngine::calculateRank(int finalScore) const {
    if (finalScore >= 350) return "S - Master Sleuth of the Yard";
    if (finalScore >= 250) return "A - Senior Detective Inspector";
    if (finalScore >= 180) return "B - Competent Investigator";
    if (finalScore >= 100) return "C - Rookie Constable";
    return "F - Miscarriage of Justice (Reprimand)";
}

void GameEngine::handleSaveGame() {
    UI::clearScreen();
    UI::printHeader("SAVE INVESTIGATION STATE");

    std::string defaultPath = SaveManager::getDefaultSavePath();
    std::string path = UI::promptString("Enter save file path (press Enter for default '" + defaultPath + "')", true);
    if (path.empty()) path = defaultPath;

    if (SaveManager::saveGame(path, static_cast<int>(m_difficulty), *m_detective, *m_activeCase, m_notebook)) {
        UI::printSuccess("Investigation state safely saved to: " + path);
    } else {
        UI::printError("Failed to write save file to: " + path);
    }
    UI::pause();
}
