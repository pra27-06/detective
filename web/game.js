// CASE CLOSED — Detective Mystery Simulator (Anime Visual Novel Edition)
// Comprehensive Procedural Audio Engine, Ambient Particle System, Clueboard Threads,
// Timeline Reconstruction, Character Dossiers, Achievements, and The Obsidian Hourglass Lore.

// =========================================================================
// 1. PROCEDURAL AUDIO SYNTHESIZER & AMBIENCE ENGINE
// =========================================================================
class AudioEngine {
  constructor() {
    this.ctx = null;
    this.masterGain = null;
    this.musicGain = null;
    this.sfxGain = null;
    this.ambienceGain = null;

    this.masterVol = 0.8;
    this.musicVol = 0.7;
    this.sfxVol = 0.85;
    this.ambienceVol = 0.6;
    this.isMuted = false;

    this.currentBgm = null;
    this.bgmTimer = null;
    this.bgmStep = 0;

    this.rainNode = null;
    this.clockTimer = null;

    this.init();
  }

  init() {
    const AudioContext = window.AudioContext || window.webkitAudioContext;
    if (AudioContext) {
      this.ctx = new AudioContext();
      this.masterGain = this.ctx.createGain();
      this.musicGain = this.ctx.createGain();
      this.sfxGain = this.ctx.createGain();
      this.ambienceGain = this.ctx.createGain();

      this.musicGain.connect(this.masterGain);
      this.sfxGain.connect(this.masterGain);
      this.ambienceGain.connect(this.masterGain);
      this.masterGain.connect(this.ctx.destination);

      this.applyVolumes();
    }
  }

  resume() {
    if (this.ctx && this.ctx.state === 'suspended') {
      this.ctx.resume();
    }
  }

  applyVolumes() {
    if (!this.ctx) return;
    const now = this.ctx.currentTime;
    const mult = this.isMuted ? 0 : 1;
    this.masterGain.gain.setValueAtTime(this.masterVol * mult, now);
    this.musicGain.gain.setValueAtTime(this.musicVol, now);
    this.sfxGain.gain.setValueAtTime(this.sfxVol, now);
    this.ambienceGain.gain.setValueAtTime(this.ambienceVol, now);
  }

  setMasterVolume(val) { this.masterVol = val; this.applyVolumes(); }
  setMusicVolume(val) { this.musicVol = val; this.applyVolumes(); }
  setSFXVolume(val) { this.sfxVol = val; this.applyVolumes(); }
  setSfxVolume(val) { this.setSFXVolume(val); }
  setAmbienceVolume(val) { this.ambienceVol = val; this.applyVolumes(); }
  toggleMute(forceVal) {
    this.isMuted = (typeof forceVal === 'boolean') ? forceVal : !this.isMuted;
    this.applyVolumes();
    return this.isMuted;
  }

  // -------------------------------------------------------------
  // Procedural Sound Effects
  // -------------------------------------------------------------
  playTypewriter() {
    if (!this.ctx || this.isMuted) return;
    this.resume();
    const osc = this.ctx.createOscillator();
    const gain = this.ctx.createGain();
    osc.type = 'sine';
    osc.frequency.setValueAtTime(750 + Math.random() * 200, this.ctx.currentTime);
    gain.gain.setValueAtTime(0.012, this.ctx.currentTime);
    gain.gain.exponentialRampToValueAtTime(0.001, this.ctx.currentTime + 0.025);
    osc.connect(gain);
    gain.connect(this.sfxGain);
    osc.start();
    osc.stop(this.ctx.currentTime + 0.025);
  }

  playChime() {
    if (!this.ctx || this.isMuted) return;
    this.resume();
    const freqs = [523.25, 659.25, 783.99, 1046.50]; // C5, E5, G5, C6
    freqs.forEach((f, i) => {
      const osc = this.ctx.createOscillator();
      const gain = this.ctx.createGain();
      osc.type = 'triangle';
      osc.frequency.setValueAtTime(f, this.ctx.currentTime + i * 0.07);
      gain.gain.setValueAtTime(0.12, this.ctx.currentTime + i * 0.07);
      gain.gain.exponentialRampToValueAtTime(0.001, this.ctx.currentTime + i * 0.07 + 0.55);
      osc.connect(gain);
      gain.connect(this.sfxGain);
      osc.start(this.ctx.currentTime + i * 0.07);
      osc.stop(this.ctx.currentTime + i * 0.07 + 0.55);
    });
  }

  playHiddenClue() {
    if (!this.ctx || this.isMuted) return;
    this.resume();
    const freqs = [1046.5, 1318.5, 1567.98, 2093.00, 2637.02];
    freqs.forEach((f, i) => {
      const osc = this.ctx.createOscillator();
      const gain = this.ctx.createGain();
      osc.type = 'sine';
      osc.frequency.setValueAtTime(f, this.ctx.currentTime + i * 0.06);
      gain.gain.setValueAtTime(0.14, this.ctx.currentTime + i * 0.06);
      gain.gain.exponentialRampToValueAtTime(0.001, this.ctx.currentTime + i * 0.06 + 0.8);
      osc.connect(gain);
      gain.connect(this.sfxGain);
      osc.start(this.ctx.currentTime + i * 0.06);
      osc.stop(this.ctx.currentTime + i * 0.06 + 0.8);
    });
  }

  playObjection() {
    if (!this.ctx || this.isMuted) return;
    this.resume();
    const freqs = [130.81, 164.81, 196.00, 261.63, 523.25];
    freqs.forEach(f => {
      const osc = this.ctx.createOscillator();
      const gain = this.ctx.createGain();
      osc.type = 'sawtooth';
      osc.frequency.setValueAtTime(f, this.ctx.currentTime);
      gain.gain.setValueAtTime(0.24, this.ctx.currentTime);
      gain.gain.exponentialRampToValueAtTime(0.001, this.ctx.currentTime + 1.2);
      osc.connect(gain);
      gain.connect(this.sfxGain);
      osc.start();
      osc.stop(this.ctx.currentTime + 1.2);
    });

    // Sub-bass punch
    const sub = this.ctx.createOscillator();
    const subGain = this.ctx.createGain();
    sub.type = 'sine';
    sub.frequency.setValueAtTime(80, this.ctx.currentTime);
    sub.frequency.exponentialRampToValueAtTime(30, this.ctx.currentTime + 0.4);
    subGain.gain.setValueAtTime(0.3, this.ctx.currentTime);
    subGain.gain.exponentialRampToValueAtTime(0.001, this.ctx.currentTime + 0.4);
    sub.connect(subGain);
    subGain.connect(this.sfxGain);
    sub.start();
    sub.stop(this.ctx.currentTime + 0.4);
  }

  playBuzzer() {
    if (!this.ctx || this.isMuted) return;
    this.resume();
    const osc = this.ctx.createOscillator();
    const gain = this.ctx.createGain();
    osc.type = 'sawtooth';
    osc.frequency.setValueAtTime(140, this.ctx.currentTime);
    osc.frequency.setValueAtTime(120, this.ctx.currentTime + 0.15);
    gain.gain.setValueAtTime(0.16, this.ctx.currentTime);
    gain.gain.exponentialRampToValueAtTime(0.001, this.ctx.currentTime + 0.35);
    osc.connect(gain);
    gain.connect(this.sfxGain);
    osc.start();
    osc.stop(this.ctx.currentTime + 0.35);
  }

  playGavel() {
    if (!this.ctx || this.isMuted) return;
    this.resume();
    const osc = this.ctx.createOscillator();
    const gain = this.ctx.createGain();
    osc.type = 'triangle';
    osc.frequency.setValueAtTime(180, this.ctx.currentTime);
    osc.frequency.exponentialRampToValueAtTime(45, this.ctx.currentTime + 0.18);
    gain.gain.setValueAtTime(0.3, this.ctx.currentTime);
    gain.gain.exponentialRampToValueAtTime(0.001, this.ctx.currentTime + 0.2);
    osc.connect(gain);
    gain.connect(this.sfxGain);
    osc.start();
    osc.stop(this.ctx.currentTime + 0.2);
  }

  playThreadConnect() {
    if (!this.ctx || this.isMuted) return;
    this.resume();
    const osc = this.ctx.createOscillator();
    const gain = this.ctx.createGain();
    osc.type = 'sine';
    osc.frequency.setValueAtTime(440, this.ctx.currentTime);
    osc.frequency.exponentialRampToValueAtTime(880, this.ctx.currentTime + 0.12);
    gain.gain.setValueAtTime(0.12, this.ctx.currentTime);
    gain.gain.exponentialRampToValueAtTime(0.001, this.ctx.currentTime + 0.25);
    osc.connect(gain);
    gain.connect(this.sfxGain);
    osc.start();
    osc.stop(this.ctx.currentTime + 0.25);
  }

  // -------------------------------------------------------------
  // Procedural BGM Engine (Looping Multi-Voice Synthesizer)
  // -------------------------------------------------------------
  playBGM(trackName) {
    if (!this.ctx) return;
    this.resume();
    if (this.currentBgm === trackName) return;
    this.stopBGM();
    this.currentBgm = trackName;
    this.bgmStep = 0;

    // Define track scale notes (Hz)
    const tracks = {
      menu: [146.83, 220.00, 293.66, 349.23, 440.00, 349.23, 293.66, 220.00], // D minor arpeggios
      investigation: [164.81, 196.00, 246.94, 293.66, 329.63, 293.66, 246.94, 196.00], // E minor mystery
      tension: [138.59, 185.00, 233.08, 277.18, 311.13, 277.18, 233.08, 185.00], // Dissonant tension
      verdict: [196.00, 246.94, 293.66, 392.00, 493.88, 587.33, 493.88, 392.00] // G major triumph
    };

    const notes = tracks[trackName] || tracks.investigation;
    const intervalMs = trackName === 'tension' ? 380 : 480;

    this.bgmTimer = setInterval(() => {
      if (!this.ctx || this.isMuted) return;
      const note = notes[this.bgmStep % notes.length];
      const now = this.ctx.currentTime;

      // Voice 1: Melodic tone
      const osc = this.ctx.createOscillator();
      const gain = this.ctx.createGain();
      osc.type = trackName === 'tension' ? 'sawtooth' : 'triangle';
      osc.frequency.setValueAtTime(note, now);
      gain.gain.setValueAtTime(0.04, now);
      gain.gain.exponentialRampToValueAtTime(0.001, now + (intervalMs / 1000) * 0.9);
      osc.connect(gain);
      gain.connect(this.musicGain);
      osc.start(now);
      osc.stop(now + (intervalMs / 1000) * 0.9);

      // Voice 2: Deep bass foundation on downbeats
      if (this.bgmStep % 4 === 0) {
        const bass = this.ctx.createOscillator();
        const bassGain = this.ctx.createGain();
        bass.type = 'sine';
        bass.frequency.setValueAtTime(note / 2, now);
        bassGain.gain.setValueAtTime(0.08, now);
        bassGain.gain.exponentialRampToValueAtTime(0.001, now + (intervalMs / 1000) * 2.2);
        bass.connect(bassGain);
        bassGain.connect(this.musicGain);
        bass.start(now);
        bass.stop(now + (intervalMs / 1000) * 2.2);
      }

      this.bgmStep++;
    }, intervalMs);
  }

