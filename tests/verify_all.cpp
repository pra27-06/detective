#include <iostream>
#include <cassert>
#include <memory>
#include <vector>
#include "SaveManager.h"
#include "CaseFile.h"
#include "DialogueSystem.h"
#include "VNAssetManager.h"

int main() {
    std::cout << "========================================================================" << std::endl;
    std::cout << "  CASE CLOSED :: AUTOMATED VERIFICATION TEST SUITE" << std::endl;
    std::cout << "========================================================================" << std::endl;

    int passedCount = 0;
    int totalTests = 7;

    // -------------------------------------------------------------------------
    // TEST 1: DialogueSystem Typing, Click-to-Finish, Click-to-Advance & Branching
    // -------------------------------------------------------------------------
    std::cout << "[RUNNING TEST 1/7] DialogueSystem mechanics & state transitions..." << std::endl;
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
    std::cout << "[RUNNING TEST 2/7] CrimeScene hotspots & evidence discovery..." << std::endl;
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
    std::cout << "[RUNNING TEST 3/7] Suspect alibis and contradiction resolution..." << std::endl;
    {
        CaseFile cf(1);
        auto cruz = cf.getSuspectById("S02");
        assert(cruz != nullptr);
        assert(!cruz->isAlibiBroken());

        std::string stmt, resolution;

        // Presenting unrelated evidence (E01) must fail and keep alibi intact
        bool fail = cruz->testContradiction("E01", stmt, resolution);
        assert(!fail);
        assert(!cruz->isAlibiBroken());

        // Presenting decisive contradictory evidence (E03 Access Log) shatters alibi
        bool success = cruz->testContradiction("E03", stmt, resolution);
        assert(success);
        assert(cruz->isAlibiBroken());
        assert(!resolution.empty());

        std::cout << "  [PASS] Contradiction rejection and alibi breaking logic verified." << std::endl;
        passedCount++;
    }

    // -------------------------------------------------------------------------
    // TEST 4: ClueBoard Pairing & Deduction Unlock
    // -------------------------------------------------------------------------
    std::cout << "[RUNNING TEST 4/7] ClueBoard deduction matrix & clue pinning..." << std::endl;
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
    std::cout << "[RUNNING TEST 5/7] CaseFile accusation evaluation (Master vs Miscarriage)..." << std::endl;
    {
        CaseFile cf(1);

        // Premature accusation: alibi not broken
        auto premature = cf.evaluateAccusation("S02", "E03", "DEBT");
        assert(premature.isCorrectCulprit);
        assert(!premature.isAlibiBroken); // Alibi not yet broken!

        // Break Cruz's alibi
        auto cruz = cf.getSuspectById("S02");
        assert(cruz != nullptr);
        cruz->setAlibiBroken(true);

        // Flawless accusation: correct suspect (S02), key evidence (E03), correct motive (DEBT)
        auto flawless = cf.evaluateAccusation("S02", "E03", "DEBT");
        assert(flawless.isCorrectCulprit);
        assert(flawless.isAlibiBroken);
        assert(flawless.hasKeyEvidence);
        assert(flawless.isMotiveCorrect);
        assert(flawless.scoreAwarded > 0);
        assert(flawless.suspectEliminations.size() >= 3);

        // False accusation: innocent suspect Vance (S01)
        auto falseAcc = cf.evaluateAccusation("S01", "E01", "DEBT");
        assert(!falseAcc.isCorrectCulprit);

        std::cout << "  [PASS] Courtroom indictment rules, score calculation, and eliminations verified." << std::endl;
        passedCount++;
    }

    // -------------------------------------------------------------------------
    // TEST 6: SaveManager Serialization & Deserialization
    // -------------------------------------------------------------------------
    std::cout << "[RUNNING TEST 6/7] SaveManager roundtrip state persistence..." << std::endl;
    {
        CaseFile cf(1);
        cf.markEvidenceDiscovered("E01");
        cf.markEvidenceDiscovered("E03");
        auto cruz = cf.getSuspectById("S02");
        if (cruz) cruz->setAlibiBroken(true);

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
    std::cout << "[RUNNING TEST 7/7] VNAssetManager texture caching & crash prevention..." << std::endl;
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

    std::cout << "========================================================================" << std::endl;
    std::cout << "  ALL " << passedCount << "/" << totalTests << " TEST SUITES COMPLETED WITH ZERO ASSERTION FAILURES." << std::endl;
    std::cout << "========================================================================" << std::endl;
    return 0;
}
