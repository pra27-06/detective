#include "Suspect.h"
#include "TerminalUI.h"
#include <iostream>

Suspect::Suspect(const std::string& id, const std::string& name,
                 const std::string& occupation, const std::string& personality,
                 const std::string& greeting, const std::string& alibi,
                 const std::string& motive, bool isGuilty,
                 const std::string& breakdownDialogue,
                 const std::string& confessionText,
                 const std::string& eliminationReason)
    : Character(id, name, occupation, personality, greeting),
      m_alibi(alibi), m_motive(motive), m_isGuilty(isGuilty),
      m_alibiBroken(false),
      m_breakdownDialogue(breakdownDialogue),
      m_confessionText(confessionText),
      m_eliminationReason(eliminationReason),
      m_currentBPM(72),
      m_composure(100) {
    // Default strategy
    m_psychProfile = std::make_unique<NervousDefensiveProfile>();
    m_currentBPM = m_psychProfile->getBaselineBPM();
}

std::string Suspect::getRole() const {
    return "Person of Interest / Suspect";
}

void Suspect::displayProfile() const {
    std::vector<std::string> lines;
    lines.push_back("Name:         " + m_name);
    lines.push_back("Occupation:   " + m_occupation);
    lines.push_back("Demeanor:     " + m_personality);
    lines.push_back("Stated Alibi: " + m_alibi);
    lines.push_back("Known Motive: " + m_motive);
    std::string alibiStr = m_alibiBroken ? "[ALIBI SHATTERED BY CONTRADICTION]" : "[ALIBI CURRENTLY STANDING]";
    lines.push_back("Alibi Status: " + alibiStr);
    lines.push_back("Contradictions Exposed: " + std::to_string(getResolvedContradictionCount()) + 
                    " / " + std::to_string(getTotalContradictionCount()));
    lines.push_back("");
    lines.push_back("Opening Words: \"" + m_greeting + "\"");
    UI::printBox("DOSSIER: " + m_id + " - " + m_name, lines, 74);
}

std::string Suspect::interact(const std::string& topic) {
    auto it = m_topicResponses.find(topic);
    if (it != m_topicResponses.end()) {
        recordInterview("Inquired about " + topic, it->second);
        return it->second;
    }
    if (topic == "alibi") {
        recordInterview("Inquired about alibi", m_alibi);
        return m_alibi;
    }
    if (topic == "motive") {
        std::string resp = "You suspect me? " + m_motive + "... but that does not make me a criminal!";
        recordInterview("Inquired about motive", resp);
        return resp;
    }
    std::string fallback = "I don't know what you're implying, Detective. I have nothing further to say about that.";
    recordInterview("Inquired about " + topic, fallback);
    return fallback;
}

void Suspect::addTopic(const std::string& topicKey, const std::string& response) {
    m_topicResponses[topicKey] = response;
}

std::vector<std::string> Suspect::getAvailableTopics() const {
    std::vector<std::string> topics;
    topics.push_back("alibi");
    topics.push_back("motive");
    for (const auto& pair : m_topicResponses) {
        if (pair.first != "alibi" && pair.first != "motive") {
            topics.push_back(pair.first);
        }
    }
    return topics;
}

std::string Suspect::getTopicResponse(const std::string& topicKey) const {
    auto it = m_topicResponses.find(topicKey);
    if (it != m_topicResponses.end()) {
        return it->second;
    }
    return "";
}

void Suspect::addConditionalTopic(const std::string& topicKey,
                                 const std::string& requiredEvidenceId,
                                 const std::string& promptLabel,
                                 const std::string& response) {
    ConditionalTopic ct;
    ct.topicKey = topicKey;
    ct.requiredEvidenceId = requiredEvidenceId;
    ct.promptLabel = promptLabel;
    ct.response = response;
    m_conditionalTopics.push_back(ct);
}

std::vector<ConditionalTopic> Suspect::getUnlockedConditionalTopics(const std::vector<std::string>& discoveredEvidenceIds) const {
    std::vector<ConditionalTopic> unlocked;
    for (const auto& ct : m_conditionalTopics) {
        for (const auto& evId : discoveredEvidenceIds) {
            if (evId == ct.requiredEvidenceId) {
                unlocked.push_back(ct);
                break;
            }
        }
    }
    return unlocked;
}

