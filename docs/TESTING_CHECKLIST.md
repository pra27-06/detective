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

### Case 1: The Midnight Museum Heist
- **Goal:** Expose Night Guard Marcus Cruz (`S02`) for the theft of the 45-carat sapphire.
- **Crime Time:** 1:15 AM, Friday
- **Solution Blueprint:**
  1. **Investigate Scenes:**
     - Go to `Scene 1: The Sapphire Display Vault`:
       - Inspect `Shattered Pedestal` -> Discover `E01: Cut Fiber Optic Cable`.
       - Inspect `Vault Floor & Mud Traces` -> Discover `E02: Tactical Boot Mudprint`.
       - Inspect `Forgotten Silk Umbrella` -> Discover `E06: Silk Monogrammed Umbrella` (*Red Herring #1*).
     - Go to `Scene 2: Basement Electrical Substation`:
       - Inspect `Security Terminal Server` -> Discover `E03: Electronic Access Log Tape` (*Critical Key Evidence*).
       - Inspect `Disposal Chute` -> Discover `E04: Solvent-Soaked Chemical Rag`.
       - Inspect `Chemical Incident Logbook` -> Discover `E08: Solvent Theft Report Sheet`.
     - Go to `Scene 3: Security Guard Locker Room`:
       - Inspect `Locker #4 (M. Cruz)` -> Discover `E05: Blackwood Syndicate Debt Ledger`.
       - Inspect `Curator's Mail Cubby` -> Discover `E07: Insurance Reassessment Catalog` (*Red Herring #2*).
  2. **Interrogate & Expose Contradictions:**
     - Question `S03: Elena Rostova` -> Select `[4] [CLUE UNLOCKED] Ask about Solvent Theft Log (E08)` -> Elena confirms she logged the missing solvent at 10 PM.
     - Question `S04: Lady Catherine Sterling` -> Select `[4] [CLUE UNLOCKED] Ask about Silk Umbrella (E06)` -> Lady Catherine identifies her forgotten umbrella, dispelling suspicion.
     - Question `S01: Arthur Vance` -> Select `[4] CONFRONT WITH CONTRADICTION` with `E03` -> Vance admits his backup badge was stolen from his drawer.
     - Question `S02: Marcus Cruz`:
       - Select `[5] CONFRONT WITH CONTRADICTION` with `E03` -> Cruz's North Wing alibi shatters against the impossible basement-to-north travel timing!
       - Select `[5] CONFRONT WITH CONTRADICTION` with `E02` -> Cross-hatch boot tread matches his uniform boots.
  3. **Clue Board Deductions:**
     - Connect `E01` and `E03` -> Synthesizes *"Calculated Security Bypass"*.
     - Connect `E02` and `E05` -> Synthesizes *"Guard Uniform & Desperation"*.
     - Connect `E04` and `E08` -> Synthesizes *"Premeditated Solvent Theft"*.
  4. **Final Accusation:**
     - Select `[9] Submit Final Accusation to Magistrate`.
     - Step 1 (Culprit): `S02` (Marcus Cruz)
     - Step 2 (Decisive Evidence): `E03` (Electronic Access Log Tape)
     - Step 3 (Motive): `[1] Financial Debt / Blackmail`
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
