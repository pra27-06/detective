// CASE CLOSED — Detective Mystery Simulator (Web Visual Novel Edition)
// 100% Client-Side Pure JavaScript Game Engine (Zero Dependencies)

class AudioFX {
  constructor() {
    this.ctx = null;
    this.init();
  }
  init() {
    const AudioContext = window.AudioContext || window.webkitAudioContext;
    if (AudioContext) {
      this.ctx = new AudioContext();
    }
  }
  resume() {
    if (this.ctx && this.ctx.state === 'suspended') {
      this.ctx.resume();
    }
  }
  playTypewriter() {
    if (!this.ctx) return;
    this.resume();
    const osc = this.ctx.createOscillator();
    const gain = this.ctx.createGain();
    osc.type = 'sine';
    osc.frequency.setValueAtTime(800 + Math.random() * 200, this.ctx.currentTime);
    gain.gain.setValueAtTime(0.015, this.ctx.currentTime);
    gain.gain.exponentialRampToValueAtTime(0.001, this.ctx.currentTime + 0.03);
    osc.connect(gain);
    gain.connect(this.ctx.destination);
    osc.start();
    osc.stop(this.ctx.currentTime + 0.03);
  }
  playChime() {
    if (!this.ctx) return;
    this.resume();
    const freqs = [523.25, 659.25, 783.99, 1046.50]; // C5, E5, G5, C6
    freqs.forEach((f, i) => {
      const osc = this.ctx.createOscillator();
      const gain = this.ctx.createGain();
      osc.type = 'triangle';
      osc.frequency.setValueAtTime(f, this.ctx.currentTime + i * 0.08);
      gain.gain.setValueAtTime(0.12, this.ctx.currentTime + i * 0.08);
      gain.gain.exponentialRampToValueAtTime(0.001, this.ctx.currentTime + i * 0.08 + 0.6);
      osc.connect(gain);
      gain.connect(this.ctx.destination);
      osc.start(this.ctx.currentTime + i * 0.08);
      osc.stop(this.ctx.currentTime + i * 0.08 + 0.6);
    });
  }
  playObjection() {
    if (!this.ctx) return;
    this.resume();
    // Dramatic chord hit
    const freqs = [130.81, 164.81, 196.00, 261.63, 523.25];
    freqs.forEach(f => {
      const osc = this.ctx.createOscillator();
      const gain = this.ctx.createGain();
      osc.type = 'sawtooth';
      osc.frequency.setValueAtTime(f, this.ctx.currentTime);
      gain.gain.setValueAtTime(0.2, this.ctx.currentTime);
      gain.gain.exponentialRampToValueAtTime(0.001, this.ctx.currentTime + 1.2);
      osc.connect(gain);
      gain.connect(this.ctx.destination);
      osc.start();
      osc.stop(this.ctx.currentTime + 1.2);
    });
  }
  playBuzzer() {
    if (!this.ctx) return;
    this.resume();
    const osc = this.ctx.createOscillator();
    const gain = this.ctx.createGain();
    osc.type = 'sawtooth';
    osc.frequency.setValueAtTime(140, this.ctx.currentTime);
    osc.frequency.setValueAtTime(120, this.ctx.currentTime + 0.15);
    gain.gain.setValueAtTime(0.15, this.ctx.currentTime);
    gain.gain.exponentialRampToValueAtTime(0.001, this.ctx.currentTime + 0.35);
    osc.connect(gain);
    gain.connect(this.ctx.destination);
    osc.start();
    osc.stop(this.ctx.currentTime + 0.35);
  }
}

