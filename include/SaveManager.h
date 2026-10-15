#ifndef SAVE_MANAGER_H
#define SAVE_MANAGER_H

#include <string>
#include <vector>
#include <memory>
#include "CaseFile.h"
#include "Detective.h"
#include "Notebook.h"

struct GameSaveState {
    int caseNumber;
    int difficulty;
    std::string detectiveName;
    int detectiveScore;
    int detectiveEnergy;
    int hintsUsed;
    std::vector<std::string> discoveredEvidenceIds;
    std::vector<std::string> brokenAlibiSuspectIds;
    std::vector<std::string> examinedHotspots; // "SCENE_ID:INDEX"
    std::vector<int> unlockedDeductionIndices;
    std::vector<std::string> customNotes;
    std::vector<std::string> recordedLeads;
};

class SaveManager {
public:
    static bool saveGame(const std::string& filepath,
                         int difficulty,
                         const Detective& detective,
                         const CaseFile& caseFile,
                         const Notebook& notebook);

    static bool loadGame(const std::string& filepath,
                         GameSaveState& outState);

    static bool hasSaveFile(const std::string& filepath);
    static std::string getDefaultSavePath();
};

#endif // SAVE_MANAGER_H
