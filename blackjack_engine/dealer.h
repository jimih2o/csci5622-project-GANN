#ifndef DEALER_H
#define DEALER_H

#include "blackjackutils.h"
#include "playerbase.h"
#include "dealerplayer.h"

#include <vector>

template<class DECK_CLASS, class PLAYER_CLASS>
class Dealer : public DealerPlayer {
public:
    
    typedef Dealer<DECK_CLASS, PLAYER_CLASS> ThisType;
    
    Dealer() {
        mDeck.shuffle();
    }
    
    enum {
        MAX_SPLIT_COUNT = 1
    };
    
private:
    
    void runPlayer(PlayerBase *player, BjUtils::CardValueEnum dealerCard) {
        using namespace BjUtils;
        
        ActionEnum action = player->getAction(dealerCard);
        bool playerDouble = false;
        
        while(action != ACTION_STAND) {
            
            switch (action) {
                case ACTION_HIT:
                    player->addCard(mDeck.getCard());
                    action = player->getAction(dealerCard);
                    break;
                case ACTION_DOUBLE:
                    player->addCard(mDeck.getCard());
                    playerDouble = true;
                    action = ACTION_STAND;
                    break;
                case ACTION_SPLIT: {
                    CardValueEnum splitCard = player->giveUpSplitCard();
                    player->addCard(mDeck.getCard());
                    runPlayer(player, dealerCard);
                    player->clearCards();
                    player->addCard(splitCard);
                    player->addCard(mDeck.getCard());
                    runPlayer(player, dealerCard);
                    // Split causes this function to be called recursively.
                    // No need to continue.
                    return;
                    break; }
                default:
                    break;
            }
            
            
        }
        
        int playerHandValue = player->getHandValue();
        int dealerHandValue = this->getHandValue();
        
        // Check for player bust
        if (playerHandValue > 21) {
            if (playerDouble) {
                player->pay(PAY_LOSS_DOUBLE);
            } else {
                player->pay(PAY_LOSS);
            }
            return;
        }
        
        // Check for dealer bust
        if (dealerHandValue > 21) {
            if (playerDouble) {
                player->pay(PAY_DOUBLE);
            } else {
                player->pay(PAY_WIN);
            }
            return;
        }
        
        // Check for push
        if (dealerHandValue == playerHandValue) {
            player->pay(PAY_PUSH);
            return;
        }
        
        // Someone won
        if (dealerHandValue > playerHandValue) {
            if (playerDouble) {
                player->pay(PAY_LOSS_DOUBLE);
            } else {
                player->pay(PAY_LOSS);
            }
        } else {
            if (playerDouble) {
                player->pay(PAY_DOUBLE);
            } else {
                player->pay(PAY_WIN);
            }
        }
        
    }
    
public:
    
    void playHand() {
        using namespace BjUtils;
        
#ifdef PLAYER_CREATE_LOG
        clearLog();
#endif
        
        PlayerBase *player = dynamic_cast<PlayerBase*>(&mPlayer);
        // Above shouldn't compile if PLAYER_CLASS is invalid
        if (player == 0) return;
        
        mDeck.signalNewHand();
        clearCards();
        
        // The first one is the 'face up' card, and the 2nd one is the hole card
        BjUtils::CardValueEnum dealerCard = mDeck.getCard();
        addCard(dealerCard);
        addCard(mDeck.getCard());
        
        player->clearCards();
        player->addCard(mDeck.getCard());
        player->addCard(mDeck.getCard());
        
        // Check for blackjacks
        if (player->getHandValue() == 21) {
            if (this->getHandValue() == 21) {
                player->pay(PAY_PUSH);
                return;
            }
            // Player has blackjack and dealer does not
            player->pay(PAY_BLACKJACK);
            return;
        } else if (this->getHandValue() == 21) {
            // Dealer has blackjack and player does not.
            // Blackjack beats 3+ card 21.
            player->pay(PAY_LOSS);
            return;
        }
        
        // Run the dealer to completion
        // For the dealer strategy obviously the dealer card doesn't matter
        ActionEnum action = getAction(CARD_TWO);
        while(action != ACTION_STAND) {
            addCard(mDeck.getCard());
            action = getAction(CARD_TWO);
        }
        
        runPlayer(player, dealerCard);
        
    }
    
#ifdef PLAYER_CREATE_LOG
    std::string getLog() {
        std::ostringstream ss;
        ss << "Dealer: " << mLog.str() << " Player: " << mPlayer.getLog();
        clearLog();
        mPlayer.clearLog();
        return ss.str();
    }
#endif
    
    int getPlayerWinCount() const { return mPlayer.getWinCount(); }
    int getPlayerLossCount() const { return mPlayer.getLossCount(); }
    int getPlayerPushCount() const { return mPlayer.getPushCount(); }
    int getPlayerBlackjackCount() const { return mPlayer.getBlackjackCount(); }
    int getPlayerTotal() const { return mPlayer.getTotal(); }
    
    bool operator>(const ThisType &other) {
        return getPlayerTotal() > other.getPlayerTotal();
    }
    
    bool operator<(const ThisType &other) {
        return getPlayerTotal() < other.getPlayerTotal();
    }
    
    PLAYER_CLASS &player() {
        return mPlayer;
    }
    
    void reset() {
        mPlayer.reset();
    }
    
private:
    
    DECK_CLASS mDeck;
    PLAYER_CLASS mPlayer;
};

#endif // DEALER_H