// Full Case Database matching C++ CaseFile.cpp
const CASE_DATA = {
  1: {
    id: "CASE-01",
    title: "Case #1: The Midnight Museum Heist",
    tagline: "The 45-carat sapphire disappears during a 4-minute blackout.",
    briefing: "Scotland Yard Dispatch has called us to the Royal Alexandria Museum. At 1:14 AM, the main circuit breaker tripped, cutting gallery power. In that four-minute window, the armored glass pedestal of the Star of Alexandria was breached and the sapphire stolen. Cross-examine the night staff, examine the crime scenes, and identify the thief!",
    culpritId: "S02", // Guard Cruz
    decisiveEvidenceId: "E03", // Electronic Access Log
    motiveKeyword: "DEBT",
    solution: "Security Guard Marcus Cruz stole Curator Vance's spare keycard from the lost-and-found locker at 1:10 AM, swiped into the basement power substation at 1:12 AM, and cut the main breaker at 1:14 AM. Using pre-staged solvent and laser cutters, he seized the sapphire and swiped the North Wing patrol terminal to fabricate an alibi.",
    scenes: [
      {
        id: "SCENE_01",
        name: "The Sapphire Display Vault",
        bg: "assets/backgrounds/scene_vault.png",
        hotspots: [
          { x: 50, y: 58, name: "Shattered Pedestal", desc: "The armored pedestal fiber optic tripwire was cleanly cut at a 45-degree angle.", evId: "E01" },
          { x: 38, y: 78, name: "Vault Floor & Mud Traces", desc: "Dried mud flakes show a distinct cross-hatch combat boot tread, size 10.", evId: "E02" },
          { x: 66, y: 65, name: "Forgotten Silk Umbrella", desc: "A stylish monogrammed silk umbrella resting against a marble pillar.", evId: "E06" }
        ]
      },
      {
        id: "SCENE_02",
        name: "Basement Electrical Substation",
        bg: "assets/backgrounds/scene_substation.png",
        hotspots: [
          { x: 75, y: 44, name: "Security Terminal Server", desc: "Access logs show Curator Vance's badge swiped at 1:12 AM and North Wing at 1:15 AM.", evId: "E03" },
          { x: 48, y: 86, name: "Disposal Chute & Laundry Cart", desc: "An oily workshop rag soaked in chemical solvent has been discarded in the chute.", evId: "E04" },
          { x: 24, y: 48, name: "Chemical Incident Logbook", desc: "Logbook notes solvent reported missing from conservation storage at 10:00 PM.", evId: "E08" }
        ]
      },
      {
        id: "SCENE_03",
        name: "Staff Locker Room & Guard Post",
        bg: "assets/backgrounds/scene_locker.png",
        hotspots: [
          { x: 20, y: 46, name: "Locker #14 (Marcus Cruz)", desc: "A formal repossession demand for £12,000 overdue debt and pawn shop cards.", evId: "E05" },
          { x: 50, y: 76, name: "Curator's Office Desk", desc: "A memo reporting his secondary museum master card missing three days prior.", evId: "E07" }
        ]
      }
    ],
    evidence: {
      "E01": { id: "E01", name: "Cut Fiber Optic Cable", cat: "PHYSICAL", rel: "CRITICAL PROOF", desc: "Cleanly sliced optical wire indicating insider knowledge of alarm timing." },
      "E02": { id: "E02", name: "Combat Boot Print Mud Flakes", cat: "FORENSIC", rel: "SUPPORTING PROOF", desc: "Heavy tactical size 10 boot treads matching standard museum guard issue." },
      "E03": { id: "E03", name: "Electronic Access Log", cat: "DOCUMENT", rel: "DECISIVE EVIDENCE", desc: "Substation swiped at 1:12 AM; North Wing terminal swiped at 1:15 AM with Vance's stolen badge. Physically impossible for one person without sprinting through secret corridors." },
      "E04": { id: "E04", name: "Solvent-Soaked Workshop Rag", cat: "CHEMICAL", rel: "CRITICAL PROOF", desc: "Industrial acetone used to dissolve anti-shatter glass sealant." },
      "E05": { id: "E05", name: "Loan Shark Repossession Notice", cat: "DOCUMENT", rel: "MOTIVE EVIDENCE", desc: "Final notice demanding £12,000 from Marcus Cruz by midnight." },
      "E06": { id: "E06", name: "Monogrammed Silk Umbrella", cat: "PERSONAL", rel: "RED HERRING", desc: "Belongs to Lady Sterling, who departed at 11:15 PM and forgot it." },
      "E07": { id: "E07", name: "Curator's Stolen Keycard Report", cat: "DOCUMENT", rel: "SUPPORTING PROOF", desc: "Notice that Vance's spare pass was taken from the guard post." },
      "E08": { id: "E08", name: "Chemical Inventory Incident Sheet", cat: "DOCUMENT", rel: "SUPPORTING PROOF", desc: "Confirms industrial solvent reported missing at 10 PM." }
    },
    suspects: [
      {
        id: "S01", name: "Arthur Vance", role: "Museum Curator",
        charId: "vance",
        alibi: "I was locked inside my 3rd-floor office reviewing catalog proofs from midnight to 1:20 AM without leaving.",
        motive: "The museum is under heavy debt, but insurance fraud would destroy our historical reputation.",
        greeting: "Detective, please tell me the Star of Alexandria has been located! Our reputation is ruined!",
        isCulprit: false,
        contradiction: { evId: "E07", res: "Curator Vance nods: 'Yes, my spare card was stolen days ago. The thief used my badge to frame me!'" }
      },
      {
        id: "S02", name: "Marcus Cruz", role: "Night Security Guard",
        charId: "cruz",
        alibi: "I was patrolling the North Wing gallery continuously from 1:00 AM to 1:20 AM. I swiped the terminal at 1:15 AM!",
        motive: "I work for my wages, Detective. I have no interest in antique jewels.",
        greeting: "Evening, Detective. Place is sealed tight. I followed patrol regulations to the letter.",
        isCulprit: true,
        contradiction: {
          evId: "E03",
          res: "Cruz's face pales as you display the server log: 'Vance's badge was swiped at the basement power breaker at 1:12 AM, then at the North Wing at 1:15 AM! Only the guard on duty could have sprinted through the staff passage!' Cruz falls to his knees: 'I had to do it... the loan sharks were going to break my legs tonight!'"
        }
      },
      {
        id: "S03", name: "Elena Rostova", role: "Master Conservator",
        charId: "elena",
        alibi: "I left the museum at 10:30 PM after logging the missing solvent with security and was asleep at home.",
        motive: "I dedicate my life to preserving art, not pawning stolen artifacts.",
        greeting: "Good evening. If this is about the missing solvent, I reported the theft to Guard Cruz at 10:00 PM sharp.",
        isCulprit: false,
        contradiction: { evId: "E08", res: "Elena sighs: 'My lab inventory confirms I logged the theft hours before the heist.'" }
      },
      {
        id: "S04", name: "Lady Catherine Sterling", role: "Benefactor & Trustee",
        charId: "sterling",
        alibi: "I departed the museum reception at 11:15 PM in my private carriage and arrived home in Kensington.",
        motive: "I donated that sapphire in honor of my late husband. Why on earth would I steal my own bequest?",
        greeting: "This is outrageous! Scotland Yard should be hunting jewel thieves, not harassing patrons!",
        isCulprit: false,
        contradiction: { evId: "E06", res: "Lady Sterling chuckles: 'My umbrella? I left it by the marble column during the patron tea!'" }
      },
      {
        id: "W01", name: "Old Bert", role: "Night Janitor (Witness)",
        charId: "bert",
        isWitness: true,
        testimony: "At 1:14 AM, right when the lights cut out, I was replacing filters near the basement breaker. Someone in heavy combat boots sprinted past me into the service corridor reeking of chemical solvent!",
        greeting: "I told the constables already, Officer: heavy combat boots ran past the substation right as the lights died!",
        isCulprit: false
      }
    ],
    deductions: [
      { id1: "E01", id2: "E03", title: "Calculated Security Bypass", insight: "The fiber optic cut and timed card swipe prove an insider used a stolen badge to fake an alibi while cutting power." },
      { id1: "E02", id2: "E05", title: "Guard Uniform & Debt Motive", insight: "Tactical size 10 boot prints match guard issue footwear, linking the scene to Cruz's £12,000 urgent debt." },
      { id1: "E04", id2: "E08", title: "Premeditated Solvent Theft", insight: "The solvent rag in the basement chute matches the chemical reported stolen at 10:00 PM." }
    ],
    eliminations: [
      { id: "S01", name: "Arthur Vance", reason: "Eliminated: Electronic door logs confirm he was locked inside his 3rd-floor office from midnight to 1:20 AM." },
      { id: "S03", name: "Elena Rostova", reason: "Eliminated: Laboratory logs show chemical bottles remained sealed; she logged the theft at 10:00 PM." },
      { id: "S04", name: "Lady Catherine Sterling", reason: "Eliminated: Gatehouse logs and chauffeur confirm she departed at 11:15 PM and was in Kensington." }
    ]
  },
  2: {
    id: "CASE-02",
    title: "Case #2: The Last Train Mystery",
    tagline: "Tycoon Victor Sterling is poisoned aboard an express train with no stops.",
    briefing: "Aboard the midnight express traversing the Blackwood Tunnel, financier Victor Sterling was found dead in Compartment 4-A, poisoned by potassium cyanide in his chamomile tea. The train made no stops between Wyvern and Glenridge. The culprit is aboard Carriage #4!",
    culpritId: "S01", // Julian Gray
    decisiveEvidenceId: "E02", // Torn Gold Eagle Cufflink
    motiveKeyword: "EMBEZZLEMENT",
    solution: "Personal secretary Julian Gray embezzled £60,000 from Sterling. Knowing Sterling was having inspectors arrest him at Glenridge, Gray impersonated Sterling wearing spare eagle cufflinks, received the tea tray from Conductor Morris at 11:40 PM, and poisoned the cup.",
    scenes: [
      {
        id: "SCENE_01",
        name: "First Class Compartment 4-A",
        bg: "assets/backgrounds/scene_vault.png",
        hotspots: [
          { x: 50, y: 55, name: "Teacup with Almond Odor", desc: "A half-empty bone china teacup smelling distinctly of bitter almonds (cyanide).", evId: "E01" },
          { x: 70, y: 75, name: "Door Runner Track", desc: "Wedged in the sliding door brass runner: an antique gold eagle cufflink torn from a shirt.", evId: "E02" }
        ]
      },
      {
        id: "SCENE_02",
        name: "Carriage Galley Pantry",
        bg: "assets/backgrounds/scene_locker.png",
        hotspots: [
          { x: 35, y: 50, name: "Service Clipboard", desc: "Conductor delivery sheet confirming tea left the galley clean at 11:38 PM.", evId: "E08" },
          { x: 65, y: 70, name: "Chef's Flavoring Vial", desc: "An amber bottle labeled pure vanilla extract, tested negative for toxins.", evId: "E06" }
        ]
      },
      {
        id: "SCENE_03",
        name: "Club Car & Secretary's Desk",
        bg: "assets/backgrounds/scene_substation.png",
        hotspots: [
          { x: 45, y: 55, name: "Sterling's Audit Letter", desc: "A draft letter in Sterling's hand confronting Julian Gray with £60,000 embezzlement.", evId: "E03" },
          { x: 75, y: 65, name: "Dr. Finch's Medical Satchel", desc: "Satchel with intact Edinburgh wax seals on all emergency narcotics.", evId: "E04" }
        ]
      }
    ],
    evidence: {
      "E01": { id: "E01", name: "Poisoned Chamomile Teacup", cat: "CHEMICAL", rel: "CRITICAL PROOF", desc: "Cyanide residue inside the deceased's teacup." },
      "E02": { id: "E02", name: "Torn Gold Eagle Cufflink", cat: "PHYSICAL", rel: "DECISIVE EVIDENCE", desc: "Gold eagle cufflink matching Gray's frayed sleeve buttonhole." },
      "E03": { id: "E03", name: "Embezzlement Audit Letter", cat: "DOCUMENT", rel: "MOTIVE EVIDENCE", desc: "Proves Sterling was having Gray arrested at the next station." },
      "E04": { id: "E04", name: "Sealed Medical Ampoules", cat: "MEDICAL", rel: "ELIMINATION EVIDENCE", desc: "Proves Dr. Finch never opened his drug supply." },
      "E06": { id: "E06", name: "Vanilla Extract Vial", cat: "SUBSTANCE", rel: "RED HERRING", desc: "Harmless baking flavoring from the galley pantry." },
      "E08": { id: "E08", name: "Conductor's Delivery Log", cat: "DOCUMENT", rel: "SUPPORTING PROOF", desc: "Confirms tea was delivered at 11:40 PM." }
    },
    suspects: [
      {
        id: "S01", name: "Julian Gray", role: "Personal Secretary",
        charId: "cruz",
        alibi: "I retired to Compartment 4-C at 11:30 PM to sleep. I never walked the corridor!",
        motive: "Why would I harm my benefactor? Without him, I have nothing.",
        greeting: "Detective, this is horrifying! Mr. Sterling was like a father to me!",
        isCulprit: true,
        contradiction: {
          evId: "E02",
          res: "You produce the eagle cufflink from the door runner. Gray stares at his bare left wrist: 'He found the books! He was having me arrested at Glenridge! When the conductor delivered the tea, I took it and poisoned it!'"
        }
      },
      {
        id: "S02", name: "Nora Vance", role: "Shipping Competitor",
        charId: "elena",
        alibi: "I was in the Club Car from 11:15 PM onward enjoying spirits with passengers.",
        motive: "We had commercial rivalry, but I fight in court, not with poisons.",
        greeting: "Good evening. Sterling made enemies everywhere, Detective.",
        isCulprit: false,
        contradiction: { evId: "E03", res: "Nora smiles: 'My dispute was strictly corporate, handled by attorneys.'" }
      },
      {
        id: "S03", name: "Dr. Alistair Finch", role: "Physician",
        charId: "vance",
        alibi: "I was reading clinical journals in Compartment 4-D until the emergency whistle blew.",
        motive: "Sterling sued my hospital board years ago, but that was settled.",
        greeting: "I attempted resuscitation, Detective, but cyanide arrests cellular life in seconds.",
        isCulprit: false,
        contradiction: { evId: "E04", res: "Dr. Finch displays his satchel: 'Every ampoule seal is intact from Edinburgh.'" }
      },
      {
        id: "W01", name: "Conductor Morris", role: "Train Conductor (Witness)",
        charId: "bert",
        isWitness: true,
        testimony: "At 11:40 PM, I delivered chamomile tea to Compartment 4-A. A gentleman opened the door wearing distinctive gold eagle cufflinks, took the tray, and said Mr. Sterling was resting.",
        greeting: "A terrible night on my train, Detective. I pride myself on passenger safety.",
        isCulprit: false
      }
    ],
    deductions: [
      { id1: "E02", id2: "E08", title: "Impersonation at the Door", insight: "The cufflink torn in the runner at 11:40 PM proves the killer received the tea from the conductor." },
      { id1: "E01", id2: "E03", title: "Lethal Embezzlement Cover-Up", insight: "The £60,000 theft audit letter gave Julian Gray an urgent life-or-death motive." }
    ],
    eliminations: [
      { id: "S02", name: "Nora Vance", reason: "Eliminated: Club Car register and passengers confirm she never left the lounge." },
      { id: "S03", name: "Dr. Finch", reason: "Eliminated: Medical satchel wax seals remain unbroken on all drugs." }
    ]
  },
  3: {
    id: "CASE-03",
    title: "Case #3: The Locked Room Incident",
    tagline: "Clockmaker Thaddeus Thorne shot behind a door bolted from the inside.",
    briefing: "Master horologist Thaddeus Thorne was found dead of a gunshot wound in his study. The heavy oak door was bolted with an iron sliding bolt from the inside, and windows were barred. Staged as a suicide, forensic ballistics and clockwork physics prove otherwise!",
    culpritId: "S01", // Cornelius Thorne
    decisiveEvidenceId: "E02", // Ballpoint Signature on Note
    motiveKeyword: "PATENT",
    solution: "Nephew Cornelius Thorne was being disinherited. He shot his uncle at 8:15 PM, typed a suicide note, signed it with a modern ballpoint pen, and rigged piano wire around the bolt and pendulum weight with an ice block. At 9:00 PM, while drinking in the tavern, the clock chimed and pulled the bolt shut.",
    scenes: [
      {
        id: "SCENE_01",
        name: "The Bolted Study",
        bg: "assets/backgrounds/briefing_room.png",
        hotspots: [
          { x: 50, y: 55, name: "Thorne's Desk & Suicide Note", desc: "A typed suicide note signed with cheap ballpoint ink. Thorne only wrote in Prussian blue fountain ink.", evId: "E02" },
          { x: 25, y: 65, name: "Heavy Interior Slide Bolt", desc: "A friction groove and melted water droplet on the brass slide bolt.", evId: "E01" }
        ]
      },
      {
        id: "SCENE_02",
        name: "Grandfather Clock & Workshop",
        bg: "assets/backgrounds/scene_vault.png",
        hotspots: [
          { x: 70, y: 50, name: "Clock Pendulum Escapement", desc: "Scratches on the 20-lb chime weight matching high-tensile wire.", evId: "E03" },
          { x: 40, y: 75, name: "Apprentice's Tool Chest", desc: "Spool of high-tensile piano wire matching the bolt groove.", evId: "E04" }
        ]
      },
      {
        id: "SCENE_03",
        name: "Manor Parlor & Safe",
        bg: "assets/backgrounds/scene_substation.png",
        hotspots: [
          { x: 55, y: 50, name: "Disinheritance Will Draft", desc: "Legal draft revoking Cornelius's inheritance and patent ownership.", evId: "E05" },
          { x: 80, y: 70, name: "Crossroads Omnibus Ticket", desc: "Ticket stub verifying Locksmith Miriam Black departed on the 8:30 PM bus.", evId: "E08" }
        ]
      }
    ],
    evidence: {
      "E01": { id: "E01", name: "Friction-Grooved Slide Bolt", cat: "MECHANICAL", rel: "CRITICAL PROOF", desc: "Shows tension wire pulled the bolt shut when the clock struck." },
      "E02": { id: "E02", name: "Ballpoint-Signed Suicide Note", cat: "DOCUMENT", rel: "DECISIVE EVIDENCE", desc: "Signed with modern ballpoint ink found in Cornelius's satchel." },
      "E03": { id: "E03", name: "Clockwork Chime Weight Scratches", cat: "PHYSICAL", rel: "SUPPORTING PROOF", desc: "Marks where wire was tied to the descending 9:00 PM weight." },
      "E04": { id: "E04", name: "Spool of Piano Wire", cat: "PHYSICAL", rel: "CRITICAL PROOF", desc: "Tensile wire from Cornelius's tool chest." },
      "E05": { id: "E05", name: "Disinheritance Will Draft", cat: "LEGAL", rel: "MOTIVE EVIDENCE", desc: "Revoked Cornelius's rights to the patent fortune." },
      "E08": { id: "E08", name: "Omnibus Ticket Stub", cat: "DOCUMENT", rel: "ELIMINATION EVIDENCE", desc: "Confirms Miriam departed on the 8:30 PM bus." }
    },
    suspects: [
      {
        id: "S01", name: "Cornelius Thorne", role: "Nephew & Apprentice",
        charId: "cruz",
        alibi: "I was drinking at the Black Bull Tavern from 8:30 PM to 9:45 PM. Ten patrons saw me when the shot allegedly went off!",
        motive: "Uncle was my mentor. I stood to inherit his horological fortune.",
        greeting: "My uncle took his own life, Detective! Why torture our grieving family?",
        isCulprit: true,
        contradiction: {
          evId: "E02",
          res: "Cornelius blanches as you present the ballpoint ink analysis: 'Your uncle wrote only in antique Prussian fountain ink! You signed this note with your ballpoint!' Cornelius breaks down: 'He was disinheriting me on Monday! I rigged the wire to the clock weight so the door locked itself at 9:00 PM while I drank openly at the tavern!'"
        }
      },
      {
        id: "S02", name: "Miriam Black", role: "Master Locksmith",
        charId: "elena",
        alibi: "I left the manor at 8:15 PM and caught the 8:30 PM omnibus at the crossroads.",
        motive: "We had royalties disputes, but dead men don't sign royalty checks.",
        greeting: "A slide bolt is child's play, Detective, but it can only be thrown from inside without a mechanism.",
        isCulprit: false,
        contradiction: { evId: "E08", res: "Miriam presents her ticket: 'The omnibus driver stamped my fare at 8:30 PM.'" }
      },
      {
        id: "W01", name: "Constable Higgins", role: "Village Officer (Witness)",
        charId: "bert",
        isWitness: true,
        testimony: "At 9:15 PM, we broke through the study window. The door bolt was thrown solid from inside, and a small puddle of water lay beneath the brass casing.",
        greeting: "Never seen a locked room like it, Inspector. Bolted solid from the inside.",
        isCulprit: false
      }
    ],
    deductions: [
      { id1: "E01", id2: "E03", title: "Automated Mechanical Lock", insight: "Tension wire linked the slide bolt to the clock's 9:00 PM chime weight." },
      { id1: "E02", id2: "E05", title: "Forged Suicide & Disinheritance", insight: "The ballpoint forged note and will draft expose Cornelius's panic over being cut off." }
    ],
    eliminations: [
      { id: "S02", name: "Miriam Black", reason: "Eliminated: Ticket stub verifies departure on 8:30 PM omnibus." }
    ]
  }
};

