#include "ClueBoard.h"
#include "TerminalUI.h"
#include <iostream>

ClueBoard::ClueBoard() {}

void ClueBoard::addPinnedEvidence(const std::string& evidenceId) {
    if (!evidenceId.empty()) {
        m_pinnedEvidenceIds.insert(evidenceId);
    }
}

bool ClueBoard::isEvidencePinned(const std::string& evidenceId) const {
    return m_pinnedEvidenceIds.find(evidenceId) != m_pinnedEvidenceIds.end();
}

const std::set<std::string>& ClueBoard::getPinnedEvidenceIds() const {
    return m_pinnedEvidenceIds;
}

void ClueBoard::registerValidDeduction(const std::string& id1, const std::string& id2,
                                      const std::string& title, const std::string& insight) {
    Deduction d;
    d.id1 = id1;
    d.id2 = id2;
    d.title = title;
    d.insight = insight;
    d.discovered = false;
    m_availableDeductions.push_back(d);
}

int ClueBoard::connectClues(const std::string& id1, const std::string& id2, 
                           std::string& outTitle, std::string& outInsight) {
    // Check if valid deduction exists (order agnostic)
    for (auto& ded : m_availableDeductions) {
        if ((ded.id1 == id1 && ded.id2 == id2) || (ded.id1 == id2 && ded.id2 == id1)) {
            if (ded.discovered) {
                outTitle = ded.title;
                outInsight = ded.insight;
                return 0; // already connected
            }
            ded.discovered = true;
            m_activeConnections.push_back({id1, id2});
            outTitle = ded.title;
            outInsight = ded.insight;
            return 1; // new breakthrough!
        }
    }
    return -1; // no logical connection
}

void ClueBoard::displayBoard() const {
    std::vector<std::string> lines;
    lines.push_back("Pinned Evidence Files: " + std::to_string(m_pinnedEvidenceIds.size()));
    std::string pins = "Pinned IDs: ";
    for (const auto& id : m_pinnedEvidenceIds) {
        pins += "[" + id + "] ";
    }
    lines.push_back(pins);
    lines.push_back("");
    lines.push_back("Confirmed Connections & Deductions (" + 
                    std::to_string(getUnlockedDeductionCount()) + " / " + 
                    std::to_string(m_availableDeductions.size()) + "):");

    size_t count = 0;
    for (const auto& d : m_availableDeductions) {
        if (d.discovered) {
            count++;
            lines.push_back("  (" + d.id1 + " <---> " + d.id2 + ") :: " + d.title);
            lines.push_back("    -> " + d.insight);
        }
    }
    if (count == 0) {
        lines.push_back("  [No links formed yet. Select two clues to examine their relationship.]");
    }

    UI::printBox("DETECTIVE'S CORKBOARD & CLUE MATRIX", lines, 74);
}

size_t ClueBoard::getUnlockedDeductionCount() const {
    size_t c = 0;
    for (const auto& d : m_availableDeductions) {
        if (d.discovered) c++;
    }
    return c;
}

size_t ClueBoard::getTotalDeductionsCount() const {
    return m_availableDeductions.size();
}

const std::vector<Deduction>& ClueBoard::getDeductions() const {
    return m_availableDeductions;
}

void ClueBoard::unlockDeductionByIndex(size_t index) {
    if (index < m_availableDeductions.size()) {
        m_availableDeductions[index].discovered = true;
        m_activeConnections.push_back({m_availableDeductions[index].id1, m_availableDeductions[index].id2});
    }
}
