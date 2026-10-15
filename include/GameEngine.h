#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <memory>
#include <string>
#include "Detective.h"
#include "CaseFile.h"
#include "Notebook.h"

enum class GameDifficulty {
    ROOKIE = 1,
    SENIOR = 2,
    CHIEF_INSPECTOR = 3
};

class GameEngine {
private:
    std::shared_ptr<Detective> m_detective;
    std::unique_ptr<CaseFile> m_activeCase;
    Notebook m_notebook;
    GameDifficulty m_difficulty;
    bool m_isRunning;
    bool m_caseSolved;
    int m_investigationMistakes;

    // Sub-menus and state controllers
    void showMainMenu();
    void startNewGame();
    void continueGame();
    void browseCaseFiles();
    void showHowToPlay();
    void showCredits();
    void toggleColorMode();

    void runCaseLoop();
    void handleInvestigation();
    void handleInterrogation();
    void handleClueBoard();
    void handleDeductionMatrix();
    void handleNotebook();
    void handleTranscripts();
    void handleHintRequest();
    void handleAccusation();
    void handleSaveGame();

    void displayHubHeader() const;
    void showFinalReport(const AccusationResult& result);
    std::string calculateRank(int finalScore) const;
    std::string getDifficultyName() const;

public:
    GameEngine();
    void run();
};

#endif // GAME_ENGINE_H