class GameState {
  constructor() {
    this.caseNum = 1;
    this.score = 100;
    this.energy = 100;
    this.discoveredEvidence = new Set();
    this.brokenAlibis = new Set();
    this.examinedHotspots = new Set();
    this.unlockedDeductions = new Set();
    this.leads = [];
    this.activeSuspect = null;
    this.cluePin1 = null;
    this.cluePin2 = null;
    this.accusedSuspect = null;
    this.accusedEvidence = null;
    this.accusedMotive = null;
  }
}

class WebGameApp {
  constructor() {
    this.audio = new AudioFX();
    this.state = new GameState();
    this.dialogueQueue = [];
    this.currentDialogue = null;
    this.typewriterTimer = null;
    this.displayedChars = 0;
    this.currentSceneIdx = 0;

    this.initDOM();
    this.bindEvents();
    this.showScreen('menu-screen');
  }

  initDOM() {
    this.bgEl = document.getElementById('scene-bg');
    this.portraitEl = document.getElementById('character-portrait');
    this.dialogueWrap = document.getElementById('dialogue-wrap');
    this.dialogueName = document.getElementById('dialogue-name');
    this.dialogueText = document.getElementById('dialogue-text');
    this.dialogueAdvance = document.getElementById('dialogue-advance');
    this.toastEl = document.getElementById('toast');
    this.objectionEl = document.getElementById('objection-banner');
    this.modalEl = document.getElementById('modal-overlay');
    this.modalContent = document.getElementById('modal-content');
  }