  stopBGM() {
    if (this.bgmTimer) {
      clearInterval(this.bgmTimer);
      this.bgmTimer = null;
    }
    this.currentBgm = null;
  }

  // -------------------------------------------------------------
  // Environmental Ambience Engine (Rain & Clock Tick)
  // -------------------------------------------------------------
  startAmbience(type) {
    if (!this.ctx) return;
    this.resume();
    this.stopAmbience();

    if (type === 'rain') {
      // White noise buffer bandpassed for rain
      const bufSize = this.ctx.sampleRate * 2;
      const buffer = this.ctx.createBuffer(1, bufSize, this.ctx.sampleRate);
      const data = buffer.getChannelData(0);
      for (let i = 0; i < bufSize; i++) {
        data[i] = Math.random() * 2 - 1;
      }
      const noise = this.ctx.createBufferSource();
      noise.buffer = buffer;
      noise.loop = true;

      const filter = this.ctx.createBiquadFilter();
      filter.type = 'bandpass';
      filter.frequency.setValueAtTime(1000, this.ctx.currentTime);
      filter.Q.setValueAtTime(1.2, this.ctx.currentTime);

      const gain = this.ctx.createGain();
      gain.gain.setValueAtTime(0.05, this.ctx.currentTime);

      noise.connect(filter);
      filter.connect(gain);
      gain.connect(this.ambienceGain);

      noise.start();
      this.rainNode = noise;

    } else if (type === 'clock') {
      // 1-second pendulum impulses
      let tick = true;
      this.clockTimer = setInterval(() => {
        if (!this.ctx || this.isMuted) return;
        const now = this.ctx.currentTime;
        const osc = this.ctx.createOscillator();
        const gain = this.ctx.createGain();
        osc.type = 'triangle';
        osc.frequency.setValueAtTime(tick ? 1100 : 850, now);
        gain.gain.setValueAtTime(0.03, now);
        gain.gain.exponentialRampToValueAtTime(0.001, now + 0.035);
        osc.connect(gain);
        gain.connect(this.ambienceGain);
        osc.start(now);
        osc.stop(now + 0.035);
        tick = !tick;
      }, 1000);
    }
  }

  stopAmbience() {
    if (this.rainNode) {
      try { this.rainNode.stop(); } catch (e) {}
      this.rainNode = null;
    }
    if (this.clockTimer) {
      clearInterval(this.clockTimer);
      this.clockTimer = null;
    }
  }
}

