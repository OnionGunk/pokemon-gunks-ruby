#include "global.h"
#include "gunk.h"
#include "event_data.h"
#include "item.h"
#include "battle.h"
#include "constants/battle.h"
#include "pokemon.h"
#include "party_menu.h"
#include "constants/item_effects.h"
#include "constants/moves.h"
#include "constants/species.h"
#include "string_util.h"
#include "constants/items.h"
#include "constants/vars.h"

bool8 GunkSettingOn(u16 bit)
{
    return (VarGet(VAR_GUNK_SETTINGS) & bit) != 0;
}

void GunkToggleSetting(u16 bit)
{
    VarSet(VAR_GUNK_SETTINGS, VarGet(VAR_GUNK_SETTINGS) ^ bit);
}

// Repel prompt (special): TRUE when another Repel of the kind last used is in the bag; its name goes to gStringVar2
u16 GunkCheckLastRepel(void)
{
    u16 item = VarGet(VAR_GUNK_LAST_REPEL);

    if (item != ITEM_REPEL && item != ITEM_SUPER_REPEL && item != ITEM_MAX_REPEL)
        return FALSE;
    if (!CheckBagHasItem(item, 1))
        return FALSE;
    CopyItemName(item, gStringVar2);
    return TRUE;
}

// Repel prompt (special): use one more of the Repel kind last used
void GunkUseLastRepel(void)
{
    u16 item = VarGet(VAR_GUNK_LAST_REPEL);

    RemoveBagItem(item, 1);
    VarSet(VAR_REPEL_STEP_COUNT, ItemId_GetHoldEffectParam(item));
}

// HMs without teaching: a Pokémon that could learn an HM the player owns can use its field move (badges are
// still checked where the game checks them)
static const u16 sGunkHMMoves[][2] =
{
    {MOVE_CUT,        ITEM_HM01_CUT},
    {MOVE_FLY,        ITEM_HM02_FLY},
    {MOVE_SURF,       ITEM_HM03_SURF},
    {MOVE_STRENGTH,   ITEM_HM04_STRENGTH},
    {MOVE_FLASH,      ITEM_HM05_FLASH},
    {MOVE_ROCK_SMASH, ITEM_HM06_ROCK_SMASH},
    {MOVE_WATERFALL,  ITEM_HM07_WATERFALL},
    {MOVE_DIVE,       ITEM_HM08_DIVE},
};

bool8 GunkCanUseFieldMoveUntaught(struct Pokemon *mon, u16 move)
{
    u8 i;

    if (GetMonData(mon, MON_DATA_SPECIES) == SPECIES_NONE || GetMonData(mon, MON_DATA_IS_EGG))
        return FALSE;
    for (i = 0; i < ARRAY_COUNT(sGunkHMMoves); i++)
    {
        if (sGunkHMMoves[i][0] == move)
            return CheckBagHasItem(sGunkHMMoves[i][1], 1) && CanMonLearnTMHM(mon, sGunkHMMoves[i][1] - ITEM_TM01_FOCUS_PUNCH);
    }
    return FALSE;
}

bool8 GunkCanUseFieldMove(struct Pokemon *mon, u16 move)
{
    return pokemon_has_move(mon, move) || GunkCanUseFieldMoveUntaught(mon, move);
}

// Trade evolutions without trading: the trade items work like evolution stones. They sit past the end of
// gItemEffectTable (ITEM_HAS_EFFECT), so they get an evolution-stone effect here.
static const u8 sGunkEvoStoneEffect[6] = {0, 0, 0, 0, ITEM4_EVO_STONE, 0};

const u8 *GunkTradeItemEffect(u16 item)
{
    switch (item)
    {
    case ITEM_KINGS_ROCK:
    case ITEM_METAL_COAT:
    case ITEM_DRAGON_SCALE:
    case ITEM_UP_GRADE:
    case ITEM_DEEP_SEA_TOOTH:
    case ITEM_DEEP_SEA_SCALE:
        return sGunkEvoStoneEffect;
    }
    return NULL;
}

// Quick Run and Quick Ball: in a plain wild battle, R runs and L throws a ball (src/battle_controller_player.c)
bool8 GunkQuickBattle(void)
{
    return (gBattleTypeFlags & BATTLE_TYPE_WILD)
        && !(gBattleTypeFlags & (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_LINK | BATTLE_TYPE_TRAINER | BATTLE_TYPE_FIRST_BATTLE
                               | BATTLE_TYPE_SAFARI | BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_WALLY_TUTORIAL
                               | BATTLE_TYPE_EREADER_TRAINER));
}

// The ball L throws: the one last thrown, else a Poke Ball, else the first ball in the bag (ITEM_NONE: none left)
u16 GunkQuickBallItem(void)
{
    u16 last = VarGet(VAR_GUNK_LAST_BALL);
    u8 i;

    if (last >= FIRST_BALL && last <= LAST_BALL && CheckBagHasItem(last, 1))
        return last;
    if (CheckBagHasItem(ITEM_POKE_BALL, 1))
        return ITEM_POKE_BALL;
    for (i = 0; i < BAG_POKEBALLS_COUNT; i++)
    {
        u16 item = gSaveBlock1.bagPocket_PokeBalls[i].itemId;
        if (item >= FIRST_BALL && item <= LAST_BALL && gSaveBlock1.bagPocket_PokeBalls[i].quantity)
            return item;
    }
    return ITEM_NONE;
}
