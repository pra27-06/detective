#ifndef WITNESS_H
#define WITNESS_H

#include "Character.h"
#include <map>

class Witness : public Character {
private:
    std::string m_testimony;
    std::string m_observedTime;
    std::string m_keyClueEvidenceId;
    bool m_hasGivenTestimony;
    std::map<std::string, std::string> m_topicResponses;

public:
    Witness(const std::string& id, const std::string& name,
            const std::string& occupation, const std::string& personality,
            const std::string& greeting, const std::string& testimony,
            const std::string& observedTime, const std::string& keyClueEvidenceId);

    // Overridden Virtual Methods (Polymorphism)
    std::string getRole() const override;
    void displayProfile() const override;
    std::string interact(const std::string& topic) override;

    // Witness Specific Methods
    void addTopic(const std::string& topicKey, const std::string& response);
    std::vector<std::string> getAvailableTopics() const;
    std::string giveTestimony();
    bool hasGivenTestimony() const;
    void setHasGivenTestimony(bool val);
    std::string getKeyClueEvidenceId() const;
    std::string getObservedTime() const;
};

#endif // WITNESS_H
