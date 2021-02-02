//
// Created by Hengruo Zhang on 2/7/20.
//

#ifndef RUNESIM_CARD_H
#define RUNESIM_CARD_H

#include "action.h"

#define K_NONE 0x00000000000000
#define K_FEARSOME 0x0000000000000001
#define K_BURST 0x0000000000000002
#define K_OVERWHELM 0x0000000000000004
#define K_LIFESTEAL 0x0000000000000008
#define K_EPHEMERAL 0x0000000000000010
#define K_REGENERATION 0x0000000000000020
#define K_SKILL 0x0000000000000040
#define K_BARRIER 0x0000000000000080
#define K_CANT_BLOCK 0x0000000000000100
#define K_FLEETING 0x0000000000000200
#define K_CHALLENGER 0x0000000000000400
#define K_DOUBLE_ATTACK 0x0000000000000800
#define K_ELUSIVE 0x0000000000001000
#define K_SLOW 0x0000000000002000
#define K_FAST 0x0000000000004000
#define K_QUICK_ATTACK 0x0000000000008000
#define K_IMBUE 0x0000000000010000
#define K_LAST_BREATH 0x0000000000020000
#define K_TOUGH 0x0000000000040000
#define K_TRAP 0x0000000000080000
#define K_DEEP 0x0000000000100000
#define K_SCOUT 0x0000000000200000
#define K_IMMOBILE 0x0000000000400000
#define K_ATTUNE 0x0000000000800000
#define K_VULNERABLE 0x0000000001000000

#define CHECK_K_NONE(X) (X == 0)
#define CHECK_K_FEARSOME(X) ((K_FEARSOME & (X)) != 0)
#define CHECK_K_BURST(X) ((K_BURST & (X)) != 0)
#define CHECK_K_OVERWHELM(X) ((K_OVERWHELM & (X)) != 0)
#define CHECK_K_LIFESTEAL(X) ((K_LIFESTEAL & (X)) != 0)
#define CHECK_K_EPHEMERAL(X) ((K_EPHEMERAL & (X)) != 0)
#define CHECK_K_REGENERATION(X) ((K_REGENERATION & (X)) != 0)
#define CHECK_K_SKILL(X) ((K_SKILL & (X)) != 0)
#define CHECK_K_BARRIER(X) ((K_BARRIER & (X)) != 0)
#define CHECK_K_CANT_BLOCK(X) ((K_CANT_BLOCK & (X)) != 0)
#define CHECK_K_FLEETING(X) ((K_FLEETING & (X)) != 0)
#define CHECK_K_CHALLENGER(X) ((K_CHALLENGER & (X)) != 0)
#define CHECK_K_DOUBLE_ATTACK(X) ((K_DOUBLE_ATTACK & (X)) != 0)
#define CHECK_K_ELUSIVE(X) ((K_ELUSIVE & (X)) != 0)
#define CHECK_K_SLOW(X) ((K_SLOW & (X)) != 0)
#define CHECK_K_FAST(X) ((K_FAST & (X)) != 0)
#define CHECK_K_QUICK_ATTACK(X) ((K_QUICK_ATTACK & (X)) != 0)
#define CHECK_K_IMBUE(X) ((K_IMBUE & (X)) != 0)
#define CHECK_K_LAST_BREATH(X) ((K_LAST_BREATH & (X)) != 0)
#define CHECK_K_TOUGH(X) ((K_TOUGH & (X)) != 0)
#define CHECK_K_TRAP(X) ((K_TRAP & (X)) != 0)
#define CHECK_K_DEEP(X) ((K_DEEP & (X)) != 0)
#define CHECK_K_SCOUT(X) ((K_SCOUT & (X)) != 0)
#define CHECK_K_IMMOBILE(X) ((K_IMMOBILE & (X)) != 0)
#define CHECK_K_ATTUNE(X) ((K_ATTUNE & (X)) != 0)
#define CHECK_K_VULNERABLE(X) ((K_VULNERABLE & (X)) != 0)

enum class CardType {
  TRAP,
  SPELL,
  UNIT,
  ABILITY
};

enum class CardSubType {
  NONE,
  TECH,
  ELITE,
  SPIDER,
  PORO,
  YETI,
  ELNUK,
  SEA_MONSTER,
  TREASURE
};

enum class CardSupType {
  NONE,
  CHAMPION
};

enum class CardRarity {
  NONE,
  COMMON,
  RARE,
  EPIC,
  CHAMPION
};

enum class CardRegion {
  BILGEWATER,
  DEMACIA,
  FRELJORD,
  IONIA,
  NOXUS,
  PILTOVER_N_ZAUN,
  SHADOW_ISLES
};

class Game;
union Action;

extern Game *GAME_PTR;

class Card {
public:
  const RSID id;
  const char *const name;
  const char *const description;
  const char *const levelUpDescription;
  const char *const code;
  const CardRegion region;
  const CardRarity rarity;
  const CardType type;
  const CardSupType supType;
  const CardSubType subType;
  const u64 keywords;
  const u8 cost;
  const u8 attack;
  const u8 health;
  const bool collectible;
  Card(const RSID Id,
       const char *const Name,
       const char *const Description,
       const char *const LevelUpDescription,
       const char *const Code,
       const CardRegion Region,
       const CardRarity Rarity,
       const CardType Type,
       const CardSupType SupType,
       const CardSubType SubType,
       const u64 Keywords,
       const u8 Cost,
       const u8 Attack,
       const u8 Health,
       const bool Collectible)
      : id(Id), name(Name), description(Description), levelUpDescription(LevelUpDescription), code(Code),
        region(Region),
        rarity(Rarity), type(Type), supType(SupType), subType(SubType), keywords(Keywords), cost(Cost), attack(Attack),
        health(Health), collectible(Collectible) {
    return;
  }
  // functions to register event listeners when game starts
  std::function<void(RSID, RSID)> onStartGame = [](RSID pid, RSID eid) {};
  // whether this card can be played
  std::function<bool(Action &)> playable = [](Action &action) -> bool { return true; };
  // whether this spell or skill can be casted
  std::function<bool(Action &)> castable = [](Action &action) -> bool { return true; };
  // process sth. for cards with "to play..."
  // return an action in case that the args in the input action are changed,
  // for example, "Get Excited!" will discard its first arg when played and deal 3 to its second arg when casted.
  std::function<Action(Action &)> toPlay = [](Action &action) -> Action { return action; };
  // reaction when play this card
  std::function<void(Action &)> onPlay = [this](Action &action) {};
  // reaction when cast this spell, skill, or trap
  std::function<void(Action &)> onCast = [](Action &action) {};
  std::function<void(Action &)> onDiscard = [](Action &action) {};
  std::function<void(Action &)> onDie = [](Action &action) {};
  std::function<void(Action &)> onSummon = [](Action &action) {};
  std::function<void(Action &)> onDeclAttack = [](Action &action) {};
  std::function<void(Action &)> onStrike = [](Action &action) {};
  std::function<void(Action &)> onSupport = [](Action &action) {};
};

#endif //RUNESIM_CARD_H