void Suspect::addContradiction(const std::string& statementTag,
                              const std::string& statementQuote,
                              const std::string& evidenceId,
                              const std::string& resolutionText) {
    Contradiction c;
    c.statementTag = statementTag;
    c.statementQuote = statementQuote;
    c.evidenceId = evidenceId;
    c.resolutionText = resolutionText;
    c.resolved = false;
    m_contradictions.push_back(c);
}

bool Suspect::testContradiction(const std::string& evidenceId, std::string& outStatement, std::string& outResolution) {
    for (auto& c : m_contradictions) {
        if (c.evidenceId == evidenceId && !c.resolved) {
            c.resolved = true;
            m_alibiBroken = true;
            outStatement = c.statementQuote;
            outResolution = c.resolutionText;
            recordInterview("CONFRONTED WITH " + evidenceId, outResolution);
            return true;
        }
    }
    return false;
}

size_t Suspect::getResolvedContradictionCount() const {
    size_t count = 0;
    for (const auto& c : m_contradictions) {
        if (c.resolved) count++;
    }
    return count;
}

size_t Suspect::getTotalContradictionCount() const {
    return m_contradictions.size();
}

const std::vector<Contradiction>& Suspect::getContradictions() const {
    return m_contradictions;
}

void Suspect::recordInterview(const std::string& inquiry, const std::string& response) {
    m_transcript.push_back({inquiry, response});
}

const std::vector<std::pair<std::string, std::string>>& Suspect::getTranscript() const {
    return m_transcript;
}

void Suspect::displayTranscript() const {
    std::vector<std::string> lines;
    lines.push_back("INTERROGATION LOG: " + m_name);
    lines.push_back("Recorded Exchanges: " + std::to_string(m_transcript.size()));
    lines.push_back("");
    if (m_transcript.empty()) {
        lines.push_back("  (No recorded questioning sessions yet.)");
    } else {
        for (size_t i = 0; i < m_transcript.size(); ++i) {
            lines.push_back("Q" + std::to_string(i + 1) + ": " + m_transcript[i].first);
            lines.push_back("A" + std::to_string(i + 1) + ": \"" + m_transcript[i].second + "\"");
            lines.push_back("");
        }
    }
    UI::printBox("TRANSCRIPT ARCHIVE", lines, 74);
}

bool Suspect::isAlibiBroken() const { return m_alibiBroken; }
void Suspect::setAlibiBroken(bool broken) { m_alibiBroken = broken; }
bool Suspect::isGuilty() const { return m_isGuilty; }
std::string Suspect::getAlibi() const { return m_alibi; }
std::string Suspect::getMotive() const { return m_motive; }
std::string Suspect::getBreakdownDialogue() const { return m_breakdownDialogue; }
std::string Suspect::getConfessionText() const { return m_confessionText; }
std::string Suspect::getEliminationReason() const { return m_eliminationReason; }

void Suspect::setPsychologicalProfile(std::unique_ptr<PsychologicalProfile> profile) {
    if (profile) {
        m_psychProfile = std::move(profile);
        m_currentBPM = m_psychProfile->getBaselineBPM();
    }
}

std::string Suspect::getCurrentMicroExpression() const {
    if (m_psychProfile) {
        return m_psychProfile->getMicroExpression(m_currentBPM);
    }
    return "Neutral expression.";
}

void Suspect::applyInterrogationPressure(const std::string& evidenceCategory, bool isContradiction) {
    if (!m_psychProfile) return;
    int spike = m_psychProfile->calculateStressSpike(evidenceCategory, isContradiction);
    m_currentBPM += spike;
    m_composure = std::max(0, m_composure - (isContradiction ? 35 : 12));

    if (m_psychProfile->evaluateBreakdown(m_currentBPM, m_composure)) {
        m_alibiBroken = true;
    }
}

bool Suspect::isPsychologicallyBroken() const {
    if (!m_psychProfile) return m_alibiBroken;
    return m_alibiBroken || m_psychProfile->evaluateBreakdown(m_currentBPM, m_composure);
}
