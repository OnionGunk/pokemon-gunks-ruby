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
| 5 | Dry RTC battery: saves keep working, and berries/tides keep moving | **new** | With the RTC reporting a power failure, vanilla freezes the clock at 2000-01-01 00:00. `RtcGetInfo` now runs the clock from the saved play time instead. The battery warning on the title menu stays. | ✅ |

## DexNav

| # | Change | Verdict | Ruby plan | Status |
|---|---|---|---|---|
| 6 | DexNav key item: area species, search, chaining, search levels, legal encounters | adapt | **DEXNAV** key item (item 54, unused in vanilla), registerable. Its list shows the area's grass Pokémon (name if seen, `NEW` if not caught, ????? if unseen). A pick hides it in tall grass 2-4 tiles away; the patch rustles (the tall-grass animation) every 40 frames; stepping in starts the battle. **R** searches again. Search level +1 per DexNav battle (saved per species); the chain counts DexNav battles won or caught in a row and resets on running. IV rolls `1 + level/2 + 2×chain` (max 20), shiny rolls `1 + level/10 + chain/5` (max 10); each roll is a whole `CreateMonWithNature` wild Pokémon, so the kept one is legal. Grass only (as in Gunk's HeartGold). | ✅ |
| 7 | DexNav gift | adapt | Mom gives it with the Running Shoes (with the Endless Repel and Exp. All). | 🟡 |

## Followers

| # | Change | Verdict | Ruby plan | Status |
|---|---|---|---|---|
| 8 | Pokémon follow the player | port | The party's first Pokémon (not an egg or fainted) walks one tile behind, HeartGold style. Sprites for all 386 species (normal and shiny) come from pokeemerald-expansion (`tools/gunk/make_followers.py` imports them); the follow logic is adapted from its `MovementType_FollowPlayer` as `MovementType_GunkFollow`. One graphics id (218) whose graphics info lives in RAM; palette in sprite slot 10. It stays in its ball while surfing, diving or biking, and on maps that show another slot-10 object (the rival, Kecleon, the S.S. Tidal...). It never blocks anyone (trainers see past it), isn't saved with the map's objects, and talking to it plays its cry. | ✅ |

## Quality of life

| # | Change | Verdict | Ruby plan | Status |
|---|---|---|---|---|
| 9 | INSTANT text speed; new games on FAST | port | `optionsTextSpeed` is a 3-bit field, so value 3 fits. The Options row keeps three places: SLOW/MID show SLOW MID FAST, FAST/INSTANT show MID FAST INSTANT. INSTANT prints up to the next button wait, pause or sound in one frame; link battles keep their fixed speed. | ✅ |
| 10 | EXP bar fills twice as fast | port | `MoveBattleBar` step ×2. | 🟡 |
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
| rtc_playtime | RTC reporting a power failure: 4 hours of play move the clock 4 hours (vanilla control: frozen) |