  bindEvents() {
    // Menu buttons
    document.getElementById('btn-new-game').onclick = () => this.showScreen('case-select-screen');
    document.getElementById('btn-continue').onclick = () => this.loadGame();
    document.getElementById('btn-how-to-play').onclick = () => this.showScreen('how-to-play-screen');
    document.getElementById('btn-credits').onclick = () => this.showScreen('credits-screen');

    // Case Selection
    document.getElementById('btn-case-1').onclick = () => this.startCase(1);
    document.getElementById('btn-case-2').onclick = () => this.startCase(2);
    document.getElementById('btn-case-3').onclick = () => this.startCase(3);
    document.getElementById('btn-case-back').onclick = () => this.showScreen('menu-screen');

    // Navigation Back buttons
    document.getElementById('btn-htp-back').onclick = () => this.showScreen('menu-screen');
    document.getElementById('btn-credits-back').onclick = () => this.showScreen('menu-screen');

    // Dialogue click-to-advance
    this.dialogueWrap.onclick = () => this.handleDialogueClick();
    document.addEventListener('keydown', (e) => {
      if (e.code === 'Space' || e.code === 'Enter') {
        if (this.modalEl.style.display !== 'none') {
          this.closeModal();
        } else if (this.dialogueWrap.style.display !== 'none') {
          this.handleDialogueClick();
        }
      }
    });

    // Hub Action Buttons
    document.getElementById('btn-nav-interrogate').onclick = () => this.openInterrogation();
    document.getElementById('btn-nav-clueboard').onclick = () => this.openClueBoard();
    document.getElementById('btn-nav-notebook').onclick = () => this.openNotebook();
    document.getElementById('btn-nav-accuse').onclick = () => this.openAccusation();
    document.getElementById('btn-hub-menu').onclick = () => this.showScreen('menu-screen');
    document.getElementById('btn-hub-save').onclick = () => this.saveGame();
    document.getElementById('btn-hub-hint').onclick = () => this.requestHint();

    // Modal Close
    this.modalEl.onclick = () => this.closeModal();
  }

