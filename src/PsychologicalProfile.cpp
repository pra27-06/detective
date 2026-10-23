#include "PsychologicalProfile.h"

PsychologicalProfile::PsychologicalProfile(const std::string& archetype, int baselineBPM)
    : m_archetype(archetype), m_baselineBPM(baselineBPM) {}

// -----------------------------------------------------------------------------
// NervousDefensiveProfile
// -----------------------------------------------------------------------------
NervousDefensiveProfile::NervousDefensiveProfile()
    : PsychologicalProfile("Nervous Defensive", 78) {}

int NervousDefensiveProfile::calculateStressSpike(const std::string& evidenceCategory, bool isContradiction) const {
    int spike = 10;
    if (evidenceCategory == "FORENSIC" || evidenceCategory == "PHYSICAL") {
        spike += 25;
    }
    if (isContradiction) {
        spike += 45;
    }
    return spike;
}

std::string NervousDefensiveProfile::getMicroExpression(int currentBPM) const {
    if (currentBPM > 135) {
        return "Severe trembling, erratic glances at the exit, heavy perspiration on forehead.";
    } else if (currentBPM > 105) {
        return "Nervous swallow, frequent collar adjustments, avoiding direct eye contact.";
    }
    return "Fidgeting with fingers, defensive arm posture.";
}

bool NervousDefensiveProfile::evaluateBreakdown(int currentBPM, int composure) const {
    return currentBPM >= 130 || composure <= 20;
}

// -----------------------------------------------------------------------------
// StoicCalculatingProfile
// -----------------------------------------------------------------------------
StoicCalculatingProfile::StoicCalculatingProfile()
    : PsychologicalProfile("Stoic Calculating", 62) {}

int StoicCalculatingProfile::calculateStressSpike(const std::string& evidenceCategory, bool isContradiction) const {
    int spike = 5;
    if (evidenceCategory == "DOCUMENT" || evidenceCategory == "FINANCIAL") {
        spike += 30;
    }
    if (isContradiction) {
        spike += 40;
    }
    return spike;
}

std::string StoicCalculatingProfile::getMicroExpression(int currentBPM) const {
    if (currentBPM > 120) {
        return "Deliberate silence, knuckles white as cane is gripped tightly, sudden shallow breaths.";
    } else if (currentBPM > 85) {
        return "Imperceptible twitch of the left eyelid, brief hesitation before answering.";
    }
    return "Composed, cold gaze, measuring every syllable with icy calm.";
}

bool StoicCalculatingProfile::evaluateBreakdown(int currentBPM, int composure) const {
    return currentBPM >= 120 || composure <= 15;
}

// -----------------------------------------------------------------------------
// NarcissisticManipulatorProfile
// -----------------------------------------------------------------------------
NarcissisticManipulatorProfile::NarcissisticManipulatorProfile()
    : PsychologicalProfile("Narcissistic Manipulator", 72) {}

int NarcissisticManipulatorProfile::calculateStressSpike(const std::string& evidenceCategory, bool isContradiction) const {
    int spike = 8;
    if (evidenceCategory == "TESTIMONY" || evidenceCategory == "DOCUMENT") {
        spike += 28;
    }
    if (isContradiction) {
        spike += 50;
    }
    return spike;
}

std::string NarcissisticManipulatorProfile::getMicroExpression(int currentBPM) const {
    if (currentBPM > 130) {
        return "Snarl replaces smug smile, vocal pitch rises sharply, erratic hand gestures.";
    } else if (currentBPM > 95) {
        return "Condescending chuckle fades, jaw tightens visibly, eyes narrow in anger.";
    }
    return "Arrogant head tilt, dismissive smirk, relaxed posture.";
}

bool NarcissisticManipulatorProfile::evaluateBreakdown(int currentBPM, int composure) const {
    return currentBPM >= 130 || composure <= 15;
}
