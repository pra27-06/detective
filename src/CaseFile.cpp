#include "CaseFile.h"
#include "TerminalUI.h"
#include <iostream>
#include <algorithm>

CaseFile::CaseFile(int caseNumber) : m_caseNumber(caseNumber) {
    if (caseNumber == 1) {
        setupCase1();
    } else if (caseNumber == 2) {
        setupCase2();
    } else if (caseNumber == 3) {
        setupCase3();
    } else {
        setupCase1();
    }
}

void CaseFile::setupCase1() {
    m_caseId = "CASE-01";
    m_title = "The Poisoned Glass of Sterling Manor";
    m_tagline = "Lord Sterling is poisoned in his locked study during a thunderstorm blackout.";
    m_crimeTime = "11:15 PM, Friday";
    m_locationName = "Sterling Manor, Second Floor Study & Veranda";
    m_victimName = "Lord Arthur Sterling";

    m_culpritId = "S03"; // Vincent Croft
    m_keyEvidenceId = "E03"; // Cracked Gold Pocket Watch
    m_requiredContradictionId = "E03";
    m_correctMotiveKeyword = "DEBT";

    m_solutionExplanation = 
        "Speculative mining partner Vincent Croft faced prison and bankruptcy after Lord Arthur Sterling discovered "
        "his South African copper venture was fraudulent and demanded £20,000 repayment tonight. During dinner, Croft "
        "stole an empty vial of potassium cyanide from Dr. Bell's bag in the coat closet. When the thunderstorm blackout "
        "struck at 11:15 PM, Croft entered the study, poisoned Lord Sterling's whisky tumbler, and forced the safe to steal "
        "the 50-carat diamond brooch. As Lord Sterling collapsed, he grabbed Croft's wrist, smashing Croft's gold pocket watch "
        "and stopping it at 11:15 PM with blood traces. Croft's alibi of remaining on the dry veranda is completely shattered "
        "by the bloodstained watch monogrammed 'V.C.', Havana cigar ash on the windowsill, and size 10 yellow mud bootprints.";

    m_briefing = 
        "At 11:15 PM during a raging thunderstorm, the lights of Sterling Manor flickered and died. "
        "A chilling scream echoed from the locked second-floor study. Inside, Lord Arthur Sterling lies lifeless "
        "over his desk with a glass of poisoned whisky. His private wall safe stands wide open, and the 50-carat "
        "Sterling Diamond brooch is missing. Three suspects were in the manor. Cross-examine them, collect forensic "
        "clues, expose their lies, and unmask the killer!";

    // Timeline
    m_timeline.push_back({"10:30 PM", "Butler Higgins overhears violent threats as Lord Sterling demands Croft repay £20,000 or face prison."});
    m_timeline.push_back({"10:45 PM", "Lady Evelyn pleads with Lord Sterling regarding the will; Sterling deadbolts the study door."});
    m_timeline.push_back({"11:00 PM", "Butler Higgins delivers untainted Scotch tumbler to Lord Sterling; Dr. Bell discovers cyanide vial missing."});
    m_timeline.push_back({"11:15 PM", "Thunderstorm blackout strikes; scream heard; Lord Sterling poisoned; Croft's watch smashed at 11:15 PM."});
    m_timeline.push_back({"11:17 PM", "Croft claims he was smoking peacefully under the dry veranda awning without moving."});
    m_timeline.push_back({"11:25 PM", "Emergency lamps lit; Butler Higgins and Dr. Bell force study door; safe emptied and Lord Sterling dead."});

    // Suspect Eliminations
    m_eliminations.push_back({"S01", "Lady Evelyn Sterling", "Eliminated: Locked out of the study at 10:45 PM; Butler Higgins confirms she was weeping in the parlour during the 11:15 PM blackout."});
    m_eliminations.push_back({"S02", "Dr. Arthur Bell", "Eliminated: Cyanide was stolen from his coat bag during dinner; cellar wine records confirm his presence in the cellar during the blackout."});

    // Objectives
    m_objectives.push_back({"Explore all 3 crime scenes and search physical hotspots", false});
    m_objectives.push_back({"Discover at least 5 pieces of evidence", false});
    m_objectives.push_back({"Expose suspect contradictions during questioning", false});
    m_objectives.push_back({"Link clues on the Clue Board / Mind Palace to establish the poison and struggle", false});

    // Scenes
    CrimeScene studyScene("SCENE_01", "The Master Study (Crime Scene)", 
        "The second-floor locked study of Lord Arthur Sterling. Smells of bitter almonds, rain, and fresh Havana cigar smoke.");
    studyScene.addHotspot("Lord Sterling's Study Desk", 
        "A crystal whisky tumbler beside Lord Sterling with faint white residue on the rim smelling strongly of bitter almonds.", "E01");
    studyScene.addHotspot("Study Terrace Window Sill", 
        "Distinct grey-white cigar ash discovered on the study window sill. Matches premium imported Havana tobacco leaf.", "E02");
    studyScene.addHotspot("Persian Rug near Desk", 
        "A gold pocket watch with crystal glass smashed, frozen at exactly 11:15 PM. Bloodstained and engraved 'V.C.' on the back.", "E03");
    studyScene.addHotspot("Forced Wall Safe Alcove", 
        "Behind the mahogany wall panel: the heavy iron safe door stands wide open, stamped with a crimson wax seal.", "E09");
    m_scenes.push_back(studyScene);

    CrimeScene parlourScene("SCENE_02", "The Drawing Room & Parlour", 
        "A warm Victorian sitting room adorned with a marble fireplace and amber wall sconces.");
    parlourScene.addHotspot("Marble Fireplace Hearth", 
        "A scorched parchment fragment rescued from the hearth. It clearly revokes all inheritances to Lady Evelyn Sterling.", "E04");
    parlourScene.addHotspot("Sideboard & Bar Cart", 
        "The crystal whisky decanter served by Butler Higgins. Completely pure and untainted, proving poison was introduced in the study.", "E07");
    parlourScene.addHotspot("Emerald Velvet Armchair", 
        "A snagged green silk velvet thread on the chair's carving, matching the torn hem of Lady Evelyn's evening gown.", "E10");
    m_scenes.push_back(parlourScene);

    CrimeScene verandaScene("SCENE_03", "The Veranda & Garden Terrace", 
        "A stone balcony overlooking the rain-soaked gardens. Wet yellow mud covers the path under the study window.");
    verandaScene.addHotspot("Rain-Soaked Flowerbed", 
        "A deep footprint in the wet yellow garden clay directly beneath the study window, matching gentleman's riding boot, size 10.", "E06");
    verandaScene.addHotspot("Coat Rack / Medical Satchel", 
        "Dr. Bell's leather medical satchel hanging by the coat stand. Inside is an empty apothecary vial labeled Potassium Cyanide.", "E05");
    verandaScene.addHotspot("Garden Hedge Bush", 
        "Hidden deep inside the thorny boxwood hedge: the missing 50-carat Sterling Diamond brooch, discarded in haste!", "E08");
    m_scenes.push_back(verandaScene);

    // Evidence
    Evidence e1("E01", "Poisoned Whisky Glass", EvidenceCategory::FORENSIC, EvidenceRelevance::SUPPORTING_PROOF,
        "A crystal tumbler found beside the victim with faint white residue on the rim. Smells strongly of bitter almonds (cyanide).",
        "Lord Sterling's Study Desk (Study)", false, false);

    Evidence e2("E02", "Havana Cigar Ash on Sill", EvidenceCategory::PHYSICAL, EvidenceRelevance::SUPPORTING_PROOF,
        "Distinct grey-white cigar ash discovered on the study window sill. Matches premium imported Havana tobacco leaf.",
        "Study Terrace Window Sill (Study)", false, false);

    Evidence e3("E03", "Cracked Gold Pocket Watch", EvidenceCategory::PHYSICAL, EvidenceRelevance::CRITICAL_KEY,
        "A gold pocket watch with crystal glass smashed, frozen at exactly 11:15 PM. Bloodstained and engraved 'V.C.' on the back.",
        "Persian Rug near Desk (Study)", true, false);

    Evidence e4("E04", "Burnt Will in Fireplace", EvidenceCategory::DOCUMENT, EvidenceRelevance::SUPPORTING_PROOF,
        "A scorched parchment fragment rescued from the parlour hearth. It clearly revokes all inheritances to Lady Evelyn Sterling.",
        "Marble Fireplace Hearth (Drawing Room)", false, false);

    Evidence e5("E05", "Empty Cyanide Vial", EvidenceCategory::FORENSIC, EvidenceRelevance::SUPPORTING_PROOF,
        "A small amber apothecary bottle bearing Dr. Bell's prescription label. The rubber stopper is missing.",
        "Coat Rack / Medical Satchel (Veranda)", false, false);

    Evidence e6("E06", "Muddy Bootprint (Size 10)", EvidenceCategory::FORENSIC, EvidenceRelevance::SUPPORTING_PROOF,
        "A deep footprint in the wet yellow garden clay leading directly from under the study window toward the veranda.",
        "Rain-Soaked Flowerbed (Veranda)", false, false);

    Evidence e7("E07", "Untainted Scotch Decanter", EvidenceCategory::FORENSIC, EvidenceRelevance::DECEPTIVE_RED_HERRING,
        "Pure Highland whisky served by Butler Higgins. Proves the poison was not in the decanter and had to be added in the study.",
        "Sideboard & Bar Cart (Drawing Room)", false, true);

    Evidence e8("E08", "Stolen Diamond Brooch", EvidenceCategory::PHYSICAL, EvidenceRelevance::SUPPORTING_PROOF,
        "Lord Sterling's missing 50-carat antique diamond brooch, stolen from the wall safe and hidden in the garden hedge.",
        "Garden Hedge Bush (Veranda)", false, false);

    Evidence e9("E09", "Obsidian Hourglass Wax Seal", EvidenceCategory::DOCUMENT, EvidenceRelevance::DECEPTIVE_RED_HERRING,
        "A crimson wax stamp on the safe depicting a winged hourglass—the signature of an underground syndicate.",
        "Forced Wall Safe Alcove (Study)", false, true);

    Evidence e10("E10", "Torn Green Velvet Hem", EvidenceCategory::PHYSICAL, EvidenceRelevance::DECEPTIVE_RED_HERRING,
        "A torn green velvet thread from Lady Evelyn's dress on the parlour chair, confirming she collapsed weeping after being locked out.",
        "Emerald Velvet Armchair (Drawing Room)", false, true);

    m_allEvidence["E01"] = e1;
    m_allEvidence["E02"] = e2;
    m_allEvidence["E03"] = e3;
    m_allEvidence["E04"] = e4;
    m_allEvidence["E05"] = e5;
    m_allEvidence["E06"] = e6;
    m_allEvidence["E07"] = e7;
    m_allEvidence["E08"] = e8;
    m_allEvidence["E09"] = e9;
    m_allEvidence["E10"] = e10;

    // Suspects
    auto evelyn = std::make_shared<Suspect>("S01", "Lady Evelyn Sterling", "Victim's Widow & Socialite", 
        "Nervous, aristocratic, pale complexion, twisting emerald ring",
        "Good evening, Inspector. Please be brief; my nerves are completely shattered by this tragedy.",
        "I was in my private boudoir powdering my face from 11:00 PM until the scream at 11:25 PM.",
        "Money? Arthur was a generous husband. I had no reason to wish him dead!",
        false, "", "", "Butler Higgins confirms she was weeping in the parlour during the 11:15 PM blackout after Arthur locked her out.");
    evelyn->addTopic("alibi", "I was in my boudoir and parlour powdering my face and resting. I never entered the study during the blackout.");
    evelyn->addTopic("motive", "Arthur controlled the purse strings, but killing him would leave me at the mercy of solicitors.");
    evelyn->addContradiction("will", "Arthur and I were devoted! He would never cut me out of his estate!",
        "E04", "Lady Evelyn turns pale as a ghost: 'Arthur's will! Where did you find that?! Alright, yes! I went to his study at 10:45 PM to plead with him! He threw me out and locked the door behind me! When the blackout happened, I was downstairs crying! I am NOT a killer!'");
    evelyn->addConditionalTopic("dress", "E10", "Confront with Torn Green Velvet Thread (E10)",
        "Lady Evelyn touches her hem: 'Yes, that torn thread matches my dress. I tore it on the parlour armchair when I collapsed in tears after Arthur rejected me.'");

    auto bell = std::make_shared<Suspect>("S02", "Dr. Arthur Bell", "Family Physician & Surgeon",
        "Elderly, intellectual, polishing spectacles with trembling fingers",
        "Inspector, this is a tragedy. Lord Sterling's cardiovascular health was always precarious.",
        "I was down in the wine cellar selecting a bottle of vintage port for dinner when the power cut out.",
        "Arthur was my closest patient and patron. His grants funded my clinic.",
        false, "", "", "Cyanide was stolen from his medical bag during soup; cellar records verify his presence in the cellar during the blackout.");
    bell->addTopic("alibi", "I was in the wine cellar looking for vintage port when the storm caused the blackout.");
    bell->addTopic("motive", "I have devoted thirty years to saving lives, Detective. Why would I poison my benefactor?");
    bell->addContradiction("cyanide", "Lord Sterling must have suffered a sudden heart arrest. There was no poison in this house!",
        "E05", "Dr. Bell drops his spectacles in horror: 'My cyanide vial! Heavens above! Inspector, someone stole that vial from my leather satchel in the coat closet during dinner! Arthur was murdered with my stolen chemicals, but I swear I did not do it!'");
    bell->addConditionalTopic("poison", "E01", "Discuss Whisky Residue (E01)",
        "Dr. Bell shakes his head gravely: 'Bitter almonds... that is pure potassium cyanide. The thief who looted my satchel slipped it into Arthur's scotch.'");

    auto croft = std::make_shared<Suspect>("S03", "Vincent Croft", "Speculative Mining Investor & Partner",
        "Smug, sharp-eyed, smells of Havana cigars, boots caked in yellow mud",
        "Evening, Inspector. Dreadful business, but rich old men make many enemies, don't they?",
        "I was enjoying a peaceful Havana cigar outside on the veranda under the dry awning the entire evening. Never went near the study or garden.",
        "Arthur and I were partners! He was preparing to wire £20,000 to finance our South African copper venture tonight.",
        true,
        "Croft's smug grin instantly vanishes. His eyes bulge as he stares at the bloodstained watch in horror: 'You... where the devil did you dig that up?! Arthur grabbed my wrist as he went down... Dammit! He had millions and was going to throw me in prison over twenty thousand pounds!'",
        "Croft drops his face into his hands, weeping: 'I stole the poison from Bell's bag at dinner, slipped it in Arthur's whisky, and cracked the wall safe for the 50-carat diamond! He was supposed to fall asleep quietly, but he grabbed me as the storm hit! I had no choice!'",
        "Guilty: Primary perpetrator behind the poisoning and safe robbery.");
    croft->addTopic("alibi", "I stayed under the dry veranda awning the entire evening smoking Havana cigars. Check the dry stone tiles!");
    croft->addTopic("motive", "Arthur and I were on the verge of making a fortune in South African copper. I needed him alive!");
    croft->addContradiction("watch", "I was never inside Arthur's study during the blackout, and I never laid a finger on him!",
        "E03", "Croft freezes in terror as you present the gold watch: 'The hands are frozen at 11:15 PM, stained with Lord Sterling's blood, and engraved V.C. for Vincent Croft! Lord Sterling ripped it from your wrist as he died!' Croft's composure completely shatters.");
    croft->addContradiction("cigar", "I never approached the study window from the outside.",
        "E02", "You display the Havana cigar ash from the study sill: 'Vuelta Abajo tobacco ash was found on the study windowsill, dropped during the storm! You stood at the window!' Croft shifts nervously.");
    croft->addContradiction("mud", "I remained on the dry veranda awning all night without touching the rain or dirt.",
        "E06", "You point to Croft's riding boots: 'Size 10 riding boots left deep prints in the wet yellow garden clay under the study window, matching the yellow mud caked on your boots right now!' Croft turns pale.");
    croft->addConditionalTopic("brooch", "E08", "Confront with Stolen Diamond Brooch (E08)",
        "Croft sweats profusely: 'The 50-carat brooch... found in the hedge?! I... someone must have thrown it out during the panic!'");

    m_characters.push_back(evelyn);
    m_characters.push_back(bell);
    m_characters.push_back(croft);

    // Witness
    auto higgins = std::make_shared<Witness>("W01", "James Higgins", "Head Butler",
        "Dignified, composed, impeccable formal livery",
        "Good evening, Inspector. A dreadful tragedy has occurred in Sterling Manor.",
        "I served Lord Sterling a fresh glass of Scotch at 11:00 PM in the study; the decanter was pure. At 10:30 PM, I overheard Mr. Croft shouting violent threats through the study door. During the blackout, Lady Evelyn was weeping in the parlour. And Mr. Croft had yellow mud on his boots when he came back inside!",
        "11:15 PM", "E07");
    higgins->addTopic("threats", "At 10:30 PM, Lord Sterling shouted that Mr. Croft's mining venture was a complete fraud and demanded repayment of £20,000 by midnight.");
    higgins->addTopic("decanter", "The decanter on the sideboard was pure. The poison must have been introduced directly into Lord Sterling's glass in the study.");
    higgins->addTopic("boots", "Mr. Croft was tracking yellow mud across the parquet floor right after the storm passed.");
    m_characters.push_back(higgins);

    // ClueBoard deductions
    m_clueBoard.registerValidDeduction("E01", "E03", 
        "Cyanide Poisoning & 11:15 PM Struggle", 
        "Lord Sterling ingested the poisoned whisky and violently struggled with Vincent Croft at exactly 11:15 PM!");
    m_clueBoard.registerValidDeduction("E01", "E05", 
        "Stolen Chemical Lethality", 
        "Lord Sterling's whisky was laced with cyanide stolen from Dr. Bell's medical satchel during dinner.");
    m_clueBoard.registerValidDeduction("E02", "E06", 
        "Veranda Intrusion Traces", 
        "Havana cigar ash on the study sill and size 10 yellow mud bootprints directly contradict Croft's claim of remaining on the dry veranda.");
    m_clueBoard.registerValidDeduction("E03", "E08",
        "11:15 PM Struggle & Safe Robbery",
        "The watch smashed at 11:15 PM with 'V.C.' monogram and blood links Vincent Croft directly to the murder and safe theft.");
    m_clueBoard.registerValidDeduction("E04", "E10",
        "Disinheritance Alibi Confirmation",
        "The burnt will and torn velvet thread confirm Lady Evelyn was crying in the parlour after being locked out, clearing her.");

    // Mind Palace Cognitive Epiphanies
    m_mindPalace.registerEpiphanyFormula("EPI_C1_1", "The Stolen Poison & Struggle",
        "Lord Sterling was poisoned with Dr. Bell's stolen cyanide, and ripped Croft's watch off during the fatal struggle!",
        "E01", "E03", 50);
    m_mindPalace.registerEpiphanyFormula("EPI_C1_2", "Shattered Veranda Alibi",
        "Croft's yellow mud boots and Havana cigar ash prove he climbed into the study during the 11:15 PM blackout!",
        "E02", "E06", 50);
}

