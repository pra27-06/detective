"""
Asset Generator for Case Closed: Anime Visual Novel Detective Game
Generates all 1280x720 backgrounds, 600x850 anime character portraits with expressions,
and UI textures with transparency using Python Pillow.
"""

import os
import math
from PIL import Image, ImageDraw, ImageFont, ImageFilter

ASSETS_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "assets")
BG_DIR = os.path.join(ASSETS_DIR, "backgrounds")
CHAR_DIR = os.path.join(ASSETS_DIR, "characters")
PORTRAIT_DIR = os.path.join(ASSETS_DIR, "portraits")
UI_DIR = os.path.join(ASSETS_DIR, "ui")

for d in [BG_DIR, CHAR_DIR, PORTRAIT_DIR, UI_DIR]:
    os.makedirs(d, exist_ok=True)

def get_font(size, bold=False):
    # Try system fonts first
    font_paths = [
        "C:\\Windows\\Fonts\\segoeui.ttf",
        "C:\\Windows\\Fonts\\arial.ttf",
        "C:\\Windows\\Fonts\\tahoma.ttf"
    ]
    if bold:
        font_paths = [
            "C:\\Windows\\Fonts\\segoeuib.ttf",
            "C:\\Windows\\Fonts\\arialbd.ttf"
        ] + font_paths
    for p in font_paths:
        if os.path.exists(p):
            try:
                return ImageFont.truetype(p, size)
            except Exception:
                pass
    return ImageFont.load_default()

def draw_vignette(img, intensity=0.6):
    w, h = img.size
    overlay = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    draw = ImageDraw.Draw(overlay)
    cx, cy = w / 2, h / 2
    max_dist = math.sqrt(cx**2 + cy**2)
    for r in range(0, int(max_dist), 8):
        alpha = int(255 * intensity * ((r / max_dist) ** 2))
        alpha = min(255, max(0, alpha))
        draw.ellipse([cx - r, cy - r, cx + r, cy + r], outline=(0, 0, 0, alpha), width=8)
    return Image.alpha_composite(img.convert("RGBA"), overlay)

# -------------------------------------------------------------
# BACKGROUND GENERATORS (1280x720)
# -------------------------------------------------------------

