#include "SimpleDetective.h"
#include <iostream>
#include <vector>
#include <memory>
#include <limits>
#include <cstdlib>

class MysteryGame {
private:
    std::string m_detectiveName;
    int m_score;
    int m_energy;
    bool m_caseSolved;

    CaseRegistry<Evidence> m_evidenceRegistry;
    CaseRegistry<Character> m_characterRegistry;
    std::vector<Room> m_rooms;

    void printDivider() const {
        std::cout << "-------------------------------------------------------------\n";
    }

    void printDoubleDivider() const {
        std::cout << "=============================================================\n";
    }

    int getIntInput(int minVal, int maxVal) const {
        int val;
        while (true) {
            std::cout << ">> Enter choice (" << minVal << "-" << maxVal << "): ";
            if (std::cin >> val && val >= minVal && val <= maxVal) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return val;
            }
            std::cout << "Invalid input. Please try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    void pressEnterToContinue() const {
        std::cout << "\n[Press ENTER to continue...]";
        std::string dummy;
        std::getline(std::cin, dummy);
    }

public:
    MysteryGame() : m_detectiveName("Holmes"), m_score(100), m_energy(100), m_caseSolved(false) {
        setupCase();
    }

    void setupCase() {
        // ---------------------------------------------------------------------
        // 1. EVIDENCE REGISTRY (Polymorphic objects created via Derived Classes)
        // ---------------------------------------------------------------------
        // Chemical Evidence
        m_evidenceRegistry.add(std::make_shared<ChemicalClue>(
            "E01", "Poisoned Whisky Glass",
            "A crystal tumbler found beside the victim with faint white residue on the rim. Smells strongly of bitter almonds.",
            "Lord Sterling's Study Desk", "Potassium Cyanide (KCN)", "Fatal Toxicity (100mg)"
        ));

        // Physical Evidence
        m_evidenceRegistry.add(std::make_shared<PhysicalClue>(
            "E02", "Cigar Ash on Windowsill",
            "Distinct grey-white cigar ash discovered on the study window sill. Matches premium imported Havana tobacco.",
            "Study Terrace Window", "Havana Tobacco Leaf", "Freshly dropped within the last hour"
        ));

        // Physical Evidence (The decisive smoking gun)
        m_evidenceRegistry.add(std::make_shared<PhysicalClue>(
            "E03", "Cracked Gold Pocket Watch",
            "A gold pocket watch with crystal glass smashed, frozen at exactly 11:15 PM. Stained with a drop of blood and engraved 'V.C.' on the back.",
            "Persian Rug near study desk", "18k Solid Gold", "Dial smashed during struggle, stopped at 11:15 PM"
        ));

        // Document Evidence
        m_evidenceRegistry.add(std::make_shared<DocumentClue>(
            "E04", "Burnt Will in Fireplace",
            "A scorched parchment fragment rescued from the parlour hearth. It clearly revokes all inheritances to Lady Evelyn.",
            "Drawing Room Hearth", "Lord Arthur Sterling", "...hereby revoke all previous bequests to my wife Evelyn Sterling..."
        ));

        // Chemical Evidence
        m_evidenceRegistry.add(std::make_shared<ChemicalClue>(
            "E05", "Empty Cyanide Vial",
            "A small amber apothecary bottle bearing Dr. Bell's prescription label. The rubber stopper is missing.",
            "Coat Closet / Medical Bag", "Potassium Cyanide", "Emptied completely"
        ));

        // Physical Evidence
        m_evidenceRegistry.add(std::make_shared<PhysicalClue>(
            "E06", "Muddy Bootprint (Size 10)",
            "A deep footprint in the wet yellow garden mud leading directly from under the study window toward the veranda.",
            "Garden Flowerbed", "Wet Terrace Clay", "Matches gentleman's leather riding boot, size 10"
        ));

        // ---------------------------------------------------------------------
        // 2. CHARACTER REGISTRY (Polymorphism: Suspects and Witnesses)
        // ---------------------------------------------------------------------
        m_characterRegistry.add(std::make_shared<Suspect>(
            "S01", "Lady Evelyn Sterling", "Victim's Widow & Socialite",
            "Good evening, Inspector. Please be brief; my nerves are completely shattered.",
            "I was in my private boudoir powdering my face from 11:00 PM until the scream at 11:25 PM.",
            "Money? Arthur was a generous husband. I had no reason to wish him dead!",
            "Arthur discovered my secret gambling debts in Monte Carlo. He threatened to cut me off without a penny tonight, but I didn't kill him!",
            "Keeps twisting her diamond ring nervously and her green velvet dress has a torn hem.",
            "E04", // Contradiction: Burnt Will
            "Lady Evelyn goes pale as a ghost! She clutches her throat:\n"
            "\"Arthur's will! Where did you find that?! Alright, yes! I went to his study at 10:45 PM to plead with him!\n"
            "He threw me out and locked the door behind me! When the blackout happened, I was downstairs in the parlour crying!\n"
            "I swear he was alive and locked the door! I am NOT a killer!\"",
            "She did not kill him.",
            false
        ));

        m_characterRegistry.add(std::make_shared<Suspect>(
            "S02", "Dr. Arthur Bell", "Family Physician & Surgeon",
            "Inspector, this is a tragedy. Lord Sterling's heart was always precarious.",
            "I was down in the wine cellar selecting a bottle of vintage port for dinner when the power cut out.",
            "Arthur was my patient and patron. Why would I harm the man who supported my hospital?",
            "Arthur discovered I had been falsifying medical prescriptions and threatened to report me to Scotland Yard. But someone stole the cyanide from my medical bag during soup!",
            "Continuously cleans his spectacles with a trembling handkerchief; soaking wet collar.",
            "E05", // Contradiction: Empty Cyanide Vial
            "Dr. Bell drops his spectacles on the floor with a clatter! His hands shake violently:\n"
            "\"My cyanide bottle! Heavens above! Inspector, someone stole that vial from my leather bag during dinner!\n"
            "I noticed it was gone at 11:00 PM and was frantically searching the house for it!\n"
            "Arthur didn't die of a heart attack—he was poisoned with my stolen chemicals! But I didn't do it!\"",
            "He did not kill him.",
            false
        ));

        m_characterRegistry.add(std::make_shared<Suspect>(
            "S03", "Vincent Croft", "Speculative Mining Investor & Partner",
            "Evening, Detective. Dreadful business, but rich old men make many enemies, don't they?",
            "I was enjoying a peaceful cigar outside on the veranda under the dry awning the entire evening. Never went near the study.",
            "We were partners! Arthur was going to finance my new copper venture in South Africa tonight.",
            "Arthur found out my mining company was a total sham! He gave me an ultimatum: repay £20,000 tonight or go to prison!",
            "Smells strongly of Havana tobacco; his leather boots are caked in yellow terrace mud; knuckles scraped.",
            "E03", // Contradiction: The Cracked Watch
            "Croft's smug grin instantly vanishes. His eyes bulge as he stares at the bloodstained watch:\n"
            "\"You... where the devil did you dig that up?! Arthur grabbed my wrist as he went down...\n"
            "Dammit! He had millions and was going to throw me in prison over twenty thousand pounds!\n"
            "I stole the poison from Bell's bag at dinner, slipped it in Arthur's whisky, and forced the safe!\n"
            "He was supposed to fall asleep quietly, but he grabbed me as the storm hit! I had no choice!\"",
            "Vincent Croft collapses into his chair in handcuffs, weeping in terror.",
            true // THE GUILTY CULPRIT!
        ));

        m_characterRegistry.add(std::make_shared<Witness>(
            "W01", "James Higgins", "Head Butler",
            "A dreadful night, Inspector. The master had been uneasy all evening.",
            "I served Lord Sterling a fresh glass of Scotch at 11:00 PM in the study. The decanter was untainted. At 10:30 PM, I overheard Mr. Croft shouting violent threats through the study door.",
            "I noticed Mr. Croft had mud on his boots when he came back inside after the blackout, and Dr. Bell left his medical bag in the coat closet."
        ));

        // ---------------------------------------------------------------------
        // 3. ROOMS & CRIME SCENES
        // ---------------------------------------------------------------------
        Room study("R01", "The Master Study (The Murder Scene)",
            "The study is cold and smelling of bitter rain. Lord Sterling sits lifeless at his mahogany desk.");
        study.addHotspot("Lord Sterling's Desk", "A crystal whisky glass with a powdery rim sits beside the deceased.", "E01");
        study.addHotspot("The Terrace Window", "The window is unlatched and swinging in the wind. Fresh grey cigar ash lies on the sill.", "E02");
        study.addHotspot("Persian Rug near Safe", "Beside the forced wall safe, a cracked gold watch glints under the desk lamp.", "E03");
        m_rooms.push_back(study);

        Room parlour("R02", "The Drawing Room & Parlour",
            "A warm room with a crackling marble fireplace. Lady Evelyn Sterling sits on the divan.");
        parlour.addHotspot("Fireplace Hearth", "Charred scraps of official parchment rest among the embers.", "E04");
        m_rooms.push_back(parlour);

        Room closet("R03", "The Hallway Coat Closet",
            "A dark corridor where dinner guests left their outerwear and personal bags.");
        closet.addHotspot("Dr. Bell's Medical Kit", "An unbuckled leather physician's bag with an empty vial compartment.", "E05");
        m_rooms.push_back(closet);

        Room terrace("R04", "The Garden Veranda & Terrace",
            "A stone terrace overlooking the rain-soaked gardens. Yellow clay flowerbeds surround the study window.");
        terrace.addHotspot("Flowerbed under Study Window", "Deep impressions of size-10 boots stamped into the yellow clay mud.", "E06");
        m_rooms.push_back(terrace);
    }

