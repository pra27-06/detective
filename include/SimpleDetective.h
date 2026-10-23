#ifndef SIMPLE_DETECTIVE_H
#define SIMPLE_DETECTIVE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <iomanip>

// ============================================================================
// OOP CONCEPT 1: ABSTRACTION (Pure Virtual Interface)
// ============================================================================
// IInspectable defines an abstract contract for any entity in the game world
// that the detective can inspect or examine.
class IInspectable {
public:
    virtual ~IInspectable() = default;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual void inspect() const = 0;
};

// ============================================================================
// OOP CONCEPT 2: INHERITANCE & POLYMORPHISM (Evidence Hierarchy)
// ============================================================================
enum class EvidenceType {
    PHYSICAL,
    DOCUMENT,
    CHEMICAL
};

class Evidence : public IInspectable {
protected:
    std::string m_id;
    std::string m_name;
    std::string m_description;
    std::string m_locationFound;
    EvidenceType m_type;
    bool m_isDiscovered;

public:
    Evidence(const std::string& id, const std::string& name,
             const std::string& description, const std::string& location,
             EvidenceType type)
        : m_id(id), m_name(name), m_description(description),
          m_locationFound(location), m_type(type), m_isDiscovered(false) {}

    virtual ~Evidence() = default;

    // Encapsulation (Getters and Setters)
    std::string getId() const { return m_id; }
    std::string getName() const override { return m_name; }
    std::string getDescription() const override { return m_description; }
    std::string getLocation() const { return m_locationFound; }
    EvidenceType getType() const { return m_type; }
    bool isDiscovered() const { return m_isDiscovered; }
    void setDiscovered(bool disc) { m_isDiscovered = disc; }

    // Polymorphic display method
    void inspect() const override {
        std::cout << "\n+-------------------------------------------------------------+\n";
        std::cout << "| EVIDENCE CARD: [" << m_id << "] " << m_name << "\n";
        std::cout << "+-------------------------------------------------------------+\n";
        std::cout << "  Location Found : " << m_locationFound << "\n";
        std::cout << "  Classification : " << getTypeName() << "\n";
        std::cout << "  Description    : " << m_description << "\n";
        displaySpecialDetail();
        std::cout << "+-------------------------------------------------------------+\n";
    }

    virtual void displaySpecialDetail() const = 0; // Pure virtual for derived classes

    std::string getTypeName() const {
        switch (m_type) {
            case EvidenceType::PHYSICAL: return "Physical Artifact";
            case EvidenceType::DOCUMENT: return "Documentary Proof";
            case EvidenceType::CHEMICAL: return "Chemical / Forensic";
        }
        return "Unknown";
    }

    // Operator Overloading for easy streaming
    friend std::ostream& operator<<(std::ostream& os, const Evidence& ev) {
        os << "[" << ev.m_id << "] " << ev.m_name << " (" << ev.getTypeName() << ")";
        return os;
    }
};

// Derived Evidence Class 1: Physical Evidence
class PhysicalClue : public Evidence {
private:
    std::string m_material;
    std::string m_condition;

public:
    PhysicalClue(const std::string& id, const std::string& name,
                 const std::string& description, const std::string& location,
                 const std::string& material, const std::string& condition)
        : Evidence(id, name, description, location, EvidenceType::PHYSICAL),
          m_material(material), m_condition(condition) {}

    void displaySpecialDetail() const override {
        std::cout << "  Material       : " << m_material << "\n";
        std::cout << "  Condition      : " << m_condition << "\n";
    }
};

// Derived Evidence Class 2: Document Evidence
class DocumentClue : public Evidence {
private:
    std::string m_author;
    std::string m_excerpt;

public:
    DocumentClue(const std::string& id, const std::string& name,
                 const std::string& description, const std::string& location,
                 const std::string& author, const std::string& excerpt)
        : Evidence(id, name, description, location, EvidenceType::DOCUMENT),
          m_author(author), m_excerpt(excerpt) {}

    void displaySpecialDetail() const override {
        std::cout << "  Author / Source: " << m_author << "\n";
        std::cout << "  Excerpt Text   : \"" << m_excerpt << "\"\n";
    }
};

// Derived Evidence Class 3: Chemical Evidence
class ChemicalClue : public Evidence {
private:
    std::string m_compound;
    std::string m_toxicity;

public:
    ChemicalClue(const std::string& id, const std::string& name,
                 const std::string& description, const std::string& location,
                 const std::string& compound, const std::string& toxicity)
        : Evidence(id, name, description, location, EvidenceType::CHEMICAL),
          m_compound(compound), m_toxicity(toxicity) {}