  showToast(msg, duration = 3000) {
    this.toastEl.innerText = msg;
    this.toastEl.classList.add('show');
    clearTimeout(this._toastTimer);
    this._toastTimer = setTimeout(() => {
      this.toastEl.classList.remove('show');
    }, duration);
  }

  showScreen(screenId) {
    document.querySelectorAll('.screen').forEach(s => s.classList.remove('active'));
    const target = document.getElementById(screenId);
    if (target) target.classList.add('active');

    // Screen specific backdrops
    if (screenId === 'menu-screen' || screenId === 'case-select-screen' || screenId === 'how-to-play-screen' || screenId === 'credits-screen') {
      this.setBg('assets/backgrounds/main_menu.png');
      this.hidePortrait();
      this.hideDialogue();
    }
  }

  setBg(src) {
    this.bgEl.style.backgroundImage = `url('${src}')`;
  }

  showPortrait(charId, expression = 'neutral') {
    const src = `assets/characters/${charId}_${expression}.png`;
    this.portraitEl.src = src;
    this.portraitEl.style.display = 'block';
  }

  hidePortrait() {
    this.portraitEl.style.display = 'none';
  }

  updateHUD() {
    const curCase = CASE_DATA[this.state.caseNum];
    document.getElementById('hud-case-title').innerText = curCase.title;
    document.getElementById('hud-score').innerText = `Score: ${this.state.score}`;
    document.getElementById('hud-energy-text').innerText = `Energy ${this.state.energy}/100`;
    document.getElementById('hud-energy-fill').style.width = `${Math.max(0, Math.min(100, this.state.energy))}%`;
  }

  startCase(num) {
    this.state = new GameState();
    this.state.caseNum = num;
    this.currentSceneIdx = 0;
    this.showScreen('briefing-screen');
    this.setBg('assets/backgrounds/briefing_room.png');
    this.showPortrait('detective', 'thinking');

    const curCase = CASE_DATA[num];
    this.startDialogue([
      { speaker: "Detective Sherlock", charId: "detective", expr: "thinking", text: "Scotland Yard Dispatch has called us in on urgent duty. A complex mystery has unfolded." },
      { speaker: "Detective Sherlock", charId: "detective", expr: "neutral", text: `${curCase.title}: ${curCase.tagline}` },
      { speaker: "Incident Briefing", charId: "detective", expr: "neutral", text: curCase.briefing },
      { speaker: "Detective Sherlock", charId: "detective", expr: "pointing", text: "Search the crime scenes, collect forensic evidence, expose contradictions, and deliver justice!", onFinish: () => this.openInvestigation() }
    ]);
  }

  startDialogue(entries) {
    this.dialogueQueue = [...entries];
    this.dialogueWrap.style.display = 'block';
    this.nextDialogue();
  }

  nextDialogue() {
    if (this.dialogueQueue.length === 0) {
      this.hideDialogue();
      return;
    }
    this.currentDialogue = this.dialogueQueue.shift();
    this.dialogueName.innerText = this.currentDialogue.speaker || "Investigator";
    if (this.currentDialogue.charId) {
      this.showPortrait(this.currentDialogue.charId, this.currentDialogue.expr || 'neutral');
    }

    this.displayedChars = 0;
    this.dialogueText.innerText = '';
    this.dialogueAdvance.style.display = 'none';
    clearInterval(this.typewriterTimer);

    const fullText = this.currentDialogue.text;
    this.typewriterTimer = setInterval(() => {
      if (this.displayedChars < fullText.length) {
        this.displayedChars++;
        this.dialogueText.innerText = fullText.slice(0, this.displayedChars);
        if (this.displayedChars % 2 === 0) {
          this.audio.playTypewriter();
        }
      } else {
        clearInterval(this.typewriterTimer);
        this.typewriterTimer = null;
        this.dialogueAdvance.style.display = 'block';
        if (this.currentDialogue.onFinish) {
          this.currentDialogue.onFinish();
        }
      }
    }, 22);
  }

  handleDialogueClick() {
    if (this.typewriterTimer) {
      // Click 1: Finish typing immediately
      clearInterval(this.typewriterTimer);
      this.typewriterTimer = null;
      this.dialogueText.innerText = this.currentDialogue.text;
      this.dialogueAdvance.style.display = 'block';
      if (this.currentDialogue.onFinish) {
        this.currentDialogue.onFinish();
      }
    } else {
      // Click 2: Advance to next entry
      this.nextDialogue();
    }
  }

  hideDialogue() {
    this.dialogueWrap.style.display = 'none';
    clearInterval(this.typewriterTimer);
    this.typewriterTimer = null;
  }

  // -------------------------------------------------------------
  // Investigation Screen & Hotspots
  // -------------------------------------------------------------
  openInvestigation() {
    this.hideDialogue();
    this.hidePortrait();
    this.showScreen('investigation-screen');
    this.updateHUD();
    this.renderSceneTabs();
    this.renderCurrentScene();
  }

  renderSceneTabs() {
    const curCase = CASE_DATA[this.state.caseNum];
    const container = document.getElementById('scene-tabs');
    container.innerHTML = '';
    curCase.scenes.forEach((sc, idx) => {
      const btn = document.createElement('button');
      btn.className = `btn ${idx === this.currentSceneIdx ? 'btn-primary' : ''}`;
      btn.innerText = sc.name;
      btn.onclick = () => {
        this.currentSceneIdx = idx;
        this.renderSceneTabs();
        this.renderCurrentScene();
      };
      container.appendChild(btn);
    });
  }