    void displayHeader() const {
        printDoubleDivider();
        std::cout << "       CASE CLOSED: THE POISONED GLASS OF STERLING MANOR       \n";
        std::cout << "  Victim: Lord Arthur Sterling | Time of Death: 11:15 PM       \n";
        printDoubleDivider();
        std::cout << "Detective: " << m_detectiveName << " | Score: " << m_score
                  << " pts | Energy: " << m_energy << "/100\n";
        printDivider();
    }

    void run() {
        std::cout << "\n=============================================================\n";
        std::cout << "          WELCOME TO CASE CLOSED: DETECTIVE SIMULATOR        \n";
        std::cout << "=============================================================\n\n";
        std::cout << "Enter your Detective's Name: ";
        std::getline(std::cin, m_detectiveName);
        if (m_detectiveName.empty()) m_detectiveName = "Inspector Holmes";

        std::cout << "\n[BRIEFING DISPATCH]:\n";
        std::cout << "At 11:15 PM last night, during a violent thunderstorm blackout,\n";
        std::cout << "billionaire art patron Lord Arthur Sterling was found dead in his study.\n";
        std::cout << "The study door was locked from the inside, but the terrace window was open.\n";
        std::cout << "On his lips is the faint scent of bitter almonds (Cyanide poisoning).\n";
        std::cout << "His wall safe was cracked open, and his diamond brooch is missing.\n";
        std::cout << "Only 3 guests were in the manor: his widow Evelyn, Dr. Bell, and Mr. Croft.\n";
        std::cout << "Find the clues, expose their lies, and bring the killer to justice!\n";
        pressEnterToContinue();

        while (!m_caseSolved && m_energy > 0) {
            displayHeader();
            std::cout << "INVESTIGATION ACTIONS:\n";
            std::cout << "  [1] Investigate Manor Rooms       (Search for Clues)\n";
            std::cout << "  [2] Interrogate Suspects & Witness(Question Alibis & Motives)\n";
            std::cout << "  [3] Confront Suspect with Evidence(Expose Contradictions!)\n";
            std::cout << "  [4] Review Detective's Notebook   (Evidence & Profiles)\n";
            std::cout << "  [5] Formulate Final Accusation    (Solve the Mystery!)\n";
            std::cout << "  [6] View OOP Concepts Showcase    (Design Architecture)\n";
            std::cout << "  [0] Quit Game\n\n";

            int choice = getIntInput(0, 6);
            switch (choice) {
                case 1: investigateRooms(); break;
                case 2: interrogateCharacters(); break;
                case 3: confrontSuspect(); break;
                case 4: viewNotebook(); break;
                case 5: makeAccusation(); break;
                case 6: showOOPArchitecture(); break;
                case 0:
                    std::cout << "\nInvestigation suspended. Goodbye, Detective!\n";
                    return;
            }
        }

        if (m_energy <= 0 && !m_caseSolved) {
            std::cout << "\n=============================================================\n";
            std::cout << "  CASE SUSPENDED: YOU HAVE RUN OUT OF INVESTIGATION ENERGY!  \n";
            std::cout << "=============================================================\n";
            std::cout << "The suspects' lawyers arrived, and the killer slipped away...\n";
            std::cout << "Try again to manage your deduction steps more carefully!\n";
        }
    }