void CaseFile::setupCase2() {
    m_caseId = "CASE-02";
    m_title = "The Last Train Mystery";
    m_tagline = "A ruthless financier is poisoned inside an express train compartment.";
    m_crimeTime = "11:45 PM, Saturday";
    m_locationName = "Express Carriage #4, Trans-County Night Liner to Glenridge";
    m_victimName = "Victor Sterling (Rail & Shipping Magnate)";

    m_culpritId = "S01"; // Julian Gray
    m_keyEvidenceId = "E02"; // Antique Gold Eagle Cufflink
    m_requiredContradictionId = "E02";
    m_correctMotiveKeyword = "EMBEZZLEMENT";

    m_solutionExplanation = 
        "Julian Gray had embezzled £60,000 from Victor Sterling's transport accounts. Earlier that evening, "
        "Victor confronted Gray with an audit letter and promised to hand him over to Scotland Yard at Glenridge. "
        "Knowing his ruin was imminent, Gray wore his master's spare gold eagle cufflinks to impersonate Sterling "
        "when receiving the chamomile tea from Conductor Morris at 11:40 PM. He poisoned the cup with potassium cyanide "
        "and fled back to his compartment, violently tearing off his left cufflink in the door runner track.";

    m_briefing = 
        "Aboard the midnight express traversing the Blackwood Tunnel, tycoon Victor Sterling was found dead "
        "in private Compartment 4-A, slumped over an unfinished cup of chamomile tea. The train made no stops "
        "between Wyvern and Glenridge. The poison was fast-acting, and the killer must be in Carriage #4.";

    // Timeline
    m_timeline.push_back({"10:50 PM", "Express Night Liner departs Wyvern Station on express run to Glenridge."});
    m_timeline.push_back({"11:15 PM", "Nora Vance enters the Club Car and orders spirits, starting a continuous tab."});
    m_timeline.push_back({"11:25 PM", "Dr. Finch reads medical journals in Compartment 4-D."});
    m_timeline.push_back({"11:30 PM", "Victor Sterling confronts Julian Gray with the £60,000 embezzlement audit letter."});
    m_timeline.push_back({"11:35 PM", "Victor rings the carriage steward for chamomile tea."});
    m_timeline.push_back({"11:38 PM", "Chef Hayes brews fresh tea in the galley kettle (tested toxin-free)."});
    m_timeline.push_back({"11:40 PM", "Conductor Morris delivers tray; Gray accepts it wearing eagle cufflinks and poisons cup."});
    m_timeline.push_back({"11:45 PM", "Train enters Blackwood Tunnel; Victor drinks poisoned tea and dies instantly."});

    // Eliminations
    m_eliminations.push_back({"S02", "Nora Vance", "Eliminated: Continuous Club Car bar register and bartender testimony confirm she never left the lounge between 11:15 and 11:55 PM."});
    m_eliminations.push_back({"S03", "Dr. Alistair Finch", "Eliminated: His traveling medical satchel has 100% intact manufacturer wax seals on all poison ampoules."});
    m_eliminations.push_back({"S04", "Chef Gregory Hayes", "Eliminated: The galley kettle and tea canister were forensically tested and found completely pure and untainted."});

    // Objectives
    m_objectives.push_back({"Search Compartment 4-A, Doctor's Cabin, and the Club Car", false});
    m_objectives.push_back({"Discover at least 5 pieces of evidence", false});
    m_objectives.push_back({"Confront suspects with physical contradictions", false});
    m_objectives.push_back({"Connect clues on Clue Board to establish method of delivery", false});

    // Scenes
    CrimeScene compScene("SCENE_01", "Compartment 4-A (Crime Scene)", 
        "The luxurious mahogany-paneled compartment of Victor Sterling. The victim sits motionless.");
    compScene.addHotspot("Victim's Table & Teacup", 
        "A porcelain cup containing dregs smelling sharply of bitter almonds.", "E01");
    compScene.addHotspot("Sliding Door Runner Track", 
        "Wedged in the bronze runner track of the compartment door is a shiny metallic object.", "E02");
    compScene.addHotspot("Sterling's Leather Briefcase", 
        "The lock was picked. Inside is a torn draft regarding an embezzlement audit.", "E03");
    m_scenes.push_back(compScene);

    CrimeScene doctorScene("SCENE_02", "Compartment 4-D (Dr. Finch's Quarters)", 
        "A tidy compartment with medical textbooks, traveling valises, and an apothecary kit.");
    doctorScene.addHotspot("Black Medical Doctor's Bag", 
        "A leather satchel containing surgical tools and sealed medical ampoules.", "E04");
    doctorScene.addHotspot("Steward's Galley Cupboard", 
        "An unmarked glass bottle resting on a galley shelf near baking ingredients.", "E06");
    m_scenes.push_back(doctorScene);

    CrimeScene loungeScene("SCENE_03", "The Club Car (Dining & Lounge)", 
        "A velvet-lined lounge car at the rear of Carriage #4 with polished walnut tables.");
    loungeScene.addHotspot("Bar Order Register", 
        "The head bartender's log of beverages signed continuously throughout the journey.", "E05");
    loungeScene.addHotspot("Briefcase in Luggage Net", 
        "A folder containing a legal injunction draft regarding corporate shipping disputes.", "E07");
    loungeScene.addHotspot("Conductor's Service Delivery Sheet", 
        "A clipboard documenting room delivery orders, timestamps, and staff initials.", "E08");
    m_scenes.push_back(loungeScene);

    // Evidence
    Evidence e1("E01", "Teacup with Bitter Almond Residue", EvidenceCategory::FORENSIC, EvidenceRelevance::SUPPORTING_PROOF,
        "A porcelain teacup containing traces of chamomile tea and a lethal dose of potassium cyanide.",
        "Victim's Table (Compartment 4-A)", false, false);

    Evidence e2("E02", "Antique Gold Eagle Cufflink", EvidenceCategory::PHYSICAL, EvidenceRelevance::CRITICAL_KEY,
        "An 18k gold cufflink engraved with an eagle. Torn off a sleeve and dropped in the door track.",
        "Sliding Door Track (Compartment 4-A)", true, false);

    Evidence e3("E03", "Torn Embezzlement Audit Letter", EvidenceCategory::DOCUMENT, EvidenceRelevance::SUPPORTING_PROOF,
        "A letter from Sterling demanding Gray surrender to Scotland Yard at Glenridge for £60,000 theft.",
        "Leather Briefcase (Compartment 4-A)", false, false);

    Evidence e4("E04", "Dr. Finch's Sealed Ampoules", EvidenceCategory::FORENSIC, EvidenceRelevance::SUPPORTING_PROOF,
        "Dr. Finch's medical bag. Every poison ampoule has unbroken wax seals intact.",
        "Medical Bag (Compartment 4-D)", false, false);

    Evidence e5("E05", "Dining Car Bar Tab & Receipts", EvidenceCategory::DOCUMENT, EvidenceRelevance::SUPPORTING_PROOF,
        "Continuous register record showing Nora Vance was billed for drinks at 11:20, 11:35, and 11:50 PM.",
        "Bar Register (Club Car)", false, false);

    Evidence e6("E06", "Unmarked Galley Chemical Vial", EvidenceCategory::PHYSICAL, EvidenceRelevance::DECEPTIVE_RED_HERRING,
        "An amber glass bottle in Chef Hayes's pantry. Testing reveals high-strength concentrated vanilla extract.",
        "Galley Cupboard (Doctor Scene)", false, true);

    Evidence e7("E07", "Hostile Takeover Injunction Draft", EvidenceCategory::DOCUMENT, EvidenceRelevance::DECEPTIVE_RED_HERRING,
        "Legal papers drafted by Nora Vance seeking a court restraining order against Sterling's buyout.",
        "Luggage Net (Club Car)", false, true);

    Evidence e8("E08", "Conductor's Delivery Sheet", EvidenceCategory::DOCUMENT, EvidenceRelevance::SUPPORTING_PROOF,
        "Logs tea leaving galley at 11:38 PM, confirming poison was introduced in the corridor before delivery.",
        "Service Clipboard (Club Car)", false, false);

    m_allEvidence["E01"] = e1;
    m_allEvidence["E02"] = e2;
    m_allEvidence["E03"] = e3;
    m_allEvidence["E04"] = e4;
    m_allEvidence["E05"] = e5;
    m_allEvidence["E06"] = e6;
    m_allEvidence["E07"] = e7;
    m_allEvidence["E08"] = e8;

    // Suspects
    auto gray = std::make_shared<Suspect>("S01", "Julian Gray", "Personal Secretary",
        "Trembling, pale, continually adjusting his shirt cuffs",
        "Detective, I am utterly shattered! Mr. Sterling was like a father to me!",
        "I retired to Compartment 4-C at 11:30 PM to sleep. I never saw Mr. Sterling again until the train stopped.",
        "I was entirely dependent on Mr. Sterling for my livelihood and future.",
        true,
        "Gray's eyes bulge as you produce the eagle cufflink. He tugs at his left sleeve, revealing a frayed buttonhole! 'No... that cannot be!'",
        "Gray collapses into a chair, weeping. 'He found the offshore books. He was having inspectors arrest me at Glenridge! When the conductor walked up with his tea tray, I took it, slipped cyanide powder into the cup, and set it on his table while he was in the lavatory. I had no choice!'",
        "Guilty: Poisoned the tea to prevent being arrested at the destination.");
    gray->addTopic("alibi", "I was sound asleep in Compartment 4-C from 11:30 PM onwards. I never walked the corridor!");
    gray->addTopic("motive", "Why would I harm my benefactor? Without him, I have nothing.");
    gray->addContradiction("alibi", "I was sound asleep in Compartment 4-C from 11:30 PM onwards and never touched the door!",
        "E02", "You present the gold eagle cufflink found in the runner track: 'This was ripped from your shirt cuff at 11:40 PM when the tea was delivered!' Gray stares at his bare left wrist in horror.");
    gray->addContradiction("audit", "Mr. Sterling and I had a wonderful professional relationship with zero financial issues.",
        "E03", "You unfold the audit letter: 'This letter in Sterling's handwriting states he discovered your sixty-thousand-pound theft and arranged your arrest tonight!' Gray stammers helplessly.");
    gray->addConditionalTopic("cufflink", "E02", "Confront with Eagle Cufflink (E02)",
        "Gray breaks into a cold sweat: 'That cufflink... Mr. Sterling owned a matching set! He must have dropped it himself!'");

    auto nora = std::make_shared<Suspect>("S02", "Nora Vance", "Shipping Competitor",
        "Aristocratically poised, calculating, sipping ice water",
        "Sterling's corporate brutality earned him enemies everywhere, Detective. But I fight in courts, not with poisons.",
        "I spent the entire journey from 11:15 PM onward in the Club Car enjoying spirits with fellow travelers.",
        "Sterling was buying up my shipping debts to force an aggressive buyout.",
        false, "", "", "Bar register E05 and passengers confirm she never left the Club Car.");
    nora->addTopic("alibi", "Ask the bartender or passengers in the lounge. I never departed the Club Car between 11:15 PM and midnight.");
    nora->addTopic("motive", "I despised his commercial greed, yes. But murder would throw his conglomerate into receivership, harming our industry.");
    nora->addConditionalTopic("lawsuit", "E07", "Ask about Injunction Papers (E07)",
        "Nora smiles coolly: 'Yes, my attorneys were filing an injunction on Monday. That is how civilized businessmen resolve corporate disputes.'");

    auto finch = std::make_shared<Suspect>("S03", "Dr. Alistair Finch", "Physician",
        "Bespectacled, weary, holding a medical journal",
        "Good evening. I attempted resuscitation when the conductor shouted, but cyanosis had already taken hold.",
        "I was reading clinical journals in Compartment 4-D until the emergency whistle blew.",
        "Sterling sued my hospital board years ago, but the matter was settled long ago.",
        false, "", "", "Medical satchel E04 has unbroken manufacturer wax seals on all lethal drugs.");
    finch->addTopic("alibi", "I was in Compartment 4-D reading. The train's rhythm makes for excellent concentration.");
    finch->addTopic("poison", "The almond aroma is unmistakable. Cyanide arrests cellular respiration in seconds.");
    finch->addContradiction("bag", "I carry emergency drugs in my bag, but none of my supplies could possibly have been used.",
        "E04", "Finch opens his medical kit with steady hands: 'As you can verify, every single ampoule seal is intact from the Edinburgh laboratory. Nothing was opened.'");

    auto chef = std::make_shared<Suspect>("S04", "Gregory Hayes", "Train Galley Chef",
        "Stout, flour-dusted apron, defensive demeanor",
        "I cook the meals and boil the kettles, Officer! I don't meddle with what happens in passenger compartments!",
        "I was in the pantry galley preparing late supper trays from 11:15 to 11:50 PM.",
        "Sterling fired me last year from the Great Western line, but I bear no grudges.",
        false, "", "", "Galley kettle and tea tins tested negative for all poisons.");
    chef->addTopic("alibi", "I was at the galley stove boiling water. The conductor carried the tray out.");
    chef->addTopic("kettle", "My tea kettle was clean as a whistle. If there was poison, it was put in after it left my kitchen!");
    chef->addConditionalTopic("vial", "E06", "Ask about Galley Amber Vial (E06)",
        "Chef Hayes laughs nervously: 'That bottle? Smell it! It's pure vanilla extract for the bread pudding!'");

    m_characters.push_back(gray);
    m_characters.push_back(nora);
    m_characters.push_back(finch);
    m_characters.push_back(chef);

    // Witness
    auto morris = std::make_shared<Witness>("W01", "Conductor Morris", "Chief Train Conductor",
        "Crisp uniform, gold pocket watch, earnest expression",
        "A dreadful incident on my carriage, Detective. I pride myself on safety.",
        "At 11:40 PM, I delivered the chamomile tea ordered for Compartment 4-A. A gentleman opened the door wearing distinctive gold eagle cufflinks, took the tray, and said Mr. Sterling was resting.",
        "11:40 PM", "E02");
    morris->addTopic("cufflinks", "Those gold eagle cufflinks gleamed under my lantern. The person taking the tray had them on both wrists.");
    m_characters.push_back(morris);

    // ClueBoard deductions
    m_clueBoard.registerValidDeduction("E01", "E02", 
        "The Poisoner's Calling Card", 
        "The tea was accepted and poisoned by the person wearing the eagle cufflink dropped at the doorway.");
    m_clueBoard.registerValidDeduction("E02", "E03", 
        "Desperate Cover-Up", 
        "Julian Gray faced criminal ruin from the embezzlement audit, providing the exact urgent motive to kill before arrival.");
    m_clueBoard.registerValidDeduction("E01", "E08",
        "Corridor Poisoning Point",
        "The delivery sheet and pure galley kettle prove the cyanide was introduced into the cup in the hallway right at 11:40 PM.");

    // Mind Palace Cognitive Epiphanies
    m_mindPalace.registerEpiphanyFormula("EPI_C2_1", "The Embezzlement Poison Trap",
        "Julian Gray used potassium cyanide in the silver samovar to silence Sterling before the Glenridge audit!",
        "E01", "E02", 50);
    m_mindPalace.registerEpiphanyFormula("EPI_C2_2", "Desperate Cover-Up",
        "Julian Gray faced criminal ruin from the embezzlement audit, providing the exact urgent motive to kill before arrival.",
        "E02", "E03", 50);
}

