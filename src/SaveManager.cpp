#include "SaveManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string SaveManager::getDefaultSavePath() {
    return "saves/case_save.dat";
}

bool SaveManager::hasSaveFile(const std::string& filepath) {
    std::ifstream file(filepath);
    return file.good();
}

bool SaveManager::saveGame(const std::string& filepath,
                           int difficulty,
                           const Detective& detective,
                           const CaseFile& caseFile,
                           const Notebook& notebook) {
    std::ofstream out(filepath);
    if (!out.is_open()) {
        return false;
    }

    out << "[HEADER]\n";
    out << "CASE=" << caseFile.getCaseNumber() << "\n";
    out << "DIFFICULTY=" << difficulty << "\n";
    out << "NAME=" << detective.getName() << "\n";
    out << "SCORE=" << detective.getScore() << "\n";
    out << "ENERGY=" << detective.getEnergy() << "\n";
    out << "HINTS=" << detective.getHintsUsed() << "\n";

    out << "[EVIDENCE]\n";
    for (const auto& ev : caseFile.getDiscoveredEvidence()) {
        out << ev.getId() << "\n";
    }

    out << "[BROKEN_ALIBIS]\n";
    for (const auto& c : caseFile.getCharacters()) {
        auto s = std::dynamic_pointer_cast<Suspect>(c);
        if (s && s->isAlibiBroken()) {
            out << s->getId() << "\n";
        }
    }

    out << "[EXAMINED_HOTSPOTS]\n";
    for (const auto& sc : caseFile.getScenes()) {
        for (size_t i = 0; i < sc.getHotspotCount(); ++i) {
            if (sc.isHotspotExamined(i)) {
                out << sc.getId() << ":" << i << "\n";
            }
        }
    }

    out << "[DEDUCTIONS]\n";
    const auto& deds = caseFile.getClueBoard().getDeductions();
    for (size_t i = 0; i < deds.size(); ++i) {
        if (deds[i].discovered) {
            out << i << "\n";
        }
    }

    out << "[NOTES]\n";
    for (const auto& note : notebook.getNotes()) {
        out << note << "\n";
    }

    out << "[LEADS]\n";
    for (const auto& lead : notebook.getLeads()) {
        out << lead << "\n";
    }

    out << "[END]\n";
    out.close();
    return true;
}

bool SaveManager::loadGame(const std::string& filepath, GameSaveState& outState) {
    std::ifstream in(filepath);
    if (!in.is_open()) {
        return false;
    }

    outState = GameSaveState();
    outState.caseNumber = 1;
    outState.difficulty = 1;
    outState.detectiveName = "Detective";
    outState.detectiveScore = 100;
    outState.detectiveEnergy = 100;
    outState.hintsUsed = 0;

    std::string line;
    std::string currentSection = "";

    while (std::getline(in, line)) {
        if (line.empty()) continue;
        if (line.front() == '[' && line.back() == ']') {
            currentSection = line;
            continue;
        }

        if (currentSection == "[HEADER]") {
            size_t eq = line.find('=');
            if (eq != std::string::npos) {
                std::string key = line.substr(0, eq);
                std::string val = line.substr(eq + 1);
                try {
                    if (key == "CASE") {
                        int c = std::stoi(val);
                        if (c < 1 || c > 3) return false;
                        outState.caseNumber = c;
                    } else if (key == "DIFFICULTY") {
                        int d = std::stoi(val);
                        if (d < 1 || d > 3) return false;
                        outState.difficulty = d;
                    } else if (key == "NAME") {
                        if (val.empty()) return false;
                        outState.detectiveName = val;
                    } else if (key == "SCORE") {
                        int s = std::stoi(val);
                        outState.detectiveScore = std::max(0, s);
                    } else if (key == "ENERGY") {
                        int e = std::stoi(val);
                        outState.detectiveEnergy = std::max(0, std::min(100, e));
                    } else if (key == "HINTS") {
                        outState.hintsUsed = std::max(0, std::stoi(val));
                    }
                } catch (const std::exception&) {
                    // Corrupt or malformed header value encountered
                    return false;
                }
            }
        } else if (currentSection == "[EVIDENCE]") {
            outState.discoveredEvidenceIds.push_back(line);
        } else if (currentSection == "[BROKEN_ALIBIS]") {
            outState.brokenAlibiSuspectIds.push_back(line);
        } else if (currentSection == "[EXAMINED_HOTSPOTS]") {
            outState.examinedHotspots.push_back(line);
        } else if (currentSection == "[DEDUCTIONS]") {
            try {
                outState.unlockedDeductionIndices.push_back(std::stoi(line));
            } catch (...) {}
        } else if (currentSection == "[NOTES]") {
            outState.customNotes.push_back(line);
        } else if (currentSection == "[LEADS]") {
            outState.recordedLeads.push_back(line);
        }
    }

    in.close();
    if (outState.caseNumber < 1 || outState.caseNumber > 3) {
        return false;
    }
    return true;
}
