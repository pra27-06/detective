#include "MindPalace.h"
#include <algorithm>
#include <sstream>

// -----------------------------------------------------------------------------
// CognitiveNode & Subclasses (Composite Pattern)
// -----------------------------------------------------------------------------
CognitiveNode::CognitiveNode(const std::string& id, const std::string& concept, const std::string& category)
    : m_id(id), m_concept(concept), m_category(category) {}

ObservationThought::ObservationThought(const std::string& id, const std::string& concept, const std::string& category, const std::string& sourceEntity)
    : CognitiveNode(id, concept, category), m_sourceEntity(sourceEntity) {}

std::string ObservationThought::getFormattedNode() const {
    std::ostringstream oss;
    oss << "[" << m_category << "] " << m_concept << " (Source: " << m_sourceEntity << ")";
    return oss.str();
}

HypothesisThought::HypothesisThought(const std::string& id, const std::string& concept, const std::string& theory)
    : CognitiveNode(id, concept, "HYPOTHESIS"), m_deductiveTheory(theory) {}

void HypothesisThought::addNode(std::shared_ptr<CognitiveNode> node) {
    if (node) {
        m_children.push_back(node);
    }
}

int HypothesisThought::getComplexityWeight() const {
    int weight = 2;
    for (const auto& child : m_children) {
        weight += child->getComplexityWeight();
    }
    return weight;
}

std::string HypothesisThought::getFormattedNode() const {
    std::ostringstream oss;
    oss << "[COMPOSITE HYPOTHESIS: " << m_concept << "] " << m_deductiveTheory
        << " (Supported by " << m_children.size() << " premise nodes, weight=" << getComplexityWeight() << ")";
    return oss.str();
}

// -----------------------------------------------------------------------------
// MindPalace (Observer Pattern & Synthesis Logic)
// -----------------------------------------------------------------------------
MindPalace::MindPalace() {}

void MindPalace::attachObserver(MindPalaceObserver* observer) {
    if (observer && std::find(m_observers.begin(), m_observers.end(), observer) == m_observers.end()) {
        m_observers.push_back(observer);
    }
}

void MindPalace::detachObserver(MindPalaceObserver* observer) {
    m_observers.erase(std::remove(m_observers.begin(), m_observers.end(), observer), m_observers.end());
}

void MindPalace::notifyObservers(const std::string& title, const std::string& insight, int bonus) {
    for (auto* obs : m_observers) {
        if (obs) {
            obs->onEpiphanyForged(title, insight, bonus);
        }
    }
}

void MindPalace::addThought(std::shared_ptr<CognitiveNode> thought) {
    if (thought) {
        m_thoughts[thought->getId()] = thought;
    }
}

std::shared_ptr<CognitiveNode> MindPalace::getThought(const std::string& id) const {
    auto it = m_thoughts.find(id);
    if (it != m_thoughts.end()) {
        return it->second;
    }
    return nullptr;
}

void MindPalace::registerEpiphanyFormula(const std::string& id, const std::string& title,
                                         const std::string& insight, const std::string& thoughtA,
                                         const std::string& thoughtB, int bonus) {
    Epiphany e;
    e.id = id;
    e.title = title;
    e.synthesisInsight = insight;
    e.requiredThoughtIdA = thoughtA;
    e.requiredThoughtIdB = thoughtB;
    e.scoreBonus = bonus;
    e.forged = false;
    m_possibleEpiphanies.push_back(e);
}

bool MindPalace::attemptSynthesis(const std::string& thoughtA, const std::string& thoughtB, Epiphany& outEpiphany) {
    for (auto& ep : m_possibleEpiphanies) {
        if (ep.forged) continue;

        bool matchDirect = (ep.requiredThoughtIdA == thoughtA && ep.requiredThoughtIdB == thoughtB);
        bool matchReverse = (ep.requiredThoughtIdA == thoughtB && ep.requiredThoughtIdB == thoughtA);

        if (matchDirect || matchReverse) {
            ep.forged = true;
            m_forgedEpiphanies.push_back(ep);
            outEpiphany = ep;
            // Notify observers of breakthrough!
            notifyObservers(ep.title, ep.synthesisInsight, ep.scoreBonus);
            return true;
        }
    }
    return false;
}