def create_main_menu_bg():
    w, h = 1280, 720
    im = Image.new("RGB", (w, h), (10, 14, 26))
    draw = ImageDraw.Draw(im)
    
    # Atmospheric night gradient
    for y in range(h):
        r = int(12 + (y / h) * 18)
        g = int(16 + (y / h) * 24)
        b = int(32 + (y / h) * 45)
        draw.line([(0, y), (w, y)], fill=(r, g, b))
    
    # Moon / Light glow
    moon_x, moon_y = 1000, 160
    for r in range(220, 0, -4):
        alpha = int(45 * (1.0 - r / 220))
        draw.ellipse([moon_x - r, moon_y - r, moon_x + r, moon_y + r], fill=(160, 200, 255, alpha))
    draw.ellipse([moon_x - 60, moon_y - 60, moon_x + 60, moon_y + 60], fill=(240, 248, 255))
    
    # Victorian skyline & museum silhouette
    # Distant buildings
    for i in range(15):
        bx = i * 90 - 30
        bw = 80 + (i % 3) * 25
        bh = 220 + ((i * 37) % 140)
        draw.rectangle([bx, h - bh, bx + bw, h], fill=(15, 20, 35))
        # Spire
        if i % 3 == 0:
            draw.polygon([(bx + bw//2, h - bh - 60), (bx + 10, h - bh), (bx + bw - 10, h - bh)], fill=(12, 17, 30))

    # Foreground Museum Columns
    col_w = 48
    col_spacing = 130
    pediment_top = 260
    # Pediment triangle
    draw.polygon([(w//2, pediment_top - 90), (140, pediment_top), (w - 140, pediment_top)], fill=(22, 28, 48))
    draw.rectangle([140, pediment_top, w - 140, pediment_top + 25], fill=(30, 38, 64))
    for x in range(180, w - 180, col_spacing):
        draw.rectangle([x, pediment_top + 25, x + col_w, h - 80], fill=(26, 34, 56))
        # Shadow fluting
        draw.line([(x + 12, pediment_top + 25), (x + 12, h - 80)], fill=(18, 24, 40), width=3)
        draw.line([(x + 24, pediment_top + 25), (x + 24, h - 80)], fill=(35, 45, 75), width=2)
        draw.line([(x + 36, pediment_top + 25), (x + 36, h - 80)], fill=(18, 24, 40), width=3)

    # Grand staircase
    for s in range(5):
        sy = h - 80 + s * 16
        draw.rectangle([60 - s * 10, sy, w - 60 + s * 10, sy + 16], fill=(20 + s*2, 25 + s*2, 42 + s*3))

    # Gaslamp warm glow
    for lx in [240, 1040]:
        ly = 430
        for r in range(90, 0, -3):
            glow = int(120 * (1.0 - r / 90))
            # draw radial warm light
            draw.ellipse([lx - r, ly - r, lx + r, ly + r], outline=(255, 195, 90, glow))
        draw.rectangle([lx - 4, ly, lx + 4, h - 80], fill=(12, 12, 16))
        draw.polygon([(lx, ly - 35), (lx - 16, ly - 10), (lx + 16, ly - 10)], fill=(255, 220, 120))

    # Cobblestone road reflection
    for y in range(h - 50, h):
        for x in range(0, w, 30):
            if (x + y * 3) % 40 < 15:
                draw.line([(x, y), (x + 20, y)], fill=(32, 40, 65))

    im = draw_vignette(im, 0.5)
    return im

def create_briefing_room_bg():
    w, h = 1280, 720
    im = Image.new("RGB", (w, h), (24, 18, 14))
    draw = ImageDraw.Draw(im)
    
    # Warm Victorian mahogany office wallpaper & wainscoting
    for y in range(h):
        r = int(38 - (y / h) * 16)
        g = int(26 - (y / h) * 12)
        b = int(20 - (y / h) * 8)
        draw.line([(0, y), (w, y)], fill=(r, g, b))

    # Wood paneling
    for x in range(0, w, 160):
        draw.rectangle([x + 10, 80, x + 150, 420], outline=(55, 38, 28), width=3)
        draw.rectangle([x + 15, 85, x + 145, 415], fill=(30, 20, 15))

    # Tall arched window on right with rain
    wx, wy, ww, wh = 880, 60, 320, 480
    draw.rectangle([wx, wy + 60, wx + ww, wy + wh], fill=(18, 24, 38))
    draw.chord([wx, wy, wx + ww, wy + 160], 180, 360, fill=(18, 24, 38))
    # Window panes
    draw.line([(wx + ww//2, wy), (wx + ww//2, wy + wh)], fill=(50, 35, 25), width=6)
    draw.line([(wx, wy + 200), (wx + ww, wy + 200)], fill=(50, 35, 25), width=4)
    draw.line([(wx, wy + 340), (wx + ww, wy + 340)], fill=(50, 35, 25), width=4)
    # Rain streaks
    for i in range(40):
        rx = wx + 15 + (i * 19) % (ww - 30)
        ry = wy + 40 + (i * 37) % (wh - 60)
        draw.line([(rx, ry), (rx - 4, ry + 16)], fill=(90, 120, 160, 140), width=1)

    # Big Scotland Yard investigation map board on left
    draw.rectangle([100, 100, 520, 400], fill=(225, 215, 190), outline=(65, 45, 30), width=8)
    draw.rectangle([115, 115, 505, 385], fill=(235, 225, 200))
    # Red thread and pinned notes
    pins = [(160, 160), (320, 200), (450, 170), (280, 310), (420, 320)]
    for i in range(len(pins)-1):
        draw.line([pins[i], pins[i+1]], fill=(190, 30, 30), width=2)
    for px, py in pins:
        draw.rectangle([px - 22, py - 18, px + 22, py + 18], fill=(255, 250, 230), outline=(140, 120, 90))
        draw.ellipse([px - 4, py - 4, px + 4, py + 4], fill=(210, 20, 20))

    # Large mahogany desk in foreground
    draw.rectangle([0, 500, w, h], fill=(32, 22, 16))
    draw.line([(0, 500), (w, 500)], fill=(65, 45, 30), width=5)
    # Green banker's lamp
    draw.rectangle([680, 470, 700, 530], fill=(180, 150, 60))
    draw.ellipse([640, 450, 740, 480], fill=(20, 90, 45), outline=(15, 70, 35), width=2)
    # Lamp glow
    for r in range(140, 0, -4):
        glow = int(90 * (1.0 - r / 140))
        draw.ellipse([690 - r, 490 - r//2, 690 + r, 490 + r//2], outline=(255, 240, 150, glow))

    im = draw_vignette(im, 0.45)
    return im

def create_scene_vault_bg():
    w, h = 1280, 720
    im = Image.new("RGB", (w, h), (14, 18, 28))
    draw = ImageDraw.Draw(im)
    
    # Museum Exhibition Vault
    for y in range(h):
        draw.line([(0, y), (w, y)], fill=(12 + y//50, 16 + y//40, 26 + y//30))

    # Marble pillars
    for x in [120, 340, 940, 1160]:
        draw.rectangle([x - 40, 0, x + 40, 520], fill=(42, 48, 64))
        draw.line([(x - 30, 0), (x - 30, 520)], fill=(55, 62, 82), width=3)
        draw.line([(x + 20, 0), (x + 20, 520)], fill=(28, 34, 46), width=4)

    # Marble floor with reflections
    draw.rectangle([0, 520, w, h], fill=(22, 26, 38))
    draw.line([(0, 520), (w, 520)], fill=(50, 60, 80), width=3)
    for y in range(530, h, 35):
        draw.line([(0, y), (w, y)], fill=(32, 38, 54), width=1)
    for x in range(0, w, 110):
        draw.line([(x, 520), (x - 80, h)], fill=(32, 38, 54), width=1)

    # Shattered Display Pedestal (Center)
    ped_cx = 640
    # Pedestal base
    draw.polygon([(ped_cx - 100, 580), (ped_cx + 100, 580), (ped_cx + 80, 420), (ped_cx - 80, 420)], fill=(35, 40, 55), outline=(60, 70, 95))
    draw.ellipse([ped_cx - 85, 405, ped_cx + 85, 435], fill=(45, 52, 70), outline=(80, 95, 125), width=2)
    # Velvet gem cushion inside
    draw.ellipse([ped_cx - 50, 412, ped_cx + 50, 428], fill=(90, 15, 30))
    # Empty indentation where sapphire rested
    draw.ellipse([ped_cx - 20, 416, ped_cx + 20, 424], fill=(40, 5, 12))

    # Broken Glass Dome Shards
    # Jagged base ring
    draw.polygon([(ped_cx - 75, 415), (ped_cx - 65, 370), (ped_cx - 50, 415), (ped_cx - 30, 360), (ped_cx - 10, 415),
                  (ped_cx + 15, 350), (ped_cx + 40, 415), (ped_cx + 65, 380), (ped_cx + 75, 415)], fill=(130, 180, 220, 90), outline=(180, 220, 255))
    # Glass shards on marble floor
    shards = [
        [(ped_cx - 120, 540), (ped_cx - 105, 532), (ped_cx - 95, 546)],
        [(ped_cx - 60, 560), (ped_cx - 40, 550), (ped_cx - 48, 565)],
        [(ped_cx + 80, 550), (ped_cx + 110, 542), (ped_cx + 95, 560)],
        [(ped_cx + 130, 570), (ped_cx + 150, 562), (ped_cx + 138, 578)],
    ]
    for s in shards:
        draw.polygon(s, fill=(180, 220, 255, 180), outline=(220, 245, 255))

    # Red Laser Tripwire sensors (deactivated/broken)
    draw.line([(100, 390), (ped_cx - 80, 410)], fill=(210, 40, 40, 180), width=2)
    draw.line([(ped_cx + 80, 410), (1180, 390)], fill=(90, 30, 30, 80), width=1) # severed/flickering

    # Velvet ropes & brass stanchions
    for sx in [440, 840]:
        draw.rectangle([sx - 6, 440, sx + 6, 560], fill=(195, 160, 60))
        draw.ellipse([sx - 14, 430, sx + 14, 450], fill=(225, 190, 80))
    # Drooping red velvet rope
    draw.arc([440, 435, 840, 510], 0, 180, fill=(150, 20, 30), width=8)

    im = draw_vignette(im, 0.4)
    return im

def create_scene_substation_bg():
    w, h = 1280, 720
    im = Image.new("RGB", (w, h), (18, 18, 20))
    draw = ImageDraw.Draw(im)
    
    # Subterranean transformer room
    for y in range(h):
        draw.line([(0, y), (w, y)], fill=(16 + y//45, 17 + y//45, 20 + y//40))

    # Exposed concrete pillars & ceiling pipes
    for x in range(0, w, 240):
        draw.rectangle([x, 0, x + 35, 560], fill=(30, 30, 34), outline=(45, 45, 50))
    # Overhead cable conduits
    draw.line([(0, 60), (w, 60)], fill=(45, 45, 55), width=10)
    draw.line([(0, 85), (w, 85)], fill=(35, 35, 45), width=14)
    draw.line([(0, 115), (w, 115)], fill=(25, 40, 60), width=8)

    # Massive industrial electrical breaker cabinet on left
    draw.rectangle([140, 140, 480, 560], fill=(38, 42, 46), outline=(65, 72, 78), width=4)
    # Hazard yellow & black caution stripes
    for i in range(10):
        draw.polygon([(150 + i*32, 150), (170 + i*32, 150), (150 + i*32, 175), (130 + i*32, 175)], fill=(220, 175, 30))
    # Breaker switches
    for row in range(4):
        for col in range(3):
            bx = 180 + col * 90
            by = 220 + row * 75
            draw.rectangle([bx, by, bx + 65, by + 45], fill=(22, 24, 26), outline=(50, 55, 60))
            # Switch position (main breaker tripped!)
            if row == 1 and col == 1:
                draw.rectangle([bx + 10, by + 22, bx + 55, by + 38], fill=(195, 35, 35)) # tripped RED
            else:
                draw.rectangle([bx + 10, by + 8, bx + 55, by + 24], fill=(45, 145, 65)) # normal GREEN

    # Security Server Terminal on right
    draw.rectangle([800, 180, 1120, 560], fill=(28, 32, 40), outline=(55, 65, 80), width=3)
    # CRT terminal screen
    draw.rectangle([840, 220, 1080, 380], fill=(10, 25, 15), outline=(40, 80, 50), width=4)
    # Green phosphor data lines
    for line in range(7):
        draw.line([(860, 245 + line * 18), (1020 - (line*23)%120, 245 + line * 18)], fill=(40, 210, 80), width=2)
    # Amber warning light
    draw.ellipse([840, 195, 856, 211], fill=(245, 160, 20))
    draw.ellipse([870, 195, 886, 211], fill=(40, 180, 60))

    # Concrete floor with industrial grating
    draw.rectangle([0, 560, w, h], fill=(24, 24, 28))
    draw.line([(0, 560), (w, 560)], fill=(50, 50, 55), width=4)
    for x in range(500, 780, 25):
        draw.line([(x, 560), (x, h)], fill=(15, 15, 18), width=4)

    # Discarded chemical solvent rag on floor
    draw.ellipse([580, 620, 660, 650], fill=(120, 100, 85), outline=(75, 60, 50))

    im = draw_vignette(im, 0.45)
    return im

def create_scene_locker_bg():
    w, h = 1280, 720
    im = Image.new("RGB", (w, h), (22, 25, 28))
    draw = ImageDraw.Draw(im)
    
    # Metal security guard locker room
    for y in range(h):
        draw.line([(0, y), (w, y)], fill=(20 + y//50, 23 + y//45, 26 + y//40))

    # Bank of lockers
    for col in range(6):
        lx = 80 + col * 180
        draw.rectangle([lx, 100, lx + 160, 560], fill=(36, 44, 48), outline=(60, 72, 80), width=3)
        # Upper vent slits
        for v in range(4):
            draw.line([(lx + 40, 130 + v * 12), (lx + 120, 130 + v * 12)], fill=(20, 24, 28), width=3)
        # Handle & lock
        draw.rectangle([lx + 20, 320, lx + 36, 360], fill=(150, 160, 170))
        draw.ellipse([lx + 24, 335, lx + 32, 345], fill=(20, 20, 20))
        # Locker numbers
        draw.rectangle([lx + 60, 180, lx + 100, 200], fill=(200, 200, 180))

    # Locker #2 (Marcus Cruz) partially ajar!
    cruz_lx = 80 + 1 * 180
    draw.rectangle([cruz_lx + 15, 105, cruz_lx + 155, 555], fill=(15, 18, 20)) # dark interior
    draw.polygon([(cruz_lx + 15, 105), (cruz_lx - 25, 115), (cruz_lx - 25, 565), (cruz_lx + 15, 555)], fill=(45, 54, 60), outline=(80, 95, 105), width=2)

    # Bench in foreground
    draw.rectangle([200, 530, 1080, 570], fill=(60, 45, 32), outline=(90, 70, 50), width=3)
    for bx in [240, 620, 1020]:
        draw.rectangle([bx, 570, bx + 24, 660], fill=(25, 25, 25))

    im = draw_vignette(im, 0.4)
    return im

def create_interrogation_bg():
    w, h = 1280, 720
    im = Image.new("RGB", (w, h), (12, 14, 18))
    draw = ImageDraw.Draw(im)
    
    # Dark interrogation chamber with harsh dramatic spotlight
    for y in range(h):
        draw.line([(0, y), (w, y)], fill=(10 + y//60, 12 + y//50, 16 + y//40))

    # Large two-way observation mirror in back
    draw.rectangle([340, 80, 940, 360], fill=(24, 32, 42), outline=(50, 60, 75), width=6)
    # Glass sheen reflection
    draw.polygon([(360, 85), (520, 85), (420, 355), (360, 355)], fill=(45, 60, 80, 60))

    # Dramatic overhead conical spotlight
    spot_cx, spot_cy = 640, 480
    for r in range(380, 0, -5):
        alpha = int(40 * (1.0 - r / 380))
        draw.ellipse([spot_cx - r, spot_cy - r//2, spot_cx + r, spot_cy + r//2], fill=(230, 240, 255, alpha))

    # Metal interrogation table in foreground
    draw.polygon([(260, 500), (1020, 500), (1160, 720), (120, 720)], fill=(28, 32, 38), outline=(60, 68, 80), width=3)
    # Reflection on metal table
    draw.ellipse([480, 520, 800, 580], fill=(50, 58, 70, 90))

    # Suspect metal chair opposite
    draw.rectangle([540, 340, 740, 500], fill=(20, 22, 26), outline=(40, 45, 52), width=2)
    draw.rectangle([560, 240, 720, 360], fill=(22, 25, 30), outline=(45, 50, 60), width=2)

    im = draw_vignette(im, 0.5)
    return im

def create_clueboard_bg():
    w, h = 1280, 720
    im = Image.new("RGB", (w, h), (180, 145, 105))
    draw = ImageDraw.Draw(im)
    
    # Corkboard texture pattern
    for y in range(0, h, 4):
        for x in range(0, w, 4):
            noise = ((x * 17 + y * 31) % 43) - 21
            r = min(255, max(0, 185 + noise))
            g = min(255, max(0, 148 + noise))
            b = min(255, max(0, 108 + noise))
            draw.point((x, y), fill=(r, g, b))
            draw.point((x+1, y), fill=(r, g, b))

    # Ornate dark wood border
    border_w = 32
    draw.rectangle([0, 0, w, border_w], fill=(55, 35, 20))
    draw.rectangle([0, h - border_w, w, h], fill=(55, 35, 20))
    draw.rectangle([0, 0, border_w, h], fill=(55, 35, 20))
    draw.rectangle([w - border_w, 0, w, h], fill=(55, 35, 20))
    draw.rectangle([border_w, border_w, w - border_w, h - border_w], outline=(35, 20, 10), width=4)

    # Sub-board headers (Pinned leads area, Red string grid)
    draw.rectangle([50, 50, w - 50, 100], fill=(45, 30, 20, 200))
    font = get_font(28, bold=True)
    draw.text((80, 60), "CASE FILE EVIDENCE BOARD & DEDUCTION MATRIX", fill=(240, 225, 190), font=font)

    im = draw_vignette(im, 0.3)
    return im

def create_courtroom_bg():
    w, h = 1280, 720
    im = Image.new("RGB", (w, h), (25, 18, 14))
    draw = ImageDraw.Draw(im)
    
    # High Magistrate Courtroom
    for y in range(h):
        draw.line([(0, y), (w, y)], fill=(32 - y//40, 22 - y//50, 16 - y//60))

    # Grand Stained Glass Window in center
    cx, cy, cw, ch = 640, 180, 260, 260
    draw.ellipse([cx - cw//2, cy - ch//2, cx + cw//2, cy + ch//2], fill=(45, 75, 120), outline=(180, 150, 60), width=8)
    # Stained glass sectors
    for angle in range(0, 360, 30):
        rad = math.radians(angle)
        ex = cx + int((cw//2 - 10) * math.cos(rad))
        ey = cy + int((ch//2 - 10) * math.sin(rad))
        draw.line([(cx, cy), (ex, ey)], fill=(200, 170, 70), width=3)
    draw.ellipse([cx - 40, cy - 40, cx + 40, cy + 40], fill=(180, 40, 40), outline=(220, 190, 80), width=4)

    # Raised Magistrate Bench
    draw.rectangle([180, 320, 1100, 540], fill=(42, 28, 18), outline=(85, 60, 35), width=5)
    # Scale of Justice carved insignia
    draw.line([(640, 360), (640, 440)], fill=(200, 165, 70), width=6)
    draw.line([(580, 385), (700, 385)], fill=(200, 165, 70), width=4)
    draw.polygon([(560, 420), (600, 420), (580, 435)], fill=(190, 155, 60))
    draw.polygon([(680, 420), (720, 420), (700, 435)], fill=(190, 155, 60))

    # Witness stand & balustrades in foreground
    draw.rectangle([0, 540, w, h], fill=(30, 20, 14))
    draw.line([(0, 540), (w, 540)], fill=(70, 48, 28), width=6)

    im = draw_vignette(im, 0.45)
    return im

# -------------------------------------------------------------
# ANIME CHARACTER PORTRAIT GENERATOR (600x850 Transparent PNG)
# -------------------------------------------------------------

def create_anime_character(char_type, expression):
    w, h = 600, 850
    im = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    draw = ImageDraw.Draw(im)

    head_cx = 300
    head_cy = 280
    head_r = 95

    # 1. Torso / Clothes based on character type
    if char_type == "cruz": # Security Guard (Navy uniform, badge, epaulets)
        # Shoulders
        draw.polygon([(head_cx - 190, 850), (head_cx - 160, 430), (head_cx - 90, 380),
                      (head_cx + 90, 380), (head_cx + 160, 430), (head_cx + 190, 850)], fill=(28, 42, 65))
        # Uniform collar & tie
        draw.polygon([(head_cx - 50, 380), (head_cx, 440), (head_cx + 50, 380)], fill=(235, 240, 248))
        draw.polygon([(head_cx - 14, 430), (head_cx + 14, 430), (head_cx + 18, 580), (head_cx, 610), (head_cx - 18, 580)], fill=(15, 20, 30))
        # Gold Security Guard Badge
        draw.polygon([(head_cx - 90, 460), (head_cx - 60, 450), (head_cx - 50, 490), (head_cx - 75, 520), (head_cx - 100, 490)], fill=(225, 185, 50), outline=(255, 220, 90), width=2)
        # Gold Epaulets
        draw.rectangle([head_cx - 170, 420, head_cx - 120, 445], fill=(210, 175, 45))
        draw.rectangle([head_cx + 120, 420, head_cx + 170, 445], fill=(210, 175, 45))

    elif char_type == "vance": # Curator (Waistcoat, burgundy cravat, glasses)
        draw.polygon([(head_cx - 180, 850), (head_cx - 150, 430), (head_cx - 85, 380),
                      (head_cx + 85, 380), (head_cx + 150, 430), (head_cx + 180, 850)], fill=(50, 40, 35))
        # Tweed vest
        draw.polygon([(head_cx - 70, 420), (head_cx + 70, 420), (head_cx + 60, 750), (head_cx - 60, 750)], fill=(85, 70, 55))
        # Burgundy silk cravat
        draw.polygon([(head_cx - 30, 390), (head_cx + 30, 390), (head_cx + 25, 500), (head_cx, 530), (head_cx - 25, 500)], fill=(135, 25, 40))
        # Gold pocket watch chain
        draw.arc([head_cx - 50, 520, head_cx + 10, 580], 0, 180, fill=(220, 190, 70), width=3)

    elif char_type == "elena": # Restorer (Apron over blouse)
        draw.polygon([(head_cx - 170, 850), (head_cx - 140, 430), (head_cx - 80, 380),
                      (head_cx + 80, 380), (head_cx + 140, 430), (head_cx + 170, 850)], fill=(225, 220, 210))
        # Brown leather artisan apron
        draw.polygon([(head_cx - 65, 440), (head_cx + 65, 440), (head_cx + 80, 850), (head_cx - 80, 850)], fill=(110, 75, 50))
        # Apron straps
        draw.line([(head_cx - 50, 380), (head_cx - 50, 450)], fill=(75, 50, 35), width=8)
        draw.line([(head_cx + 50, 380), (head_cx + 50, 450)], fill=(75, 50, 35), width=8)

    elif char_type == "sterling": # Lady Patron (Emerald gown & pearls)
        draw.polygon([(head_cx - 180, 850), (head_cx - 145, 440), (head_cx - 75, 390),
                      (head_cx + 75, 390), (head_cx + 145, 440), (head_cx + 180, 850)], fill=(15, 65, 45))
        # Lace neckline
        draw.arc([head_cx - 60, 370, head_cx + 60, 450], 0, 180, fill=(240, 240, 235), width=6)
        # Pearl necklace
        for p in range(9):
            px = head_cx - 48 + p * 12
            py = 430 + int(math.sin(p / 8.0 * math.pi) * 16)
            draw.ellipse([px - 4, py - 4, px + 4, py + 4], fill=(255, 255, 250), outline=(200, 200, 200))

    elif char_type == "bert": # Janitor (Denim overalls & gray shirt)
        draw.polygon([(head_cx - 190, 850), (head_cx - 160, 430), (head_cx - 85, 380),
                      (head_cx + 85, 380), (head_cx + 160, 430), (head_cx + 190, 850)], fill=(70, 75, 80))
        # Denim bib overalls
        draw.polygon([(head_cx - 70, 450), (head_cx + 70, 450), (head_cx + 80, 850), (head_cx - 80, 850)], fill=(40, 65, 95))
        draw.line([(head_cx - 55, 380), (head_cx - 55, 460)], fill=(40, 65, 95), width=10)
        draw.line([(head_cx + 55, 380), (head_cx + 55, 460)], fill=(40, 65, 95), width=10)

    else: # Detective Sherlock (Charcoal trench coat & scarf)
        draw.polygon([(head_cx - 190, 850), (head_cx - 160, 430), (head_cx - 90, 380),
                      (head_cx + 90, 380), (head_cx + 160, 430), (head_cx + 190, 850)], fill=(38, 40, 45))
        # Upturned collar lapels
        draw.polygon([(head_cx - 85, 380), (head_cx - 110, 340), (head_cx - 40, 420)], fill=(28, 30, 35))
        draw.polygon([(head_cx + 85, 380), (head_cx + 110, 340), (head_cx + 40, 420)], fill=(28, 30, 35))
        # Crimson detective scarf
        draw.polygon([(head_cx - 45, 375), (head_cx + 45, 375), (head_cx + 30, 520), (head_cx, 550), (head_cx - 20, 520)], fill=(160, 30, 40))

    # 2. Neck
    neck_w = 40
    draw.polygon([(head_cx - neck_w, head_cy + 50), (head_cx + neck_w, head_cy + 50),
                  (head_cx + neck_w + 10, head_cy + 130), (head_cx - neck_w - 10, head_cy + 130)], fill=(250, 222, 195))
    # Neck shadow
    draw.polygon([(head_cx - neck_w, head_cy + 70), (head_cx + neck_w, head_cy + 70),
                  (head_cx + neck_w + 5, head_cy + 110), (head_cx - neck_w - 5, head_cy + 110)], fill=(230, 195, 170))

    # 3. Head & Anime Jawline
    skin_color = (255, 230, 205)
    jaw_pts = [
        (head_cx - 85, head_cy - 40),
        (head_cx - 80, head_cy + 30),
        (head_cx - 40, head_cy + 95),
        (head_cx, head_cy + 120), # Chin
        (head_cx + 40, head_cy + 95),
        (head_cx + 80, head_cy + 30),
        (head_cx + 85, head_cy - 40),
    ]
    draw.polygon(jaw_pts, fill=skin_color)
    draw.ellipse([head_cx - head_r, head_cy - head_r, head_cx + head_r, head_cy + 20], fill=skin_color)

    # 4. Hair Back
    hair_colors = {
        "cruz": (35, 38, 45),
        "vance": (180, 185, 195),
        "elena": (145, 75, 40),
        "sterling": (230, 200, 120),
        "bert": (120, 120, 125),
        "detective": (45, 35, 28)
    }
    hc = hair_colors.get(char_type, (40, 40, 40))

    # 5. Eyes & Expression
    eye_y = head_cy + 10
    eye_spacing = 42

    # Eyebrows
    brow_y = eye_y - 25
    if expression == "angry":
        draw.line([(head_cx - eye_spacing - 25, brow_y - 5), (head_cx - eye_spacing + 25, brow_y + 12)], fill=hc, width=4)
        draw.line([(head_cx + eye_spacing - 25, brow_y + 12), (head_cx + eye_spacing + 25, brow_y - 5)], fill=hc, width=4)
    elif expression in ["worried", "nervous"]:
        draw.line([(head_cx - eye_spacing - 25, brow_y + 10), (head_cx - eye_spacing + 25, brow_y - 4)], fill=hc, width=4)
        draw.line([(head_cx + eye_spacing - 25, brow_y - 4), (head_cx + eye_spacing + 25, brow_y + 10)], fill=hc, width=4)
    elif expression == "surprised":
        draw.arc([head_cx - eye_spacing - 25, brow_y - 15, head_cx - eye_spacing + 25, brow_y + 15], 180, 360, fill=hc, width=4)
        draw.arc([head_cx + eye_spacing - 25, brow_y - 15, head_cx + eye_spacing + 25, brow_y + 15], 180, 360, fill=hc, width=4)
    else: # neutral / smug
        draw.arc([head_cx - eye_spacing - 25, brow_y - 8, head_cx - eye_spacing + 25, brow_y + 8], 190, 350, fill=hc, width=4)
        draw.arc([head_cx + eye_spacing - 25, brow_y - 8, head_cx + eye_spacing + 25, brow_y + 8], 190, 350, fill=hc, width=4)

    # Eyes (Anime style)
    for side in [-1, 1]:
        ex = head_cx + side * eye_spacing
        if expression == "broken":
            # Hollow darkened eyes
            draw.arc([ex - 22, eye_y - 8, ex + 22, eye_y + 12], 180, 360, fill=(30, 30, 35), width=4)
            draw.ellipse([ex - 12, eye_y, ex + 12, eye_y + 16], fill=(50, 50, 60))
        elif expression == "nervous":
            # Wide pupils darting
            draw.arc([ex - 24, eye_y - 14, ex + 24, eye_y + 16], 180, 360, fill=(20, 20, 25), width=4)
            draw.ellipse([ex - 16, eye_y - 6, ex + 10, eye_y + 18], fill=(35, 55, 80)) # darted pupil
            draw.ellipse([ex - 8, eye_y - 2, ex - 2, eye_y + 4], fill=(255, 255, 255))
        elif expression == "surprised":
            draw.arc([ex - 24, eye_y - 20, ex + 24, eye_y + 20], 180, 360, fill=(20, 20, 25), width=4)
            draw.ellipse([ex - 15, eye_y - 10, ex + 15, eye_y + 16], fill=(45, 65, 95))
            draw.ellipse([ex - 6, eye_y - 4, ex + 4, eye_y + 6], fill=(255, 255, 255))
        else:
            # Classic expressive anime eye
            draw.arc([ex - 24, eye_y - 12, ex + 24, eye_y + 14], 180, 360, fill=(25, 25, 30), width=4)
            # Iris
            iris_color = (40, 75, 120) if char_type != "elena" else (40, 125, 65)
            draw.ellipse([ex - 15, eye_y - 6, ex + 15, eye_y + 18], fill=iris_color)
            draw.ellipse([ex - 7, eye_y - 2, ex + 7, eye_y + 12], fill=(15, 25, 40)) # pupil
            draw.ellipse([ex - 9, eye_y - 4, ex - 1, eye_y + 4], fill=(255, 255, 255)) # highlight

    # Glasses for Vance
    if char_type == "vance":
        for side in [-1, 1]:
            ex = head_cx + side * eye_spacing
            draw.rectangle([ex - 26, eye_y - 14, ex + 26, eye_y + 16], outline=(180, 180, 190), width=3)
        draw.line([(head_cx - 16, eye_y), (head_cx + 16, eye_y)], fill=(180, 180, 190), width=3)

    # Nose
    draw.polygon([(head_cx - 2, head_cy + 42), (head_cx + 5, head_cy + 52), (head_cx - 2, head_cy + 54)], fill=(215, 175, 150))

    # Mouth
    mouth_y = head_cy + 80
    if expression == "angry":
        # Grimace teeth
        draw.polygon([(head_cx - 25, mouth_y), (head_cx + 25, mouth_y), (head_cx + 18, mouth_y + 16), (head_cx - 18, mouth_y + 16)], fill=(160, 45, 45), outline=(50, 20, 20))
        draw.line([(head_cx - 15, mouth_y + 7), (head_cx + 15, mouth_y + 7)], fill=(240, 240, 240), width=3)
    elif expression in ["worried", "nervous"]:
        # Wavy uncertain line
        draw.line([(head_cx - 18, mouth_y + 4), (head_cx - 6, mouth_y), (head_cx + 6, mouth_y + 6), (head_cx + 18, mouth_y + 2)], fill=(140, 60, 60), width=3)
    elif expression == "surprised":
        # O-shaped open mouth
        draw.ellipse([head_cx - 12, mouth_y - 5, head_cx + 12, mouth_y + 18], fill=(140, 35, 40), outline=(50, 15, 20), width=2)
    elif expression == "broken":
        # Downward open grimace
        draw.arc([head_cx - 20, mouth_y, head_cx + 20, mouth_y + 25], 0, 180, fill=(120, 40, 40), width=4)
    else: # neutral / slight confident smirk
        draw.line([(head_cx - 18, mouth_y), (head_cx + 8, mouth_y + 2)], fill=(150, 75, 75), width=3)
        draw.line([(head_cx + 8, mouth_y + 2), (head_cx + 18, mouth_y - 2)], fill=(150, 75, 75), width=3)

    # Sweat bead for nervous/worried
    if expression in ["nervous", "worried"]:
        draw.polygon([(head_cx + 70, head_cy - 10), (head_cx + 65, head_cy + 15), (head_cx + 75, head_cy + 15)], fill=(160, 215, 255))
        draw.ellipse([head_cx + 65, head_cy + 10, head_cx + 75, head_cy + 22], fill=(160, 215, 255))

    # 6. Hair Front (Bangs & style)
    if char_type == "cruz": # Spiky guard hair
        bangs = [
            [(head_cx - 95, head_cy - 40), (head_cx - 50, head_cy - 100), (head_cx - 40, head_cy - 20)],
            [(head_cx - 45, head_cy - 20), (head_cx - 10, head_cy - 115), (head_cx + 15, head_cy - 15)],
            [(head_cx + 10, head_cy - 15), (head_cx + 50, head_cy - 105), (head_cx + 65, head_cy - 10)],
            [(head_cx + 60, head_cy - 10), (head_cx + 95, head_cy - 70), (head_cx + 95, head_cy - 20)],
        ]
        for b in bangs:
            draw.polygon(b, fill=hc)
        # Cap top
        draw.arc([head_cx - 100, head_cy - 120, head_cx + 100, head_cy - 20], 180, 360, fill=hc, width=20)

    elif char_type == "vance": # Elegant swept silver hair
        draw.chord([head_cx - 105, head_cy - 130, head_cx + 105, head_cy - 20], 180, 360, fill=hc)
        draw.polygon([(head_cx - 90, head_cy - 30), (head_cx - 30, head_cy - 10), (head_cx - 50, head_cy - 60)], fill=hc)
        draw.polygon([(head_cx + 20, head_cy - 20), (head_cx + 85, head_cy - 35), (head_cx + 50, head_cy - 70)], fill=hc)

    elif char_type == "elena": # Auburn bangs & side ponytail
        draw.chord([head_cx - 100, head_cy - 130, head_cx + 100, head_cy - 10], 180, 360, fill=hc)
        draw.polygon([(head_cx - 80, head_cy - 10), (head_cx - 30, head_cy + 25), (head_cx - 10, head_cy - 20)], fill=hc)
        draw.polygon([(head_cx, head_cy - 20), (head_cx + 40, head_cy + 20), (head_cx + 80, head_cy - 10)], fill=hc)
        # Side ponytail
        draw.polygon([(head_cx + 80, head_cy - 40), (head_cx + 140, head_cy + 60), (head_cx + 110, head_cy + 120), (head_cx + 70, head_cy + 20)], fill=hc)

    elif char_type == "sterling": # High Victorian updo with feather
        draw.chord([head_cx - 110, head_cy - 170, head_cx + 110, head_cy - 30], 180, 360, fill=hc)
        draw.ellipse([head_cx - 60, head_cy - 190, head_cx + 60, head_cy - 100], fill=hc)
        # Fascinator feather
        draw.polygon([(head_cx + 30, head_cy - 150), (head_cx + 90, head_cy - 260), (head_cx + 50, head_cy - 170)], fill=(30, 95, 70))

    elif char_type == "bert": # Janitor flat cap
        draw.ellipse([head_cx - 110, head_cy - 120, head_cx + 110, head_cy - 40], fill=(85, 75, 65))
        draw.chord([head_cx - 95, head_cy - 150, head_cx + 95, head_cy - 60], 180, 360, fill=(70, 60, 50))
        # Gray mustache
        draw.polygon([(head_cx - 35, head_cy + 68), (head_cx, head_cy + 62), (head_cx + 35, head_cy + 68),
                      (head_cx + 25, head_cy + 82), (head_cx, head_cy + 75), (head_cx - 25, head_cy + 82)], fill=(160, 160, 165))

    else: # Detective Fedora
        # Fedora brim
        draw.ellipse([head_cx - 145, head_cy - 95, head_cx + 145, head_cy - 45], fill=(50, 42, 38), outline=(30, 25, 22), width=3)
        # Fedora crown
        draw.polygon([(head_cx - 85, head_cy - 80), (head_cx - 70, head_cy - 190), (head_cx + 70, head_cy - 190), (head_cx + 85, head_cy - 80)], fill=(55, 46, 42))
        # Ribbon band
        draw.rectangle([head_cx - 83, head_cy - 110, head_cx + 83, head_cy - 85], fill=(140, 25, 35))

    return im

# -------------------------------------------------------------
# UI ASSET GENERATORS
# -------------------------------------------------------------

def create_dialogue_box():
    w, h = 1200, 200
    im = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    draw = ImageDraw.Draw(im)

    # Semi-transparent dark luxury navy background
    draw.rounded_rectangle([10, 10, w - 10, h - 10], radius=16, fill=(14, 18, 28, 230), outline=(215, 175, 75, 255), width=3)
    # Inner border
    draw.rounded_rectangle([16, 16, w - 16, h - 16], radius=12, outline=(100, 80, 35, 160), width=1)

    # Gold corner filigree flourishes
    corners = [(25, 25), (w - 25, 25), (25, h - 25), (w - 25, h - 25)]
    for cx, cy in corners:
        draw.ellipse([cx - 5, cy - 5, cx + 5, cy + 5], fill=(240, 200, 95))

    return im

def create_nameplate():
    w, h = 320, 52
    im = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    draw = ImageDraw.Draw(im)

    # Embossed gold nameplate banner
    draw.rounded_rectangle([4, 4, w - 4, h - 4], radius=8, fill=(28, 36, 52, 245), outline=(225, 185, 75), width=2)
    draw.line([(10, h - 8), (w - 10, h - 8)], fill=(160, 130, 50), width=2)
    return im

def create_advance_marker():
    w, h = 32, 32
    im = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    draw = ImageDraw.Draw(im)
    # Glowing pulsing gold arrow
    draw.polygon([(6, 8), (26, 8), (16, 24)], fill=(255, 215, 80), outline=(255, 250, 180), width=2)
    return im

def create_hotspot_icon():
    w, h = 64, 64
    im = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    draw = ImageDraw.Draw(im)
    # Outer pulsing forensic reticle
    draw.ellipse([8, 8, 56, 56], outline=(255, 215, 75, 220), width=3)
    # Crosshairs
    draw.line([(32, 2), (32, 16)], fill=(255, 225, 90), width=3)
    draw.line([(32, 48), (32, 62)], fill=(255, 225, 90), width=3)
    draw.line([(2, 32), (16, 32)], fill=(255, 225, 90), width=3)
    draw.line([(48, 32), (62, 32)], fill=(255, 225, 90), width=3)
    # Center magnifying glass
    draw.ellipse([20, 20, 44, 44], fill=(40, 140, 240, 100), outline=(255, 255, 255), width=2)
    return im

def create_objection_banner():
    w, h = 800, 180
    im = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    draw = ImageDraw.Draw(im)

    # Dramatic explosive crimson burst banner
    draw.polygon([(20, 90), (140, 20), (660, 20), (780, 90), (660, 160), (140, 160)], fill=(185, 25, 35, 245), outline=(255, 220, 90), width=4)
    draw.polygon([(40, 90), (150, 32), (650, 32), (760, 90), (650, 148), (150, 148)], outline=(255, 180, 50, 180), width=2)

    font = get_font(42, bold=True)
    text = "CONTRADICTION EXPOSED!"
    # Text shadow
    draw.text((w//2 - 278, 62), text, fill=(50, 10, 15), font=font)
    draw.text((w//2 - 280, 60), text, fill=(255, 245, 210), font=font)
    return im

def main():
    print("Generating Visual Novel backgrounds...")
    bgs = {
        "main_menu.png": create_main_menu_bg(),
        "briefing_room.png": create_briefing_room_bg(),
        "scene_vault.png": create_scene_vault_bg(),
        "scene_substation.png": create_scene_substation_bg(),
        "scene_locker.png": create_scene_locker_bg(),
        "interrogation_room.png": create_interrogation_bg(),
        "clueboard_bg.png": create_clueboard_bg(),
        "courtroom.png": create_courtroom_bg()
    }
    for name, img in bgs.items():
        path = os.path.join(BG_DIR, name)
        img.save(path, "PNG")
        print(f"  [+] Saved {path}")

    print("Generating Visual Novel character portraits...")
    characters = [
        ("cruz", "neutral"), ("cruz", "nervous"), ("cruz", "angry"), ("cruz", "broken"),
        ("vance", "neutral"), ("vance", "worried"), ("vance", "surprised"),
        ("elena", "neutral"), ("elena", "thoughtful"), ("elena", "annoyed"),
        ("sterling", "neutral"), ("sterling", "shocked"),
        ("bert", "neutral"), ("bert", "earnest"),
        ("detective", "neutral"), ("detective", "pointing"), ("detective", "thinking")
    ]
    for char_type, expr in characters:
        fname = f"{char_type}_{expr}.png"
        img = create_anime_character(char_type, expr)
        # Save in characters and portraits
        p1 = os.path.join(CHAR_DIR, fname)
        p2 = os.path.join(PORTRAIT_DIR, fname)
        img.save(p1, "PNG")
        img.save(p2, "PNG")
        print(f"  [+] Saved {fname}")

    print("Generating UI elements...")
    uis = {
        "dialogue_box.png": create_dialogue_box(),
        "nameplate.png": create_nameplate(),
        "advance_marker.png": create_advance_marker(),
        "hotspot_icon.png": create_hotspot_icon(),
        "objection_banner.png": create_objection_banner()
    }
    for name, img in uis.items():
        path = os.path.join(UI_DIR, name)
        img.save(path, "PNG")
        print(f"  [+] Saved {path}")

    print("\nAll assets successfully generated!")

if __name__ == "__main__":
    main()