void CaseFile::setupCase3() {
    m_caseId = "CASE-03";
    m_title = "The Locked Room Incident";
    m_tagline = "An eccentric clockmaker is shot dead behind bolted doors and barred windows.";
    m_crimeTime = "9:00 PM, Sunday";
    m_locationName = "The Clock Tower Study, Thorne Manor";
    m_victimName = "Thaddeus Thorne (Master Horologist)";

    m_culpritId = "S01"; // Cornelius Thorne
    m_keyEvidenceId = "E01"; // Friction-Scorched Piano Wire & Ice Residue
    m_requiredContradictionId = "E02"; // Ballpoint signature on suicide note
    m_correctMotiveKeyword = "INHERITANCE";

    m_solutionExplanation = 
        "Cornelius Thorne shot his uncle at 8:15 PM after discovering he was being disinherited. To stage an impossible "
        "locked-room suicide, Cornelius used horological trickery: he looped high-tensile piano wire around the interior slide bolt, "
        "threaded it through the door seam, and tied it to a counterweight held by an ice block inside the grandfather clock. "
        "At 9:00 PM, while Cornelius was openly drinking at the Black Bull Tavern, the clock chimed, releasing the falling weight, "
        "which pulled the iron bolt shut from the outside. The ice melted into a puddle, leaving behind only scorch marks.";

    m_briefing = 
        "At 9:30 PM, police responded to a gunshot report at Thorne Manor. The third-floor study door "
        "was locked from the inside with a heavy iron slide bolt. The high window was sealed with iron "
        "bars. Police had to batter down the door. Inside, master clockmaker Thaddeus Thorne was dead "
        "at his desk with a bullet wound to the chest and a suicide note beside him. Every locked room has a secret.";

    // Timeline
    m_timeline.push_back({"07:30 PM", "Butler Silas Vance serves Thorne chamomile tea in the clock tower study."});
    m_timeline.push_back({"07:45 PM", "Cornelius Thorne arrives; angry shouting heard regarding patent revocations."});
    m_timeline.push_back({"08:00 PM", "Locksmith Miriam Black arrives to demand royalties; departs at 8:15 PM."});
    m_timeline.push_back({"08:15 PM", "Cornelius shoots Thaddeus; types suicide note with modern ballpoint signature."});
    m_timeline.push_back({"08:20 PM", "Cornelius rigs piano wire from bolt to grandfather clock weight with ice counter-release."});
    m_timeline.push_back({"08:30 PM", "Cornelius arrives at Black Bull Tavern, ordering pints and establishing public alibi."});
    m_timeline.push_back({"09:00 PM", "Grandfather clock chimes 9:00 PM; ice releases weight, sliding the door bolt shut."});
    m_timeline.push_back({"09:30 PM", "Constable Higgins arrives and splinters the door frame to enter."});

    // Eliminations
    m_eliminations.push_back({"S02", "Miriam Black", "Eliminated: Omnibus conductor ticket stub E08 confirms she boarded the 8:30 PM omnibus at the crossroads."});
    m_eliminations.push_back({"S03", "Silas Vance", "Eliminated: Verified polishing 24 pieces of estate silver in pantry; severe arthritis makes high-tension wire manipulation impossible."});
    m_eliminations.push_back({"S04", "Evelyn Cross", "Eliminated: Western Union telegraph dispatch confirms she was in London drafting legal briefs at 8:45 PM."});

    // Objectives
    m_objectives.push_back({"Examine the Study Door, Grandfather Clock, and Manor Hall", false});
    m_objectives.push_back({"Discover at least 5 pieces of evidence", false});
    m_objectives.push_back({"Expose contradictions in Cornelius's locked room suicide story", false});
    m_objectives.push_back({"Connect clues on Clue Board to unravel the clockwork mechanism", false});

    // Scenes
    CrimeScene studyScene("SCENE_01", "The Third Floor Clock Tower Study", 
        "A grand room filled with ticking pendulums, brass gears, and horological tools. Gunsmoke lingers.");
    studyScene.addHotspot("The Heavy Oak Door & Bolt", 
        "The splintered door frame. The iron slide bolt has a scorched groove and a small puddle of water underneath.", "E01");
    studyScene.addHotspot("Thorne's Desk & Suicide Note", 
        "A typed note reads: 'Forgive me, my work is complete.' The signature is in modern blue ballpoint ink.", "E02");
    studyScene.addHotspot("Discarded Teacup on Mantel", 
        "A teacup containing herbal residue. Smells strongly of calming valerian root.", "E07");
    m_scenes.push_back(studyScene);

    CrimeScene clockScene("SCENE_02", "The Grandfather Chime Clock", 
        "A towering eight-foot horological clock ticking steadily near the study door.");
    clockScene.addHotspot("Internal Weight Escapement Reel", 
        "Inside the clock casing, an auxiliary wire spool has been spliced into the 9:00 PM chime hammer mechanism.", "E03");
    clockScene.addHotspot("Manor Veranda Flagstones", 
        "A set of specialized lock skeleton picks lying near the garden flowerbed.", "E06");
    m_scenes.push_back(clockScene);

    CrimeScene foyerScene("SCENE_03", "The Manor Entrance Hall & Wardrobe", 
        "The grand entrance hall where visitors leave their coats and canes.");
    foyerScene.addHotspot("Cornelius's Discarded Greatcoat", 
        "In the pocket of Cornelius Thorne's tweed coat: a coil of 0.5mm piano wire and watchmaker's wax.", "E04");
    foyerScene.addHotspot("Writing Bureau in the Hall", 
        "A legal draft from Thorne's solicitor revoking Cornelius's inheritance and patent co-ownership.", "E05");
    foyerScene.addHotspot("Side Table Mail Tray", 
        "An omnibus passenger ticket stub stamped at 8:30 PM from the local crossroad turnpike.", "E08");
    m_scenes.push_back(foyerScene);

    // Evidence
    Evidence e1("E01", "Friction-Scorched Latch & Water Puddle", EvidenceCategory::PHYSICAL, EvidenceRelevance::CRITICAL_KEY,
        "A scorched horizontal line on the interior slide-bolt, accompanied by a small pool of melted ice water on the floor.",
        "Heavy Oak Door (Study)", true, false);

    Evidence e2("E02", "Ballpoint Signed Suicide Note", EvidenceCategory::FORENSIC, EvidenceRelevance::SUPPORTING_PROOF,
        "A typed suicide note signed in modern ballpoint ink. Thorne was an antiquarian who strictly wrote in Prussian blue fountain ink.",
        "Desk (Study)", false, false);

    Evidence e3("E03", "Modified Clockwork Tension Reel", EvidenceCategory::FORENSIC, EvidenceRelevance::SUPPORTING_PROOF,
        "A brass escapement reel rigged to the clock's strike weight, capable of exerting 30 lbs of tension at 9:00 PM.",
        "Internal Clock Reel (Grandfather Clock)", false, false);

    Evidence e4("E04", "Coil of Piano Wire & Wax", EvidenceCategory::PHYSICAL, EvidenceRelevance::SUPPORTING_PROOF,
        "A coil of high-tensile 0.5mm piano wire and hardened watchmaker wax found inside Cornelius's coat.",
        "Greatcoat Pocket (Foyer)", false, false);

    Evidence e5("E05", "Solicitor's Disinheritance Draft", EvidenceCategory::DOCUMENT, EvidenceRelevance::SUPPORTING_PROOF,
        "A draft revoking Cornelius's apprenticeship, estate inheritance, and patent rights effective Monday morning.",
        "Writing Bureau (Foyer)", false, false);

    Evidence e6("E06", "Miriam's Skeleton Pick Key", EvidenceCategory::PHYSICAL, EvidenceRelevance::DECEPTIVE_RED_HERRING,
        "A set of locksmith skeleton picks dropped on the veranda. Cannot throw an interior slide bolt, only tumbler locks.",
        "Veranda Flagstones (Clock Scene)", false, true);

    Evidence e7("E07", "Valerian Sedative Teacup", EvidenceCategory::FORENSIC, EvidenceRelevance::DECEPTIVE_RED_HERRING,
        "A teacup containing valerian root tea brewed by Silas for Thorne's insomnia. Non-toxic sleeping remedy.",
        "Mantel Teacup (Study)", false, true);

    Evidence e8("E08", "Omnibus Stamped Ticket Stub", EvidenceCategory::DOCUMENT, EvidenceRelevance::SUPPORTING_PROOF,
        "Ticket stub stamped 8:30 PM at the crossroads, verifying Miriam Black's prompt departure.",
        "Side Table Mail Tray (Foyer)", false, false);

    m_allEvidence["E01"] = e1;
    m_allEvidence["E02"] = e2;
    m_allEvidence["E03"] = e3;
    m_allEvidence["E04"] = e4;
    m_allEvidence["E05"] = e5;
    m_allEvidence["E06"] = e6;
    m_allEvidence["E07"] = e7;
    m_allEvidence["E08"] = e8;

    // Suspects
    auto cornelius = std::make_shared<Suspect>("S01", "Cornelius Thorne", "Nephew & Apprentice",
        "Arrogant, fidgety, smelling of alcohol and pipe smoke",
        "My uncle was an eccentric genius, Detective. He chose to end his own life behind a locked door!",
        "I was at the Black Bull Tavern from 8:30 PM until 9:45 PM. Ten patrons saw me drinking when the shot supposedly occurred!",
        "Uncle Thaddeus was my mentor. I stood to inherit his workshop and continue our horological patents.",
        true,
        "Cornelius stammers as you reveal the ballpoint signature: 'A pen? What does a pen prove?! Anyone could have given him that pen!'",
        "Cornelius covers his face with trembling hands. 'He was throwing me onto the street on Monday! Disinheriting me after ten years of unpaid work! I shot him at 8:15 PM, typed the note, rigged the piano wire around the bolt and clock weight with an ice block, and walked out. At 9:00 PM, while I drank openly in the tavern, the clock struck and locked the room from the inside!'",
        "Guilty: Murdered uncle over disinheritance and staged the locked room.");
    cornelius->addTopic("alibi", "I have an ironclad alibi! I was at the Black Bull tavern drinking from 8:30 to 9:45 PM. The door was locked from inside!");
    cornelius->addTopic("motive", "I loved the old man. Why would I kill him when I was his sole heir?");
    cornelius->addContradiction("suicide", "My uncle wrote that suicide note at his desk and signed it before ending his life.",
        "E02", "You point to the ink: 'Your uncle wrote exclusively with antique Prussian blue fountain ink. This note is signed in cheap modern ballpoint ink—the exact ballpoint found in your satchel!' Cornelius blanches.");
    cornelius->addContradiction("locked", "No one could have locked that heavy slide bolt from outside! It is physically impossible!",
        "E01", "You present the scorched bolt and water puddle: 'The friction groove and melted ice prove a tension wire pulled the bolt shut when the clock struck 9:00 PM!'");
    cornelius->addConditionalTopic("wire", "E04", "Confront with Piano Wire in Coat (E04)",
        "Cornelius sweats profusely: 'I... I am a clockmaker! Piano wire is used in chime hammers! That proves nothing!'");

    auto miriam = std::make_shared<Suspect>("S02", "Miriam Black", "Master Locksmith",
        "Calculating, observant, hands calloused from metalwork",
        "I build locks for banking vaults, Detective. A simple slide bolt is child's play.",
        "I left the manor at 8:15 PM after discussing royalties with Thorne, and took the 8:30 omnibus.",
        "Thorne withheld royalties on our combination safe patent.",
        false, "", "", "Omnibus ticket stub E08 verifies her departure at 8:30 PM.");
    miriam->addTopic("alibi", "I caught the 8:30 PM omnibus at the crossroads. The conductor has my ticket stamp.");
    miriam->addTopic("motive", "We argued over money, yes. But dead men cannot sign royalty checks.");
    miriam->addConditionalTopic("picks", "E06", "Ask about Dropped Skeleton Picks (E06)",
        "Miriam sighs: 'I dropped those picks when I rushed down the veranda steps. But look at them: they are tumbler picks. They cannot throw an interior slide bolt.'");

    auto silas = std::make_shared<Suspect>("S03", "Silas Vance", "House Butler",
        "Elderly, dignified, deeply shaken",
        "I have served the Master for forty years. This house has lost its heartbeat.",
        "I served tea at 7:30 PM, then remained in the basement pantry polishing silver all evening.",
        "I had no dispute with the Master; he treated me with utmost dignity.",
        false, "", "", "Silverware inventory confirms 4 hours of polishing; severe arthritis prevents wire manipulation.");
    silas->addTopic("alibi", "I was downstairs in the pantry all evening. I only came up when the constable knocked.");
    silas->addTopic("tea", "I brought the Master valerian tea for his nerves at 7:30 PM.");
    silas->addContradiction("dispute", "The Master had a calm and peaceful evening with no arguments.",
        "E05", "You present the disinheritance draft: 'Mr. Thorne was in a fury, preparing to disinherit Cornelius and remove his patent rights!' Silas lowers his eyes: 'Forgive me, Detective... they were shouting terribly at 7:45 PM.'");

    auto cross = std::make_shared<Suspect>("S04", "Evelyn Cross", "Thorne's Patent Attorney",
        "Sharp, sharply tailored suit, brisk professional manner",
        "Good evening, Detective. I represent Mr. Thorne's legal interests.",
        "I was in my central London office preparing patent filings and sent telegraphs throughout the evening.",
        "Thorne was auditing our firm's patent trust accounts.",
        false, "", "", "Western Union telegrams establish her presence in London at 8:45 PM.");
    cross->addTopic("alibi", "I was in London all evening. My telegraph receipts verify my location.");
    cross->addTopic("motive", "Mr. Thorne was an important client. His death is a severe professional loss.");

    m_characters.push_back(cornelius);
    m_characters.push_back(miriam);
    m_characters.push_back(silas);
    m_characters.push_back(cross);

    // Witness
    auto higgins = std::make_shared<Witness>("W01", "Constable Higgins", "Responding Officer",
        "Pompous, breathless, notebook in hand",
        "I broke the door myself, Detective! I know what I saw!",
        "When we battered down the door with the ram, the iron slide bolt was still securely seated in the brass wall receiver. No human could have slipped out after that bolt was thrown!",
        "9:30 PM", "E01");
    higgins->addTopic("bolt", "The bolt was pushed all the way across. We had to crack the door frame to get in.");
    m_characters.push_back(higgins);

    // ClueBoard deductions
    m_clueBoard.registerValidDeduction("E01", "E03", 
        "The Mechanical Locked-Room Trick", 
        "The clock chime weight pulled the wire looped around the bolt, locking the door automatically from outside when the clock struck 9:00 PM.");
    m_clueBoard.registerValidDeduction("E02", "E04", 
        "The Staged Suicide", 
        "Cornelius forged the suicide note with a modern ballpoint pen and had the matching piano wire in his coat to execute the trap.");
    m_clueBoard.registerValidDeduction("E01", "E05",
        "Premeditated Retaliation",
        "The disinheritance draft provides the urgent motive for Cornelius to kill Thorne before Monday's legal execution.");

    // Mind Palace Cognitive Epiphanies
    m_mindPalace.registerEpiphanyFormula("EPI_C3_1", "The Mechanical Locked-Room Trick",
        "The clock chime weight pulled the wire looped around the bolt, locking the door automatically from outside when the clock struck 9:00 PM.",
        "E01", "E03", 50);
    m_mindPalace.registerEpiphanyFormula("EPI_C3_2", "The Staged Suicide",
        "Cornelius forged the suicide note with a modern ballpoint pen and had the matching piano wire in his coat to execute the trap.",
        "E02", "E04", 50);
}