// =========================================================================
// 2. EXPANDED STORY DATABASE & OVERARCHING LORE
// =========================================================================
const CASE_DATA = {
  1: {
    id: "CASE-01",
    title: "Case #1: The Midnight Museum Heist",
    tagline: "The 45-carat sapphire disappears during a 4-minute blackout.",
    briefing: "Scotland Yard Dispatch has called us to the Royal Alexandria Museum at 1:30 AM. At 1:14 AM, the primary circuit breaker tripped, cloaking the gallery in darkness. In that exact four-minute window, the armored glass pedestal of the Star of Alexandria was breached and the sapphire stolen. Cross-examine the night staff, examine the crime scenes, and unmask the thief!",
    culpritId: "S02", // Guard Marcus Cruz
    decisiveEvidenceId: "E03", // Electronic Access Log
    motiveKeyword: "DEBT",
    solution: "Security Guard Marcus Cruz stole Curator Vance's spare keycard from the lost-and-found locker at 1:10 AM, swiped into the basement power substation at 1:12 AM, and cut the main breaker at 1:14 AM. Using pre-staged solvent and laser cutters, he seized the sapphire and sprinted through the maintenance shaft to swipe the North Wing patrol terminal at 1:15 AM to fabricate an alibi.",
    ambience: "rain",
    scenes: [
      {
        id: "SCENE_01",
        name: "The Sapphire Display Vault",
        bg: "assets/backgrounds/scene_vault.png",
        hotspots: [
          { x: 50, y: 58, name: "Shattered Pedestal", desc: "The armored pedestal fiber optic tripwire was cleanly cut at a 45-degree angle.", evId: "E01" },
          { x: 38, y: 78, name: "Vault Floor & Mud Traces", desc: "Dried mud flakes show a distinct cross-hatch combat boot tread, size 10.", evId: "E02" },
          { x: 66, y: 65, name: "Forgotten Silk Umbrella", desc: "A stylish monogrammed silk umbrella resting against a marble pillar.", evId: "E06" },
          { x: 82, y: 45, name: "Concealed Pillar Alcove", desc: "Behind the marble moulding: a red wax seal stamped with an Obsidian Hourglass insignia!", evId: "E09" }
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
          { x: 20, y: 46, name: "Locker #14 (Marcus Cruz)", desc: "A formal repossession demand for £12,000 overdue debt and loan shark threats.", evId: "E05" },
          { x: 50, y: 76, name: "Curator's Office Desk", desc: "A formal memorandum reporting his secondary museum master pass missing three days prior.", evId: "E07" }
        ]
      }
    ],
    evidence: {
      "E01": { id: "E01", name: "Cut Fiber Optic Cable", cat: "PHYSICAL", rel: "CRITICAL PROOF", desc: "Cleanly sliced optical wire indicating insider knowledge of alarm timing and junction routing." },
      "E02": { id: "E02", name: "Combat Boot Print Mud Flakes", cat: "FORENSIC", rel: "SUPPORTING PROOF", desc: "Heavy tactical size 10 boot treads matching standard museum night security uniform issue." },
      "E03": { id: "E03", name: "Electronic Access Log", cat: "DOCUMENT", rel: "DECISIVE EVIDENCE", desc: "Substation swiped at 1:12 AM; North Wing terminal swiped at 1:15 AM with Vance's stolen badge. Physically impossible for one person on foot without sprinting through the maintenance shaft." },
      "E04": { id: "E04", name: "Solvent-Soaked Workshop Rag", cat: "CHEMICAL", rel: "CRITICAL PROOF", desc: "Industrial acetone used to dissolve anti-shatter glass sealant around the Star of Alexandria." },
      "E05": { id: "E05", name: "Loan Shark Repossession Notice", cat: "DOCUMENT", rel: "MOTIVE EVIDENCE", desc: "Final notice demanding £12,000 from Marcus Cruz by midnight under threat of violence." },
      "E06": { id: "E06", name: "Monogrammed Silk Umbrella", cat: "PERSONAL", rel: "RED HERRING", desc: "Belongs to Lady Catherine Sterling, who departed at 11:15 PM and forgot it by the pillar." },
      "E07": { id: "E07", name: "Curator's Stolen Keycard Report", cat: "DOCUMENT", rel: "SUPPORTING PROOF", desc: "Notice proving Vance's secondary master pass was stolen from the guard desk." },
      "E08": { id: "E08", name: "Chemical Inventory Incident Sheet", cat: "DOCUMENT", rel: "SUPPORTING PROOF", desc: "Confirms industrial solvent was logged as missing at 10:00 PM by Elena Rostova." },
      "E09": { id: "E09", name: "Obsidian Hourglass Wax Seal", cat: "CONSPIRACY", rel: "HIDDEN SECRET", desc: "A black wax stamp depicting a winged hourglass—the signature of an underground syndicate." }
    },
    suspects: [
      {
        id: "S01", name: "Arthur Vance", role: "Museum Curator",
        charId: "vance", age: 52,
        alibi: "I was locked inside my 3rd-floor office reviewing catalog proofs from midnight to 1:20 AM without leaving.",
        motive: "The museum is under heavy debt, but insurance fraud would destroy our historical reputation.",
        greeting: "Detective, please tell me the Star of Alexandria has been located! Our reputation is ruined!",
        secret: "Curator Vance was heavily in debt from purchasing Renaissance artifacts on personal credit, but electronic office logs prove he never departed his desk.",
        isCulprit: false,
        contradiction: { evId: "E07", res: "Curator Vance nods: 'Yes, my spare card was stolen days ago. The thief used my badge to frame me!'" }
      },
      {
        id: "S02", name: "Marcus Cruz", role: "Night Security Guard",
        charId: "cruz", age: 34,
        alibi: "I was patrolling the North Wing gallery continuously from 1:00 AM to 1:20 AM. I swiped the terminal at 1:15 AM!",
        motive: "I work for my wages, Detective. I have no interest in antique jewels.",
        greeting: "Evening, Detective. Place is sealed tight. I followed patrol regulations to the letter.",
        secret: "Cruz's daughter Sofia needed £12,000 for emergency medical surgeries. A syndicate agent named 'Cain' offered to pay the debt in exchange for the sapphire.",
        isCulprit: true,
        contradiction: {
          evId: "E03",
          res: "Cruz's face pales as you display the server log: 'Vance's badge was swiped at the basement power breaker at 1:12 AM, then at the North Wing at 1:15 AM! Only the guard on duty could have sprinted through the staff passage!' Cruz falls to his knees: 'I had to do it... the loan sharks were going to break my legs tonight!'"
        }
      },
      {
        id: "S03", name: "Elena Rostova", role: "Master Conservator",
        charId: "elena", age: 29,
        alibi: "I left the museum at 10:30 PM after logging the missing solvent with security and was asleep at home.",
        motive: "I dedicate my life to preserving art, not pawning stolen artifacts.",
        greeting: "Good evening. If this is about the missing solvent, I reported the theft to Guard Cruz at 10:00 PM sharp.",
        secret: "Elena suspected Cruz was in trouble weeks ago when suspicious men in dark trench coats visited the staff entrance.",
        isCulprit: false,
        contradiction: { evId: "E08", res: "Elena sighs: 'My lab inventory confirms I logged the theft hours before the heist.'" }
      },
      {
        id: "S04", name: "Lady Catherine Sterling", role: "Benefactor & Trustee",
        charId: "sterling", age: 58,
        alibi: "I departed the museum reception at 11:15 PM in my private carriage and arrived home in Kensington.",
        motive: "I donated that sapphire in honor of my late husband. Why on earth would I steal my own bequest?",
        greeting: "This is outrageous! Scotland Yard should be hunting jewel thieves, not harassing patrons!",
        secret: "Mentions her husband Victor Sterling was traveling on the midnight express to Glenridge, auditing suspicious financial losses.",
        isCulprit: false,
        contradiction: { evId: "E06", res: "Lady Sterling chuckles: 'My umbrella? I left it by the marble column during the patron tea!'" }
      },
      {
        id: "W01", name: "Old Bert", role: "Night Janitor (Witness)",
        charId: "bert", age: 67,
        isWitness: true,
        testimony: "At 1:14 AM, right when the lights cut out, I was replacing filters near the basement breaker. Someone in heavy combat boots sprinted past me into the service corridor reeking of chemical solvent!",
        greeting: "I told the constables already, Officer: heavy combat boots ran past the substation right as the lights died!",
        secret: "Bert found a gold cigarette case with the letter 'C' near the basement entrance.",
        isCulprit: false
      }
    ],
    timeline: [
      { id: "T1", time: "01:00 AM", desc: "Guard Cruz logs the start of his North Wing gallery patrol.", contradictory: false },
      { id: "T2", time: "01:10 AM", desc: "Curator Vance's secondary master keycard taken from guard station locker.", contradictory: false },
      { id: "T3", time: "01:12 AM", desc: "Substation breaker door swiped open with Vance's stolen keycard.", contradictory: false },
      { id: "T4", time: "01:14 AM", desc: "Main circuit breaker thrown; museum blacks out; Janitor Bert hears combat boots sprinting.", contradictory: false },
      { id: "T5", time: "01:15 AM", desc: "North Wing terminal swiped with Vance's passcard (Impossible to traverse from basement on foot in 3 min without maintenance duct!).", contradictory: true },
      { id: "T6", time: "01:18 AM", desc: "Emergency backup generators engage; vault found shattered and empty.", contradictory: false }
    ],
    deductions: [
      { id1: "E01", id2: "E03", title: "Calculated Security Bypass", insight: "The fiber optic cut and timed card swipe prove an insider used a stolen badge to fake an alibi while cutting power." },
      { id1: "E02", id2: "E05", title: "Guard Uniform & Debt Motive", insight: "Tactical size 10 boot prints match guard issue footwear, linking the scene to Cruz's £12,000 urgent debt." },
      { id1: "E04", id2: "E08", title: "Premeditated Solvent Theft", insight: "The solvent rag in the basement chute matches the chemical reported stolen at 10:00 PM." },
      { id1: "E05", id2: "E09", title: "Syndicate Blackmail Connection", insight: "Cruz's debt note was issued by front companies linked to the Obsidian Hourglass syndicate." }
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
    briefing: "Aboard the midnight express traversing the Blackwood Tunnel, financier Victor Sterling was found dead in Compartment 4-A, poisoned by potassium cyanide in his chamomile tea. The train made zero stops between Wyvern and Glenridge. The assassin is riding Carriage #4!",
    culpritId: "S01", // Julian Gray
    decisiveEvidenceId: "E02", // Torn Gold Eagle Cufflink
    motiveKeyword: "EMBEZZLEMENT",
    solution: "Personal secretary Julian Gray embezzled £60,000 from Sterling to fund syndicate shell firms. Knowing Sterling was having inspectors arrest him at Glenridge, Gray impersonated Sterling wearing antique eagle cufflinks, received the tea tray from Conductor Morris at 11:40 PM, and poisoned the cup.",
    ambience: "rain",
    scenes: [
      {
        id: "SCENE_01",
        name: "First Class Compartment 4-A",
        bg: "assets/backgrounds/scene_vault.png",
        hotspots: [
          { x: 50, y: 55, name: "Teacup with Almond Odor", desc: "A half-empty bone china teacup smelling distinctly of bitter almonds (potassium cyanide).", evId: "E01" },
          { x: 70, y: 75, name: "Door Runner Track", desc: "Wedged in the sliding door brass runner: an antique gold eagle cufflink torn from a shirt.", evId: "E02" },
          { x: 28, y: 45, name: "Victorian Brass Luggage Rack", desc: "Sterling's private briefcase containing passenger dossiers.", evId: "E07" }
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
          { x: 75, y: 65, name: "Dr. Finch's Medical Satchel", desc: "Satchel with intact Edinburgh wax seals on all emergency narcotics and poisons.", evId: "E04" },
          { x: 86, y: 48, name: "Smoldering Club Car Ashtray", desc: "A charred telegram reading: 'Alexandria consignment confirmed. Silence Sterling before Glenridge. - Hourglass'", evId: "E09" }
        ]
      }
    ],
    evidence: {
      "E01": { id: "E01", name: "Poisoned Chamomile Teacup", cat: "CHEMICAL", rel: "CRITICAL PROOF", desc: "Lethal potassium cyanide residue coating the bottom of the deceased's teacup." },
      "E02": { id: "E02", name: "Torn Gold Eagle Cufflink", cat: "PHYSICAL", rel: "DECISIVE EVIDENCE", desc: "Antique gold eagle cufflink matching Julian Gray's frayed left sleeve buttonhole." },
      "E03": { id: "E03", name: "Embezzlement Audit Letter", cat: "DOCUMENT", rel: "MOTIVE EVIDENCE", desc: "Proof that Sterling had wired railway police to arrest Gray at Glenridge Station." },
      "E04": { id: "E04", name: "Sealed Medical Ampoules", cat: "MEDICAL", rel: "ELIMINATION EVIDENCE", desc: "Proves Dr. Finch never unsealed his emergency drug ampoules." },
      "E06": { id: "E06", name: "Vanilla Extract Vial", cat: "SUBSTANCE", rel: "RED HERRING", desc: "Harmless organic baking flavoring from the galley pantry." },
      "E07": { id: "E07", name: "Passenger Compartment Dossier", cat: "DOCUMENT", rel: "SUPPORTING PROOF", desc: "Notes identifying all ticket holders in Carriage #4." },
      "E08": { id: "E08", name: "Conductor's Delivery Log", cat: "DOCUMENT", rel: "SUPPORTING PROOF", desc: "Confirms tea was delivered directly to 4-A at 11:40 PM." },
      "E09": { id: "E09", name: "Burned Syndicate Telegram", cat: "CONSPIRACY", rel: "HIDDEN SECRET", desc: "A charred syndicate telegram ordering Sterling silenced before his financial audit." }
    },
    suspects: [
      {
        id: "S01", name: "Julian Gray", role: "Personal Secretary",
        charId: "cruz", age: 31,
        alibi: "I retired to Compartment 4-C at 11:30 PM to sleep. I never walked the corridor!",
        motive: "Why would I harm my benefactor? Without him, I have nothing.",
        greeting: "Detective, this is horrifying! Mr. Sterling was like a father to me!",
        secret: "Gray had transferred £60,000 of Sterling's railway bonds into Obsidian Hourglass accounts in Zurich.",
        isCulprit: true,
        contradiction: {
          evId: "E02",
          res: "You produce the eagle cufflink from the door runner. Gray stares at his bare left wrist: 'He found the books! He was having me arrested at Glenridge! When the conductor delivered the tea, I took it and poisoned it!'"
        }
      },
      {
        id: "S02", name: "Nora Vance", role: "Shipping Competitor",
        charId: "elena", age: 38,
        alibi: "I was in the Club Car from 11:15 PM onward enjoying spirits with passengers.",
        motive: "We had commercial rivalry, but I fight in court, not with poisons.",
        greeting: "Good evening. Sterling made enemies everywhere, Detective.",
        secret: "Nora is the estranged cousin of Curator Vance from Case 1; she warned him against dealing with syndicate antiquities brokers.",
        isCulprit: false,
        contradiction: { evId: "E03", res: "Nora smiles: 'My dispute was strictly corporate, handled by attorneys.'" }
      },
      {
        id: "S03", name: "Dr. Alistair Finch", role: "Physician",
        charId: "vance", age: 61,
        alibi: "I was reading clinical journals in Compartment 4-D until the emergency whistle blew.",
        motive: "Sterling sued my hospital board years ago, but that was settled.",
        greeting: "I attempted resuscitation, Detective, but cyanide arrests cellular life in seconds.",
        secret: "Dr. Finch was once pressured by the syndicate to supply unlisted strychnine, but refused.",
        isCulprit: false,
        contradiction: { evId: "E04", res: "Dr. Finch displays his satchel: 'Every ampoule seal is intact from Edinburgh.'" }
      },
      {
        id: "W01", name: "Conductor Morris", role: "Train Conductor (Witness)",
        charId: "bert", age: 45,
        isWitness: true,
        testimony: "At 11:40 PM, I delivered chamomile tea to Compartment 4-A. A gentleman opened the door wearing distinctive gold eagle cufflinks, took the tray, and said Mr. Sterling was resting.",
        greeting: "A terrible night on my train, Detective. I pride myself on passenger safety.",
        secret: "Morris noticed Gray sweating profusely in the corridor minutes before the train entered the tunnel.",
        isCulprit: false
      }
    ],
    timeline: [
      { id: "T1", time: "11:15 PM", desc: "Nora Vance enters the Club Car lounge, verified by fellow passengers.", contradictory: false },
      { id: "T2", time: "11:30 PM", desc: "Julian Gray claims he entered Compartment 4-C and was fast asleep.", contradictory: false },
      { id: "T3", time: "11:38 PM", desc: "Conductor Morris brews fresh chamomile tea in the galley pantry.", contradictory: false },
      { id: "T4", time: "11:40 PM", desc: "Tea delivered to 4-A door; a man wearing gold eagle cufflinks accepts the tray (Contradicts Gray's 11:30 PM sleep claim!).", contradictory: true },
      { id: "T5", time: "11:48 PM", desc: "Express train enters Blackwood Mountain Tunnel.", contradictory: false },
      { id: "T6", time: "11:55 PM", desc: "Emergency bell rung; Victor Sterling found dead.", contradictory: false }
    ],
    deductions: [
      { id1: "E02", id2: "E08", title: "Impersonation at the Door", insight: "The cufflink torn in the runner at 11:40 PM proves the killer received the tea from the conductor." },
      { id1: "E01", id2: "E03", title: "Lethal Embezzlement Cover-Up", insight: "The £60,000 theft audit letter gave Julian Gray an urgent life-or-death motive." },
      { id1: "E03", id2: "E09", title: "Railway Laundering Conspiracy", insight: "Gray's embezzlement was directed by the Obsidian Hourglass syndicate to fund dock operations." }
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
    briefing: "Master horologist Thaddeus Thorne was found dead of a gunshot wound in his high-tower workshop. The heavy oak door was bolted with a solid iron slide bolt from the inside, and windows were barred with wrought iron. Staged as a suicide, forensic ballistics and clockwork physics prove murder!",
    culpritId: "S01", // Cornelius Thorne
    decisiveEvidenceId: "E02", // Ballpoint Signature on Note
    motiveKeyword: "PATENT",
    solution: "Nephew Cornelius Thorne was being disinherited. He shot his uncle at 8:15 PM, typed a suicide note, signed it with a modern ballpoint pen, and rigged high-tensile piano wire around the slide bolt and clock pendulum chime weight with an ice block. At 9:00 PM, while Cornelius was visible drinking in the tavern, the clock chimed, the weight dropped, and the bolt slammed shut.",
    ambience: "clock",
    scenes: [
      {
        id: "SCENE_01",
        name: "The Bolted Study",
        bg: "assets/backgrounds/briefing_room.png",
        hotspots: [
          { x: 50, y: 55, name: "Thorne's Desk & Suicide Note", desc: "A typed suicide note signed with cheap ballpoint ink. Thorne only wrote in Prussian blue fountain ink.", evId: "E02" },
          { x: 25, y: 65, name: "Heavy Interior Slide Bolt", desc: "A friction groove and melted water droplet on the brass slide bolt.", evId: "E01" },
          { x: 80, y: 40, name: "Concealed Safe Wall Panel", desc: "A blueprint of 'The Great Clock of Alexandria' designed to decode astronomical sapphire ciphers!", evId: "E09" }
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
      "E08": { id: "E08", name: "Omnibus Ticket Stub", cat: "DOCUMENT", rel: "ELIMINATION EVIDENCE", desc: "Confirms Miriam departed on the 8:30 PM bus." },
      "E09": { id: "E09", name: "Alexandria Clockwork Blueprint", cat: "CONSPIRACY", rel: "HIDDEN SECRET", desc: "Schematics for a planetary gear decoder matching the Star of Alexandria cipher from Case #1." }
    },
    suspects: [
      {
        id: "S01", name: "Cornelius Thorne", role: "Nephew & Apprentice",
        charId: "cruz", age: 27,
        alibi: "I was drinking at the Black Bull Tavern from 8:30 PM to 9:45 PM. Ten patrons saw me when the shot allegedly went off!",
        motive: "Uncle was my mentor. I stood to inherit his horological fortune.",
        greeting: "My uncle took his own life, Detective! Why torture our grieving family?",
        secret: "Cornelius promised the syndicate his uncle's decoder blueprint in exchange for cancelling his gambling markers.",
        isCulprit: true,
        contradiction: {
          evId: "E02",
          res: "Cornelius blanches as you present the ballpoint ink analysis: 'Your uncle wrote only in antique Prussian fountain ink! You signed this note with your ballpoint!' Cornelius breaks down: 'He was disinheriting me on Monday! I rigged the wire to the clock weight so the door locked itself at 9:00 PM while I drank openly at the tavern!'"
        }
      },
      {
        id: "S02", name: "Miriam Black", role: "Master Locksmith",
        charId: "elena", age: 42,
        alibi: "I left the manor at 8:15 PM and caught the 8:30 PM omnibus at the crossroads.",
        motive: "We had royalties disputes, but dead men don't sign royalty checks.",
        greeting: "A slide bolt is child's play, Detective, but it can only be thrown from inside without a mechanism.",
        secret: "Miriam knew Thaddeus was terrified someone was tracking his astronomical decoder blueprints.",
        isCulprit: false,
        contradiction: { evId: "E08", res: "Miriam presents her ticket: 'The omnibus driver stamped my fare at 8:30 PM.'" }
      },
      {
        id: "W01", name: "Constable Higgins", role: "Village Officer (Witness)",
        charId: "bert", age: 50,
        isWitness: true,
        testimony: "At 9:15 PM, we broke through the study window. The door bolt was thrown solid from inside, and a small puddle of water lay beneath the brass casing.",
        greeting: "Never seen a locked room like it, Inspector. Bolted solid from the inside.",
        secret: "Higgins remembers finding melted ice cubes in the workshop wastebasket.",
        isCulprit: false
      }
    ],
    timeline: [
      { id: "T1", time: "08:00 PM", desc: "Locksmith Miriam Black finishes consulting with Thaddeus on door locks.", contradictory: false },
      { id: "T2", time: "08:15 PM", desc: "Murder occurs; Cornelius types fake note and loops wire through bolt into ice block on chime weight.", contradictory: false },
      { id: "T3", time: "08:30 PM", desc: "Miriam departs on the crossroads omnibus, verified by ticket.", contradictory: false },
      { id: "T4", time: "08:45 PM", desc: "Cornelius arrives at Black Bull Tavern, ordering drinks to fake presence.", contradictory: false },
      { id: "T5", time: "09:00 PM", desc: "Clock chimes 9:00; 20-lb weight drops, pulling deadbolt shut while Cornelius drinks at tavern (Rigged automated trigger!).", contradictory: true },
      { id: "T6", time: "09:15 PM", desc: "Constable Higgins forces window; finds melted puddle beneath bolt.", contradictory: false }
    ],
    deductions: [
      { id1: "E01", id2: "E03", title: "Automated Mechanical Lock", insight: "Tension wire linked the slide bolt to the clock's 9:00 PM chime weight." },
      { id1: "E02", id2: "E05", title: "Forged Suicide & Disinheritance", insight: "The ballpoint forged note and will draft expose Cornelius's panic over being cut off." },
      { id1: "E01", id2: "E09", title: "The Astronomical Decoder Link", insight: "Thorne's murder was orchestrated to steal the astronomical clockwork capable of resolving the Star of Alexandria cipher!" }
    ],
    eliminations: [
      { id: "S02", name: "Miriam Black", reason: "Eliminated: Ticket stub verifies departure on 8:30 PM omnibus." }
    ]
  }
};

// =========================================================================
// 3. ACHIEVEMENTS SYSTEM
// =========================================================================
const ACHIEVEMENTS_LIST = [
  { id: "first_clue", icon: "🔍", title: "First Footprint", desc: "Discover your first piece of forensic evidence." },
  { id: "hidden_clue", icon: "👁️", title: "Keen Eye", desc: "Discover a secret Obsidian Hourglass conspiracy clue." },
  { id: "objection", icon: "⚡", title: "OBJECTION!", desc: "Shatter a suspect's false alibi with physical proof." },
  { id: "clue_deduction", icon: "🧵", title: "Master of Deductions", desc: "Connect two pieces of evidence on the Clue Board." },
  { id: "timeline_sleuth", icon: "⏳", title: "Chronological Precision", desc: "Isolate a timeline contradiction during reconstruction." },
  { id: "case_solved", icon: "⚖️", title: "Justice Delivered", desc: "Conclude any case with a Flawless Grade S verdict." },
  { id: "conspiracy", icon: "🏛️", title: "The Obsidian Hourglass", desc: "Unveil the overarching syndicate conspiracy." },
  { id: "master_sleuth", icon: "👑", title: "Flawless Detective", desc: "Solve all three cases with Master Sleuth Grade S." },
  { id: "forensic_chemist", icon: "🧪", title: "Forensic Alchemist", desc: "Execute a chemical, spectroscopy, or metallurgical assay in the Lab." },
  { id: "mind_palace_epiphany", icon: "🧠", title: "Cognitive Nexus", desc: "Synthesize a breakthrough Epiphany in the Mind Palace." }
];

class AchievementManager {
  constructor() {
    this.unlocked = new Set();
    this.load();
  }
  load() {
    try {
      const raw = localStorage.getItem('case_closed_achievements');
      if (raw) {
        this.unlocked = new Set(JSON.parse(raw));
      }
    } catch (e) {}
  }
  save() {
    try {
      localStorage.setItem('case_closed_achievements', JSON.stringify(Array.from(this.unlocked)));
    } catch (e) {}
  }
  unlock(achId, app) {
    if (!this.unlocked.has(achId)) {
      this.unlocked.add(achId);
      this.save();
      const ach = ACHIEVEMENTS_LIST.find(a => a.id === achId);
      if (ach && app) {
        app.audio.playHiddenClue();
        app.showToast(`🏆 TROPHY UNLOCKED: [${ach.title}] — ${ach.desc}`, 4500);
      }
    }
  }
  isUnlocked(achId) {
    return this.unlocked.has(achId);
  }
  has(achId) {
    return this.isUnlocked(achId);
  }
}

// =========================================================================
// 4. GAME STATE
// =========================================================================
class GameState {
  constructor() {
    this.caseNum = 1;
    this.score = 100;
    this.energy = 100;
    this.discoveredEvidence = new Set();
    this.brokenAlibis = new Set();
    this.examinedHotspots = new Set();
    this.unlockedDeductions = new Set();
    this.timelineDiscovered = new Set();
    this.leads = [];
    this.activeSuspect = null;
    this.cluePin1 = null;
    this.cluePin2 = null;
    this.accusedSuspect = null;
    this.accusedEvidence = null;
    this.accusedMotive = null;
    this.settings = {
      reducedMotion: false,
      screenShake: true,
      textSpeed: 22
    };
    this.forensicReports = new Map();
    this.selectedForensicSpecimen = null;
    this.selectedForensicAssay = 'chemical';
    this.suspectHeartRates = { S01: 72, S02: 78, S03: 65, S04: 70 };
    this.mindPalacePin1 = null;
    this.mindPalacePin2 = null;
    this.forgedEpiphanies = new Set();
  }
}

// =========================================================================
// 5. MAIN WEB GAME APPLICATION
// =========================================================================
class WebGameApp {
  constructor() {
    this.audio = new AudioEngine();
    this.achievements = new AchievementManager();
    this.state = new GameState();
    this.dialogueQueue = [];
    this.currentDialogue = null;
    this.typewriterTimer = null;
    this.displayedChars = 0;
    this.currentSceneIdx = 0;

    this.ambientAnimId = null;
    this.rainDrops = [];

    this.initDOM();
    this.bindEvents();
    this.initAmbientCanvas();
    this.showScreen('menu-screen');
    this.audio.playBGM('menu');
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

    this.speedlinesEl = document.getElementById('speedlines-overlay');
    this.cutinEl = document.getElementById('anime-cutin');
    this.cutinImg = document.getElementById('cutin-img');
    this.cutinText = document.getElementById('cutin-text');

    this.settingsModal = document.getElementById('settings-modal');
    this.achievementsModal = document.getElementById('achievements-modal');
    this.clueCanvas = document.getElementById('clueboard-canvas');

    // Forensic Lab & Mind Palace & EKG DOM references
    this.ekgCanvas = document.getElementById('ekg-canvas');
    this.ekgCtx = this.ekgCanvas ? this.ekgCanvas.getContext('2d') : null;
    this.ekgAnimId = null;
    this.ekgPhase = 0;
    this.mindPalaceCanvas = document.getElementById('mindpalace-canvas');
    this.mindPalaceCtx = this.mindPalaceCanvas ? this.mindPalaceCanvas.getContext('2d') : null;
  }

  bindEvents() {
    // Menu buttons
    document.getElementById('btn-new-game').onclick = () => {
      this.audio.resume();
      this.showScreen('case-select-screen');
    };
    document.getElementById('btn-continue').onclick = () => {
      this.audio.resume();
      this.loadGame();
    };
    document.getElementById('btn-how-to-play').onclick = () => this.showScreen('how-to-play-screen');
    document.getElementById('btn-credits').onclick = () => this.showScreen('credits-screen');

    document.getElementById('btn-menu-achievements').onclick = () => this.openAchievements();
    document.getElementById('btn-menu-settings').onclick = () => this.openSettings();

    // HUD Quick Controls
    document.getElementById('btn-hud-audio').onclick = () => {
      const isMuted = this.audio.toggleMute();
      document.getElementById('btn-hud-audio').innerText = isMuted ? '🔇' : '🎵';
      this.showToast(isMuted ? "Audio Muted" : "Audio Active", 2000);
    };
    document.getElementById('btn-hud-journal').onclick = () => this.openJournal();
    document.getElementById('btn-hud-settings').onclick = () => this.openSettings();
    const fHud = document.getElementById('btn-hud-forensics');
    if (fHud) fHud.onclick = () => this.openForensicLab();
    const mHud = document.getElementById('btn-hud-mindpalace');
    if (mHud) mHud.onclick = () => this.openMindPalace();

    const fHub = document.getElementById('btn-hub-forensics');
    if (fHub) fHub.onclick = () => this.openForensicLab();
    const mHub = document.getElementById('btn-hub-mindpalace');
    if (mHub) mHub.onclick = () => this.openMindPalace();

    const fBack = document.getElementById('btn-forensics-back');
    if (fBack) fBack.onclick = () => this.openInvestigation();
    const mBack = document.getElementById('btn-mindpalace-back');
    if (mBack) mBack.onclick = () => this.openInvestigation();

    // Forensic assay selection
    document.querySelectorAll('.assay-card').forEach(card => {
      card.onclick = () => {
        document.querySelectorAll('.assay-card').forEach(c => c.classList.remove('active'));
        card.classList.add('active');
        this.state.selectedForensicAssay = card.dataset.assay;
      };
    });

    const runAssayBtn = document.getElementById('btn-run-assay');
    if (runAssayBtn) runAssayBtn.onclick = () => this.executeForensicAssay();

    const synthBtn = document.getElementById('btn-synthesize-epiphany');
    if (synthBtn) synthBtn.onclick = () => this.synthesizeMindEpiphany();

    // Case Selection
    document.getElementById('btn-case-1').onclick = () => this.startCase(1);
    document.getElementById('btn-case-2').onclick = () => this.startCase(2);
    document.getElementById('btn-case-3').onclick = () => this.startCase(3);
    document.getElementById('btn-case-back').onclick = () => {
      this.showScreen('menu-screen');
      this.audio.playBGM('menu');
    };

    // Navigation Back buttons
    document.getElementById('btn-htp-back').onclick = () => this.showScreen('menu-screen');
    document.getElementById('btn-credits-back').onclick = () => this.showScreen('menu-screen');

    // Dialogue click-to-advance & keyboard
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
    document.getElementById('btn-nav-timeline').onclick = () => this.openTimeline();
    document.getElementById('btn-nav-journal').onclick = () => this.openJournal();
    document.getElementById('btn-nav-accuse').onclick = () => this.openAccusation();
    document.getElementById('btn-hub-menu').onclick = () => {
      this.showScreen('menu-screen');
      this.audio.playBGM('menu');
    };
    document.getElementById('btn-hub-save').onclick = () => this.saveGame();
    document.getElementById('btn-hub-hint').onclick = () => this.requestHint();

    // Journal & Timeline Back
    document.getElementById('btn-journal-back').onclick = () => this.openInvestigation();
    document.getElementById('btn-timeline-back').onclick = () => this.openInvestigation();

    // Journal Navigation Tabs
    document.getElementById('tab-journal-dossiers').onclick = () => this.switchJournalTab('dossiers');
    document.getElementById('tab-journal-evidence').onclick = () => this.switchJournalTab('evidence');
    document.getElementById('tab-journal-leads').onclick = () => this.switchJournalTab('leads');

    // Settings Modal
    document.getElementById('btn-close-settings').onclick = () => this.closeSettings();
    document.getElementById('slider-master-vol').oninput = (e) => {
      const v = e.target.value / 100;
      this.audio.setMasterVolume(v);
      document.getElementById('val-master-vol').innerText = `${e.target.value}%`;
    };
    document.getElementById('slider-music-vol').oninput = (e) => {
      const v = e.target.value / 100;
      this.audio.setMusicVolume(v);
      document.getElementById('val-music-vol').innerText = `${e.target.value}%`;
    };
    document.getElementById('slider-sfx-vol').oninput = (e) => {
      const v = e.target.value / 100;
      this.audio.setSFXVolume(v);
      document.getElementById('val-sfx-vol').innerText = `${e.target.value}%`;
    };
    document.getElementById('slider-ambience-vol').oninput = (e) => {
      const v = e.target.value / 100;
      this.audio.setAmbienceVolume(v);
      document.getElementById('val-ambience-vol').innerText = `${e.target.value}%`;
    };
    document.getElementById('toggle-mute').onchange = (e) => {
      if (this.audio.isMuted !== e.target.checked) {
        this.audio.toggleMute();
      }
    };
    document.getElementById('toggle-screen-shake').onchange = (e) => {
      this.state.settings.screenShake = e.target.checked;
    };
    document.getElementById('toggle-reduced-motion').onchange = (e) => {
      this.state.settings.reducedMotion = e.target.checked;
      document.body.classList.toggle('reduced-motion', e.target.checked);
    };

    // Achievements Modal
    document.getElementById('btn-close-achievements').onclick = () => this.closeAchievements();

    // Epilogue buttons
    document.getElementById('btn-verdict-epilogue').onclick = () => this.openEpilogue();
    document.getElementById('btn-epilogue-menu').onclick = () => {
      this.showScreen('menu-screen');
      this.audio.playBGM('menu');
    };

    // Modal Close
    this.modalEl.onclick = () => this.closeModal();
  }

  // -------------------------------------------------------------
  // Ambient Canvas Graphics (Rain & Atmospheric Dust)
  // -------------------------------------------------------------
  initAmbientCanvas() {
    const canvas = document.getElementById('ambient-canvas');
    if (!canvas) return;
    const ctx = canvas.getContext('2d');

    const resize = () => {
      canvas.width = canvas.parentElement.clientWidth || 1280;
      canvas.height = canvas.parentElement.clientHeight || 720;
    };
    resize();
    window.addEventListener('resize', resize);

    this.rainDrops = [];
    for (let i = 0; i < 75; i++) {
      this.rainDrops.push({
        x: Math.random() * canvas.width,
        y: Math.random() * canvas.height,
        len: 12 + Math.random() * 18,
        speed: 10 + Math.random() * 12
      });
    }

    const loop = () => {
      if (!this.state.settings.reducedMotion) {
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        ctx.strokeStyle = 'rgba(148, 163, 184, 0.22)';
        ctx.lineWidth = 1.2;
        ctx.beginPath();
        for (let drop of this.rainDrops) {
          ctx.moveTo(drop.x, drop.y);
          ctx.lineTo(drop.x - 3, drop.y + drop.len);
          drop.y += drop.speed;
          drop.x -= 1.5;
          if (drop.y > canvas.height) {
            drop.y = -drop.len;
            drop.x = Math.random() * canvas.width;
          }
        }
        ctx.stroke();
      }
      this.ambientAnimId = requestAnimationFrame(loop);
    };
    loop();
  }

  showToast(msg, duration = 3000) {
    this.toastEl.innerText = msg;
    this.toastEl.classList.add('show');
    clearTimeout(this._toastTimer);
    this._toastTimer = setTimeout(() => {
      this.toastEl.classList.remove('show');
    }, duration);
  }

  shakeScreen() {
    if (this.state.settings.reducedMotion || !this.state.settings.screenShake) return;
    const container = document.getElementById('game-container');
    container.classList.remove('shake');
    void container.offsetWidth; // Trigger reflow
    container.classList.add('shake');
    setTimeout(() => container.classList.remove('shake'), 450);
  }

  triggerAnimeCutIn(charId, text = "OBJECTION!") {
    this.cutinText.innerText = text;
    this.cutinImg.style.backgroundImage = `url('assets/characters/${charId}_angry.png')`;
    this.speedlinesEl.classList.add('active');
    this.cutinEl.classList.add('active');
    this.shakeScreen();

    setTimeout(() => {
      this.cutinEl.classList.remove('active');
      this.speedlinesEl.classList.remove('active');
    }, 1400);
  }

  showScreen(screenId) {
    document.querySelectorAll('.screen').forEach(s => s.classList.remove('active'));
    const target = document.getElementById(screenId);
    if (target) target.classList.add('active');

    // Backdrops
    if (['menu-screen', 'case-select-screen', 'how-to-play-screen', 'credits-screen'].includes(screenId)) {
      this.setBg('assets/backgrounds/main_menu.png');
      this.hidePortrait();
      this.hideDialogue();
      document.getElementById('hud-bar').style.display = 'none';
    } else {
      document.getElementById('hud-bar').style.display = 'flex';
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
    this.audio.playBGM('investigation');
    this.audio.startAmbience(curCase.ambience || 'rain');

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
    }, this.state.settings.textSpeed);
  }

  handleDialogueClick() {
    if (this.typewriterTimer) {
      clearInterval(this.typewriterTimer);
      this.typewriterTimer = null;
      this.dialogueText.innerText = this.currentDialogue.text;
      this.dialogueAdvance.style.display = 'block';
      if (this.currentDialogue.onFinish) {
        this.currentDialogue.onFinish();
      }
    } else {
      this.nextDialogue();
    }
  }

  hideDialogue() {
    this.dialogueWrap.style.display = 'none';
    clearInterval(this.typewriterTimer);
    this.typewriterTimer = null;
  }

  // -------------------------------------------------------------
  // Crime Scene Investigation & Hotspots
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
        if (ev.cat === 'CONSPIRACY') {
          this.audio.playHiddenClue();
          this.achievements.unlock('hidden_clue', this);
        } else {
          this.audio.playChime();
          this.achievements.unlock('first_clue', this);
        }

        this.state.score += 25;
        this.state.energy = Math.max(0, this.state.energy - 10);
        this.state.leads.push(`Discovered [${ev.id}] ${ev.name} at ${scene.name}`);
        this.updateHUD();

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
      <h3 style="color:var(--gold); font-size:1.1rem; margin-bottom:10px;">[FORENSIC EVIDENCE SECURED]</h3>
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
    this.audio.playBGM('tension');

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
    this.updateEKGForSuspect(s);

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

    this.startDialogue([{ speaker: s.name, charId: s.charId, expr: 'neutral', text: s.greeting }]);
  }

  // -------------------------------------------------------------
  // Contradiction Screen & Objection Anime Cut-In
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
      // Success! Play dramatic objection cut-in
      this.audio.playObjection();
      this.triggerAnimeCutIn('detective', 'OBJECTION!');
      this.achievements.unlock('objection', this);

      this.state.suspectHeartRates[s.id] = 148;
      this.updateEKGForSuspect(s);

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
      this.shakeScreen();
      this.state.suspectHeartRates[s.id] = (this.state.suspectHeartRates[s.id] || 75) + 14;
      this.updateEKGForSuspect(s);

      this.state.score = Math.max(0, this.state.score - 15);
      this.updateHUD();
      this.openInterrogation();
      this.showPortrait(s.charId, 'angry');
      this.startDialogue([{ speaker: s.name, charId: s.charId, expr: 'angry', text: "That proves nothing against me, Detective! Check your facts before making reckless claims." }]);
      this.showToast("Contradiction Failed! (-15 Points)", 3000);
    }
  }

  // -------------------------------------------------------------
  // Clue Board & Red Thread Connections
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
      card.id = `clue-card-${eid}`;
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

    this.renderClueThreads();
  }

  renderClueThreads() {
    if (!this.clueCanvas) return;
    const canvas = this.clueCanvas;
    const ctx = canvas.getContext('2d');
    canvas.width = canvas.parentElement.clientWidth || 1280;
    canvas.height = canvas.parentElement.clientHeight || 720;
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    const curCase = CASE_DATA[this.state.caseNum];
    this.state.unlockedDeductions.forEach(key => {
      const [id1, id2] = key.split('_');
      const el1 = document.getElementById(`clue-card-${id1}`);
      const el2 = document.getElementById(`clue-card-${id2}`);
      if (el1 && el2) {
        const r1 = el1.getBoundingClientRect();
        const r2 = el2.getBoundingClientRect();
        const pR = canvas.getBoundingClientRect();

        const x1 = r1.left + r1.width / 2 - pR.left;
        const y1 = r1.top + r1.height / 2 - pR.top;
        const x2 = r2.left + r2.width / 2 - pR.left;
        const y2 = r2.top + r2.height / 2 - pR.top;

        // Draw red yarn string
        ctx.strokeStyle = '#dc2626';
        ctx.lineWidth = 3;
        ctx.shadowColor = 'rgba(239, 68, 68, 0.7)';
        ctx.shadowBlur = 8;
        ctx.beginPath();
        ctx.moveTo(x1, y1);
        ctx.quadraticCurveTo((x1 + x2) / 2, (y1 + y2) / 2 + 25, x2, y2);
        ctx.stroke();

        // Pin heads
        ctx.fillStyle = '#f4c442';
        ctx.beginPath();
        ctx.arc(x1, y1, 5, 0, Math.PI * 2);
        ctx.arc(x2, y2, 5, 0, Math.PI * 2);
        ctx.fill();
      }
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
        this.audio.playThreadConnect();
        this.audio.playChime();
        this.achievements.unlock('clue_deduction', this);

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
  // Timeline Reconstruction Screen
  // -------------------------------------------------------------
  openTimeline() {
    this.hideDialogue();
    this.hidePortrait();
    this.showScreen('timeline-screen');
    this.setBg('assets/backgrounds/briefing_room.png');

    const curCase = CASE_DATA[this.state.caseNum];
    const container = document.getElementById('timeline-events-container');
    container.innerHTML = '';

    curCase.timeline.forEach(t => {
      const isDiscovered = this.state.timelineDiscovered.has(t.id);
      const card = document.createElement('div');
      card.className = `timeline-card ${isDiscovered && t.contradictory ? 'contradictory' : ''}`;
      card.innerHTML = `
        <div class="timeline-badge">${t.time}</div>
        <p style="color:#f1f5f9; font-size:1rem; margin-top:4px;">${t.desc}</p>
        ${t.contradictory ? `<span style="font-size:0.8rem; color:var(--red); font-weight:bold;">${isDiscovered ? '[CHRONOLOGICAL CONTRADICTION EXPOSED]' : '[Click to Analyze Timestamp]'}</span>` : ''}
      `;

      card.onclick = () => {
        if (t.contradictory) {
          this.state.timelineDiscovered.add(t.id);
          this.audio.playHiddenClue();
          this.achievements.unlock('timeline_sleuth', this);
          this.state.score += 25;
          this.updateHUD();

          const bEl = document.getElementById('timeline-breakthrough');
          bEl.style.display = 'block';
          bEl.innerHTML = `★ TIMELINE BREAKTHROUGH: You isolated the chronological impossibility! The suspect's timeline is physically incompatible with forensic logs! (+25 Points)`;
          this.openTimeline();
        } else {
          this.showToast(`Timestamp ${t.time}: Corroborated by forensic logs.`);
        }
      };

      container.appendChild(card);
    });
  }

  // -------------------------------------------------------------
  // Detective Journal & Suspect Dossiers
  // -------------------------------------------------------------
  openJournal() {
    this.hideDialogue();
    this.hidePortrait();
    this.showScreen('journal-screen');
    this.setBg('assets/backgrounds/briefing_room.png');
    this.switchJournalTab('dossiers');
  }

  switchJournalTab(tab) {
    document.getElementById('journal-dossiers-view').style.display = (tab === 'dossiers') ? 'block' : 'none';
    document.getElementById('journal-evidence-view').style.display = (tab === 'evidence') ? 'block' : 'none';
    document.getElementById('journal-leads-view').style.display = (tab === 'leads') ? 'block' : 'none';

    document.getElementById('tab-journal-dossiers').className = `btn ${tab === 'dossiers' ? 'btn-primary' : ''}`;
    document.getElementById('tab-journal-evidence').className = `btn ${tab === 'evidence' ? 'btn-primary' : ''}`;
    document.getElementById('tab-journal-leads').className = `btn ${tab === 'leads' ? 'btn-primary' : ''}`;

    const curCase = CASE_DATA[this.state.caseNum];

    if (tab === 'dossiers') {
      const grid = document.getElementById('dossiers-grid');
      grid.innerHTML = '';
      curCase.suspects.forEach(s => {
        const isBroken = this.state.brokenAlibis.has(s.id);
        const card = document.createElement('div');
        card.className = 'dossier-card';
        card.innerHTML = `
          <div class="dossier-header">
            <div class="dossier-avatar" style="background-image:url('assets/characters/${s.charId}_neutral.png');"></div>
            <div>
              <h3 style="color:#fff; font-size:1.15rem;">${s.name}</h3>
              <p style="color:var(--cyan); font-size:0.85rem;">Role: ${s.role} | Age: ${s.age || 'Unknown'}</p>
              <p style="color:var(--gold); font-size:0.8rem;">Status: ${isBroken ? 'ALIBI SHATTERED' : 'Person of Interest'}</p>
            </div>
          </div>
          <p style="font-size:0.9rem; color:#cbd5e1;"><strong>Alibi:</strong> "${s.alibi || s.testimony}"</p>
          <div class="dossier-secret ${isBroken ? '' : 'locked'}">
            <strong>Hidden Intel / Secret:</strong> ${isBroken ? s.secret : '[Unlocks when alibi or contradiction is broken]'}
          </div>
        `;
        grid.appendChild(card);
      });

    } else if (tab === 'evidence') {
      const grid = document.getElementById('evidence-archive-grid');
      grid.innerHTML = '';
      this.state.discoveredEvidence.forEach(eid => {
        const ev = curCase.evidence[eid];
        if (!ev) return;
        const card = document.createElement('div');
        card.className = 'evidence-archive-card';
        card.innerHTML = `
          <div style="display:flex; justify-content:space-between;">
            <strong style="color:var(--gold); font-size:0.9rem;">${ev.id}</strong>
            <span style="color:var(--cyan); font-size:0.8rem;">${ev.cat}</span>
          </div>
          <h4 style="color:#fff; font-size:1.05rem;">${ev.name}</h4>
          <p style="color:#94a3b8; font-size:0.85rem; line-height:1.4;">${ev.desc}</p>
          <span style="color:var(--gold-light); font-size:0.75rem;">Relevance: ${ev.rel}</span>
        `;
        grid.appendChild(card);
      });

    } else if (tab === 'leads') {
      const list = document.getElementById('journal-leads-list');
      list.innerHTML = '';
      if (this.state.leads.length === 0) {
        list.innerHTML = '<p style="color:var(--text-dim); padding:20px;">No investigative leads recorded yet.</p>';
      } else {
        this.state.leads.forEach(l => {
          const item = document.createElement('div');
          item.style.padding = '10px 14px';
          item.style.marginBottom = '8px';
          item.style.background = 'rgba(30, 41, 59, 0.6)';
          item.style.borderLeft = '3px solid var(--gold)';
          item.style.fontSize = '0.95rem';
          item.innerText = l;
          list.appendChild(item);
        });
      }
    }
  }

  // -------------------------------------------------------------
  // Accusation & Verdict
  // -------------------------------------------------------------
  openAccusation() {
    this.hideDialogue();
    this.hidePortrait();
    this.showScreen('accusation-screen');
    this.setBg('assets/backgrounds/courtroom.png');
    this.audio.playGavel();

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
      { key: "DEBT", label: "1. Financial Debt / Syndicate Coercion" },
      { key: "EMBEZZLEMENT", label: "2. Embezzlement & Asset Laundering" },
      { key: "PATENT", label: "3. Disinheritance & Clockwork Theft" },
      { key: "VENDETTA", label: "4. Personal Revenge & Envy" }
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
      this.showToast("Select a suspect, decisive evidence, and criminal motive!");
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
    this.audio.playGavel();

    const epilogueBtn = document.getElementById('btn-verdict-epilogue');

    if (isMaster) {
      this.audio.playBGM('verdict');
      this.achievements.unlock('case_solved', this);

      document.getElementById('verdict-title').innerText = "MASTER DETECTIVE — CASE CLOSED!";
      document.getElementById('verdict-title').style.color = "var(--gold)";
      document.getElementById('verdict-rank').innerText = `Final Score: ${this.state.score + 250} | Performance Rank: S (Flawless Sleuth)`;
      document.getElementById('verdict-desc').innerText = curCase.solution;

      // Check if all 3 cases achieved S rank
      const solvedCases = JSON.parse(localStorage.getItem('case_closed_solved') || '[]');
      if (!solvedCases.includes(this.state.caseNum)) {
        solvedCases.push(this.state.caseNum);
        localStorage.setItem('case_closed_solved', JSON.stringify(solvedCases));
      }

      if (solvedCases.length >= 3) {
        this.achievements.unlock('master_sleuth', this);
        epilogueBtn.style.display = 'inline-block';
      } else {
        epilogueBtn.style.display = 'none';
      }

    } else {
      this.audio.playBuzzer();
      this.shakeScreen();
      document.getElementById('verdict-title').innerText = "ACCUSATION COLLAPSED";
      document.getElementById('verdict-title').style.color = "var(--red)";
      document.getElementById('verdict-rank').innerText = `Final Score: ${this.state.score} | Performance Rank: C (Insufficient Proof)`;
      document.getElementById('verdict-desc').innerText = "The prosecution failed to substantiate decisive physical evidence and timeline logic before the High Magistrate. The true culprit escaped conviction.";
      epilogueBtn.style.display = 'none';
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

    document.getElementById('btn-verdict-menu').onclick = () => {
      this.showScreen('menu-screen');
      this.audio.playBGM('menu');
    };
  }

  openEpilogue() {
    this.achievements.unlock('conspiracy', this);
    this.showScreen('epilogue-screen');
    this.setBg('assets/backgrounds/main_menu.png');
    this.audio.playBGM('investigation');
  }

  // -------------------------------------------------------------
  // Modals: Settings & Achievements
  // -------------------------------------------------------------
  openSettings() {
    this.settingsModal.style.display = 'flex';
  }
  closeSettings() {
    this.settingsModal.style.display = 'none';
  }

  openAchievements() {
    const grid = document.getElementById('achievements-grid');
    grid.innerHTML = '';
    ACHIEVEMENTS_LIST.forEach(a => {
      const isUn = this.achievements.isUnlocked(a.id);
      const card = document.createElement('div');
      card.className = `achievement-card ${isUn ? 'unlocked' : ''}`;
      card.innerHTML = `
        <div class="achievement-icon">${a.icon}</div>
        <div>
          <h4 style="color:${isUn ? 'var(--gold-light)' : '#94a3b8'}; font-size:1.05rem;">${a.title}</h4>
          <p style="color:#cbd5e1; font-size:0.85rem; margin-top:2px;">${a.desc}</p>
          <span style="font-size:0.75rem; color:${isUn ? 'var(--green)' : 'var(--text-dim)'}; font-weight:bold;">
            ${isUn ? '✓ UNLOCKED' : '🔒 LOCKED'}
          </span>
        </div>
      `;
      grid.appendChild(card);
    });
    this.achievementsModal.style.display = 'flex';
  }
  closeAchievements() {
    this.achievementsModal.style.display = 'none';
  }

  // -------------------------------------------------------------
  // Save & Load
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
      timelineDiscovered: Array.from(this.state.timelineDiscovered),
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
      this.state.timelineDiscovered = new Set(data.timelineDiscovered || []);
      this.state.leads = data.leads || [];

      this.showToast("Investigation resumed!", 3000);
      this.openInvestigation();
    } catch (e) {
      this.showToast("Failed to parse save data!");
    }
  }

  // -------------------------------------------------------------
  // Psychological Profiler & EKG Cardiograph Engine
  // -------------------------------------------------------------
  updateEKGForSuspect(s) {
    const card = document.getElementById('suspect-ekg-card');
    if (!card) return;
    if (s.isWitness) {
      card.style.display = 'none';
      if (this.ekgAnimId) cancelAnimationFrame(this.ekgAnimId);
      return;
    }
    card.style.display = 'block';

    const bpmEl = document.getElementById('ekg-bpm-display');
    const stressFill = document.getElementById('ekg-stress-fill');
    const microEl = document.getElementById('ekg-micro-expression');

    let bpm = this.state.suspectHeartRates[s.id] || 75;
    if (this.state.brokenAlibis.has(s.id)) {
      bpm = Math.max(bpm, 142);
    }
    this.state.suspectHeartRates[s.id] = bpm;

    bpmEl.innerText = `${bpm} BPM`;
    bpmEl.className = 'ekg-bpm';
    if (bpm >= 130) {
      bpmEl.classList.add('critical');
      stressFill.style.width = '96%';
      microEl.innerText = "CRITICAL CARDIAC SURGE: Rapid respiration, perspiration beads, alibi collapsing!";
    } else if (bpm >= 95) {
      bpmEl.classList.add('elevated');
      stressFill.style.width = '65%';
      microEl.innerText = "ELEVATED TENSION: Guarded swallow, defensive stance, pulse accelerating.";
    } else {
      stressFill.style.width = '30%';
      microEl.innerText = "BASELINE DEMEANOR: Steady pulse; cautious and deliberate responses.";
    }

    if (!this.ekgAnimId) {
      this.renderEKGFrame();
    }
  }

  renderEKGFrame() {
    if (!this.ekgCanvas || !this.ekgCtx) return;
    const ctx = this.ekgCtx;
    const w = this.ekgCanvas.width;
    const h = this.ekgCanvas.height;

    ctx.fillStyle = '#090d16';
    ctx.fillRect(0, 0, w, h);

    // Grid lines
    ctx.strokeStyle = 'rgba(56, 189, 248, 0.08)';
    ctx.lineWidth = 1;
    for (let x = 0; x < w; x += 20) {
      ctx.beginPath(); ctx.moveTo(x, 0); ctx.lineTo(x, h); ctx.stroke();
    }
    for (let y = 0; y < h; y += 10) {
      ctx.beginPath(); ctx.moveTo(0, y); ctx.lineTo(w, y); ctx.stroke();
    }

    // Determine current active suspect BPM
    const s = this.state.activeSuspect;
    const bpm = (s && this.state.suspectHeartRates[s.id]) ? this.state.suspectHeartRates[s.id] : 75;
    const speed = (bpm / 60) * 3.5;
    this.ekgPhase = (this.ekgPhase + speed) % w;

    // Wave color
    ctx.strokeStyle = bpm >= 130 ? '#ef4444' : (bpm >= 95 ? '#eab308' : '#22c55e');
    ctx.lineWidth = 2;
    ctx.beginPath();

    const midY = h / 2;
    for (let x = 0; x < w; x++) {
      const pos = (x + this.ekgPhase) % 120;
      let y = midY;
      if (pos > 40 && pos < 48) {
        y = midY - 4; // P wave
      } else if (pos >= 48 && pos < 52) {
        y = midY + 5; // Q wave
      } else if (pos >= 52 && pos < 57) {
        y = 5; // R wave (sharp peak)
      } else if (pos >= 57 && pos < 62) {
        y = h - 6; // S wave (sharp dip)
      } else if (pos >= 68 && pos < 78) {
        y = midY - 6; // T wave
      }
      if (x === 0) ctx.moveTo(x, y);
      else ctx.lineTo(x, y);
    }
    ctx.stroke();

    this.ekgAnimId = requestAnimationFrame(() => this.renderEKGFrame());
  }

  // -------------------------------------------------------------
  // Forensic Laboratory (Template Method & Spectroscopy Pattern)
  // -------------------------------------------------------------
  openForensicLab() {
    this.hideDialogue();
    this.hidePortrait();
    this.showScreen('forensic-lab-screen');
    this.setBg('assets/backgrounds/briefing_room.png');

    const curCase = CASE_DATA[this.state.caseNum];
    const listEl = document.getElementById('forensic-specimens-list');
    listEl.innerHTML = '';

    if (this.state.discoveredEvidence.size === 0) {
      listEl.innerHTML = '<div style="color:var(--text-dim); padding:10px;">No forensic evidence discovered yet. Search crime scenes first!</div>';
      return;
    }

    this.state.discoveredEvidence.forEach(eid => {
      const ev = curCase.evidence[eid];
      if (!ev) return;
      const card = document.createElement('div');
      card.className = `specimen-card ${this.state.selectedForensicSpecimen === eid ? 'selected' : ''}`;
      card.innerHTML = `<strong>${ev.id}</strong> — ${ev.name} <span style="font-size:0.75rem; color:var(--text-dim);">(${ev.cat})</span>`;
      card.onclick = () => {
        this.state.selectedForensicSpecimen = eid;
        this.openForensicLab();
      };
      listEl.appendChild(card);
    });

    if (!this.state.selectedForensicSpecimen && this.state.discoveredEvidence.size > 0) {
      this.state.selectedForensicSpecimen = Array.from(this.state.discoveredEvidence)[0];
    }
  }

  executeForensicAssay() {
    if (!this.state.selectedForensicSpecimen) {
      this.showToast("Select an evidence specimen first!");
      return;
    }

    const curCase = CASE_DATA[this.state.caseNum];
    const ev = curCase.evidence[this.state.selectedForensicSpecimen];
    const assay = this.state.selectedForensicAssay || 'chemical';
    const statusEl = document.getElementById('reaction-status');
    const beakerLiquid = document.getElementById('beaker-liquid');
    const certCard = document.getElementById('forensic-certificate');

    statusEl.innerHTML = `<em>Initiating ${assay.toUpperCase()} assay on [${ev.id}: ${ev.name}]...</em>`;
    
    // Animate Beaker Liquid
    if (assay === 'chemical') {
      beakerLiquid.style.background = 'linear-gradient(180deg, #38bdf8, #1e3a8a)';
    } else if (assay === 'spectroscopy') {
      beakerLiquid.style.background = 'linear-gradient(180deg, #c084fc, #581c87)';
    } else {
      beakerLiquid.style.background = 'linear-gradient(180deg, #fbbf24, #b45309)';
    }

    this.audio.playTypewriter();

    setTimeout(() => {
      let formula = "AgNO3 + Fe4[Fe(CN)6]3 Reaction Matrix";
      let finding = "Positive reaction for chemical residue and mechanical tampering.";
      let merit = 40;

      if (ev.id === 'E01' && this.state.caseNum === 1) {
        formula = "UV Refraction Lambda = 365nm Polarized Field";
        finding = "Micro-engraved coordinates matrix discovered on the sapphire facet. Links directly to the Obsidian Hourglass!";
        merit = 50;
      } else if (ev.id === 'E03' && this.state.caseNum === 1) {
        formula = "Electronic Pulse Latency Analyzer";
        finding = "Door server log reveals impossible 3-minute travel time between North Wing and Basement.";
        merit = 40;
      } else if (ev.id === 'E01' && this.state.caseNum === 2) {
        formula = "Prussian Blue Hydrocyanic Acid Assay";
        finding = "Potassium cyanide confirmed in tea dregs. Ingestion occurred exactly 12 minutes prior to demise.";
        merit = 45;
      } else if (ev.id === 'E02' && this.state.caseNum === 3) {
        formula = "Synthetic Aniline Ink Spectroscopy (420nm Absorption)";
        finding = "Forged suicide note written with modern 1894 ballpoint paste, disproving authentic quill pen origin.";
        merit = 45;
      }

      this.audio.playHiddenClue();
      this.achievements.unlock('forensic_chemist', this);
      this.state.score += merit;
      this.updateHUD();

      statusEl.innerHTML = `<strong>Assay Complete:</strong> Decisive evidentiary breakthrough authenticated!`;
      certCard.style.display = 'block';
      document.getElementById('cert-specimen').innerText = `Target Specimen: ${ev.id} — ${ev.name}`;
      document.getElementById('cert-formula').innerText = `Reagent Assay: ${formula}`;
      document.getElementById('cert-finding').innerText = `Forensic Finding: ${finding}`;
      document.getElementById('cert-badge').innerText = `+${merit} Forensic Merit Points Awarded`;

      const key = `${ev.id}_${assay}`;
      this.state.forensicReports.set(key, { evId: ev.id, formula, finding, merit });
      this.state.leads.push(`Forensic Certificate [${ev.id}]: ${finding}`);
    }, 600);
  }

  // -------------------------------------------------------------
  // Cognitive Mind Palace (Composite Graph & Epiphany Synthesis)
  // -------------------------------------------------------------
  openMindPalace() {
    this.hideDialogue();
    this.hidePortrait();
    this.showScreen('mindpalace-screen');
    this.setBg('assets/backgrounds/crime_scene_main.png');

    const curCase = CASE_DATA[this.state.caseNum];
    const grid = document.getElementById('mindpalace-thoughts-grid');
    grid.innerHTML = '';

    // Generate thoughts from discovered clues and premises
    const thoughts = [];
    this.state.discoveredEvidence.forEach(eid => {
      const ev = curCase.evidence[eid];
      if (ev) {
        thoughts.push({ id: ev.id, cat: ev.cat, title: ev.name, snippet: ev.desc.slice(0, 75) + '...' });
      }
    });

    if (thoughts.length === 0) {
      grid.innerHTML = '<div style="color:var(--text-dim); padding:16px;">No cognitive thoughts established yet. Examine crime scenes to populate premises!</div>';
    } else {
      thoughts.forEach(t => {
        const card = document.createElement('div');
        const isPinned = (this.state.mindPalacePin1 === t.id || this.state.mindPalacePin2 === t.id);
        card.className = `thought-node-card ${isPinned ? 'pinned' : ''}`;
        card.innerHTML = `
          <div class="thought-node-badge">[${t.cat}] ${t.id}</div>
          <div class="thought-node-title">${t.title}</div>
          <p style="font-size:0.78rem; color:var(--text-dim); margin-top:4px;">${t.snippet}</p>
        `;
        card.onclick = () => this.pinMindPalaceThought(t.id);
        grid.appendChild(card);
      });
    }

    this.renderForgedEpiphanies();
    this.initMindPalaceCanvas();
  }

  pinMindPalaceThought(thoughtId) {
    if (this.state.mindPalacePin1 === thoughtId) {
      this.state.mindPalacePin1 = null;
    } else if (this.state.mindPalacePin2 === thoughtId) {
      this.state.mindPalacePin2 = null;
    } else if (!this.state.mindPalacePin1) {
      this.state.mindPalacePin1 = thoughtId;
    } else if (!this.state.mindPalacePin2) {
      this.state.mindPalacePin2 = thoughtId;
    } else {
      this.state.mindPalacePin1 = thoughtId;
    }

    const curCase = CASE_DATA[this.state.caseNum];
    const t1 = this.state.mindPalacePin1 ? curCase.evidence[this.state.mindPalacePin1]?.name : "Select Thought Alpha";
    const t2 = this.state.mindPalacePin2 ? curCase.evidence[this.state.mindPalacePin2]?.name : "Select Thought Beta";

    document.getElementById('thought-slot-1').innerText = t1 || "Select Thought Alpha";
    document.getElementById('thought-slot-2').innerText = t2 || "Select Thought Beta";

    this.openMindPalace();
  }

  synthesizeMindEpiphany() {
    if (!this.state.mindPalacePin1 || !this.state.mindPalacePin2) {
      this.showToast("Pin two complementary thoughts to forge an Epiphany!");
      return;
    }

    const p1 = this.state.mindPalacePin1;
    const p2 = this.state.mindPalacePin2;
    const curCase = CASE_DATA[this.state.caseNum];

    const match = curCase.deductions.find(d => 
      (d.id1 === p1 && d.id2 === p2) || (d.id1 === p2 && d.id2 === p1)
    );

    if (match) {
      const epKey = `${match.id1}_${match.id2}`;
      if (!this.state.forgedEpiphanies.has(epKey)) {
        this.state.forgedEpiphanies.add(epKey);
        this.audio.playObjection();
        this.triggerAnimeCutIn('detective', 'EPIPHANY!');
        this.achievements.unlock('mind_palace_epiphany', this);

        this.state.score += 50;
        this.state.leads.push(`Mind Palace Epiphany: [${match.title}] — ${match.insight}`);
        this.updateHUD();
        this.showToast(`COGNITIVE EPIPHANY FORGED: ${match.title} (+50 Points)`, 4500);
      } else {
        this.showToast("This cognitive epiphany has already been forged.");
      }
    } else {
      this.audio.playBuzzer();
      this.shakeScreen();
      this.showToast("No logical cognitive resonance between these thought premises.");
    }

    this.state.mindPalacePin1 = null;
    this.state.mindPalacePin2 = null;
    document.getElementById('thought-slot-1').innerText = "Select Thought Alpha";
    document.getElementById('thought-slot-2').innerText = "Select Thought Beta";
    this.openMindPalace();
  }

  renderForgedEpiphanies() {
    const list = document.getElementById('forged-epiphanies-list');
    if (!list) return;
    list.innerHTML = '';

    const curCase = CASE_DATA[this.state.caseNum];
    const forgedList = curCase.deductions.filter(d => this.state.forgedEpiphanies.has(`${d.id1}_${d.id2}`));

    if (forgedList.length === 0) {
      list.innerHTML = '<div style="color:var(--text-dim); font-size:0.95rem; font-style:italic; padding:20px; text-align:center;">No cognitive epiphanies synthesized yet. Cross-link complementary thought premises!</div>';
      return;
    }

    forgedList.forEach(f => {
      const card = document.createElement('div');
      card.className = 'epiphany-card';
      card.innerHTML = `
        <div class="epiphany-title">⚡ ${f.title}</div>
        <div class="epiphany-desc">${f.insight}</div>
      `;
      list.appendChild(card);
    });
  }

  initMindPalaceCanvas() {
    if (!this.mindPalaceCanvas || !this.mindPalaceCtx) return;
    const cvs = this.mindPalaceCanvas;
    const ctx = this.mindPalaceCtx;
    const rect = cvs.parentElement.getBoundingClientRect();
    cvs.width = rect.width;
    cvs.height = rect.height;

    ctx.clearRect(0, 0, cvs.width, cvs.height);

    // Draw neural cosmic stars
    for (let i = 0; i < 40; i++) {
      const x = (Math.sin(i * 99) * 0.5 + 0.5) * cvs.width;
      const y = (Math.cos(i * 77) * 0.5 + 0.5) * cvs.height;
      const r = (i % 3) + 1;
      ctx.fillStyle = i % 2 === 0 ? 'rgba(192, 132, 252, 0.4)' : 'rgba(244, 196, 66, 0.4)';
      ctx.beginPath();
      ctx.arc(x, y, r, 0, Math.PI * 2);
      ctx.fill();
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
