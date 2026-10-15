# CASE CLOSED — UML Class Diagram & Architecture

This document provides a comprehensive structural and behavioral blueprint of **CASE CLOSED — Detective Mystery Simulator**, highlighting standard UML notation, class specifications, and design patterns.

---

## 1. Complete UML Class Hierarchy

```mermaid
classDiagram
    %% Core Abstract Class
    class Character {
        <<abstract>>
        #string m_id
        #string m_name
        #string m_occupation
        #string m_personality
        #string m_greeting
        +Character(id, name, occupation, personality, greeting)
        +virtual ~Character() = default
        +virtual string getRole() const = 0
        +virtual void displayProfile() const = 0
        +virtual string interact(topic: string) = 0
        +getId() string
        +getName() string
        +getOccupation() string
        +getPersonality() string
        +getGreeting() string
    }

    %% Derived Character Types
    class Detective {
        -string m_badgeNumber
        -string m_rank
        -int m_score
        -int m_energy
        -int m_maxEnergy
        -int m_hintsUsed
        +Detective(id, name, badgeNumber, rank)
        +string getRole() const override
        +void displayProfile() const override
        +string interact(topic: string) override
        +getScore() int
        +addScore(delta: int) void
        +deductScore(delta: int) void
        +getEnergy() int
        +spendEnergy(cost: int) bool
        +restoreEnergy(amount: int) void
        +getHintsUsed() int
        +incrementHints() void
    }

    class Suspect {
        -string m_alibi
        -string m_motive
        -bool m_isGuilty
        -bool m_alibiBroken
        -string m_breakdownDialogue
        -string m_confessionText
        -string m_eliminationReason
        -map~string, string~ m_topicResponses
        -vector~Contradiction~ m_contradictions
        -vector~ConditionalTopic~ m_conditionalTopics
        -vector~pair~string, string~~ m_transcript
        +Suspect(...)
        +string getRole() const override
        +void displayProfile() const override
        +string interact(topic: string) override
        +addTopic(key: string, response: string) void
        +addConditionalTopic(key, reqEvId, prompt, resp) void
        +getUnlockedConditionalTopics(discoveredIds) vector~ConditionalTopic~
        +addContradiction(tag, quote, evId, resolution) void
        +testContradiction(evidenceId, outQuote, outResolution) bool
        +recordInterview(inquiry, response) void
        +displayTranscript() void
        +isAlibiBroken() const bool
        +isGuilty() const bool
        +getEliminationReason() string
    }

    class Witness {
        -string m_testimony
        -string m_observedTime
        -string m_keyClueEvidenceId
        -bool m_hasGivenTestimony
        -map~string, string~ m_topicResponses
        +Witness(id, name, occupation, personality, greeting, testimony, time, clueId)
        +string getRole() const override
        +void displayProfile() const override
        +string interact(topic: string) override
        +giveTestimony() string
        +hasGivenTestimony() const bool
        +getKeyClueEvidenceId() const string
    }

    Character <|-- Detective : Inheritance (IS-A)
    Character <|-- Suspect : Inheritance (IS-A)
    Character <|-- Witness : Inheritance (IS-A)

    %% Supporting Domain Models
    class Evidence {
        -string m_id
        -string m_name
        -EvidenceCategory m_category
        -EvidenceRelevance m_relevance
        -string m_description
        -string m_locationFound
        -bool m_isKeyEvidence
        -bool m_isRedHerring
        -bool m_isDiscovered
        +Evidence(...)
        +getId() string
        +getName() string
        +getCategoryName() string
        +getRelevanceName() string
        +isKeyEvidence() bool
        +isRedHerring() bool
        +isDiscovered() bool
        +setDiscovered(discovered: bool) void
        +displayCard() void
        +displaySummary() void
        +serialize() string
        +deserialize(line: string)$ Evidence
    }

    class CrimeScene {
        -string m_id
        -string m_name
        -string m_description
        -vector~Hotspot~ m_hotspots
        +CrimeScene(id, name, description)
        +getId() string
        +getName() string
        +getDescription() string
        +addHotspot(name, description, evidenceId) void
        +examineHotspot(index: size_t) string
        +isFullyExamined() bool
        +displaySceneOverview() void
    }

    class ClueBoard {
        -set~string~ m_pinnedEvidenceIds
        -vector~pair~string, string~~ m_activeConnections
        -vector~Deduction~ m_availableDeductions
        +ClueBoard()
        +addPinnedEvidence(evidenceId: string) void
        +registerValidDeduction(id1, id2, title, insight) void
        +connectClues(id1, id2, outTitle, outInsight) int
        +displayBoard() void
        +getUnlockedDeductionCount() size_t
    }

    class Notebook {
        -vector~string~ m_caseNotes
        -vector~string~ m_recordedLeads
        +Notebook()
        +addNote(note: string) void
        +addLead(lead: string) void
        +displayNotebook() void
        +serializeNotes() string
        +serializeLeads() string
    }

    %% Aggregate & Facade Models
    class CaseFile {
        -int m_caseNumber
        -string m_caseId
        -string m_title
        -string m_briefing
        -string m_crimeTime
        -string m_locationName
        -string m_culpritId
        -string m_keyEvidenceId
        -string m_requiredContradictionId
        -string m_correctMotiveKeyword
        -string m_solutionExplanation
        -vector~TimelineEvent~ m_timeline
        -vector~SuspectElimination~ m_eliminations
        -vector~CaseObjective~ m_objectives
        -vector~shared_ptr~Character~~ m_characters
        -vector~CrimeScene~ m_scenes
        -map~string, Evidence~ m_allEvidence
        -ClueBoard m_clueBoard
        +CaseFile(caseNumber: int)
        +evaluateAccusation(suspectId, evidenceId, motiveKeyword) AccusationResult
        +getCharacterById(id: string) shared_ptr~Character~
        +getSuspectById(id: string) shared_ptr~Suspect~
        +markEvidenceDiscovered(id: string) bool
        +displayBriefingScreen(diff: string) void
        +displayProgress() void
        +displayDeductionMatrix() void
    }

    class SaveManager {
        <<utility>>
        +saveGame(filepath, difficulty, detective, caseFile, notebook)$ bool
        +loadGame(filepath, outState)$ bool
        +hasSaveFile(filepath)$ bool
        +getDefaultSavePath()$ string
    }

    class TerminalUI {
        <<utility>>
        +setColorEnabled(enabled: bool)$ void
        +isColorEnabled()$ bool
        +initTerminal()$ void
        +clearScreen()$ void
        +pause()$ void
        +printBanner()$ void
        +printBox(title, lines, width)$ void
        +promptInt(prompt, min, max)$ int
        +promptString(prompt, allowEmpty)$ string
        +promptConfirm(prompt)$ bool
    }

    class GameEngine {
        -shared_ptr~Detective~ m_detective
        -unique_ptr~CaseFile~ m_activeCase
        -Notebook m_notebook
        -GameDifficulty m_difficulty
        -bool m_isRunning
        -bool m_caseSolved
        -int m_investigationMistakes
        +GameEngine()
        +run() void
        -showMainMenu() void
        -runCaseLoop() void
        -handleInvestigation() void
        -handleInterrogation() void
        -handleClueBoard() void
        -handleDeductionMatrix() void
        -handleTranscripts() void
        -handleAccusation() void
        -handleSaveGame() void
    }

    %% Relationships
    CaseFile *-- Character : Polymorphic Aggregation
    CaseFile *-- CrimeScene : Composition
    CaseFile *-- Evidence : Composition
    CaseFile *-- ClueBoard : Composition

    GameEngine *-- Detective : Composition
    GameEngine *-- CaseFile : Composition
    GameEngine *-- Notebook : Composition
    GameEngine ..> SaveManager : Uses
    GameEngine ..> TerminalUI : Uses
```

---

## 2. Key Object-Oriented Relationships Explained

| Relationship Type | Source Class | Target Class | Practical Implementation |
| :--- | :--- | :--- | :--- |
| **Inheritance** | `Character` | `Detective`, `Suspect`, `Witness` | Derived classes inherit common attributes (`m_id`, `m_name`, `m_occupation`) and implement the abstract interface. |
| **Polymorphism** | `CaseFile` | `std::shared_ptr<Character>` | Characters are stored uniformly in a `vector<shared_ptr<Character>>`. Dynamic dispatch executes `displayProfile()` and `interact()` based on the runtime type. |
| **Composition** | `CaseFile` | `CrimeScene`, `Evidence`, `ClueBoard` | `CaseFile` manages the lifecycle of its scenes, evidence records, and deduction matrix. When a case ends, these entities are released together. |
| **Composition** | `GameEngine` | `Detective`, `CaseFile`, `Notebook` | `GameEngine` acts as the central coordinator, owning the detective profile, active case file, and player notebook. |
| **Dependency / Usage** | `GameEngine` | `SaveManager`, `TerminalUI` | Stateless utility classes handle cross-platform file serialization and virtual terminal rendering. |
