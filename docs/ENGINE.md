# Engine decision

**Base: [pret/pokeruby](https://github.com/pret/pokeruby)** (forked at `5784633ce`), built with
[pret/agbcc](https://github.com/pret/agbcc) and MSYS2's `arm-none-eabi` binutils.

## Why pokeruby
- It is the full C/asm source of Ruby and Sapphire, and `make ruby COMPARE=1` rebuilds Ruby (USA) Rev 0 byte for byte
  (sha1 `f28b6ffc97847e94a6c21a63cacf633ee5c8df1e`). Every Gunk's change is a source change on top of that.
- The ROM is shiftable: code and data can grow and move freely, so new features don't need free-space hunting
  or hooks.
- It's close to pokeemerald, so pokeemerald and pokeemerald-expansion code (DexNav, following Pokémon, Repel
  prompt, bag sorting…) ports with modest changes. Gunk's Emerald works on pokeemerald in parallel.

## Alternatives considered
- **pokeemerald / pokeemerald-expansion re-skinned as Ruby:** more features ready-made, but it would not be Ruby
  (Battle Frontier, Emerald story and maps). The task is Ruby with its story kept.
- **Binary patching the retail ROM** (as Gunk's Black does): only worth it when no usable decomp exists.

## Targets
- `make ruby` builds `pokeruby.gba` (Gunk's Ruby). Vanilla is the upstream commit; `COMPARE=1` on a vanilla tree
  checks the sha1. Sapphire shares this decomp (`make sapphire`), but Gunk's Sapphire lives in its own repo.
- Rev 0 is the target: it matches the user's dump. The release BPS patches Rev 0 only.

## Toolchain notes (Windows)
- MSYS2 (`MSYSTEM=MSYS`, `bash -l`), libpng built into `/usr/local`, `arm-none-eabi-*` from `/ucrt64/bin`.
- `PKG_CONFIG_LIBDIR=/usr/local/lib/pkgconfig:/usr/lib/pkgconfig`; `TMP=/tmp` (the Windows temp path has an
  apostrophe that the tools mishandle).
