#include "Detective.h"
#include "TerminalUI.h"
#include <iostream>
#include <vector>

Detective::Detective(const std::string& id, const std::string& name,
                     const std::string& badgeNumber, const std::string& rank)
    : Character(id, name, "Investigator", "Analytical & Methodical", "Ready to examine the facts."),
      m_badgeNumber(badgeNumber), m_rank(rank), m_score(100), m_energy(100), m_maxEnergy(100), m_hintsUsed(0) {}

std::string Detective::getRole() const {
    return "Lead Detective (" + m_rank + ")";
}

void Detective::displayProfile() const {
    std::vector<std::string> lines;
    lines.push_back("Officer:      " + m_name);
    lines.push_back("Rank:         " + m_rank);
    lines.push_back("Badge #:      " + m_badgeNumber);
    lines.push_back("Energy:       " + std::to_string(m_energy) + " / " + std::to_string(m_maxEnergy));
    lines.push_back("Current Score:" + std::to_string(m_score));
    lines.push_back("Hints Used:   " + std::to_string(m_hintsUsed));
    lines.push_back("");
    lines.push_back("Motto: 'Eliminate all other factors, and the one which remains must be the truth.'");
    UI::printBox("OFFICIAL BADGE - " + m_name, lines, 74);
}

std::string Detective::interact(const std::string& topic) {
    if (topic == "self") {
        return "I need to stay objective and weigh facts, not assumptions.";
    }
    return "Focusing my analysis on " + topic + "... I must look for logical inconsistencies.";
}

int Detective::getScore() const { return m_score; }

void Detective::addScore(int delta) {
    m_score += delta;
}

void Detective::deductScore(int delta) {
    m_score -= delta;
    if (m_score < 0) m_score = 0;
}

int Detective::getEnergy() const { return m_energy; }
int Detective::getMaxEnergy() const { return m_maxEnergy; }

bool Detective::spendEnergy(int cost) {
    if (m_energy >= cost) {
        m_energy -= cost;
        return true;
    }
    return false;
}

void Detective::restoreEnergy(int amount) {
    m_energy += amount;
    if (m_energy > m_maxEnergy) m_energy = m_maxEnergy;
}

int Detective::getHintsUsed() const { return m_hintsUsed; }

void Detective::incrementHints() {
    m_hintsUsed++;
}

void Detective::setScore(int score) { m_score = score; }
void Detective::setEnergy(int energy) { m_energy = energy; }
void Detective::setHintsUsed(int hints) { m_hintsUsed = hints; }