    void investigateRooms() {
        printDoubleDivider();
        std::cout << "MANOR ROOMS AVAILABLE FOR INVESTIGATION:\n";
        printDivider();
        for (size_t i = 0; i < m_rooms.size(); ++i) {
            std::cout << "  [" << (i + 1) << "] " << m_rooms[i].getName() << "\n";
        }
        std::cout << "  [0] Return to Hub\n";

        int roomChoice = getIntInput(0, static_cast<int>(m_rooms.size()));
        if (roomChoice == 0) return;

        Room& room = m_rooms[roomChoice - 1];
        printDoubleDivider();
        std::cout << "EXAMINING: " << room.getName() << "\n";
        std::cout << room.getAtmosphere() << "\n";
        printDivider();

        std::cout << "POINTS OF INTEREST:\n";
        for (size_t j = 0; j < room.getHotspotCount(); ++j) {
            const auto& hs = room.getHotspots()[j];
            std::cout << "  [" << (j + 1) << "] " << hs.name;
            if (hs.examined) std::cout << " (Examined)";
            std::cout << "\n";
        }
        std::cout << "  [0] Back to Room Selection\n";

        int hsChoice = getIntInput(0, static_cast<int>(room.getHotspotCount()));
        if (hsChoice == 0) return;

        RoomHotspot& hs = room.getHotspot(hsChoice - 1);
        std::cout << "\n>> You examine the " << hs.name << "...\n";
        std::cout << hs.description << "\n";

        if (!hs.examined) {
            hs.examined = true;
            m_energy = std::max(0, m_energy - 5);
            auto ev = m_evidenceRegistry.getById(hs.evidenceId);
            if (ev && !ev->isDiscovered()) {
                ev->setDiscovered(true);
                m_score += 25;
                std::cout << "\n*** NEW EVIDENCE LOGGED TO NOTEBOOK: ***\n";
                ev->inspect();
            }
        } else {
            std::cout << "(You have already examined this hotspot.)\n";
        }
        pressEnterToContinue();
    }

