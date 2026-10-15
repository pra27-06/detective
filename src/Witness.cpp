#include "Witness.h"
#include "TerminalUI.h"
#include <iostream>
#include <vector>

Witness::Witness(const std::string& id, const std::string& name,
                 const std::string& occupation, const std::string& personality,
                 const std::string& greeting, const std::string& testimony,
                 const std::string& observedTime, const std::string& keyClueEvidenceId)
    : Character(id, name, occupation, personality, greeting),
      m_testimony(testimony), m_observedTime(observedTime),
      m_keyClueEvidenceId(keyClueEvidenceId), m_hasGivenTestimony(false) {}

std::string Witness::getRole() const {
    return "Eye / Ear Witness";
}

void Witness::displayProfile() const {
    std::vector<std::string> lines;
    lines.push_back("Name:         " + m_name);
    lines.push_back("Occupation:   " + m_occupation);
    lines.push_back("Demeanor:     " + m_personality);
    lines.push_back("Time Noticed: " + m_observedTime);
    lines.push_back("Status:       " + std::string(m_hasGivenTestimony ? "[INTERVIEWED - TESTIMONY ON FILE]" : "[AWAITING INTERVIEW]"));
    lines.push_back("");
    lines.push_back("Statement Preview: \"" + m_greeting + "\"");
    UI::printBox("WITNESS LOG: " + m_id + " - " + m_name, lines, 74);
}

std::string Witness::interact(const std::string& topic) {
    auto it = m_topicResponses.find(topic);
    if (it != m_topicResponses.end()) {
        return it->second;
    }
    if (topic == "testimony") {
        return giveTestimony();
    }
    return "I only know what I saw around " + m_observedTime + ", Officer. Ask me about what I saw.";
}

void Witness::addTopic(const std::string& topicKey, const std::string& response) {
    m_topicResponses[topicKey] = response;
}

std::vector<std::string> Witness::getAvailableTopics() const {
    std::vector<std::string> topics;
    topics.push_back("testimony");
    for (const auto& pair : m_topicResponses) {
        if (pair.first != "testimony") {
            topics.push_back(pair.first);
        }
    }
    return topics;
}

std::string Witness::giveTestimony() {
    m_hasGivenTestimony = true;
    return m_testimony;
}

bool Witness::hasGivenTestimony() const { return m_hasGivenTestimony; }
void Witness::setHasGivenTestimony(bool val) { m_hasGivenTestimony = val; }
std::string Witness::getKeyClueEvidenceId() const { return m_keyClueEvidenceId; }
std::string Witness::getObservedTime() const { return m_observedTime; }