  renderCurrentScene() {
    const curCase = CASE_DATA[this.state.caseNum];
    const scene = curCase.scenes[this.currentSceneIdx];
    this.setBg(scene.bg);

    const hotspotContainer = document.getElementById('hotspots-layer');
    hotspotContainer.innerHTML = '';

    scene.hotspots.forEach((hs, idx) => {
      const key = `${scene.id}_${idx}`;
      const isExamined = this.state.examinedHotspots.has(key);

      const el = document.createElement('div');
      el.className = 'hotspot-node';
      el.style.left = `${hs.x}%`;
      el.style.top = `${hs.y}%`;

      el.innerHTML = `
        <div class="hotspot-ring ${isExamined ? 'examined' : ''}"></div>
        <div class="hotspot-core ${isExamined ? 'examined' : ''}"></div>
        <div class="hotspot-tooltip">${hs.name} ${isExamined ? '[EXAMINED]' : ''}</div>
      `;

      el.onclick = () => this.examineHotspot(scene, hs, key);
      hotspotContainer.appendChild(el);
    });
  }

  examineHotspot(scene, hs, key) {
    this.state.examinedHotspots.add(key);
    this.renderCurrentScene();

    if (hs.evId) {
      const isNew = !this.state.discoveredEvidence.has(hs.evId);
      this.state.discoveredEvidence.add(hs.evId);
      const ev = CASE_DATA[this.state.caseNum].evidence[hs.evId];

      if (isNew && ev) {
        this.audio.playChime();
        this.state.score += 25;
        this.state.energy = Math.max(0, this.state.energy - 10);
        this.state.leads.push(`Discovered [${ev.id}] ${ev.name} at ${scene.name}`);
        this.updateHUD();

        // Show Evidence Card Modal
        this.openEvidenceModal(ev);
      } else {
        this.showToast(`Already documented: ${ev ? ev.name : hs.evId}`);
      }
    } else {
      this.showToast(`Inspected: ${hs.desc}`);
    }
  }

  openEvidenceModal(ev) {
    this.modalContent.innerHTML = `
      <h3 style="color:var(--gold); font-size:1.1rem; margin-bottom:10px;">[NEW EVIDENCE SECURED]</h3>
      <h1 style="color:#fff; font-size:1.6rem; margin-bottom:8px;">${ev.id} — ${ev.name}</h1>
      <p style="color:var(--cyan); font-size:0.9rem; margin-bottom:18px;">Category: ${ev.cat} | Relevance: ${ev.rel}</p>
      <p style="line-height:1.6; font-size:1.05rem; margin-bottom:24px; color:#cbd5e1;">${ev.desc}</p>
      <button class="btn btn-primary" onclick="window.gameApp.closeModal()">Continue Investigation >></button>
    `;
    this.modalEl.style.display = 'flex';
  }

  closeModal() {
    this.modalEl.style.display = 'none';
  }

  // -------------------------------------------------------------
  // Interrogation Screen
  // -------------------------------------------------------------
  openInterrogation() {
    this.showScreen('interrogation-screen');
    this.setBg('assets/backgrounds/interrogation_room.png');
    this.updateHUD();

    const curCase = CASE_DATA[this.state.caseNum];
    const listEl = document.getElementById('suspect-list');
    listEl.innerHTML = '';

    curCase.suspects.forEach(s => {
      const btn = document.createElement('button');
      btn.className = `btn ${this.state.activeSuspect && this.state.activeSuspect.id === s.id ? 'btn-primary' : ''}`;
      btn.style.width = '100%';
      btn.style.marginBottom = '10px';
      btn.innerText = `${s.name} (${s.role})`;
      btn.onclick = () => this.selectSuspect(s);
      listEl.appendChild(btn);
    });

    if (!this.state.activeSuspect && curCase.suspects.length > 0) {
      this.selectSuspect(curCase.suspects[0]);
    } else if (this.state.activeSuspect) {
      this.selectSuspect(this.state.activeSuspect);
    }
  }

  selectSuspect(s) {
    this.state.activeSuspect = s;
    const isBroken = this.state.brokenAlibis.has(s.id);
    this.showPortrait(s.charId, isBroken ? 'broken' : 'neutral');

    // Action buttons
    const actionsEl = document.getElementById('interrogate-actions');
    actionsEl.innerHTML = '';

    if (s.isWitness) {
      const b = document.createElement('button');
      b.className = 'btn btn-primary';
      b.innerText = 'Request Sworn Testimony';
      b.onclick = () => {
        this.showPortrait(s.charId, 'earnest');
        this.startDialogue([{ speaker: s.name, charId: s.charId, expr: 'earnest', text: s.testimony }]);
      };
      actionsEl.appendChild(b);
    } else {
      // 1. Alibi
      const b1 = document.createElement('button');
      b1.className = 'btn';
      b1.innerText = '1. Inquire about Alibi';
      b1.onclick = () => {
        const expr = isBroken ? 'broken' : 'neutral';
        this.showPortrait(s.charId, expr);
        this.startDialogue([{ speaker: s.name, charId: s.charId, expr: expr, text: s.alibi }]);
      };
      actionsEl.appendChild(b1);

      // 2. Motive
      const b2 = document.createElement('button');
      b2.className = 'btn';
      b2.innerText = '2. Inquire about Motive';
      b2.onclick = () => {
        this.showPortrait(s.charId, 'nervous');
        this.startDialogue([{ speaker: s.name, charId: s.charId, expr: 'nervous', text: s.motive }]);
      };
      actionsEl.appendChild(b2);

      // 3. Confront Contradiction!
      const b3 = document.createElement('button');
      b3.className = 'btn btn-danger';
      b3.innerText = '3. CONFRONT CONTRADICTION!';
      b3.onclick = () => this.openContradictionScreen(s);
      actionsEl.appendChild(b3);
    }

    // Play greeting
    this.startDialogue([{ speaker: s.name, charId: s.charId, expr: 'neutral', text: s.greeting }]);
  }

  // -------------------------------------------------------------
  // Contradiction Screen
  // -------------------------------------------------------------
  openContradictionScreen(s) {
    this.hideDialogue();
    this.showScreen('contradiction-screen');
    this.setBg('assets/backgrounds/interrogation_room.png');
    this.showPortrait(s.charId, 'neutral');

    document.getElementById('contra-statement').innerText = `Suspect Claim: "${s.alibi}"`;
    const grid = document.getElementById('contra-evidence-grid');
    grid.innerHTML = '';

    const curCase = CASE_DATA[this.state.caseNum];
    this.state.discoveredEvidence.forEach(eid => {
      const ev = curCase.evidence[eid];
      if (!ev) return;

      const card = document.createElement('div');
      card.className = 'btn';
      card.style.display = 'flex';
      card.style.flexDirection = 'column';
      card.style.alignItems = 'flex-start';
      card.style.padding = '14px';
      card.style.textAlign = 'left';

      card.innerHTML = `
        <span style="color:var(--gold); font-size:0.85rem; font-weight:bold;">${ev.id} • ${ev.cat}</span>
        <strong style="color:#fff; font-size:1.05rem; margin:4px 0;">${ev.name}</strong>
        <span style="color:var(--text-dim); font-size:0.8rem;">${ev.rel}</span>
      `;

      card.onclick = () => this.submitContradiction(s, ev);
      grid.appendChild(card);
    });

    document.getElementById('btn-contra-cancel').onclick = () => this.openInterrogation();
  }

