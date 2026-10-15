#ifndef SUSPECT_H
#define SUSPECT_H

#include "Character.h"
#include <map>
#include <vector>
#include <string>

struct Contradiction {
    std::string statementTag;
    std::string statementQuote;
    std::string evidenceId;
    std::string resolutionText;
    bool resolved;
};

struct ConditionalTopic {
    std::string topicKey;
    std::string requiredEvidenceId;
    std::string promptLabel;
    std::string response;
};

class Suspect : public Character {
private:
    std::string m_alibi;
    std::string m_motive;
    bool m_isGuilty;
    bool m_alibiBroken;
    std::string m_breakdownDialogue;
    std::string m_confessionText;
    std::string m_eliminationReason; // Why this suspect is cleared if innocent

    std::map<std::string, std::string> m_topicResponses;
    std::vector<Contradiction> m_contradictions;
    std::vector<ConditionalTopic> m_conditionalTopics;
    std::vector<std::pair<std::string, std::string>> m_transcript; // (Question, Answer)

public:
    Suspect(const std::string& id, const std::string& name,
            const std::string& occupation, const std::string& personality,
            const std::string& greeting, const std::string& alibi,
            const std::string& motive, bool isGuilty,
            const std::string& breakdownDialogue,
            const std::string& confessionText,
            const std::string& eliminationReason = "");

    // Overridden Virtual Methods (Polymorphism)
    std::string getRole() const override;
    void displayProfile() const override;
    std::string interact(const std::string& topic) override;

    // Dialogue & Interrogation
    void addTopic(const std::string& topicKey, const std::string& response);
    std::vector<std::string> getAvailableTopics() const;
    std::string getTopicResponse(const std::string& topicKey) const;

    void addConditionalTopic(const std::string& topicKey,
                             const std::string& requiredEvidenceId,
                             const std::string& promptLabel,
                             const std::string& response);
    std::vector<ConditionalTopic> getUnlockedConditionalTopics(const std::vector<std::string>& discoveredEvidenceIds) const;

    // Contradictions
    void addContradiction(const std::string& statementTag,
                          const std::string& statementQuote,
                          const std::string& evidenceId,
                          const std::string& resolutionText);
    bool testContradiction(const std::string& evidenceId, std::string& outStatement, std::string& outResolution);
    size_t getResolvedContradictionCount() const;
    size_t getTotalContradictionCount() const;
    const std::vector<Contradiction>& getContradictions() const;

    // Transcript
    void recordInterview(const std::string& inquiry, const std::string& response);
    const std::vector<std::pair<std::string, std::string>>& getTranscript() const;
    void displayTranscript() const;

    // Getters & Mutators
    bool isAlibiBroken() const;
    void setAlibiBroken(bool broken);
    bool isGuilty() const;
    std::string getAlibi() const;
    std::string getMotive() const;
    std::string getBreakdownDialogue() const;
    std::string getConfessionText() const;
    std::string getEliminationReason() const;
};

#endif // SUSPECT_H
