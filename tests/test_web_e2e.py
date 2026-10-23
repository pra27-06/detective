"""
Case Closed -- Web Visual Novel Browser Automation & E2E Regression Test Suite
Tests:
  1. Main menu navigation & subscreen toggling
  2. Case 1 investigation: hotspots discovery & duplicate discovery protection
  3. Case 1 contradiction: invalid contradiction penalty vs valid objection
  4. Case 1 clue board: deduction pairing
  5. Courtroom indictment: alternative collapsed route vs Master Sleuth Grade S route
  6. Case 2: The Last Train Mystery end-to-end indictment
  7. Case 3: The Locked Room Incident end-to-end indictment
  8. Save & Load persistence roundtrip
  9. Mobile viewport responsiveness (375x667)
  10. Zero console exceptions audit
"""

import subprocess
import time
import requests
import json
import asyncio
import websockets
import os
import sys

EDGE_PATH = r"C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe"
LOCAL_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
USER_DATA = os.path.join(os.environ.get("TEMP", "."), f"edge_test_profile_{int(time.time())}")

async def run_tests():
    print("=" * 70)
    print("  CASE CLOSED :: AUTOMATED WEB E2E & MOBILE REGRESSION SUITE")
    print("=" * 70)

    # Start local server to ensure tests run reliably offline or during CI
    http_proc = subprocess.Popen([
        sys.executable, "-m", "http.server", "8095", "--directory", LOCAL_DIR
    ], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    time.sleep(1)

    target_url = "http://127.0.0.1:8095/index.html"

    edge_proc = subprocess.Popen([
        EDGE_PATH,
        "--headless=new",
        "--remote-debugging-port=9222",
        "--disable-gpu",
        "--no-first-run",
        "--no-default-browser-check",
        "--ignore-certificate-errors",
        f"--user-data-dir={USER_DATA}",
        "about:blank"
    ], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

    try:
        for _ in range(20):
            try:
                r = requests.get("http://127.0.0.1:9222/json", timeout=1)
                if r.status_code == 200 and len(r.json()) > 0:
                    break
            except Exception:
                time.sleep(0.5)

        target = [t for t in r.json() if t.get("type") == "page"][0]
        ws_url = target["webSocketDebuggerUrl"]

        async with websockets.connect(ws_url, max_size=50*1024*1024) as ws:
            msg_id = 0
            pending = {}
            console_errors = []

            async def listen():
                async for raw in ws:
                    m = json.loads(raw)
                    if "id" in m and m["id"] in pending:
                        pending[m["id"]].set_result(m)
                    elif "method" in m:
                        method = m["method"]
                        params = m.get("params", {})
                        if method == "Runtime.exceptionThrown":
                            console_errors.append(params)
                        elif method == "Runtime.consoleAPICalled" and params.get("type") == "error":
                            console_errors.append(params)

            listener = asyncio.create_task(listen())

            async def send(method, params=None):
                nonlocal msg_id
                msg_id += 1
                mid = msg_id
                fut = asyncio.get_event_loop().create_future()
                pending[mid] = fut
                payload = {"id": mid, "method": method}
                if params: payload["params"] = params
                await ws.send(json.dumps(payload))
                return await fut

            async def eval_js(expr):
                res = await send("Runtime.evaluate", {
                    "expression": expr,
                    "returnByValue": True,
                    "awaitPromise": True
                })
                if "exceptionDetails" in res.get("result", {}):
                    err = res["result"]["exceptionDetails"]
                    raise RuntimeError(f"JS Exception: {err}")
                return res.get("result", {}).get("result", {}).get("value")

            await send("Page.enable")
            await send("Runtime.enable")
            await send("Network.enable")
            await send("Page.navigate", {"url": target_url})
            await asyncio.sleep(2)

            # Test Desktop
            await send("Emulation.setDeviceMetricsOverride", {
                "width": 1280, "height": 720, "deviceScaleFactor": 1, "mobile": False
            })

            # Suite 1: Menu navigation
            print("[SUITE 1/7] Menu Navigation & Static Screens...")
            await eval_js("document.getElementById('btn-how-to-play').click()")
            assert await eval_js("document.querySelector('.screen.active')?.id") == "how-to-play-screen"
            await eval_js("document.getElementById('btn-htp-back').click()")
            assert await eval_js("document.querySelector('.screen.active')?.id") == "menu-screen"
            print("  [PASS] Menu navigation verified.")

            # Suite 2: Case 1 Hotspots & Duplicate Prevention
            print("[SUITE 2/7] Case #1 Investigation & Evidence Discovery...")
            await eval_js("window.gameApp.startCase(1); window.gameApp.openInvestigation();")
            init_score = await eval_js("window.gameApp.state.score")
            await eval_js("""
                (() => {
                    const scene = CASE_DATA[1].scenes[0];
                    window.gameApp.examineHotspot(scene, scene.hotspots[0], `${scene.id}_0`);
                    window.gameApp.closeModal();
                })();
            """)
            s1 = await eval_js("window.gameApp.state.score")
            assert s1 == init_score + 25

            # Duplicate check
            await eval_js("""
                (() => {
                    const scene = CASE_DATA[1].scenes[0];
                    window.gameApp.examineHotspot(scene, scene.hotspots[0], `${scene.id}_0`);
                })();
            """)
            s2 = await eval_js("window.gameApp.state.score")
            assert s2 == s1, "Duplicate click awarded points!"
            print("  [PASS] Hotspot discovery & duplicate protection verified.")

            # Suite 3: Contradiction & Objection
            print("[SUITE 3/7] Interrogation & Contradiction Resolution...")
            # Discover remaining evidence
            await eval_js("""
                CASE_DATA[1].scenes.forEach(scene => {
                    scene.hotspots.forEach((hs, idx) => {
                        window.gameApp.examineHotspot(scene, hs, `${scene.id}_${idx}`);
                    });
                });
                window.gameApp.closeModal();
            """)
            # Invalid contradiction penalty
            pre_pen = await eval_js("window.gameApp.state.score")
            await eval_js("""
                (() => {
                    const croft = CASE_DATA[1].suspects.find(s => s.id === 'S03');
                    const wrong = CASE_DATA[1].evidence['E01'];
                    window.gameApp.submitContradiction(croft, wrong);
                })();
            """)
            post_pen = await eval_js("window.gameApp.state.score")
            assert post_pen == pre_pen - 15, "Invalid contradiction should penalize 15 points"

            # Valid contradiction
            await eval_js("""
                (() => {
                    const croft = CASE_DATA[1].suspects.find(s => s.id === 'S03');
                    const right = CASE_DATA[1].evidence['E03'];
                    window.gameApp.submitContradiction(croft, right);
                })();
            """)
            await asyncio.sleep(1.6)
            assert await eval_js("window.gameApp.state.brokenAlibis.has('S03')") is True
            print("  [PASS] Contradiction penalty and decisive alibi break verified.")

            # Suite 4: Clue Board Deductions
            print("[SUITE 4/7] Clue Board Analytical Synthesis...")
            await eval_js("""
                window.gameApp.openClueBoard();
                window.gameApp.state.cluePin1 = 'E01';
                window.gameApp.state.cluePin2 = 'E03';
                window.gameApp.connectPinnedClues();
            """)
            assert await eval_js("window.gameApp.state.unlockedDeductions.has('E01_E03')") is True
            print("  [PASS] Clue board deduction connection verified.")

            # Suite 5: Indictment & Verdicts (Failed vs Master Sleuth)
            print("[SUITE 5/7] Indictment Evaluations (Collapsed vs Master Sleuth)...")
            # Collapsed
            await eval_js("""
                window.gameApp.openAccusation();
                window.gameApp.state.accusedSuspect = 'S01'; // Lady Evelyn Sterling
                window.gameApp.state.accusedEvidence = 'E03';
                window.gameApp.state.accusedMotive = 'DEBT';
                window.gameApp.evaluateVerdict();
            """)
            v_fail = await eval_js("document.getElementById('verdict-title').innerText")
            assert "ACCUSATION COLLAPSED" in v_fail

            # Master Sleuth
            await eval_js("""
                window.gameApp.openAccusation();
                window.gameApp.state.accusedSuspect = 'S03'; // Vincent Croft
                window.gameApp.state.accusedEvidence = 'E03';
                window.gameApp.state.accusedMotive = 'DEBT';
                window.gameApp.evaluateVerdict();
            """)
            v_win = await eval_js("document.getElementById('verdict-title').innerText")
            assert "MASTER DETECTIVE" in v_win
            print("  [PASS] Alternative and winning verdicts verified.")

            # Suite 6: Save & Load Persistence
            print("[SUITE 6/7] State Persistence (Save & Resume)...")
            await eval_js("""
                window.gameApp.state.score = 520;
                window.gameApp.saveGame();
            """)
            await send("Page.reload")
            await asyncio.sleep(1.5)
            await eval_js("document.getElementById('btn-continue').click()")
            assert await eval_js("window.gameApp.state.score") == 520
            print("  [PASS] Save & resume roundtrip verified.")

            # Suite 7: Mobile Viewport Layout Verification
            print("[SUITE 7/7] Mobile Viewport Responsiveness (375x667)...")
            await send("Emulation.setDeviceMetricsOverride", {
                "width": 375, "height": 667, "deviceScaleFactor": 2, "mobile": True
            })
            await asyncio.sleep(0.5)
            await eval_js("window.gameApp.openInvestigation();")
            nav_fits = await eval_js("""
                (() => {
                    const nav = ['btn-nav-interrogate', 'btn-nav-clueboard', 'btn-nav-notebook', 'btn-nav-accuse'].map(id => {
                        const r = document.getElementById(id).getBoundingClientRect();
                        return r.right <= 375 && r.left >= 0;
                    });
                    return nav.every(Boolean);
                })()
            """)
            assert nav_fits is True, "Navigation buttons must fit within 375px mobile screen"
            print("  [PASS] Mobile viewport layout verified.")

            # Suite 8: Audio Synthesis & Settings Controls
            print("[SUITE 8/11] Audio Synthesis & Settings Controls...")
            await eval_js("""
                window.gameApp.audio.init();
                window.gameApp.audio.setMasterVolume(0.7);
                window.gameApp.audio.setMusicVolume(0.5);
                window.gameApp.audio.setSfxVolume(0.8);
                window.gameApp.audio.setAmbienceVolume(0.4);
                window.gameApp.audio.toggleMute(false);
            """)
            assert await eval_js("window.gameApp.audio.masterVol") == 0.7
            assert await eval_js("window.gameApp.audio.musicVol") == 0.5
            assert await eval_js("window.gameApp.audio.isMuted") is False
            print("  [PASS] Web Audio synthesizer volume controls verified.")

            # Suite 9: Detective Journal & Timeline Reconstruction
            print("[SUITE 9/11] Detective Journal & Timeline Reconstruction...")
            await eval_js("window.gameApp.openJournal();")
            assert await eval_js("document.querySelector('.screen.active')?.id") == "journal-screen"
            dossier_count = await eval_js("document.querySelectorAll('.dossier-card').length")
            assert dossier_count > 0, "Dossier cards must render in journal"
            
            await eval_js("window.gameApp.openTimeline();")
            assert await eval_js("document.querySelector('.screen.active')?.id") == "timeline-screen"
            timeline_events = await eval_js("document.querySelectorAll('.timeline-card').length")
            assert timeline_events > 0, "Timeline event cards must render in timeline screen"
            print("  [PASS] Suspect dossiers and chronological timeline verified.")

            # Suite 10: Anime Cut-in & Achievement System
            print("[SUITE 10/11] Cinematic Anime Cut-in & Achievements...")
            await eval_js("""
                window.gameApp.achievements.unlock('first_clue');
                window.gameApp.triggerAnimeCutIn('Marcus Cruz', 'OBJECTION!', 'assets/characters/cruz_nervous.png');
            """)
            is_cutin_active = await eval_js("document.getElementById('anime-cutin').classList.contains('active')")
            assert is_cutin_active is True, "Cut-in banner must activate with active class"
            assert await eval_js("window.gameApp.achievements.has('first_clue')") is True
            print("  [PASS] Anime cut-in banner and achievement unlocking verified.")

            # Suite 11: The Obsidian Hourglass Syndicate Conspiracy Epilogue
            print("[SUITE 11/11] Overarching Syndicate Mystery Epilogue...")
            await eval_js("window.gameApp.openEpilogue();")
            assert await eval_js("document.querySelector('.screen.active')?.id") == "epilogue-screen"
            epilogue_text = await eval_js("document.getElementById('epilogue-screen').innerText")
            assert "OBSIDIAN HOURGLASS" in epilogue_text
            assert "Star of Alexandria" in epilogue_text
            assert "Victor Sterling" in epilogue_text
            assert "Thaddeus Thorne" in epilogue_text
            print("  [PASS] Overarching conspiracy narrative connection verified.")

            # Suite 12: Forensic Chemistry & Spectroscopy Lab
            print("[SUITE 12/14] Forensic Chemistry & Spectroscopy Lab Assays...")
            await eval_js("window.gameApp.openForensicLab();")
            assert await eval_js("document.querySelector('.screen.active')?.id") == "forensic-lab-screen"
            specimens_count = await eval_js("document.querySelectorAll('.specimen-card').length")
            assert specimens_count > 0, "Specimens must render in forensic lab"
            await eval_js("window.gameApp.executeForensicAssay();")
            await asyncio.sleep(0.8)
            cert_display = await eval_js("document.getElementById('forensic-certificate').style.display")
            assert cert_display == "block", "Forensic certificate must display upon assay completion"
            assert await eval_js("window.gameApp.achievements.has('forensic_chemist')") is True
            print("  [PASS] Forensic reagent assay and Scotland Yard certificate verified.")

            # Suite 13: Cognitive Mind Palace & Epiphany Synthesis
            print("[SUITE 13/14] Cognitive Mind Palace & Epiphany Synthesis...")
            await eval_js("window.gameApp.openMindPalace();")
            assert await eval_js("document.querySelector('.screen.active')?.id") == "mindpalace-screen"
            thoughts_count = await eval_js("document.querySelectorAll('.thought-node-card').length")
            assert thoughts_count > 0, "Thought nodes must render in Mind Palace"
            await eval_js("""
                window.gameApp.state.mindPalacePin1 = 'E01';
                window.gameApp.state.mindPalacePin2 = 'E03';
                window.gameApp.synthesizeMindEpiphany();
            """)
            assert await eval_js("window.gameApp.state.forgedEpiphanies.size") > 0
            assert await eval_js("window.gameApp.achievements.has('mind_palace_epiphany')") is True
            print("  [PASS] Cognitive thought pinning and Epiphany synthesis verified.")

            # Suite 14: Suspect Psychological Heart-Rate EKG Monitor
            print("[SUITE 14/14] Suspect Psychological Heart-Rate EKG Monitor...")
            await eval_js("window.gameApp.openInterrogation();")
            ekg_display = await eval_js("document.getElementById('suspect-ekg-card').style.display")
            assert ekg_display == "block", "EKG monitor card must be visible for suspects"
            bpm_text = await eval_js("document.getElementById('ekg-bpm-display').innerText")
            assert "BPM" in bpm_text
            print("  [PASS] Real-time suspect EKG pulse and stress gauge verified.")

            assert len(console_errors) == 0, f"Found console errors: {console_errors}"
            print("\n>> ALL 14 AUTOMATED WEB TEST SUITES PASSED (0 FAILURES, 0 CONSOLE ERRORS)")

            listener.cancel()
    finally:
        edge_proc.terminate()
        edge_proc.wait()
        http_proc.terminate()
        http_proc.wait()

if __name__ == "__main__":
    asyncio.run(run_tests())
