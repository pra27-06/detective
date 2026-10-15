#include "Evidence.h"
#include "TerminalUI.h"
#include <iostream>
#include <sstream>
#include <vector>

Evidence::Evidence() 
    : m_id(""), m_name(""), m_category(EvidenceCategory::PHYSICAL),
      m_relevance(EvidenceRelevance::CIRCUMSTANTIAL_LEAD),
      m_description(""), m_locationFound(""), m_isKeyEvidence(false),
      m_isRedHerring(false), m_isDiscovered(false) {}

Evidence::Evidence(const std::string& id, const std::string& name, EvidenceCategory category,
                   EvidenceRelevance relevance,
                   const std::string& description, const std::string& locationFound,
                   bool isKey, bool isRedHerring)
    : m_id(id), m_name(name), m_category(category), m_relevance(relevance),
      m_description(description), m_locationFound(locationFound),
      m_isKeyEvidence(isKey), m_isRedHerring(isRedHerring), m_isDiscovered(false) {}

std::string Evidence::getId() const { return m_id; }
std::string Evidence::getName() const { return m_name; }
EvidenceCategory Evidence::getCategory() const { return m_category; }
EvidenceRelevance Evidence::getRelevance() const { return m_relevance; }

std::string Evidence::getCategoryName() const {
    switch (m_category) {
        case EvidenceCategory::PHYSICAL:  return "Physical Clue";
        case EvidenceCategory::FORENSIC:  return "Forensic Clue";
        case EvidenceCategory::DOCUMENT:  return "Documentary Clue";
        case EvidenceCategory::TESTIMONY: return "Testimonial Clue";
        default:                          return "General Clue";
    }
}

std::string Evidence::getRelevanceName() const {
    switch (m_relevance) {
        case EvidenceRelevance::CRITICAL_KEY:        return "CRITICAL INDICTMENT EVIDENCE";
        case EvidenceRelevance::SUPPORTING_PROOF:    return "CORROBORATING PROOF";
        case EvidenceRelevance::CIRCUMSTANTIAL_LEAD: return "CIRCUMSTANTIAL LEAD";
        case EvidenceRelevance::DECEPTIVE_RED_HERRING: return "PLAUSIBLE RED HERRING";
        default:                                     return "INVESTIGATIVE DATA";
    }
}

std::string Evidence::getDescription() const { return m_description; }
std::string Evidence::getLocationFound() const { return m_locationFound; }
bool Evidence::isKeyEvidence() const { return m_isKeyEvidence; }
bool Evidence::isRedHerring() const { return m_isRedHerring; }
bool Evidence::isDiscovered() const { return m_isDiscovered; }

void Evidence::setDiscovered(bool discovered) {
    m_isDiscovered = discovered;
}

void Evidence::displaySummary() const {
    std::string tag = "[LEAD]";
    if (m_isKeyEvidence) tag = "[KEY PROOF]";
    else if (m_isRedHerring) tag = "[QUESTIONABLE]";

    std::cout << UI::Color::BRIGHT_YELLOW << "[" << m_id << "] " << UI::Color::RESET
              << UI::Color::BOLD << m_name << UI::Color::RESET << "  "
              << UI::Color::CYAN << tag << UI::Color::RESET << "\n"
              << UI::Color::DIM << "     Category: " << getCategoryName() 
              << " | Scene: " << m_locationFound << UI::Color::RESET << "\n";
}

void Evidence::displayCard() const {
    std::vector<std::string> lines;
    lines.push_back("Clue Name:   " + m_name);
    lines.push_back("Category:    " + getCategoryName());
    lines.push_back("Relevance:   " + getRelevanceName());
    lines.push_back("Origin:      " + m_locationFound);
    lines.push_back("");
    lines.push_back("Forensic Details:");
    lines.push_back(m_description);
    if (m_isKeyEvidence) {
        lines.push_back("");
        lines.push_back("[NOTICE] Crucial physical link required for formal court indictment.");
    } else if (m_isRedHerring) {
        lines.push_back("");
        lines.push_back("[CAUTION] Examine carefully: circumstantial appearances can deceive.");
    }
    UI::printBox("EVIDENCE DOSSIER: " + m_id, lines, 74);
}

std::string Evidence::serialize() const {
    return m_id + "|" + (m_isDiscovered ? "1" : "0");
}

Evidence Evidence::deserialize(const std::string& line) {
    Evidence e;
    size_t pipe = line.find('|');
    if (pipe != std::string::npos) {
        e.m_id = line.substr(0, pipe);
        std::string discStr = line.substr(pipe + 1);
        e.m_isDiscovered = (discStr == "1");
    }
    return e;
}
