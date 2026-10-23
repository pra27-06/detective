#ifndef MIND_PALACE_H
#define MIND_PALACE_H

#include <string>
#include <vector>
#include <memory>
#include <map>

// Observer Pattern Interface: Listeners respond when detective discovers a cognitive epiphany
class MindPalaceObserver {
public:
    virtual ~MindPalaceObserver() = default;
    virtual void onEpiphanyForged(const std::string& epiphanyTitle, const std::string& insight, int scoreBonus) = 0;
};

// Composite Pattern Base Component: Cognitive Thought Node
class CognitiveNode {
protected:
    std::string m_id;
    std::string m_concept;
    std::string m_category; // "FORENSIC", "BEHAVIORAL", "TIMELINE"

public:
    CognitiveNode(const std::string& id, const std::string& concept, const std::string& category);
    virtual ~CognitiveNode() = default;

    std::string getId() const { return m_id; }
    std::string getConcept() const { return m_concept; }
    std::string getCategory() const { return m_category; }

    virtual bool isComposite() const { return false; }
    virtual int getComplexityWeight() const = 0;
    virtual std::string getFormattedNode() const = 0;
};

// Composite Pattern Leaf: Observation Thought (Derived from physical clue or witness quote)
class ObservationThought : public CognitiveNode {
private:
    std::string m_sourceEntity;

public:
    ObservationThought(const std::string& id, const std::string& concept, const std::string& category, const std::string& sourceEntity);
    int getComplexityWeight() const override { return 1; }
    std::string getFormattedNode() const override;
};

// Composite Pattern Composite: Hypothesis Node (Combines multiple observation thoughts)
class HypothesisThought : public CognitiveNode {
private:
    std::vector<std::shared_ptr<CognitiveNode>> m_children;
    std::string m_deductiveTheory;

public:
    HypothesisThought(const std::string& id, const std::string& concept, const std::string& theory);
    bool isComposite() const override { return true; }
    void addNode(std::shared_ptr<CognitiveNode> node);
    const std::vector<std::shared_ptr<CognitiveNode>>& getChildren() const { return m_children; }
    int getComplexityWeight() const override;
    std::string getFormattedNode() const override;
    std::string getTheory() const { return m_deductiveTheory; }
};

// Represents a crystallized cognitive breakthrough
struct Epiphany {
    std::string id;
    std::string title;
    std::string synthesisInsight;
    std::string requiredThoughtIdA;
    std::string requiredThoughtIdB;
    int scoreBonus;
    bool forged;
};

// MindPalace: Manages thought synthesis, Composite graph, and Observer notifications
class MindPalace {
private:
    std::map<std::string, std::shared_ptr<CognitiveNode>> m_thoughts;
    std::vector<Epiphany> m_possibleEpiphanies;
    std::vector<Epiphany> m_forgedEpiphanies;
    std::vector<MindPalaceObserver*> m_observers;

public:
    MindPalace();

    // Observer management
    void attachObserver(MindPalaceObserver* observer);
    void detachObserver(MindPalaceObserver* observer);
    void notifyObservers(const std::string& title, const std::string& insight, int bonus);

    // Thought management (Composite)
    void addThought(std::shared_ptr<CognitiveNode> thought);
    std::shared_ptr<CognitiveNode> getThought(const std::string& id) const;
    const std::map<std::string, std::shared_ptr<CognitiveNode>>& getAllThoughts() const { return m_thoughts; }

    // Epiphany synthesis
    void registerEpiphanyFormula(const std::string& id, const std::string& title,
                                 const std::string& insight, const std::string& thoughtA,
                                 const std::string& thoughtB, int bonus = 50);
    bool attemptSynthesis(const std::string& thoughtA, const std::string& thoughtB, Epiphany& outEpiphany);
    const std::vector<Epiphany>& getForgedEpiphanies() const { return m_forgedEpiphanies; }
    size_t getForgedEpiphanyCount() const { return m_forgedEpiphanies.size(); }
};

#endif // MIND_PALACE_H