    void interrogateCharacters() {
        printDoubleDivider();
        std::cout << "PEOPLE OF INTEREST IN STERLING MANOR:\n";
        printDivider();
        for (size_t i = 0; i < m_characterRegistry.size(); ++i) {
            auto ch = m_characterRegistry.getByIndex(i);
            std::cout << "  [" << (i + 1) << "] " << ch->getName()
                      << " (" << ch->getRole() << " - " << ch->getOccupation() << ")\n";
        }
        std::cout << "  [0] Return to Hub\n";

        int chChoice = getIntInput(0, static_cast<int>(m_characterRegistry.size()));
        if (chChoice == 0) return;

        auto ch = m_characterRegistry.getByIndex(chChoice - 1);
        printDoubleDivider();
        std::cout << "INTERROGATING: " << ch->getName() << "\n";
        std::cout << "\"" << ch->getGreeting() << "\"\n";
        printDivider();

        std::cout << "TOPICS TO QUESTION:\n";
        std::cout << "  [1] Ask about their Alibi during the 11:15 PM blackout\n";
        std::cout << "  [2] Ask about their Motive & relationship with Lord Sterling\n";
        std::cout << "  [3] Press about their Hidden Secret\n";
        std::cout << "  [0] Step away\n";

        int topic = getIntInput(0, 3);
        if (topic == 0) return;

        ch->interrogate(topic);
        m_energy = std::max(0, m_energy - 5);
        pressEnterToContinue();
    }