int CaseFile::getCaseNumber() const { return m_caseNumber; }
std::string CaseFile::getCaseId() const { return m_caseId; }
std::string CaseFile::getTitle() const { return m_title; }
std::string CaseFile::getTagline() const { return m_tagline; }
std::string CaseFile::getBriefing() const { return m_briefing; }
std::string CaseFile::getTimeOfCrime() const { return m_crimeTime; }
std::string CaseFile::getLocationName() const { return m_locationName; }
std::string CaseFile::getVictimName() const { return m_victimName; }

const std::vector<std::shared_ptr<Character>>& CaseFile::getCharacters() const {
    return m_characters;
}

std::shared_ptr<Character> CaseFile::getCharacterById(const std::string& id) const {
    for (const auto& c : m_characters) {
        if (c->getId() == id) return c;
    }
    return nullptr;
}

std::shared_ptr<Suspect> CaseFile::getSuspectById(const std::string& id) const {
    for (const auto& c : m_characters) {
        if (c->getId() == id) {
            return std::dynamic_pointer_cast<Suspect>(c);
        }
    }
    return nullptr;
}

std::shared_ptr<Witness> CaseFile::getWitnessById(const std::string& id) const {
    for (const auto& c : m_characters) {
        if (c->getId() == id) {
            return std::dynamic_pointer_cast<Witness>(c);
        }
    }
    return nullptr;
}

