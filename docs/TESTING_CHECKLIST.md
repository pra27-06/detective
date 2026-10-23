# CASE CLOSED — Testing Checklist & Solution Walkthroughs

This document provides a comprehensive test plan for manual and automated verification of **CASE CLOSED — Detective Mystery Simulator**, including complete step-by-step solutions for all three cases.

---

## 1. System & Input Robustness Tests

| Test ID | Test Scenario | Expected Behavior | Actual Result |
| :--- | :--- | :--- | :--- |
| **SYS-01** | Empty/blank input on menu prompts | Displays clear warning message; reprompts user without crashing. | PASS |
| **SYS-02** | Non-numeric input for integer prompts (e.g. `abc`, `@#$`) | Displays input validation error; prompts user for valid integer range. | PASS |
| **SYS-03** | Number out of allowable range (e.g. `99` on 1-11 menu) | Informs user of valid range (1 to 11) and waits for correct input. | PASS |
| **SYS-04** | Energy depletion during excessive scene searches | Informs detective of fatigue; automatically recovers minimum energy buffer. | PASS |
| **SYS-05** | Save and Load integrity | Saves case state to `saves/case_save.dat`; reloading restores evidence, score, energy, broken alibis, and deductions. | PASS |
| **SYS-06** | Missing or corrupted save file | Detects missing file gracefully; informs player and returns to main menu without crashing. | PASS |
| **SYS-07** | Visual Mode Toggle (ANSI / Plain Text) | Switches dynamically between colored virtual terminal formatting and plain-text output. | PASS |
| **SYS-08** | End-of-File (EOF) input handling | Handles standard input redirection stream closure gracefully with clean termination. | PASS |

---

## 2. Complete Case Walkthroughs (Flawless S-Rank Solutions)