    void displaySpecialDetail() const override {
        std::cout << "  Compound Name  : " << m_compound << "\n";
        std::cout << "  Toxicity Level : " << m_toxicity << "\n";
    }
};

// ============================================================================
// OOP CONCEPT 3: CHARACTER HIERARCHY (Inheritance & Polymorphism)
// ============================================================================
class Character : public IInspectable {
protected:
    std::string m_id;
    std::string m_name;
    std::string m_occupation;
    std::string m_greeting;

public:
    Character(const std::string& id, const std::string& name,
              const std::string& occupation, const std::string& greeting)
        : m_id(id), m_name(name), m_occupation(occupation), m_greeting(greeting) {}

    virtual ~Character() = default;

    std::string getId() const { return m_id; }
    std::string getName() const override { return m_name; }
    std::string getOccupation() const { return m_occupation; }
    std::string getGreeting() const { return m_greeting; }

    // Pure virtual methods
    virtual std::string getRole() const = 0;
    virtual void interrogate(int topicChoice) = 0;
    virtual bool confrontWithEvidence(const std::string& evidenceId) = 0;
};

// Derived Class: Suspect
class Suspect : public Character {
private:
    std::string m_alibi;
    std::string m_motive;
    std::string m_secret;
    std::string m_nervousTell;
    std::string m_breakdownConfession;
    std::string m_contradictingEvidenceId;
    std::string m_confrontationReaction;
    bool m_isGuilty;
    bool m_alibiBroken;
    int m_suspicionScore; // 0 to 100%

public:
    Suspect(const std::string& id, const std::string& name,
            const std::string& occupation, const std::string& greeting,
            const std::string& alibi, const std::string& motive,
            const std::string& secret, const std::string& tell,
            const std::string& contradictEvId,
            const std::string& confrontReaction,
            const std::string& confession, bool isGuilty)
        : Character(id, name, occupation, greeting),
          m_alibi(alibi), m_motive(motive), m_secret(secret),
          m_nervousTell(tell), m_breakdownConfession(confession),
          m_contradictingEvidenceId(contradictEvId),
          m_confrontationReaction(confrontReaction),
          m_isGuilty(isGuilty), m_alibiBroken(false), m_suspicionScore(20) {}

    std::string getRole() const override { return "Suspect"; }
    std::string getDescription() const override {
        return m_occupation + " - " + m_nervousTell;
    }

    int getSuspicionScore() const { return m_suspicionScore; }
    bool isAlibiBroken() const { return m_alibiBroken; }
    bool isGuilty() const { return m_isGuilty; }
    std::string getAlibi() const { return m_alibi; }
    std::string getMotive() const { return m_motive; }
    std::string getSecret() const { return m_secret; }
    std::string getTell() const { return m_nervousTell; }
    std::string getConfession() const { return m_breakdownConfession; }

    void inspect() const override {
        std::cout << "\n+-------------------------------------------------------------+\n";
        std::cout << "| SUSPECT DOSSIER: " << m_name << " (" << m_occupation << ")\n";
        std::cout << "+-------------------------------------------------------------+\n";
        std::cout << "  Claimed Alibi  : " << m_alibi << "\n";
        std::cout << "  Apparent Motive: " << m_motive << "\n";
        std::cout << "  Observed Tell  : " << m_nervousTell << "\n";
        std::cout << "  Suspicion Level: [" << m_suspicionScore << "%]";
        if (m_alibiBroken) {
            std::cout << " *** ALIBI SHATTERED ***";
        }
        std::cout << "\n";
        if (m_alibiBroken) {
            std::cout << "  Exposed Secret : " << m_secret << "\n";
        }
        std::cout << "+-------------------------------------------------------------+\n";
    }

    void interrogate(int topicChoice) override {
        std::cout << "\n[" << m_name << "]: ";
        if (topicChoice == 1) {
            std::cout << "\"" << m_alibi << "\"\n";
            std::cout << "*(You notice: " << m_nervousTell << ")*\n";
        } else if (topicChoice == 2) {
            std::cout << "\"" << m_motive << "\"\n";
        } else if (topicChoice == 3) {
            if (m_alibiBroken) {
                std::cout << "\"" << m_secret << "\"\n";
            } else {
                std::cout << "\"I have nothing more to say to you, Inspector! Speak to my lawyer!\"\n";
                std::cout << "*(Their guard is still up. You need evidence to break their lie!)*\n";
            }
        }
    }

