#include "Notebook.h"
#include "TerminalUI.h"
#include <iostream>

Notebook::Notebook() {}

void Notebook::addNote(const std::string& note) {
    if (!note.empty()) {
        m_caseNotes.push_back(note);
    }
}

void Notebook::addLead(const std::string& lead) {
    if (!lead.empty()) {
        // avoid exact duplicates
        for (const auto& l : m_recordedLeads) {
            if (l == lead) return;
        }
        m_recordedLeads.push_back(lead);
    }
}

const std::vector<std::string>& Notebook::getNotes() const { return m_caseNotes; }
const std::vector<std::string>& Notebook::getLeads() const { return m_recordedLeads; }

void Notebook::displayNotebook() const {
    std::vector<std::string> lines;
    lines.push_back("--- OFFICIAL CASE LEADS & VERIFIED FACTS ---");
    if (m_recordedLeads.empty()) {
        lines.push_back("  (No confirmed leads established yet.)");
    } else {
        for (size_t i = 0; i < m_recordedLeads.size(); ++i) {
            lines.push_back("  [*] " + m_recordedLeads[i]);
        }
    }
    lines.push_back("");
    lines.push_back("--- DETECTIVE'S FIELD NOTES ---");
    if (m_caseNotes.empty()) {
        lines.push_back("  (No custom notes recorded yet. Use 'Add Note' to record your thoughts.)");
    } else {
        for (size_t i = 0; i < m_caseNotes.size(); ++i) {
            lines.push_back("  [" + std::to_string(i + 1) + "] " + m_caseNotes[i]);
        }
    }
    UI::printBox("LEATHER NOTEBOOK", lines, 74);
}

void Notebook::clear() {
    m_caseNotes.clear();
    m_recordedLeads.clear();
}

std::string Notebook::serializeNotes() const {
    std::string res;
    for (size_t i = 0; i < m_caseNotes.size(); ++i) {
        res += m_caseNotes[i];
        if (i + 1 < m_caseNotes.size()) res += "%%";
    }
    return res;
}

std::string Notebook::serializeLeads() const {
    std::string res;
    for (size_t i = 0; i < m_recordedLeads.size(); ++i) {
        res += m_recordedLeads[i];
        if (i + 1 < m_recordedLeads.size()) res += "%%";
    }
    return res;
}

void Notebook::loadNotes(const std::vector<std::string>& notes) {
    m_caseNotes = notes;
}

void Notebook::loadLeads(const std::vector<std::string>& leads) {
    m_recordedLeads = leads;
}