    void confrontSuspect() {
        printDoubleDivider();
        std::cout << "CONFRONT SUSPECT WITH EVIDENCE (EXPOSE A CONTRADICTION):\n";
        printDivider();

        std::vector<std::shared_ptr<Suspect>> suspects;
        for (size_t i = 0; i < m_characterRegistry.size(); ++i) {
            auto s = std::dynamic_pointer_cast<Suspect>(m_characterRegistry.getByIndex(i));
            if (s) suspects.push_back(s);
        }

        for (size_t i = 0; i < suspects.size(); ++i) {
            std::cout << "  [" << (i + 1) << "] " << suspects[i]->getName()
                      << " (Suspicion: " << suspects[i]->getSuspicionScore() << "%)\n";
        }
        std::cout << "  [0] Cancel\n";

        int sChoice = getIntInput(0, static_cast<int>(suspects.size()));
        if (sChoice == 0) return;

        auto targetSuspect = suspects[sChoice - 1];

        // List discovered evidence
        std::vector<std::shared_ptr<Evidence>> discovered;
        for (const auto& ev : m_evidenceRegistry.getAll()) {
            if (ev->isDiscovered()) discovered.push_back(ev);
        }

        if (discovered.empty()) {
            std::cout << "\n[!] You haven't discovered any evidence yet! Go investigate rooms first.\n";
            pressEnterToContinue();
            return;
        }

        std::cout << "\nSUSPECT CLAIM: \"" << targetSuspect->getAlibi() << "\"\n";
        std::cout << "Observed Tell: " << targetSuspect->getTell() << "\n";
        std::cout << "\nSELECT EVIDENCE TO CONFRONT " << targetSuspect->getName() << ":\n";
        printDivider();
        for (size_t j = 0; j < discovered.size(); ++j) {
            std::cout << "  [" << (j + 1) << "] " << discovered[j]->getName()
                      << " (" << discovered[j]->getId() << ") - "
                      << discovered[j]->getTypeName() << "\n";
        }
        std::cout << "  [0] Cancel\n";

        int evChoice = getIntInput(0, static_cast<int>(discovered.size()));
        if (evChoice == 0) return;

        auto chosenEv = discovered[evChoice - 1];
        std::cout << "\n>> You present [" << chosenEv->getName() << "] to "
                  << targetSuspect->getName() << "!\n";

        bool hit = targetSuspect->confrontWithEvidence(chosenEv->getId());
        if (hit) {
            m_score += 50;
            std::cout << "[+] SUCCESS! Contradiction exposed! (+50 Points)\n";
        } else {
            m_score = std::max(0, m_score - 10);
            m_energy = std::max(0, m_energy - 10);
            std::cout << "[-] Contradiction failed: " << targetSuspect->getName() 
                      << " scoffs: 'That clue has nothing to do with me!' (-10 Points, -10 Energy)\n";
            std::cout << "    Tip: Re-read their alibi statement and examine clues found in crime scene rooms.\n";
        }
        pressEnterToContinue();
    }

