#ifndef PSYCHOLOGICAL_PROFILE_H
#define PSYCHOLOGICAL_PROFILE_H

#include <string>
#include <memory>

// Strategy & State Pattern Base Class for Suspect Psychological Reactions
class PsychologicalProfile {
protected:
    std::string m_archetype;
    int m_baselineBPM;

public:
    PsychologicalProfile(const std::string& archetype, int baselineBPM);
    virtual ~PsychologicalProfile() = default;

    std::string getArchetype() const { return m_archetype; }
    int getBaselineBPM() const { return m_baselineBPM; }

    // Pure virtual strategy methods
    virtual int calculateStressSpike(const std::string& evidenceCategory, bool isContradiction) const = 0;
    virtual std::string getMicroExpression(int currentBPM) const = 0;
    virtual bool evaluateBreakdown(int currentBPM, int composure) const = 0;
};

// Strategy 1: Nervous Defensive (Spikes violently under physical proof)
class NervousDefensiveProfile : public PsychologicalProfile {
public:
    NervousDefensiveProfile();
    int calculateStressSpike(const std::string& evidenceCategory, bool isContradiction) const override;
    std::string getMicroExpression(int currentBPM) const override;
    bool evaluateBreakdown(int currentBPM, int composure) const override;
};

// Strategy 2: Stoic Calculating (Calm under pressure, breaks only to financial proof)
class StoicCalculatingProfile : public PsychologicalProfile {
public:
    StoicCalculatingProfile();
    int calculateStressSpike(const std::string& evidenceCategory, bool isContradiction) const override;
    std::string getMicroExpression(int currentBPM) const override;
    bool evaluateBreakdown(int currentBPM, int composure) const override;
};

// Strategy 3: Narcissistic Manipulator (Arrogant until disinheritance/patent proof)
class NarcissisticManipulatorProfile : public PsychologicalProfile {
public:
    NarcissisticManipulatorProfile();
    int calculateStressSpike(const std::string& evidenceCategory, bool isContradiction) const override;
    std::string getMicroExpression(int currentBPM) const override;
    bool evaluateBreakdown(int currentBPM, int composure) const override;
};

#endif // PSYCHOLOGICAL_PROFILE_H
