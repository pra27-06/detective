#ifndef EVIDENCE_H
#define EVIDENCE_H

#include <string>

enum class EvidenceCategory {
    PHYSICAL,
    FORENSIC,
    DOCUMENT,
    TESTIMONY
};

enum class EvidenceRelevance {
    CRITICAL_KEY,
    SUPPORTING_PROOF,
    CIRCUMSTANTIAL_LEAD,
    DECEPTIVE_RED_HERRING
};

class Evidence {
private:
    std::string m_id;
    std::string m_name;
    EvidenceCategory m_category;
    EvidenceRelevance m_relevance;
    std::string m_description;
    std::string m_locationFound;
    bool m_isKeyEvidence;
    bool m_isRedHerring;
    bool m_isDiscovered;

public:
    Evidence();
    Evidence(const std::string& id, const std::string& name, EvidenceCategory category,
             EvidenceRelevance relevance,
             const std::string& description, const std::string& locationFound,
             bool isKey = false, bool isRedHerring = false);

    // Getters
    std::string getId() const;
    std::string getName() const;
    EvidenceCategory getCategory() const;
    EvidenceRelevance getRelevance() const;
    std::string getCategoryName() const;
    std::string getRelevanceName() const;
    std::string getDescription() const;
    std::string getLocationFound() const;
    bool isKeyEvidence() const;
    bool isRedHerring() const;
    bool isDiscovered() const;

    // Mutators
    void setDiscovered(bool discovered);

    // Presentation
    void displayCard() const;
    void displaySummary() const;

    // Operator Overloading
    bool operator==(const Evidence& other) const;
    bool operator!=(const Evidence& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Evidence& ev);

    // String serialization for Save/Load
    std::string serialize() const;
    static Evidence deserialize(const std::string& line);
};

#endif // EVIDENCE_H