std::vector<CrimeScene>& CaseFile::getScenes() { return m_scenes; }
const std::vector<CrimeScene>& CaseFile::getScenes() const { return m_scenes; }

CrimeScene* CaseFile::getSceneById(const std::string& sceneId) {
    for (auto& s : m_scenes) {
        if (s.getId() == sceneId) return &s;
    }
    return nullptr;
}

std::map<std::string, Evidence>& CaseFile::getAllEvidence() { return m_allEvidence; }
const std::map<std::string, Evidence>& CaseFile::getAllEvidence() const { return m_allEvidence; }

Evidence* CaseFile::getEvidenceById(const std::string& id) {
    auto it = m_allEvidence.find(id);
    if (it != m_allEvidence.end()) {
        return &(it->second);
    }
    return nullptr;
}

std::vector<Evidence> CaseFile::getDiscoveredEvidence() const {
    std::vector<Evidence> disc;
    for (const auto& pair : m_allEvidence) {
        if (pair.second.isDiscovered()) {
            disc.push_back(pair.second);
        }
    }
    return disc;
}

std::vector<std::string> CaseFile::getDiscoveredEvidenceIds() const {
    std::vector<std::string> ids;
    for (const auto& pair : m_allEvidence) {
        if (pair.second.isDiscovered()) {
            ids.push_back(pair.second.getId());
        }
    }
    return ids;
}