  submitContradiction(s, ev) {
    if (s.isCulprit && s.contradiction && s.contradiction.evId === ev.id) {
      // Success! OBJECTION!
      this.audio.playObjection();
      this.triggerObjectionBanner();
      this.state.score += 50;
      this.state.brokenAlibis.add(s.id);
      this.state.leads.push(`${s.name}'s statement shattered using ${ev.id}!`);
      this.updateHUD();

      setTimeout(() => {
        this.openInterrogation();
        this.showPortrait(s.charId, 'broken');
        this.startDialogue([{ speaker: s.name, charId: s.charId, expr: 'broken', text: s.contradiction.res }]);
        this.showToast("CONTRADICTION EXPOSED! Alibi Shattered! (+50 Points)", 4000);
      }, 1400);

    } else {
      // Failed!
      this.audio.playBuzzer();
      this.state.score = Math.max(0, this.state.score - 15);
      this.updateHUD();
      this.openInterrogation();
      this.showPortrait(s.charId, 'angry');
      this.startDialogue([{ speaker: s.name, charId: s.charId, expr: 'angry', text: "That proves nothing against me, Detective! Check your facts before making reckless claims." }]);
      this.showToast("Contradiction Failed! (-15 Points)", 3000);
    }
  }

  triggerObjectionBanner() {
    this.objectionEl.classList.add('show');
    setTimeout(() => {
      this.objectionEl.classList.remove('show');
    }, 1200);
  }

  // -------------------------------------------------------------
  // Clue Board Screen
  // -------------------------------------------------------------
  openClueBoard() {
    this.hideDialogue();
    this.hidePortrait();
    this.showScreen('clueboard-screen');
    this.setBg('assets/backgrounds/clueboard_bg.png');
    this.updateHUD();

    this.state.cluePin1 = null;
    this.state.cluePin2 = null;
    this.renderCluePins();

    document.getElementById('btn-clue-connect').onclick = () => this.connectPinnedClues();
    document.getElementById('btn-clue-return').onclick = () => this.openInvestigation();
  }

  renderCluePins() {
    const grid = document.getElementById('clue-pins-grid');
    grid.innerHTML = '';

    const curCase = CASE_DATA[this.state.caseNum];
    this.state.discoveredEvidence.forEach(eid => {
      const ev = curCase.evidence[eid];
      if (!ev) return;

      const isSelected = (this.state.cluePin1 === eid || this.state.cluePin2 === eid);

      const card = document.createElement('div');
      card.className = `btn ${isSelected ? 'btn-primary' : ''}`;
      card.style.display = 'flex';
      card.style.flexDirection = 'column';
      card.style.alignItems = 'flex-start';
      card.style.padding = '12px';
      card.style.border = isSelected ? '2px solid var(--gold)' : '1px solid var(--border)';

      card.innerHTML = `
        <span style="color:var(--gold); font-size:0.8rem; font-weight:bold;">${ev.id}</span>
        <strong style="color:#fff; font-size:0.95rem;">${ev.name}</strong>
      `;

      card.onclick = () => {
        if (!this.state.cluePin1) {
          this.state.cluePin1 = eid;
          this.showToast(`Pin 1: ${eid}`);
        } else if (!this.state.cluePin2 && this.state.cluePin1 !== eid) {
          this.state.cluePin2 = eid;
          this.showToast(`Pin 2: ${eid}. Click Connect!`);
        } else {
          this.state.cluePin1 = eid;
          this.state.cluePin2 = null;
        }
        this.renderCluePins();
      };

      grid.appendChild(card);
    });
  }

  connectPinnedClues() {
    if (!this.state.cluePin1 || !this.state.cluePin2) {
      this.showToast("Select two clues to connect.");
      return;
    }

    const curCase = CASE_DATA[this.state.caseNum];
    const match = curCase.deductions.find(d =>
      (d.id1 === this.state.cluePin1 && d.id2 === this.state.cluePin2) ||
      (d.id1 === this.state.cluePin2 && d.id2 === this.state.cluePin1)
    );

    if (match) {
      const key = `${match.id1}_${match.id2}`;
      if (!this.state.unlockedDeductions.has(key)) {
        this.audio.playChime();
        this.state.unlockedDeductions.add(key);
        this.state.score += 35;
        this.state.leads.push(`Deduction: [${match.title}] ${match.insight}`);
        this.updateHUD();
        this.showToast(`DEDUCTION UNLOCKED: ${match.title}`, 4500);
      } else {
        this.showToast("These clues are already connected.");
      }
    } else {
      this.showToast("No logical deduction between these clues.");
    }

    this.state.cluePin1 = null;
    this.state.cluePin2 = null;
    this.renderCluePins();
  }

  // -------------------------------------------------------------
  // Notebook Screen
  // -------------------------------------------------------------
  openNotebook() {
    this.hideDialogue();
    this.hidePortrait();
    this.showScreen('notebook-screen');
    this.setBg('assets/backgrounds/briefing_room.png');

    const container = document.getElementById('notebook-leads');
    container.innerHTML = '';

    if (this.state.leads.length === 0) {
      container.innerHTML = '<p style="color:var(--text-dim); padding:20px;">No leads recorded yet. Examine hotspots and interrogate suspects.</p>';
    } else {
      this.state.leads.forEach(l => {
        const item = document.createElement('div');
        item.style.padding = '10px 14px';
        item.style.marginBottom = '8px';
        item.style.background = 'rgba(30, 41, 59, 0.6)';
        item.style.borderLeft = '3px solid var(--gold)';
        item.style.fontSize = '0.95rem';
        item.innerText = l;
        container.appendChild(item);
      });
    }

    document.getElementById('btn-notebook-back').onclick = () => this.openInvestigation();
  }

