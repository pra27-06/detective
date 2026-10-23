#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <memory>
#include <vector>
#include "SaveManager.h"
#include "CaseFile.h"
#include "DialogueSystem.h"
#include "VNAssetManager.h"
#include "ForensicLab.h"
#include "MindPalace.h"
#include "PsychologicalProfile.h"

int main() {
    std::cout << "========================================================================" << std::endl;
    std::cout << "  CASE CLOSED :: AUTOMATED VERIFICATION TEST SUITE" << std::endl;
    std::cout << "========================================================================" << std::endl;

    int passedCount = 0;
    int totalTests = 14;

    // -------------------------------------------------------------------------
    // TEST 1: DialogueSystem Typing, Click-to-Finish, Click-to-Advance & Branching
    // -------------------------------------------------------------------------
    std::cout << "[RUNNING TEST 1/14] DialogueSystem mechanics & state transitions..." << std::endl;
    {
        DialogueSystem ds;
        DialogueEntry e1("d1", "Sherlock", "detective", "thinking", "Examining crime scene forensics.");
        DialogueEntry e2("d2", "Sherlock", "detective", "pointing", "We found the culprit's trail.");
        ds.setEntries({ e1, e2 });
        ds.start();

        assert(ds.isTyping());
        assert(!ds.isComplete());
        assert(!ds.isEnded());

        // Frame update
        ds.update(0.1f);
        assert(ds.getDisplayedText().length() > 0);

        // Click 1: Finish typing immediately
        bool adv1 = ds.advance();
        assert(adv1 == true);
        assert(!ds.isTyping());
        assert(ds.isComplete());
        assert(ds.getDisplayedText() == e1.text);

        // Click 2: Advance to next entry
        bool adv2 = ds.advance();
        assert(adv2 == true);
        assert(ds.getCurrentEntry()->id == "d2");
        assert(ds.isTyping());

        ds.finishTyping();
        assert(ds.isComplete());

        // Click 3: Finish dialogue sequence cleanly
        bool adv3 = ds.advance();
        assert(adv3 == false);
        assert(ds.isEnded());

        std::cout << "  [PASS] DialogueSystem typing, finish, advance, and clean termination verified." << std::endl;
        passedCount++;
    }

    // -------------------------------------------------------------------------
    // TEST 2: CrimeScene Hotspot Examination & Duplicate Protection
    // -------------------------------------------------------------------------
    std::cout << "[RUNNING TEST 2/14] CrimeScene hotspots & evidence discovery..." << std::endl;
    {
        CaseFile cf(1);
        auto& scenes = cf.getScenes();
        assert(scenes.size() >= 3);

        auto& vault = scenes[0];
        assert(vault.getHotspotCount() > 0);
        assert(!vault.isHotspotExamined(0));

        // First examination yields evidence ID
        std::string evId = vault.examineHotspot(0);
        assert(!evId.empty());
        assert(vault.isHotspotExamined(0));

        // First discovery in CaseFile returns true
        bool firstDisc = cf.markEvidenceDiscovered(evId);
        assert(firstDisc == true);

        Evidence* ev = cf.getEvidenceById(evId);
        assert(ev != nullptr);
        assert(ev->isDiscovered());

        // Duplicate discovery of same evidence returns false (prevents duplicate points)
        bool dupDisc = cf.markEvidenceDiscovered(evId);
        assert(dupDisc == false);

        std::cout << "  [PASS] Hotspot discovery and duplicate discovery prevention verified." << std::endl;
        passedCount++;
    }

    // -------------------------------------------------------------------------
    // TEST 3: Suspect Contradiction Engine & Alibi Shattering
    // -------------------------------------------------------------------------
    std::cout << "[RUNNING TEST 3/14] Suspect alibis and contradiction resolution..." << std::endl;
    {
        CaseFile cf(1);
        auto croft = cf.getSuspectById("S03");
        assert(croft != nullptr);
        assert(!croft->isAlibiBroken());

        std::string stmt, resolution;

        // Presenting unrelated evidence (E01) must fail and keep alibi intact
        bool fail = croft->testContradiction("E01", stmt, resolution);
        assert(!fail);
        assert(!croft->isAlibiBroken());

        // Presenting decisive contradictory evidence (E03 Pocket Watch) shatters alibi
        bool success = croft->testContradiction("E03", stmt, resolution);
        assert(success);
        assert(croft->isAlibiBroken());
        assert(!resolution.empty());

        std::cout << "  [PASS] Contradiction rejection and alibi breaking logic verified." << std::endl;
        passedCount++;
    }

    // -------------------------------------------------------------------------
    // TEST 4: ClueBoard Pairing & Deduction Unlock
    // -------------------------------------------------------------------------
    std::cout << "[RUNNING TEST 4/14] ClueBoard deduction matrix & clue pinning..." << std::endl;
    {
        CaseFile cf(1);
        auto& cb = cf.getClueBoard();

        cb.addPinnedEvidence("E01");
        cb.addPinnedEvidence("E03");

        std::string title, insight;
        // Connecting valid pair E01 and E03 produces breakthrough
        int res = cb.connectClues("E01", "E03", title, insight);
        assert(res == 1);
        assert(!title.empty());
        assert(!insight.empty());

        // Connecting same pair again returns already connected
        int dupRes = cb.connectClues("E01", "E03", title, insight);
        assert(dupRes == 0);

        // Connecting invalid pair returns no connection
        int invalidRes = cb.connectClues("E01", "E08", title, insight);
        assert(invalidRes == -1);

        std::cout << "  [PASS] ClueBoard deduction synthesis and connection validation verified." << std::endl;
        passedCount++;
    }

    // -------------------------------------------------------------------------
    // TEST 5: CaseFile Magistrate Accusation & Verdict Outcomes
    // -------------------------------------------------------------------------
    std::cout << "[RUNNING TEST 5/14] CaseFile accusation evaluation (Master vs Miscarriage)..." << std::endl;
    {
        CaseFile cf(1);

        // Premature accusation: alibi not broken
        auto premature = cf.evaluateAccusation("S03", "E03", "DEBT");
        assert(premature.isCorrectCulprit);
        assert(!premature.isAlibiBroken); // Alibi not yet broken!

        // Break Croft's alibi
        auto croft = cf.getSuspectById("S03");
        assert(croft != nullptr);
        croft->setAlibiBroken(true);

        // Flawless accusation: correct suspect (S03), key evidence (E03), correct motive (DEBT)
        auto flawless = cf.evaluateAccusation("S03", "E03", "DEBT");
        assert(flawless.isCorrectCulprit);
        assert(flawless.isAlibiBroken);
        assert(flawless.hasKeyEvidence);
        assert(flawless.isMotiveCorrect);
        assert(flawless.scoreAwarded > 0);
        assert(flawless.suspectEliminations.size() >= 2);

        // False accusation: innocent suspect Evelyn (S01)
        auto falseAcc = cf.evaluateAccusation("S01", "E01", "DEBT");
        assert(!falseAcc.isCorrectCulprit);

        std::cout << "  [PASS] Courtroom indictment rules, score calculation, and eliminations verified." << std::endl;
        passedCount++;
    }

    // -------------------------------------------------------------------------
    // TEST 6: SaveManager Serialization & Deserialization
    // -------------------------------------------------------------------------
    std::cout << "[RUNNING TEST 6/14] SaveManager roundtrip state persistence..." << std::endl;
    {
        CaseFile cf(1);
        cf.markEvidenceDiscovered("E01");
        cf.markEvidenceDiscovered("E03");
        auto croft = cf.getSuspectById("S03");
        if (croft) croft->setAlibiBroken(true);

        Detective det("D01", "Sherlock", "SCOT-7749", "Chief Inspector");
        det.setScore(380);
        det.setEnergy(65);

        Notebook nb;
        nb.addNote("Field note: Vault lock showed chemical residue.");
        nb.addLead("Lead: Security Guard Cruz alibi contradicted by logs.");

        std::string testSavePath = "saves/automated_test_save.dat";
        bool saveOk = SaveManager::saveGame(testSavePath, 3, det, cf, nb);
        assert(saveOk);

        GameSaveState state;
        bool loadOk = SaveManager::loadGame(testSavePath, state);
        assert(loadOk);
        assert(state.caseNumber == 1);
        assert(state.difficulty == 3);
        assert(state.detectiveName == "Sherlock");
        assert(state.detectiveScore == 380);
        assert(state.detectiveEnergy == 65);
        assert(state.discoveredEvidenceIds.size() >= 2);
        assert(state.brokenAlibiSuspectIds.size() >= 1);
        assert(state.customNotes.size() == 1);
        assert(state.recordedLeads.size() == 1);

        // Clean up test save file
        std::remove(testSavePath.c_str());
        std::cout << "  [PASS] SaveManager game state serialization and deserialization verified." << std::endl;
        passedCount++;
    }

    // -------------------------------------------------------------------------
    // TEST 7: VNAssetManager Cache & Graceful Missing Asset Fallback
    // -------------------------------------------------------------------------
    std::cout << "[RUNNING TEST 7/14] VNAssetManager texture caching & crash prevention..." << std::endl;
    {
        VNAssetManager am;
        bool init = am.initialize("assets");
        assert(init);

        // 1. Real background returns valid pointer
        Gdiplus::Bitmap* realBg = am.getBackground("main_menu");
        assert(realBg != nullptr);
        assert(realBg->GetWidth() > 0);

        // 2. Real character portrait returns valid pointer
        Gdiplus::Bitmap* realPort = am.getPortrait("cruz", "neutral");
        assert(realPort != nullptr);
        assert(realPort->GetWidth() > 0);

        // 3. Non-existent asset must NOT crash and must return procedural fallback bitmap
        Gdiplus::Bitmap* fakeBg = am.getBackground("non_existent_background_xyz");
        assert(fakeBg != nullptr);
        assert(fakeBg->GetWidth() > 0);

        Gdiplus::Bitmap* fakePort = am.getPortrait("non_existent_character", "unknown_expr");
        assert(fakePort != nullptr);
        assert(fakePort->GetWidth() > 0);

        std::cout << "  [PASS] Texture caching and procedural missing-asset fallbacks verified." << std::endl;
        passedCount++;
    }

    // -------------------------------------------------------------------------
    // TEST 8: ForensicLab Template Method & Polymorphic Analyzers
    // -------------------------------------------------------------------------
    std::cout << "[RUNNING TEST 8/14] ForensicLab Template Method & chemical assays..." << std::endl;
    {
        ForensicLab lab;
        Evidence teaCup("E01", "Porcelain Teacup", EvidenceCategory::FORENSIC, EvidenceRelevance::CRITICAL_KEY, "Contains tea dregs", "Victim Table");

        ForensicReport chemReport;
        bool analyzedChem = lab.runAnalysis("Chemical Reagent Assay", teaCup, chemReport);
        assert(analyzedChem);
        assert(chemReport.isDecisiveProof);
        assert(chemReport.forensicPoints > 0);
        assert(chemReport.secretRevealed.find("Hydrocyanic acid") != std::string::npos);

        ForensicReport spectroReport;
        bool analyzedSpectro = lab.runAnalysis("Microscopic Spectroscopy", teaCup, spectroReport);
        assert(analyzedSpectro);
        assert(lab.hasReport("E01", "Chemical Reagent Assay"));
        assert(lab.hasReport("E01", "Microscopic Spectroscopy"));

        std::string cert = chemReport.formatCertificate();
        assert(cert.find("SCOTLAND YARD METROPOLITAN FORENSIC CERTIFICATE") != std::string::npos);

        std::cout << "  [PASS] Template method execution, polymorphic assays, and certificates verified." << std::endl;
        passedCount++;
    }

    // -------------------------------------------------------------------------
    // TEST 9: MindPalace Composite Nodes, Thought Synthesis & Observer Pattern
    // -------------------------------------------------------------------------
    std::cout << "[RUNNING TEST 9/14] MindPalace Composite graph & Observer notifications..." << std::endl;
    {
        class TestMindObserver : public MindPalaceObserver {
        public:
            bool notified = false;
            std::string lastTitle;
            void onEpiphanyForged(const std::string& title, const std::string& /*insight*/, int /*bonus*/) override {
                notified = true;
                lastTitle = title;
            }
        };

        MindPalace mp;
        TestMindObserver observer;
        mp.attachObserver(&observer);

        // Composite pattern
        auto obs1 = std::make_shared<ObservationThought>("T01", "Keycard logged at 1:15 AM", "TIMELINE", "Access Logs");
        auto obs2 = std::make_shared<ObservationThought>("T02", "Basement logged at 1:12 AM", "TIMELINE", "Access Logs");
        auto hypo = std::make_shared<HypothesisThought>("H01", "Physical Impossibility", "No human could run between both locations in 3 minutes.");
        hypo->addNode(obs1);
        hypo->addNode(obs2);

        assert(hypo->isComposite());
        assert(hypo->getComplexityWeight() == 4); // 2 + 1 + 1

        mp.addThought(obs1);
        mp.addThought(obs2);
        mp.addThought(hypo);
        assert(mp.getThought("H01") != nullptr);

        // Epiphany synthesis & Observer notification
        mp.registerEpiphanyFormula("EPI_TEST", "The Impossible Transit Paradox", "Curator badge was duplicated", "T01", "T02", 75);
        Epiphany result;
        bool forged = mp.attemptSynthesis("T01", "T02", result);
        assert(forged);
        assert(observer.notified);
        assert(observer.lastTitle == "The Impossible Transit Paradox");
        assert(mp.getForgedEpiphanyCount() == 1);

        mp.detachObserver(&observer);
        std::cout << "  [PASS] Cognitive composite nodes, weight calculations, and observer notifications verified." << std::endl;
        passedCount++;
    }

    // -------------------------------------------------------------------------
    // TEST 10: PsychologicalProfile Strategies & Suspect Heart-Rate Elevation
    // -------------------------------------------------------------------------
    std::cout << "[RUNNING TEST 10/14] PsychologicalProfile strategy & suspect cardiac spikes..." << std::endl;
    {
        Suspect cruz("S02", "Marcus Cruz", "Security Guard", "Nervous", "I was on patrol", "North Wing", "Debt", true, "I broke!", "I did it");
        cruz.setPsychologicalProfile(std::make_unique<NervousDefensiveProfile>());
        assert(cruz.getCurrentBPM() == 78);
        assert(cruz.getComposure() == 100);
        assert(!cruz.isPsychologicallyBroken());

        // Pressure with non-contradiction
        cruz.applyInterrogationPressure("PHYSICAL", false);
        assert(cruz.getCurrentBPM() > 78);
        assert(cruz.getComposure() < 100);

        // Shatter with contradiction
        cruz.applyInterrogationPressure("FORENSIC", true);
        assert(cruz.getCurrentBPM() >= 130);
        assert(cruz.isPsychologicallyBroken());
        assert(cruz.isAlibiBroken());
        assert(!cruz.getCurrentMicroExpression().empty());

        std::cout << "  [PASS] Suspect psychological state machine, cardiac spikes, and composure breakdown verified." << std::endl;
        passedCount++;
    }

    // -------------------------------------------------------------------------
    // TEST 11: Case 2: The Last Train Mystery Contradiction & Indictment
    // -------------------------------------------------------------------------
    std::cout << "[RUNNING TEST 11/14] Case 2: The Last Train Mystery contradiction & indictment..." << std::endl;
    {
        CaseFile cf2(2);
        assert(cf2.getCaseNumber() == 2);

        auto gray = cf2.getSuspectById("S01");
        assert(gray != nullptr);
        assert(!gray->isAlibiBroken());

        std::string stmt, res;
        // False clue fails
        bool wrongHit = gray->testContradiction("E04", stmt, res);
        assert(!wrongHit);
        assert(!gray->isAlibiBroken());

        // Decisive clue E02 breaks alibi
        bool rightHit = gray->testContradiction("E02", stmt, res);
        assert(rightHit);
        assert(gray->isAlibiBroken());

        // Evaluate Master Detective Indictment
        auto verdict = cf2.evaluateAccusation("S01", "E02", "EMBEZZLEMENT");
        assert(verdict.isCorrectCulprit);
        assert(verdict.isAlibiBroken);
        assert(verdict.hasKeyEvidence);
        assert(verdict.isMotiveCorrect);
        assert(verdict.scoreAwarded > 0);

        std::cout << "  [PASS] Case 2 Julian Gray alibi shattering and flawless indictment verified." << std::endl;
        passedCount++;
    }

    // -------------------------------------------------------------------------
    // TEST 12: Case 3: The Locked Room Incident Contradiction & Indictment
    // -------------------------------------------------------------------------
    std::cout << "[RUNNING TEST 12/14] Case 3: The Locked Room Incident contradiction & indictment..." << std::endl;
    {
        CaseFile cf3(3);
        assert(cf3.getCaseNumber() == 3);

        auto cornelius = cf3.getSuspectById("S01");
        assert(cornelius != nullptr);
        assert(!cornelius->isAlibiBroken());

        std::string stmt, res;
        // False clue fails
        bool wrongHit = cornelius->testContradiction("E08", stmt, res);
        assert(!wrongHit);
        assert(!cornelius->isAlibiBroken());

        // Decisive clue E02 breaks alibi
        bool rightHit = cornelius->testContradiction("E02", stmt, res);
        assert(rightHit);
        assert(cornelius->isAlibiBroken());

        // Evaluate Master Detective Indictment (supports PATENT or INHERITANCE)
        auto verdict = cf3.evaluateAccusation("S01", "E02", "PATENT");
        assert(verdict.isCorrectCulprit);
        assert(verdict.isAlibiBroken);
        assert(verdict.hasKeyEvidence);
        assert(verdict.isMotiveCorrect);
        assert(verdict.scoreAwarded > 0);

        std::cout << "  [PASS] Case 3 Cornelius Thorne alibi shattering and locked room indictment verified." << std::endl;
        passedCount++;
    }

    // -------------------------------------------------------------------------
    // TEST 13: Corrupt & Malformed Save File Rejection
    // -------------------------------------------------------------------------
    std::cout << "[RUNNING TEST 13/14] Corrupt and malformed save file resilience..." << std::endl;
    {
        // 1. Missing file returns false
        GameSaveState dummy;
        bool missingOk = SaveManager::loadGame("saves/non_existent_fake_file.dat", dummy);
        assert(!missingOk);

        // 2. Corrupt file with invalid non-numeric CASE value
        std::string corruptPath = "saves/corrupt_test_save.dat";
        {
            std::ofstream out(corruptPath);
            out << "[HEADER]\n";
            out << "CASE=CORRUPT_NON_NUMERIC_STRING\n";
            out << "DIFFICULTY=1\n";
            out << "NAME=Sherlock\n";
            out << "SCORE=100\n";
            out << "ENERGY=80\n";
        }
        GameSaveState corruptState;
        bool corruptLoad = SaveManager::loadGame(corruptPath, corruptState);
        assert(!corruptLoad); // Must gracefully reject without crashing or throwing
        std::remove(corruptPath.c_str());

        // 3. Out-of-bounds case number (e.g. CASE=999)
        {
            std::ofstream out(corruptPath);
            out << "[HEADER]\n";
            out << "CASE=999\n";
            out << "DIFFICULTY=1\n";
            out << "NAME=Sherlock\n";
            out << "SCORE=100\n";
            out << "ENERGY=80\n";
        }
        bool oobLoad = SaveManager::loadGame(corruptPath, corruptState);
        assert(!oobLoad);
        std::remove(corruptPath.c_str());

        std::cout << "  [PASS] Corrupt string values and out-of-bounds save parameters safely rejected." << std::endl;
        passedCount++;
    }

    // -------------------------------------------------------------------------
    // TEST 14: Evidence Operator Overloading & Presentation
    // -------------------------------------------------------------------------
    std::cout << "[RUNNING TEST 14/14] Evidence operator overloading (==, !=, <<)..." << std::endl;
    {
        Evidence evA("E03", "Cracked Gold Pocket Watch", EvidenceCategory::PHYSICAL, EvidenceRelevance::CRITICAL_KEY, "Monogrammed V.C.", "Study Desk");
        Evidence evB("E03", "Cracked Gold Pocket Watch", EvidenceCategory::PHYSICAL, EvidenceRelevance::CRITICAL_KEY, "Monogrammed V.C.", "Study Desk");
        Evidence evC("E01", "Poisoned Whisky Glass", EvidenceCategory::FORENSIC, EvidenceRelevance::SUPPORTING_PROOF, "Cyanide residue", "Study Desk");

        // operator==
        assert(evA == evB);
        assert(!(evA == evC));

        // operator!=
        assert(evA != evC);
        assert(!(evA != evB));

        // operator<<
        std::stringstream ss;
        ss << evA;
        std::string outStr = ss.str();
        assert(outStr.find("E03") != std::string::npos);
        assert(outStr.find("Cracked Gold Pocket Watch") != std::string::npos);

        std::cout << "  [PASS] Evidence equality, inequality, and ostream operator overloading verified." << std::endl;
        passedCount++;
    }

    std::cout << "========================================================================" << std::endl;
    std::cout << "  ALL " << passedCount << "/" << totalTests << " TEST SUITES COMPLETED WITH ZERO ASSERTION FAILURES." << std::endl;
    std::cout << "========================================================================" << std::endl;
    return 0;
}