bool CaseFile::markEvidenceDiscovered(const std::string& id) {
    auto it = m_allEvidence.find(id);
    if (it != m_allEvidence.end()) {
        if (!it->second.isDiscovered()) {
            it->second.setDiscovered(true);
            m_clueBoard.addPinnedEvidence(id);
            updateObjectives();
            return true;
        }
    }
    return false;
}

ClueBoard& CaseFile::getClueBoard() { return m_clueBoard; }
const ClueBoard& CaseFile::getClueBoard() const { return m_clueBoard; }

void CaseFile::updateObjectives() {
    bool allScenesExamined = true;
    for (const auto& s : m_scenes) {
        if (!s.isFullyExamined()) allScenesExamined = false;
    }
    if (m_objectives.size() > 0) m_objectives[0].completed = allScenesExamined;

    size_t discCount = getDiscoveredEvidence().size();
    if (m_objectives.size() > 1) m_objectives[1].completed = (discCount >= 5);

    size_t brokenAlibis = 0;
    for (const auto& c : m_characters) {
        auto susp = std::dynamic_pointer_cast<Suspect>(c);
        if (susp && susp->getResolvedContradictionCount() > 0) brokenAlibis++;
    }
    if (m_objectives.size() > 2) m_objectives[2].completed = (brokenAlibis >= 1);

    size_t dedCount = m_clueBoard.getUnlockedDeductionCount();
    if (m_objectives.size() > 3) m_objectives[3].completed = (dedCount >= 1);
}

