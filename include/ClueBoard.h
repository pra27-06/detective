#ifndef CLUE_BOARD_H
#define CLUE_BOARD_H

#include <string>
#include <vector>
#include <set>
#include <map>

struct Deduction {
    std::string id1;
    std::string id2;
    std::string title;
    std::string insight;
    bool discovered;
};

class ClueBoard {
private:
    std::set<std::string> m_pinnedEvidenceIds;
    std::vector<std::pair<std::string, std::string>> m_activeConnections;
    std::vector<Deduction> m_availableDeductions;

public:
    ClueBoard();

    void addPinnedEvidence(const std::string& evidenceId);
    bool isEvidencePinned(const std::string& evidenceId) const;
    const std::set<std::string>& getPinnedEvidenceIds() const;

    void registerValidDeduction(const std::string& id1, const std::string& id2,
                                const std::string& title, const std::string& insight);

    // Attempt to connect two clues
    // Returns: 1 if successful new deduction, 0 if already connected, -1 if no link
    int connectClues(const std::string& id1, const std::string& id2, std::string& outTitle, std::string& outInsight);

    void displayBoard() const;
    size_t getUnlockedDeductionCount() const;
    size_t getTotalDeductionsCount() const;
    const std::vector<Deduction>& getDeductions() const;
    void unlockDeductionByIndex(size_t index);
};

#endif // CLUE_BOARD_H