    bool confrontWithEvidence(const std::string& evidenceId) override {
        if (evidenceId == m_contradictingEvidenceId) {
            m_alibiBroken = true;
            m_suspicionScore = std::min(100, m_suspicionScore + 50);
            std::cout << "\n=============================================================\n";
            std::cout << "  !! OBJECTION !! CONTRADICTION EXPOSED !!\n";
            std::cout << "=============================================================\n";
            std::cout << m_confrontationReaction << "\n\n";
            std::cout << ">> Suspicion meter surged to [" << m_suspicionScore << "%]!\n";
            std::cout << ">> New Secret Discovered: " << m_secret << "\n";
            return true;
        } else {
            std::cout << "\n[" << m_name << "] shakes their head with a dismissive chuckle:\n";
            std::cout << "\"What does that have to do with me, Inspector? You are clutching at straws.\"\n";
            std::cout << "*(This evidence did not contradict their statement.)*\n";
            return false;
        }
    }
};

// Derived Class: Witness
class Witness : public Character {
private:
    std::string m_testimony;
    std::string m_keyObservation;

public:
    Witness(const std::string& id, const std::string& name,
            const std::string& occupation, const std::string& greeting,
            const std::string& testimony, const std::string& observation)
        : Character(id, name, occupation, greeting),
          m_testimony(testimony), m_keyObservation(observation) {}

    std::string getRole() const override { return "Witness"; }
    std::string getDescription() const override { return m_occupation + " (Neutral Eyewitness)"; }

    void inspect() const override {
        std::cout << "\n+-------------------------------------------------------------+\n";
        std::cout << "| WITNESS PROFILE: " << m_name << " (" << m_occupation << ")\n";
        std::cout << "+-------------------------------------------------------------+\n";
        std::cout << "  Initial Statement: \"" << m_testimony << "\"\n";
        std::cout << "+-------------------------------------------------------------+\n";
    }

    void interrogate(int /*topicChoice*/) override {
        std::cout << "\n[" << m_name << "]:\n";
        std::cout << "\"" << m_testimony << "\"\n";
        std::cout << "\n*Key Observation*: \"" << m_keyObservation << "\"\n";
    }

    bool confrontWithEvidence(const std::string& /*evidenceId*/) override {
        std::cout << "\n[" << m_name << "]: \"Inspector, I am only telling you what I saw with my own eyes.\"\n";
        return false;
    }
};

// ============================================================================
// OOP CONCEPT 4: TEMPLATES & GENERICS
// ============================================================================
template <typename T>
class CaseRegistry {
private:
    std::vector<std::shared_ptr<T>> m_items;

public:
    void add(std::shared_ptr<T> item) {
        m_items.push_back(item);
    }

    size_t size() const {
        return m_items.size();
    }

    std::shared_ptr<T> getByIndex(size_t index) const {
        if (index < m_items.size()) return m_items[index];
        return nullptr;
    }

    std::shared_ptr<T> getById(const std::string& id) const {
        for (const auto& item : m_items) {
            if (item->getId() == id) return item;
        }
        return nullptr;
    }

    const std::vector<std::shared_ptr<T>>& getAll() const {
        return m_items;
    }
};

// ============================================================================
// ROOM & CRIME SCENE
// ============================================================================
struct RoomHotspot {
    std::string name;
    std::string description;
    std::string evidenceId;
    bool examined;
};

class Room {
private:
    std::string m_id;
    std::string m_name;
    std::string m_atmosphere;
    std::vector<RoomHotspot> m_hotspots;

public:
    Room(const std::string& id, const std::string& name, const std::string& atmosphere)
        : m_id(id), m_name(name), m_atmosphere(atmosphere) {}

    std::string getId() const { return m_id; }
    std::string getName() const { return m_name; }
    std::string getAtmosphere() const { return m_atmosphere; }

    void addHotspot(const std::string& name, const std::string& desc, const std::string& evId) {
        m_hotspots.push_back({name, desc, evId, false});
    }

    size_t getHotspotCount() const { return m_hotspots.size(); }
    RoomHotspot& getHotspot(size_t idx) { return m_hotspots.at(idx); }
    const std::vector<RoomHotspot>& getHotspots() const { return m_hotspots; }
};

#endif // SIMPLE_DETECTIVE_H