const std::vector<CaseObjective>& CaseFile::getObjectives() const { return m_objectives; }
const std::vector<TimelineEvent>& CaseFile::getTimeline() const { return m_timeline; }
const std::vector<SuspectElimination>& CaseFile::getEliminations() const { return m_eliminations; }

AccusationResult CaseFile::evaluateAccusation(const std::string& suspectId,
                                            const std::string& evidenceId,
                                            const std::string& motiveKeyword) {
    AccusationResult res;
    res.isCorrectCulprit = (suspectId == m_culpritId);
    res.hasKeyEvidence = (evidenceId == m_keyEvidenceId);

    auto susp = getSuspectById(suspectId);
    res.isAlibiBroken = (susp != nullptr && susp->isAlibiBroken());

    std::string upperMotive = UI::toUpper(motiveKeyword);
    res.isMotiveCorrect = (upperMotive.find(m_correctMotiveKeyword) != std::string::npos);

    res.chronologicalTimeline = m_timeline;
    res.suspectEliminations = m_eliminations;

    // Collect resolved contradictions
    for (const auto& c : m_characters) {
        auto s = std::dynamic_pointer_cast<Suspect>(c);
        if (s) {
            for (const auto& cont : s->getContradictions()) {
                if (cont.resolved) {
                    res.resolvedContradictions.push_back(s->getName() + ": \"" + cont.statementQuote + 
                        "\" disproved with [" + cont.evidenceId + "].");
                }
            }
        }
    }

    if (res.isCorrectCulprit && res.hasKeyEvidence && res.isAlibiBroken && res.isMotiveCorrect) {
        res.scoreAwarded = 250;
        res.verdictTitle = "MASTER DETECTIVE — FLAWLESS INDICTMENT!";
        res.outcomeDescription = 
            "Your deduction was airtight. With the decisive key evidence presented, the shattered alibi, "
            "and the unassailable motive laid bare before the Magistrate, the accused completely broke down "
            "and gave a full written confession. Justice is fully served!";
        res.reportDetails.push_back("Culprit Accused: Correct (" + (susp ? susp->getName() : suspectId) + ")");
        res.reportDetails.push_back("Decisive Evidence: Verified (" + evidenceId + ")");
        res.reportDetails.push_back("Alibi Destruction: Successful");
        res.reportDetails.push_back("Motive Theory: Accurately established (" + m_correctMotiveKeyword + ")");
        res.reportDetails.push_back("");
        res.reportDetails.push_back("CASE SUMMARY & TRUTH:");
        res.reportDetails.push_back(m_solutionExplanation);
    } else if (res.isCorrectCulprit && (!res.hasKeyEvidence || !res.isAlibiBroken)) {
        res.scoreAwarded = 100;
        res.verdictTitle = "PARTIAL JUSTICE — CONTROVERSIAL CONVICTION";
        res.outcomeDescription = 
            "You correctly identified the perpetrator, but your evidentiary foundation had critical gaps! "
            "The defense lawyer argued reasonable doubt due to missing conclusive physical proof or an unbroken alibi. "
            "While the suspect is detained on secondary charges, the prosecution was forced to accept a plea deal.";
        res.reportDetails.push_back("Culprit Accused: Correct (" + (susp ? susp->getName() : suspectId) + ")");
        res.reportDetails.push_back("Decisive Evidence: " + std::string(res.hasKeyEvidence ? "Verified" : "Missing / Secondary Clue"));
        res.reportDetails.push_back("Alibi Destruction: " + std::string(res.isAlibiBroken ? "Broken" : "Left Intact (Created Doubt)"));
        res.reportDetails.push_back("");
        res.reportDetails.push_back("CASE SUMMARY & TRUTH:");
        res.reportDetails.push_back(m_solutionExplanation);
    } else {
        res.scoreAwarded = 0;
        res.verdictTitle = "MISCARRIAGE OF JUSTICE — CASE COLLAPSE";
        res.outcomeDescription = 
            "Disaster! You pointed the finger of accusation at an innocent person! "
            "Their defense easily dismantled your theory in court, and they were acquitted with full apologies from the department. "
            "Meanwhile, the true culprit slipped away into the night, free of suspicion.";
        res.reportDetails.push_back("Culprit Accused: INCORRECT (" + (susp ? susp->getName() : suspectId) + ")");
        res.reportDetails.push_back("Result: Department reprimand & false arrest liability.");
        res.reportDetails.push_back("");
        res.reportDetails.push_back("CASE SUMMARY & TRUTH:");
        res.reportDetails.push_back(m_solutionExplanation);
    }

    return res;
}

