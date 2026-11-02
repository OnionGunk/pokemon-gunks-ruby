# Pokémon Gunk's Ruby Version

A quality-of-life hack of Pokémon Ruby, built on [pret's Ruby/Sapphire decompilation](https://github.com/pret/pokeruby).
It keeps the game's difficulty and story as they are and takes the grind and friction out, for long playthroughs.

**Work in progress.** No release yet: releases wait until the hack is playable start to finish.

## Features

- **DexNav.** A key item lists the grass Pokémon of the area you're in: seen ones by name (marked NEW until
  caught), unseen ones as ?????. Pick one and it hides in a rustling patch of tall grass nearby; press **R** to
  search for it again. Search levels and chains raise the odds of good IVs and shinies, and every Pokémon is a
  legal wild roll.
- **Pokémon follow you.** The first Pokémon in your party walks behind you, HeartGold style (all 386, shinies
  included). Talk to it to hear its cry.
- **16 PC boxes** (14 in the original game): a living dex with room to spare.
- **Faster play:** instant text, a faster EXP bar, auto-run (hold B to walk), eggs hatch twice as fast.
- **Items:** reusable TMs, two registered items (Select and L), Start sorts the bag, a prompt to use another
  Repel when one runs out, and an Endless Repel toggle.
- **Pokémon:** IV and EV view in the summary (A on the skills page), Exp. All, vitamins up to 252 EVs, and trade
  evolutions without trading. HM moves work without teaching the move (badges still needed), and every
  Pokémon Center PC has a free Move Tutor that remembers and forgets moves.
- **Clock:** if the cartridge's clock battery has run dry, the clock runs from your play time instead of
  stopping, so berries keep growing and the tides keep turning. Saving never depends on the clock.

Mom gives you the Endless Repel, Exp. All and DexNav along with the Running Shoes.

Save safety comes first: every build passes an emulator test suite that covers save cycles, power cycles, a
power cut at every point during a save, all 480 PC slots full, damaged-save recovery and a dead clock battery.

The full list, with what was ported, adapted or skipped and why, is in [`docs/GUNK_CHANGES.md`](docs/GUNK_CHANGES.md).

## Play it

Releases ship a **BPS patch**, not a ROM. You need your own dump of **Pokémon Ruby (USA) Rev 0** (v1.0):

```
sha1: f28b6ffc97847e94a6c21a63cacf633ee5c8df1e
```

1. Download the `.bps` file from [Releases](../../releases).
2. Apply it to your ROM with [Rom Patcher JS](https://www.marcrobledo.com/RomPatcher.js/) (in the browser),
   Floating IPS, or `py tools/patch/bps.py apply <ruby.gba> <patch.bps> <out.gba>`.
3. Play the result in any GBA emulator or flashcart. The patcher checks your ROM first and refuses the wrong one.

Use an emulator or cart with 128 KiB flash saves and the real-time clock (any that runs retail Ruby).
Saves carry over between releases unless the release notes say otherwise. Back up your save before updating.
Saves from the original Ruby don't load: the extra PC boxes moved the save layout, so start a new game.

## Build it

Follow pret's [`INSTALL.md`](INSTALL.md), then run `make ruby`. A clean build reproduces the release ROM, and
`tools/patch/bps.py make` makes the patch from it. On Windows/MSYS2, see [`docs/ENGINE.md`](docs/ENGINE.md) for
the toolchain notes.

## Credits

- [pret](https://github.com/pret/pokeruby) for the Ruby/Sapphire decompilation this hack is built on, and
  [agbcc](https://github.com/pret/agbcc).
- [pokeemerald-expansion](https://github.com/rh-hideout/pokeemerald-expansion) (RHH) and its contributors: the
  following-Pokémon sprites (`graphics/gunk_followers/`) and the follow logic come from its overworld followers, and
  its DexNav (by ghoulslash) and quality-of-life options were the reference for several other changes.
- The DexNav idea comes from Pokémon Omega Ruby and Alpha Sapphire and fan hacks such as Pokémon Unbound.
- Pokémon Gunk's Platinum, HeartGold and Black: the sister hacks this change list comes from.

Pokémon is © Nintendo, Creatures Inc. and GAME FREAK inc. This is a non-commercial fan project and contains
no ROM data; you need your own legally dumped copy of the game.
