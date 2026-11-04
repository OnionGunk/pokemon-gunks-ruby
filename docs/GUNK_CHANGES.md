# Pokémon Gunk's Ruby Version: change list

Every change in Pokémon Gunk's Platinum Version, with a verdict for Ruby:

- **port**: same feature, rewritten against pokeruby's code.
- **adapt**: same goal, but Ruby needs a different shape.
- **skip**: Ruby already has it, or the thing it changes doesn't exist in Ruby.

Rules carried over from the other Gunk's hacks: no difficulty changes, story kept, save safety first. New script
commands, specials, items, vars and text are appended or use slots vanilla leaves unused; nothing is renumbered.

Status: ✅ done and checked in the emulator, 🟡 built and passes the save suite but the feature itself not yet
exercised in the emulator, ⏳ planned.

## Identity

| # | Change | Verdict | Ruby plan | Status |
|---|---|---|---|---|
| 1 | ROM header title `GUNK RU`; game code stays `AXVE` so emulators pick flash 128K + RTC | port | `config.mk` `TITLE`. | ✅ |
| 2 | Title-screen logo "Gunk's Ruby Version" | adapt | Needs art: the user's own (as for HeartGold) or new lettering. Open question. | ⏳ |

## Save and storage

| # | Change | Verdict | Ruby plan | Status |
|---|---|---|---|---|
| 3 | More PC boxes | adapt | **16 boxes** (14 in vanilla; 16 is the flash ceiling): 15 sectors per save slot, the Hall of Fame moved to sectors 30-31, `TOTAL_BOXES_COUNT` / `PC_PARTY_BOX` instead of the PC's literal 14s (box count and party pseudo-box). See `PC_BOXES_PLAN.md`. Old saves don't load. The PokéNav's condition and ribbon lists still cover boxes 1-14 only (its list buffer has room for 420 + 6 and other code depends on the layout after it). | ✅ |
| 4 | DexNav search levels saved | adapt | 387 bytes appended to `SaveBlock2` (its sector has ~1.3 KB spare). | ✅ |
| 5 | Dry RTC battery: saves keep working, and berries/tides keep moving | **new** | With the RTC reporting a power failure, vanilla freezes the clock at 2000-01-01 00:00. `RtcGetInfo` now runs the clock from the saved play time instead, and `DoTimeBasedEvents` never lets it run behind the last berry update (a battery that dies after the clock ran on a working RTC would otherwise put it years back; fix by Gunk's Sapphire). The battery warning on the title menu stays. | ✅ |

## DexNav

| # | Change | Verdict | Ruby plan | Status |
|---|---|---|---|---|
| 6 | DexNav key item: area species, search, chaining, search levels, legal encounters | adapt | **DEXNAV** key item (item 54, unused in vanilla), registerable. Its list shows the area's grass Pokémon (name if seen, `NEW` if not caught, ????? if unseen). A pick hides it in tall grass 2-4 tiles away; the patch rustles (the tall-grass animation) every 40 frames; stepping in starts the battle. **R** searches again. Search level +1 per DexNav battle (saved per species); the chain counts DexNav battles won or caught in a row and resets on running. IV rolls `1 + level/2 + 2×chain` (max 20), shiny rolls `1 + level/10 + chain/5` (max 10); each roll is a whole `CreateMonWithNature` wild Pokémon, so the kept one is legal. Grass only (as in Gunk's HeartGold). Shared fixes from Gunk's Sapphire: the level comes from one of the species' slots (518d4c1c6: levels between slots were illegal); a search lasts 600 field frames and then the Pokémon "got away" (chain reset), the patch shakes every 30 frames, any other wild battle keeps the chain only for the chained species (84bab5f3a); each roll makes only CreateMonWithNature's RNG calls and only the winner is built (3ce6c6480: the step into the patch took up to 45 frames at 20 rolls). | ✅ |
| 7 | DexNav gift | adapt | Mom gives it with the Running Shoes (with the Endless Repel and Exp. All). | ✅ |

## Followers