void CaseFile::displayCaseHeader() const {
    std::cout << UI::Color::BRIGHT_YELLOW << UI::Color::BOLD
              << "[" << m_caseId << "] " << m_title << UI::Color::RESET << "\n";
    std::cout << UI::Color::CYAN << "  " << m_tagline << UI::Color::RESET << "\n";
    std::cout << UI::Color::DIM << "  Time: " << m_crimeTime 
              << " | Scene: " << m_locationName 
              << " | Victim: " << m_victimName << UI::Color::RESET << "\n";
    UI::printDivider('-', 74);
}

void CaseFile::displayBriefingScreen(const std::string& difficultyName) const {
    std::vector<std::string> lines;
    lines.push_back("CASE FILE:    " + m_caseId + " - " + m_title);
    lines.push_back("DIFFICULTY:   " + difficultyName);
    lines.push_back("CRIME SCENE:  " + m_locationName);
    lines.push_back("TIME OF ACT:  " + m_crimeTime);
    lines.push_back("VICTIM/LOSS:  " + m_victimName);
    lines.push_back("");
    lines.push_back("OFFICIAL INCIDENT REPORT:");
    lines.push_back(m_briefing);
    lines.push_back("");
    lines.push_back("PRIMARY INVESTIGATION OBJECTIVES:");
    for (size_t i = 0; i < m_objectives.size(); ++i) {
        std::string status = m_objectives[i].completed ? "[COMPLETED]" : "[PENDING]";
        lines.push_back("  (" + std::to_string(i + 1) + ") " + m_objectives[i].description + " " + status);
    }
    UI::printBox("SCOTLAND YARD CASE DISPATCH", lines, 74);
}

void CaseFile::displaySuspectsList() const {
    std::vector<std::string> lines;
    for (const auto& c : m_characters) {
        auto susp = std::dynamic_pointer_cast<Suspect>(c);
        if (susp) {
            std::string alibiStatus = susp->isAlibiBroken() ? "[ALIBI SHATTERED]" : "[ALIBI UNBROKEN]";
            lines.push_back("[" + susp->getId() + "] " + susp->getName() + " - " + susp->getOccupation() + " " + alibiStatus);
        }
    }
    UI::printBox("REGISTERED SUSPECTS", lines, 74);
}

void CaseFile::displayWitnessesList() const {
    std::vector<std::string> lines;
    for (const auto& c : m_characters) {
        auto wit = std::dynamic_pointer_cast<Witness>(c);
        if (wit) {
            std::string status = wit->hasGivenTestimony() ? "[TESTIMONY RECORDED]" : "[NOT INTERVIEWED]";
            lines.push_back("[" + wit->getId() + "] " + wit->getName() + " - " + wit->getOccupation() + " " + status);
        }
    }
    UI::printBox("REGISTERED WITNESSES", lines, 74);
}

void CaseFile::displayDiscoveredEvidenceList() const {
    auto disc = getDiscoveredEvidence();
    if (disc.empty()) {
        UI::printInfo("No evidence discovered yet. Search crime scene hotspots!");
        return;
    }
    std::cout << "\n" << UI::Color::BOLD << UI::Color::BRIGHT_YELLOW 
              << "--- DISCOVERED EVIDENCE INVENTORY (" << disc.size() << " / " << m_allEvidence.size() << ") ---" << UI::Color::RESET << "\n";
    for (const auto& e : disc) {
        e.displaySummary();
    }
    std::cout << "\n";
}

void CaseFile::displayProgress() const {
    size_t discEv = getDiscoveredEvidence().size();
    size_t totalEv = m_allEvidence.size();
    size_t brokenAlibis = 0;
    size_t totalSuspects = 0;
    for (const auto& c : m_characters) {
        auto s = std::dynamic_pointer_cast<Suspect>(c);
        if (s) {
            totalSuspects++;
            if (s->isAlibiBroken()) brokenAlibis++;
        }
    }
    size_t deductions = m_clueBoard.getUnlockedDeductionCount();
    size_t totalDeductions = m_clueBoard.getTotalDeductionsCount();

    size_t completedObjs = 0;
    for (const auto& obj : m_objectives) {
        if (obj.completed) completedObjs++;
    }

    std::cout << UI::Color::CYAN << "[PROGRESS] " << UI::Color::RESET
              << "Clues: " << UI::Color::BOLD << discEv << "/" << totalEv << UI::Color::RESET << " | "
              << "Alibis Broken: " << UI::Color::BOLD << brokenAlibis << "/" << totalSuspects << UI::Color::RESET << " | "
              << "Deductions: " << UI::Color::BOLD << deductions << "/" << totalDeductions << UI::Color::RESET << " | "
              << "Objectives: " << UI::Color::BOLD << completedObjs << "/" << m_objectives.size() << UI::Color::RESET << "\n";
}

void CaseFile::displayDeductionMatrix() const {
    std::vector<std::string> lines;
    lines.push_back("=== DEDUCTION MATRIX & EVIDENCE CONNECTIONS ===");
    lines.push_back("");
    lines.push_back("[SECTION A: CONFIRMED PHYSICAL FACTS]");
    auto disc = getDiscoveredEvidence();
    if (disc.empty()) {
        lines.push_back("  (No confirmed physical facts recovered yet.)");
    } else {
        for (const auto& e : disc) {
            lines.push_back("  [*] " + e.getId() + ": " + e.getName() + " (" + e.getRelevanceName() + ")");
        }
    }
    lines.push_back("");
    lines.push_back("[SECTION B: DEDUCTIVE BREAKTHROUGHS & THEORIES]");
    const auto& deds = m_clueBoard.getDeductions();
    size_t dedCount = 0;
    for (const auto& d : deds) {
        if (d.discovered) {
            dedCount++;
            lines.push_back("  [LINK] " + d.id1 + " <---> " + d.id2 + " :: " + d.title);
            lines.push_back("         -> " + d.insight);
        }
    }
    if (dedCount == 0) {
        lines.push_back("  (No clue connections formed yet. Use Clue Board to link clues.)");
    }
    lines.push_back("");
    lines.push_back("[SECTION C: EXPOSED SUSPECT CONTRADICTIONS]");
    size_t contCount = 0;
    for (const auto& c : m_characters) {
        auto s = std::dynamic_pointer_cast<Suspect>(c);
        if (s) {
            for (const auto& cont : s->getContradictions()) {
                if (cont.resolved) {
                    contCount++;
                    lines.push_back("  [EXPOSED] " + s->getName() + " (" + s->getId() + "): \"" + 
                        cont.statementQuote + "\" was shattered by Evidence [" + cont.evidenceId + "].");
                }
            }
        }
    }
    if (contCount == 0) {
        lines.push_back("  (No suspect contradictions exposed yet. Question suspects with evidence!)");
    }
    UI::printBox("DEDUCTION MATRIX", lines, 74);
}