### Case 1: The Poisoned Glass of Sterling Manor
- **Goal:** Expose speculative mining partner Vincent Croft (`S03`) for the cyanide poisoning of Lord Arthur Sterling and theft of the 50-carat diamond.
- **Crime Time:** 11:15 PM, Friday
- **Solution Blueprint:**
  1. **Investigate Scenes:**
     - Go to `Scene 1: The Master Study (Crime Scene)`:
       - Inspect `Lord Sterling's Study Desk` -> Discover `E01: Poisoned Whisky Glass` (*Lethal cyanide residue*).
       - Inspect `Study Terrace Window Sill` -> Discover `E02: Havana Cigar Ash on Sill` (*Vuelta Abajo tobacco*).
       - Inspect `Persian Rug near Desk` -> Discover `E03: Cracked Gold Pocket Watch` (*Stopped at 11:15 PM, "V.C."*).
       - Inspect `Forced Wall Safe Alcove` -> Discover `E09: Obsidian Hourglass Wax Seal`.
     - Go to `Scene 2: The Drawing Room & Parlour`:
       - Inspect `Marble Fireplace Hearth` -> Discover `E04: Burnt Will in Fireplace` (*Revoking Evelyn's inheritance*).
       - Inspect `Sideboard & Bar Cart` -> Discover `E07: Untainted Scotch Decanter` (*Untainted whisky*).
       - Inspect `Emerald Velvet Armchair` -> Discover `E10: Torn Green Velvet Hem`.
     - Go to `Scene 3: The Veranda & Garden Terrace`:
       - Inspect `Rain-Soaked Flowerbed` -> Discover `E06: Muddy Bootprint (Size 10)` (*Yellow terrace mud*).
       - Inspect `Coat Rack / Medical Satchel` -> Discover `E05: Empty Cyanide Vial` (*Dr. Bell's stolen vial*).
       - Inspect `Garden Hedge Bush` -> Discover `E08: Stolen Diamond Brooch` (*Stolen from wall safe*).
  2. **Interrogate & Expose Contradictions:**
     - Question `S01: Lady Evelyn Sterling` -> Select `CONFRONT CONTRADICTION` with `E04: Burnt Will` -> Lady Evelyn admits she pleaded with Arthur at 10:45 PM and was crying in the parlour during the 11:15 PM blackout.
     - Question `S02: Dr. Arthur Bell` -> Select `CONFRONT CONTRADICTION` with `E05: Empty Cyanide Vial` -> Dr. Bell admits his medical cyanide was stolen from his bag during dinner.
     - Question `S03: Vincent Croft`:
       - Select `CONFRONT CONTRADICTION` with `E03: Cracked Gold Pocket Watch` -> OBJECTION! Croft breaks down: Arthur grabbed his wrist at 11:15 PM as the cyanide took effect!
       - Select `CONFRONT CONTRADICTION` with `E02: Havana Cigar Ash` -> HOLD IT! Shatters his claim of staying on the veranda.
       - Select `CONFRONT CONTRADICTION` with `E06: Muddy Bootprint` -> TAKE THAT! Proves he stepped in the mud under the study window.
  3. **Clue Board Deductions:**
     - Connect `E01` and `E05` -> Synthesizes *"Stolen Chemical Lethality"*.
     - Connect `E02` and `E06` -> Synthesizes *"Veranda Intrusion Traces"*.
     - Connect `E03` and `E08` -> Synthesizes *"11:15 PM Struggle & Safe Robbery"*.
     - Connect `E04` and `E10` -> Synthesizes *"Disinheritance Alibi Confirmation"*.
  4. **Final Accusation:**
     - Select `Submit Final Accusation to Magistrate`.
     - Step 1 (Culprit): `S03` (Vincent Croft)
     - Step 2 (Decisive Evidence): `E03` (Cracked Gold Pocket Watch)
     - Step 3 (Motive): `[1] Financial Debt / Syndicate Coercion` (Ultimatum on £20,000 debt)
     - *Outcome:* **MASTER DETECTIVE — FLAWLESS INDICTMENT!** Full confession obtained; Grade **S**.

---

### Case 2: The Last Train Mystery
- **Goal:** Indict Personal Secretary Julian Gray (`S01`) for the cyanide poisoning of Victor Sterling.
- **Crime Time:** 11:45 PM, Saturday
- **Solution Blueprint:**
  1. **Investigate Scenes:**
     - Go to `Scene 1: Compartment 4-A`:
       - Inspect `Victim's Table & Teacup` -> Discover `E01: Teacup with Bitter Almond Residue`.
       - Inspect `Sliding Door Runner Track` -> Discover `E02: Antique Gold Eagle Cufflink` (*Critical Key Evidence*).
       - Inspect `Sterling's Leather Briefcase` -> Discover `E03: Torn Embezzlement Audit Letter`.
     - Go to `Scene 2: Compartment 4-D`:
       - Inspect `Black Medical Doctor's Bag` -> Discover `E04: Dr. Finch's Sealed Ampoules`.
       - Inspect `Steward's Galley Cupboard` -> Discover `E06: Unmarked Galley Chemical Vial` (*Red Herring #1*).
     - Go to `Scene 3: The Club Car`:
       - Inspect `Bar Order Register` -> Discover `E05: Dining Car Bar Tab & Receipts`.
       - Inspect `Briefcase in Luggage Net` -> Discover `E07: Hostile Takeover Injunction Draft` (*Red Herring #2*).
       - Inspect `Conductor's Service Delivery Sheet` -> Discover `E08: Conductor's Delivery Sheet`.
  2. **Question Witness & Suspects:**
     - Question `W01: Conductor Morris` -> Testifies that a man wearing gold eagle cufflinks took delivery of the tea tray at 11:40 PM.
     - Question `S04: Chef Gregory Hayes` -> Ask about Galley Vial (E06) -> Proves bottle was harmless vanilla extract for desserts.
     - Question `S03: Dr. Alistair Finch` -> Confront with `E04` -> Proves all medical poison seals are 100% factory intact.
     - Question `S01: Julian Gray`:
       - Confront with `E02` -> Gray stares in horror at his bare left cuff; alibi shatters!
       - Confront with `E03` -> Audit letter disproves harmonious relationship.
  3. **Clue Board Deductions:**
     - Connect `E01` and `E02` -> Synthesizes *"The Poisoner's Calling Card"*.
     - Connect `E02` and `E03` -> Synthesizes *"Desperate Cover-Up"*.
     - Connect `E01` and `E08` -> Synthesizes *"Corridor Poisoning Point"*.
  4. **Final Accusation:**
     - Select `[9] Submit Final Accusation`.
     - Step 1 (Culprit): `S01` (Julian Gray)
     - Step 2 (Decisive Evidence): `E02` (Antique Gold Eagle Cufflink)
     - Step 3 (Motive): `[2] Embezzlement Cover-Up`
     - *Outcome:* **MASTER DETECTIVE — FLAWLESS INDICTMENT!** Grade **S**.

---

### Case 3: The Locked Room Incident
- **Goal:** Prove Cornelius Thorne (`S01`) murdered his uncle and staged an impossible locked-room suicide using clockwork mechanics.
- **Crime Time:** 9:00 PM, Sunday
- **Solution Blueprint:**
  1. **Investigate Scenes:**
     - Go to `Scene 1: Third Floor Clock Tower Study`:
       - Inspect `Heavy Oak Door & Bolt` -> Discover `E01: Friction-Scorched Latch & Water Puddle` (*Critical Key Evidence*).
       - Inspect `Thorne's Desk & Suicide Note` -> Discover `E02: Ballpoint Signed Suicide Note`.
       - Inspect `Discarded Teacup on Mantel` -> Discover `E07: Valerian Sedative Teacup` (*Red Herring #2*).
     - Go to `Scene 2: Grandfather Chime Clock`:
       - Inspect `Internal Weight Escapement Reel` -> Discover `E03: Modified Clockwork Tension Reel`.
       - Inspect `Manor Veranda Flagstones` -> Discover `E06: Miriam's Skeleton Pick Key` (*Red Herring #1*).
     - Go to `Scene 3: Manor Entrance Hall`:
       - Inspect `Cornelius's Discarded Greatcoat` -> Discover `E04: Coil of Piano Wire & Wax`.
       - Inspect `Writing Bureau in the Hall` -> Discover `E05: Solicitor's Disinheritance Draft`.
       - Inspect `Side Table Mail Tray` -> Discover `E08: Omnibus Stamped Ticket Stub`.
  2. **Question Witness & Suspects:**
     - Question `W01: Constable Higgins` -> Confirms study door was securely bolted from the inside.
     - Question `S02: Miriam Black` -> Ask about picks (E06) -> Proves picks cannot throw an interior slide bolt.
     - Question `S03: Silas Vance` -> Confront with `E05` -> Butler admits shouting occurred at 7:45 PM.
     - Question `S01: Cornelius Thorne`:
       - Confront with `E02` -> Ballpoint signature exposes forged suicide note!
       - Confront with `E01` -> Friction scorch and water puddle expose the exterior mechanical lock trick!
  3. **Clue Board Deductions:**
     - Connect `E01` and `E03` -> Synthesizes *"The Mechanical Locked-Room Trick"*.
     - Connect `E02` and `E04` -> Synthesizes *"The Staged Suicide"*.
     - Connect `E01` and `E05` -> Synthesizes *"Premeditated Retaliation"*.
  4. **Final Accusation:**
     - Select `[9] Submit Final Accusation`.
     - Step 1 (Culprit): `S01` (Cornelius Thorne)
     - Step 2 (Decisive Evidence): `E01` (Friction-Scorched Latch & Water Puddle)
     - Step 3 (Motive): `[3] Disinheritance / Patent Theft`
     - *Outcome:* **MASTER DETECTIVE — FLAWLESS INDICTMENT!** Grade **S**.

---

## 3. Negative & Branching Ending Verification

- **Branch A (Partial Justice):** Accuse the correct suspect (`S02` in Case 1) but leave their alibi unbroken.
  *Actual Output:* Court finds reasonable doubt due to intact alibi; defendant convicted on secondary charges with plea bargain; Grade B.
- **Branch B (Miscarriage of Justice):** Accuse an innocent suspect (`S01: Arthur Vance` in Case 1).
  *Actual Output:* Innocent suspect acquitted; true culprit escapes; detective reprimanded; Grade F.
- **Branch C (Insufficient Key Evidence):** Accuse the correct suspect but present secondary evidence (`E01`).
  *Actual Output:* Prosecution fails to meet burden of proof; technical plea deal entered; Grade B.
