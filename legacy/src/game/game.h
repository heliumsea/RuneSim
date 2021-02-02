#ifndef RUNESIM_GAME_H
#define RUNESIM_GAME_H

#include "entity.h"
#include "event.h"
#include "event_listener.h"
#include "action.h"
#include "game_state.h"
#include "frontier.h"
#include "lor_deck_codes.h"
#include "../cardset/gallery.h"

#define SINGLE_CARD_LIMIT 3
#define REGION_LIMIT 2
#define CHAMPION_LIMIT 6
#define DECK_LIMIT 40
#define HAND_LIMIT 10
#define TABLE_LIMIT 6
#define SPELL_STACK_LIMIT 10
#define MAX_MANA 10
#define MAX_SPELL_MANA 3

extern umap<RSID, Card *> GALLERY;
extern umap<RSID, RSID> CHAMPION_TO_SPELL;

class Player {
public:
  RSID playerId{};
  RSID nexusId;
  RSID opNexusId;
  i8 nexusHealth = 20;
  i8 unitMana = 0;
  i8 spellMana = 0;
  i8 targetCnt = 0;
  i8 deadAllyCnt = 0;
  i8 deadAllyInRoundCnt = 0;
  bool hasToken = false;
  bool attackDone = false;
  bool isPlunderer = false;
  rsvec deck;
  rsvec hand;
  rsvec table;
  rsvec graveyard;
  rsvec tossed;
  Frontier frontier;

  static Result<rsvec> buildDeck(const str & deckCode, RSID pid);
  static Result<sptr<Player>> build(RSID pid, const str & deckCode);
};

class Game final {
private:
  // check game state
  void killEphemeralOnTable(RSID playerId);
  void discardFleetingInHand(RSID playerId);
  bool canChallenge(RSID pid, RSID eid1, RSID eid2);
  void battle(Entity aunit, Entity bunit);
public:
  // Player acting first in odd number round
  RSID firstPlayerId = -1;
  // Player acting first in even number round
  RSID secondPlayerId = -1;
  // Player acting first in this round
  RSID starterInRound = -1;
  GameState state;
  std::function<void(RSID)> afterGame;
  umap<RSID, Entity> ents;
  umap<RSID, EventListener> evlsnr;
  umap<EventType, rsvec> elByType;
  umap<RSID, rsvec> elByEntId;
  umap<RSID, rsvec> elByCardId;
  sptr<Player> players[2];
  vec<Action> spellStack;
  i32 round = 0;
  i32 passCnt = 0;
  RSID winner = -1;

  static Result<Game *> build(const str & deckCode1,
                              const str & deckCode2,
                              RSID firstPlayer,
                              std::function<void(RSID)> afterGame);
  static Result<void *> checkDeck(std::map<RSID, i32> &deck);

  vec<RSID> firstDraw(RSID pid);

  // Game or card action
  void startRound();
  bool drawACard(RSID pid);
  void putSkill(Action &action);
  void resolveSpells();
  void battle();
  void endRound();

  // Player action
  void replaceFirstDraw(RSID pid, vec<RSID> &draw, vec<bool> toRep);
  void putFirstDrawInHandAndShuffleDeck(RSID pid, vec<RSID> &draw);
  bool canPlayUnit(Action &action);
  void playUnit(Action &action);
  bool canPlaySpell(Action &action);
  void playSlowOrFastSpell(Action &action);
  void playBurstSpell(Action &action);
  bool canPutUnitToAttack(Action &action);
  void putUnitToAttack(Action &action);
  bool canPutUnitToBlock(Action &action);
  void putUnitToBlock(Action &action);
  void hitButton(RSID pid);

  bool isEnded() const;
  void end(RSID Winner);

  bool isInHand(RSID playerId, RSID entityId);
  bool isInPlay(RSID entityId);
  bool isStrikableEntity(RSID entityId);
  bool isAlly(RSID playerId, RSID entityId);
  bool isEnemy(RSID playerId, RSID entityId);
  bool isUnitInGameView(RSID entityId);
  bool isFollowerInGameView(RSID entityId);
  bool hasSummonedCard(RSID playerId, RSID cardId);
  bool hasCardInHand(RSID playerId, RSID cardId);
  bool hasCardInDeck(RSID playerId, RSID cardId);
  void moveFirstAppearingCardToTop(RSID playerId, RSID cardId);

  // Return the entityId
  RSID putCardInHand(RSID playerId, RSID cardId);
  RSID summonAUnitOnTable(RSID playerId, RSID cardId);
  RSID summonAUnitInAttack(RSID playerId, RSID cardId);

  void trigger(Event event);

  void printEntity(RSID entityId);
  void printGameView();

  virtual ~Game();
};

#endif //RUNESIM_GAME_H
