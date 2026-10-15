#ifndef CASE_FILE_H
#define CASE_FILE_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Character.h"
#include "Detective.h"
#include "Suspect.h"
#include "Witness.h"
#include "Evidence.h"
#include "CrimeScene.h"
#include "ClueBoard.h"
#include "ForensicLab.h"
#include "MindPalace.h"

struct TimelineEvent {
    std::string time;
    std::string description;
};

struct SuspectElimination {
    std::string suspectId;
    std::string suspectName;
    std::string reason;
};

struct CaseObjective {
    std::string description;
    bool completed;
};

struct AccusationResult {
    bool isCorrectCulprit;
    bool hasKeyEvidence;
    bool isAlibiBroken;
    bool isMotiveCorrect;
    int scoreAwarded;
    std::string verdictTitle;
    std::string outcomeDescription;
    std::vector<std::string> reportDetails;
    std::vector<TimelineEvent> chronologicalTimeline;
    std::vector<SuspectElimination> suspectEliminations;
    std::vector<std::string> resolvedContradictions;
};

class CaseFile {
private:
    int m_caseNumber;
    std::string m_caseId;
    std::string m_title;
    std::string m_tagline;
    std::string m_briefing;
    std::string m_crimeTime;
    std::string m_locationName;
    std::string m_victimName;

    // Solution details (kept private and protected!)
    std::string m_culpritId;
    std::string m_keyEvidenceId;
    std::string m_requiredContradictionId;
    std::string m_correctMotiveKeyword;
    std::string m_solutionExplanation;

    // Timeline, objectives, and elimination data
    std::vector<TimelineEvent> m_timeline;
    std::vector<SuspectElimination> m_eliminations;
    std::vector<CaseObjective> m_objectives;

    // Composition of domain entities
    std::vector<std::shared_ptr<Character>> m_characters;
    std::vector<CrimeScene> m_scenes;
    std::map<std::string, Evidence> m_allEvidence;
    ClueBoard m_clueBoard;
    ForensicLab m_forensicLab;
    MindPalace m_mindPalace;

    void setupCase1();
    void setupCase2();
    void setupCase3();

public:
    CaseFile(int caseNumber);

    // Getters
    int getCaseNumber() const;
    std::string getCaseId() const;
    std::string getTitle() const;
    std::string getTagline() const;
    std::string getBriefing() const;
    std::string getTimeOfCrime() const;
    std::string getLocationName() const;
    std::string getVictimName() const;

    // Character Management (Polymorphic)
    const std::vector<std::shared_ptr<Character>>& getCharacters() const;
    std::shared_ptr<Character> getCharacterById(const std::string& id) const;
    std::shared_ptr<Suspect> getSuspectById(const std::string& id) const;
    std::shared_ptr<Witness> getWitnessById(const std::string& id) const;

    // Crime Scene Management
    std::vector<CrimeScene>& getScenes();
    const std::vector<CrimeScene>& getScenes() const;
    CrimeScene* getSceneById(const std::string& sceneId);

    // Evidence Management
    std::map<std::string, Evidence>& getAllEvidence();
    const std::map<std::string, Evidence>& getAllEvidence() const;
    Evidence* getEvidenceById(const std::string& id);
    std::vector<Evidence> getDiscoveredEvidence() const;
    std::vector<std::string> getDiscoveredEvidenceIds() const;
    bool markEvidenceDiscovered(const std::string& id);

    // Clue Board
    ClueBoard& getClueBoard();
    const ClueBoard& getClueBoard() const;

    // Forensic Lab & Mind Palace (Advanced OOP Engines)
    ForensicLab& getForensicLab() { return m_forensicLab; }
    const ForensicLab& getForensicLab() const { return m_forensicLab; }
    MindPalace& getMindPalace() { return m_mindPalace; }
    const MindPalace& getMindPalace() const { return m_mindPalace; }

    // Objectives & Progress
    void updateObjectives();
    const std::vector<CaseObjective>& getObjectives() const;
    const std::vector<TimelineEvent>& getTimeline() const;
    const std::vector<SuspectElimination>& getEliminations() const;

    // Accusation Evaluation
    AccusationResult evaluateAccusation(const std::string& suspectId,
                                        const std::string& evidenceId,
                                        const std::string& motiveKeyword);

    // Presentation Helpers
    void displayCaseHeader() const;
    void displayBriefingScreen(const std::string& difficultyName) const;
    void displaySuspectsList() const;
    void displayWitnessesList() const;
    void displayDiscoveredEvidenceList() const;
    void displayProgress() const;
    void displayDeductionMatrix() const;
};

#endif // CASE_FILE_H