  // -------------------------------------------------------------
  // Accusation & Verdict
  // -------------------------------------------------------------
  openAccusation() {
    this.hideDialogue();
    this.hidePortrait();
    this.showScreen('accusation-screen');
    this.setBg('assets/backgrounds/courtroom.png');

    const curCase = CASE_DATA[this.state.caseNum];

    // 1. Suspects
    const suspContainer = document.getElementById('accuse-suspects');
    suspContainer.innerHTML = '';
    curCase.suspects.filter(s => !s.isWitness).forEach(s => {
      const btn = document.createElement('button');
      btn.className = `btn ${this.state.accusedSuspect === s.id ? 'btn-primary' : ''}`;
      btn.style.width = '100%';
      btn.style.marginBottom = '8px';
      btn.innerText = s.name;
      btn.onclick = () => {
        this.state.accusedSuspect = s.id;
        this.openAccusation();
      };
      suspContainer.appendChild(btn);
    });

    // 2. Evidence
    const evContainer = document.getElementById('accuse-evidence');
    evContainer.innerHTML = '';
    this.state.discoveredEvidence.forEach(eid => {
      const ev = curCase.evidence[eid];
      if (!ev) return;
      const btn = document.createElement('button');
      btn.className = `btn ${this.state.accusedEvidence === eid ? 'btn-primary' : ''}`;
      btn.style.width = '100%';
      btn.style.marginBottom = '8px';
      btn.innerText = `${ev.id}: ${ev.name}`;
      btn.onclick = () => {
        this.state.accusedEvidence = eid;
        this.openAccusation();
      };
      evContainer.appendChild(btn);
    });

    // 3. Motives
    const motives = [
      { key: "DEBT", label: "1. Financial Debt / Blackmail" },
      { key: "EMBEZZLEMENT", label: "2. Embezzlement Cover-Up" },
      { key: "PATENT", label: "3. Disinheritance / Patent Theft" },
      { key: "VENDETTA", label: "4. Personal Revenge / Jealousy" }
    ];
    const motContainer = document.getElementById('accuse-motives');
    motContainer.innerHTML = '';
    motives.forEach(m => {
      const btn = document.createElement('button');
      btn.className = `btn ${this.state.accusedMotive === m.key ? 'btn-primary' : ''}`;
      btn.style.width = '100%';
      btn.style.marginBottom = '8px';
      btn.innerText = m.label;
      btn.onclick = () => {
        this.state.accusedMotive = m.key;
        this.openAccusation();
      };
      motContainer.appendChild(btn);
    });

    document.getElementById('btn-submit-accusation').onclick = () => this.evaluateVerdict();
    document.getElementById('btn-accuse-cancel').onclick = () => this.openInvestigation();
  }

  evaluateVerdict() {
    if (!this.state.accusedSuspect || !this.state.accusedEvidence || !this.state.accusedMotive) {
      this.showToast("Select a suspect, decisive evidence, and motive keyword!");
      return;
    }

    const curCase = CASE_DATA[this.state.caseNum];
    const isCorrectCulprit = (this.state.accusedSuspect === curCase.culpritId);
    const hasKeyEvidence = (this.state.accusedEvidence === curCase.decisiveEvidenceId);
    const isAlibiBroken = this.state.brokenAlibis.has(curCase.culpritId);
    const isMotiveCorrect = (this.state.accusedMotive === curCase.motiveKeyword);

    const isMaster = isCorrectCulprit && hasKeyEvidence && isAlibiBroken && isMotiveCorrect;

    this.showScreen('verdict-screen');
    this.setBg('assets/backgrounds/courtroom.png');

    if (isMaster) {
      this.audio.playChime();
      document.getElementById('verdict-title').innerText = "MASTER DETECTIVE — CASE CLOSED!";
      document.getElementById('verdict-title').style.color = "var(--gold)";
      document.getElementById('verdict-rank').innerText = `Final Score: ${this.state.score + 250} | Performance Rank: S (Flawless Sleuth)`;
      document.getElementById('verdict-desc').innerText = curCase.solution;
    } else {
      this.audio.playBuzzer();
      document.getElementById('verdict-title').innerText = "ACCUSATION COLLAPSED";
      document.getElementById('verdict-title').style.color = "var(--red)";
      document.getElementById('verdict-rank').innerText = `Final Score: ${this.state.score} | Performance Rank: C (Insufficient Proof)`;
      document.getElementById('verdict-desc').innerText = "The prosecution failed to substantiate the decisive physical evidence and timeline before the High Magistrate. The true culprit escaped conviction.";
    }

    const elimContainer = document.getElementById('verdict-eliminations');
    elimContainer.innerHTML = '';
    curCase.eliminations.forEach(el => {
      const p = document.createElement('p');
      p.style.fontSize = '0.9rem';
      p.style.marginBottom = '6px';
      p.style.color = 'var(--text-dim)';
      p.innerHTML = `<strong style="color:var(--text-main);">${el.name}:</strong> ${el.reason}`;
      elimContainer.appendChild(p);
    });

    document.getElementById('btn-verdict-menu').onclick = () => this.showScreen('menu-screen');
  }

  // -------------------------------------------------------------
  // Save & Load Functionality
  // -------------------------------------------------------------
  saveGame() {
    const data = {
      caseNum: this.state.caseNum,
      score: this.state.score,
      energy: this.state.energy,
      discoveredEvidence: Array.from(this.state.discoveredEvidence),
      brokenAlibis: Array.from(this.state.brokenAlibis),
      examinedHotspots: Array.from(this.state.examinedHotspots),
      unlockedDeductions: Array.from(this.state.unlockedDeductions),
      leads: this.state.leads
    };
    localStorage.setItem('case_closed_web_save', JSON.stringify(data));
    this.showToast("Investigation saved to browser storage!", 3000);
  }

  loadGame() {
    const raw = localStorage.getItem('case_closed_web_save');
    if (!raw) {
      this.showToast("No saved investigation found in browser storage!");
      return;
    }
    try {
      const data = JSON.parse(raw);
      this.state = new GameState();
      this.state.caseNum = data.caseNum || 1;
      this.state.score = data.score || 100;
      this.state.energy = data.energy || 100;
      this.state.discoveredEvidence = new Set(data.discoveredEvidence || []);
      this.state.brokenAlibis = new Set(data.brokenAlibis || []);
      this.state.examinedHotspots = new Set(data.examinedHotspots || []);
      this.state.unlockedDeductions = new Set(data.unlockedDeductions || []);
      this.state.leads = data.leads || [];

      this.showToast("Investigation resumed!", 3000);
      this.openInvestigation();
    } catch (e) {
      this.showToast("Failed to parse save data!");
    }
  }

  requestHint() {
    if (this.state.energy >= 10) {
      this.state.energy -= 10;
      this.updateHUD();
      if (this.state.caseNum === 1) {
        this.showToast("Hint: Compare Marcus Cruz's claimed patrol time with the Electronic Access Log (E03). Can a guard be in two places at once?", 6000);
      } else if (this.state.caseNum === 2) {
        this.showToast("Hint: The Conductor saw gold eagle cufflinks on the tea recipient. Examine the runner track (E02)!", 6000);
      } else {
        this.showToast("Hint: Compare the ink on the suicide note with Thorne's authentic fountain ink!", 6000);
      }
    } else {
      this.showToast("Not enough energy for an investigative hint!");
    }
  }
}

// Global bootstrap
window.addEventListener('DOMContentLoaded', () => {
  window.gameApp = new WebGameApp();
});