| # | Change | Verdict | Ruby plan | Status |
|---|---|---|---|---|
| 8 | Pokémon follow the player | port | The party's first Pokémon (not an egg or fainted) walks one tile behind, HeartGold style. Sprites for all 386 species (normal and shiny) come from pokeemerald-expansion (`tools/gunk/make_followers.py` imports them); the follow logic is adapted from its `MovementType_FollowPlayer` as `MovementType_GunkFollow`. One graphics id (218) whose graphics info lives in RAM; palette in sprite slot 10. It stays in its ball while surfing, diving or biking, and on maps that show another slot-10 object (the rival, Kecleon, the S.S. Tidal...). It never blocks anyone (trainers see past it), isn't saved with the map's objects, and talking to it plays its cry. **Talk (v1.1, from Gunk's Sapphire d9e46c15f):** A on it plays its cry and gives a line by friendship (under 50, 50–99, 100–199, 200–254, 255), or a tired line under a quarter of its HP; lines from FireRed v1.2. | ✅ |

## Quality of life

| # | Change | Verdict | Ruby plan | Status |
|---|---|---|---|---|
| 9 | INSTANT text speed; new games on FAST | port | `optionsTextSpeed` is a 3-bit field, so value 3 fits. The Options row keeps three places: SLOW/MID show SLOW MID FAST, FAST/INSTANT show MID FAST INSTANT. INSTANT prints up to the next button wait, pause or sound in one frame; link battles keep their fixed speed. | ✅ |
| 10 | EXP bar fills twice as fast | port | Two pixels a frame: `MoveBattleBar`'s step ×2, and in `sub_8045F58`'s fixed-point path (used while a level spans fewer than 64 EXP, i.e. the early levels) the EXP bar's per-frame step ×2. Measured (`exp_bar`): 0.80 frames per pixel, vanilla 1.42. | ✅ |
| 11 | HP bars 1 px/frame | skip | Ruby's HP bars already move at a fixed pixel rate. | — |
| 12 | Run by default with the Running Shoes, hold B to walk | port | `PlayerNotOnBikeMoving` inverts the B check. | ✅ |
| 13 | Repel prompt | port | `S_RepelWoreOff` asks "Use another X?" when one of the kind last used (`VAR_GUNK_LAST_REPEL` 0x40F0, unused in vanilla) is in the bag. Specials `GunkCheckLastRepel` / `GunkUseLastRepel`. | ✅ |
| 14 | Endless Repel key item | adapt | Item 52 (unused), from Mom with the Running Shoes. USE switches it (bit 0 of `VAR_GUNK_SETTINGS` 0x40F1); on, the Repel level check applies with no Repel counting down. | ✅ |
| 15 | Reusable TMs | port | `Task_TeamMonTMMove2` no longer removes the TM. | ✅ |
| 16 | HM field moves without teaching | port | A Pokémon that could learn an HM in the bag can use its field move: `checkpartymove` (Cut, Strength, Rock Smash, Waterfall, Dive, Flash, Secret Power scripts), the Surf prompt, and the party menu (up to 4 field moves). Badges are still required. | ✅ |
| 17 | Move Tutor in every Pokémon Center | adapt | Ruby's Centers are all different maps, so it's in the **PC**: MOVE TUTOR → REMEMBER (Fallarbor's relearner, no Heart Scale) / FORGET (Lilycove's deleter). Works from any PC. | ✅ |
| 18 | IV/EV view in the summary | adapt | **A** on the POKéMON SKILLS page cycles stats → IVs → EVs (HP shows `IV 31` / `EV 252`). | ✅ |
| 19 | Exp. All key item | adapt | Item 53 (unused), from Mom. USE switches it (bit 1 of `VAR_GUNK_SETTINGS`). On: party Pokémon that didn't battle get half the base EXP and the EVs (Exp. Share holders get their usual share instead). | ✅ |
| 20 | Vitamins up to 252 EVs | port | `EV_ITEM_LIMIT` 252 in `pokemon_item_effect.c`; the 510 total still applies. | ✅ |
| 21 | Bag sorting | port | **Start** in the bag sorts the open pocket by item number (Berries and TMs are kept sorted by the game). | ✅ |
| 22 | Two registered items | adapt | The GBA has no Y: Select keeps the newest, **L** uses the previous one (`VAR_GUNK_REGISTERED_2` 0x40F2). Not in the L=A button mode. Only the Select item shows the bag's SELECT icon. | ✅ |
| 23 | Faster egg hatching | port | Two egg cycles per 256 steps. | ✅ |
| 24 | Trade evolutions without trading | port | Kadabra 36, Machoke / Graveler / Haunter 37 by level (trading still works); King's Rock, Metal Coat, Dragon Scale, Up-Grade, DeepSeaTooth and DeepSeaScale are usable like evolution stones. An Everstone blocks the level-up ones (Ruby's level-up check already honours it). | ✅ |
| 25 | Honey trees, Poké Radar info | skip | Not in Ruby. | — |
| 25b | Quick Run and Quick Ball in battle (Platinum b156ee5ca) | adapt | Gunk's Sapphire's implementation (8bc3314b6, shared so both games run the same code): **R** on the action menu runs (not in link battles). In wild battles where a ball can be thrown (not trainer, double, link, Safari, Wally's tutorial or the first battle), a bottom-left widget shows the ball, its count (≤ 99) and an L hint; tap **L** to throw it from the action menu, hold L + Left/Right to cycle the kinds in the bag; it starts on the ball thrown last (`VAR_GUNK_LAST_BALL` 0x40F3, a ball thrown from the bag counts too). | ✅ |

## Test infrastructure (not in the shipped game)

| # | Change | Status |
|---|---|---|
| 26 | Own BizHawk copy (`C:\Projects\ruby-tools\bizhawk\EmuHawk-ruby.exe`), PID-only stop, minimized, windows on monitor 2 | ✅ |
| 27 | Save suite (`ruby-tools\emu\suite\suite.py`): save cycles, crash after save, torn save every 3 frames, damaged-slot recovery, dead RTC | ✅ |

## Test results (2026-11-02, build f00d71bea and later)

BizHawk (mGBA core) in `C:\Projects\ruby-tools`, emulator minimized. Start states come from a real new game (title,
Birch's speech, naming, first player control in the truck); feature states are real saves edited in RAM (party, bag,
flags, location), saved through the menu and loaded with CONTINUE.

**Save suite** (`suite.py`): save_cycles, crash_after_save, torn_save, backup_recovery, rtc_dead, full_boxes. See
the results table below.

**Feature checks** (`play.py`), all pass on Gunk's Ruby:

| Check | Result |
|---|---|
| follower | Mudkip comes out on the tile the player left and trails one tile behind, with its own palette |
| instant_text | INSTANT shows the whole line 3 frames after A; FAST (control) is still printing |
| options_instant | Right from FAST selects INSTANT in Options, saved on leaving |
| autorun | 48 frames of Right: 5 tiles running, 3 with B held |
| dexnav | list opens on Select, a search hides the species 2-4 tiles away, stepping in starts that battle |
| repel_prompt | "Use another REPEL?" YES: one fewer Repel, 100 steps |
| endless_repel | 80 grass steps with no battle; off (control) a battle |
| vitamin | Protein: Attack EVs 100 -> 110 |
| tm_reusable | TM03 taught to Mudkip, still in the bag |
| trade_item | Metal Coat used on Onix: Steelix |
| kadabra | Rare Candy to 36: Alakazam |
| bag_sort | Start sorts the Items pocket by item number |
| register_l | L uses the previous registered item, Select the newest |
| exp_all | Zigzagoon out of the battle gains 10 EXP with it on, 0 off |
| hm_menu | SURF in Mudkip's party menu with HM03 in the bag, not known |
| summary_ivs | A on the skills page: IVs, EVs, stats |
| move_tutor | Center PC -> MOVE TUTOR -> FORGET removes Water Gun, no Heart Scale |
| pc_boxes | MOVE POKéMON: BOX1 -> BOX16 -> BOX15 -> BOX16 -> BOX1 |
| eggs | egg cycles 10 -> 8 at the 256th step |
| mom_gift | Littleroot with VAR_LITTLEROOT_STATE 3: Mom's scene gives the Running Shoes, Endless Repel, Exp. All and DexNav; state 4 as in vanilla |
| exp_bar | Mudkip Lv5 wins a wild battle: the EXP bar animation takes 0.80 frames per pixel (vanilla 1.42) |
| rtc_dies | battery dies after ~24 years on a working clock: the clock carries on from the last berry update, then 30 h of play move it and the berry update 30 h |
| dexnav_rules | no input: the search ends at 600 frames with "got away", chain 5 -> 0; the shake timer wraps every 30 frames; a wild battle with the chained species keeps chain 5, another species ends it |
| quick_run | R at the action menu of a wild battle runs (outcome RAN) |
| quick_ball | L at the action menu throws the widget's ball (a Great Ball: one fewer in the bag) |
| qb_widget | widget shown in a wild battle, hidden in a trainer battle (Route 102 youngster) |
| rtc_playtime | RTC reporting a power failure: 4 hours of play move the clock 4 hours (vanilla control: frozen) |

**PKHeX.Core legality** (`legaldump.py`, PKHeX.Core 26.10.18 in `C:\Projects\pkhex-legality-ru`): 49 Pokémon dumped from RAM on Route 101 — 12 wild, 6 caught in a Poké Ball, 22 DexNav (chain 0 / search level 0, chain 10 / level 50, chain 20 / level 255), 4 DexNav and 5 wild with the RTC reporting a dead battery. **49 valid, 0 invalid**: wild Method 1/2/4 as in retail, every DexNav one Method 1, levels from the species' slots, met Route 101, OT and ball correct. A dead RTC changes nothing. No shiny came up in that batch, so a second one was seeded (`legaldump.py --shiny`): each encounter replayed from the same state and RNG seed with a trainer ID the game itself can generate (`InitPlayerTrainerId`: SID, then TID, from consecutive LCRNG draws) that makes its PID shiny. **25 shinies, 25 valid**: 6 wild, 4 caught, 15 DexNav (chain 0 / level 0, chain 10 / level 50, chain 20 / level 255, and chain 10 / level 50 with a dead RTC).

**Save suite results** (all six rerun on 5dbcd99aa: pass; torn_save 84 distinct images):

| Test | vanilla (= retail) | Gunk's Ruby (16 boxes) |
|---|---|---|
| save_cycles: 3 saves, power cycle, CONTINUE; RAM after loading = cart; position kept | pass | pass |
| crash_after_save: emulator killed after a save | pass | pass |
| torn_save: flash captured every 3 frames through a save and booted | pass: 79 images, previous save until the write completes | pass: 84 images, previous save ("corrupt, previous loaded") until +279f, the new save at +282f |
| backup_recovery: newest slot damaged | pass | pass |
| rtc_dead: RTC power failure, save and continue | pass | pass |
| full_boxes: every PC slot filled, save, reload | pass (420) | pass (480) |

**Full map sweep** (`play.py map_sweep_all`): CONTINUE onto every map with a warp tile (361, minus 24 Secret Base
variants and the 4 link rooms, which aren't reachable by a normal warp), saves edited in Python, soft reset between
maps, A through any entry scene, steps in all four directions. No freeze on Gunk's Ruby except the Mossdeep Game Corner
basement (an unused map), which freezes identically on vanilla. Indoor maps where the spawn tile is the exit door end
on the outside map (expected).

**Story-trigger sweep** (`play.py story_littleroot` on both ROMs, then `story_compare`): every coord trigger in
Littleroot (the only map whose scripts Gunk changes), from the same edited save: all 9 end on the same map and tile
with the same flags and story vars as vanilla; the only difference is Gunk's three key items with the Running Shoes.

**Story run** (`story.py`, real play from the truck, A through scenes, BFS walking; legs adapted from Gunk's
Sapphire): truck, Mom, clock, the rival's house, Birch's rescue and the starter, the Route 103 rival battle, the
Pokédex, Mom's Running Shoes scene with Gunk's gifts, Oldale, Route 102, Petalburg (Dad, Wally's catching tutorial),
Route 104, Petalburg Woods (the Aqua grunt), Rustboro. Ten Rare Candies after the starter (and fifteen before the
Gym) are a test aid so battles aren't lost to luck. The Torchic follower was out through all of it.

**Map sweep** (`play.py map_sweep`): CONTINUE onto 18 maps (towns, routes, caves, the Abandoned Ship, the Sealed
Chamber) with Mudkip out, steps in all directions: no freeze. The follower stays in its ball on 4 of them in this
setup: Littleroot (the moving truck, a slot-10 object, is still there because the edited save skips the intro),
Route 131, Meteor Falls 1F and the Abandoned Ship deck (spawned on or next to water/a warp).

Not yet exercised: a full story playthrough.