    void viewNotebook() const {
        printDoubleDivider();
        std::cout << "                   DETECTIVE'S NOTEBOOK                      \n";
        printDoubleDivider();

        std::cout << "\n[DISCOVERED EVIDENCE]:\n";
        int discCount = 0;
        for (const auto& ev : m_evidenceRegistry.getAll()) {
            if (ev->isDiscovered()) {
                std::cout << "  * " << *ev << " - Found at: " << ev->getLocation() << "\n";
                discCount++;
            }
        }
        if (discCount == 0) {
            std::cout << "  (No clues discovered yet. Search the manor rooms!)\n";
        }

        std::cout << "\n[SUSPECT PROFILES & SUSPICION METERS]:\n";
        for (size_t i = 0; i < m_characterRegistry.size(); ++i) {
            auto ch = m_characterRegistry.getByIndex(i);
            auto s = std::dynamic_pointer_cast<Suspect>(ch);
            if (s) {
                std::cout << "  * " << s->getName() << " (" << s->getOccupation() << ")\n";
                std::cout << "    Alibi: " << s->getAlibi() << "\n";
                std::cout << "    Suspicion: [" << s->getSuspicionScore() << "%]";
                if (s->isAlibiBroken()) std::cout << " -> ALIBI BROKEN! (" << s->getSecret() << ")";
                std::cout << "\n";
            }
        }
        pressEnterToContinue();
    }

    void makeAccusation() {
        printDoubleDivider();
        std::cout << "               FINAL ACCUSATION & INDICTMENT                \n";
        std::cout << "  Beware: A false accusation will ruin your reputation!     \n";
        printDoubleDivider();

        std::vector<std::shared_ptr<Suspect>> suspects;
        for (size_t i = 0; i < m_characterRegistry.size(); ++i) {
            auto s = std::dynamic_pointer_cast<Suspect>(m_characterRegistry.getByIndex(i));
            if (s) suspects.push_back(s);
        }

        std::cout << "\nSTEP 1: WHO COMMITTED THE MURDER?\n";
        for (size_t i = 0; i < suspects.size(); ++i) {
            std::cout << "  [" << (i + 1) << "] " << suspects[i]->getName() << "\n";
        }
        std::cout << "  [0] Withdraw\n";
        int culpritPick = getIntInput(0, static_cast<int>(suspects.size()));
        if (culpritPick == 0) return;

        auto accused = suspects[culpritPick - 1];

        std::cout << "\nSTEP 2: WHAT WAS THE MURDER WEAPON / METHOD?\n";
        std::cout << "  [1] Shot with a pistol through the window\n";
        std::cout << "  [2] Cyanide poison slipped into his whisky glass\n";
        std::cout << "  [3] Blunt force trauma with a marble statuette\n";
        int weaponPick = getIntInput(1, 3);

        std::cout << "\nSTEP 3: WHAT IS THE DECISIVE SMOKING GUN PROOF?\n";
        std::vector<std::shared_ptr<Evidence>> discovered;
        for (const auto& ev : m_evidenceRegistry.getAll()) {
            if (ev->isDiscovered()) discovered.push_back(ev);
        }

        if (discovered.empty()) {
            std::cout << "You have no evidence to present! Your accusation falls flat.\n";
            pressEnterToContinue();
            return;
        }

        for (size_t j = 0; j < discovered.size(); ++j) {
            std::cout << "  [" << (j + 1) << "] " << discovered[j]->getName() << "\n";
        }
        int proofPick = getIntInput(1, static_cast<int>(discovered.size()));
        auto chosenProof = discovered[proofPick - 1];

        printDoubleDivider();
        std::cout << ">> You step into the centre of the drawing room...\n";
        std::cout << ">> All eyes turn to you as thunder rattles the French doors...\n";
        printDivider();

        bool isCorrectCulprit = accused->isGuilty(); // Vincent Croft
        bool isCorrectWeapon = (weaponPick == 2);    // Cyanide in whisky glass
        bool isCorrectProof = (chosenProof->getId() == "E03" || chosenProof->getId() == "E02"); // Pocket watch or cigar ash

        if (isCorrectCulprit && isCorrectWeapon && isCorrectProof) {
            m_caseSolved = true;
            m_score += 150;
            std::cout << "\n=============================================================\n";
            std::cout << "          CASE CLOSED: FLAWLESS DEDUCTION !!               \n";
            std::cout << "=============================================================\n\n";
            std::cout << "\"The killer is YOU, Vincent Croft!\"\n\n";
            std::cout << accused->getConfession() << "\n\n";
            std::cout << "The local constables rush in and slap cold iron handcuffs on Croft.\n";
            std::cout << "Lady Evelyn thanks you with tears in her eyes, and Dr. Bell salutes your brilliance!\n\n";
            std::cout << "FINAL DETECTIVE RANK: S-RANK MASTER SLEUTH!\n";
            std::cout << "Final Investigation Score: " << m_score << " points!\n";
            printDoubleDivider();
        } else {
            std::cout << "\n=============================================================\n";
            std::cout << "  MISCARRIAGE OF JUSTICE: ACCUSATION REJECTED!              \n";
            std::cout << "=============================================================\n\n";
            if (!isCorrectCulprit) {
                std::cout << accused->getName() << " presents an ironclad legal defense!\n";
                std::cout << "\"You have accused an innocent person, Inspector! Your career is over!\"\n";
            } else if (!isCorrectWeapon) {
                std::cout << "Your theory regarding the murder weapon was disproven by the autopsy!\n";
            } else {
                std::cout << "Your proof was insufficient! The defense attorney laughed your evidence out of court.\n";
            }
            m_score = std::max(0, m_score - 40);
            m_energy = std::max(0, m_energy - 30);
            std::cout << "Deduction score penalized by 40 points.\n";
        }
        pressEnterToContinue();
    }

    void showOOPArchitecture() const {
        printDoubleDivider();
        std::cout << "      OBJECT-ORIENTED PROGRAMMING (OOPS) ARCHITECTURE GUIDE  \n";
        printDoubleDivider();
        std::cout << "This project demonstrates key OOP principles in clean Modern C++:\n\n";

        std::cout << "1. ABSTRACTION:\n";
        std::cout << "   - 'IInspectable' abstract interface defines pure virtual inspect().\n";
        std::cout << "   - 'Character' abstract base class defines pure virtual interrogate().\n\n";

        std::cout << "2. INHERITANCE:\n";
        std::cout << "   - 'Character' -> 'Suspect', 'Witness'\n";
        std::cout << "   - 'Evidence'  -> 'PhysicalClue', 'DocumentClue', 'ChemicalClue'\n\n";

        std::cout << "3. POLYMORPHISM:\n";
        std::cout << "   - Virtual function overrides: confrontWithEvidence(), inspect(), interrogate().\n";
        std::cout << "   - Dynamic dispatch through std::shared_ptr<Character> and std::shared_ptr<Evidence>.\n";
        std::cout << "   - Operator Overloading: operator<< for printing Evidence objects cleanly.\n\n";

        std::cout << "4. ENCAPSULATION:\n";
        std::cout << "   - Member variables (alibi, motive, secrets, suspicion score) are private.\n";
        std::cout << "   - Manipulated only via validated public getters, setters, and confront() methods.\n\n";

        std::cout << "5. TEMPLATES & GENERICS:\n";
        std::cout << "   - 'template <typename T> class CaseRegistry': Generic container managing\n";
        std::cout << "     both Evidence and Character collections with type safety.\n\n";

        std::cout << "6. MODERN C++ MEMORY SAFETY:\n";
        std::cout << "   - Smart pointers (std::shared_ptr, std::make_shared) ensuring zero memory leaks.\n";
        std::cout << "   - Scoped enums (enum class EvidenceType) for strong type safety.\n";
        printDoubleDivider();
        pressEnterToContinue();
    }
};

int main() {
    try {
        MysteryGame game;
        game.run();
    } catch (const std::exception& ex) {
        std::cerr << "Fatal Exception: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
