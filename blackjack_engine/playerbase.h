#ifndef PLAYERBASE_H
#define PLAYERBASE_H

#include "blackjackutils.h"

#include <deque>
#include <sstream>

class PlayerBase
{
public:
    
    PlayerBase() {
        reset();
    }
    
public:
    
    enum {
        BET = 10,
        BLACKJACK = 15
    };
    
    void addCard(BjUtils::CardValueEnum c) {
        // In order for all the utilities to work, aces need to be
        // at the end of the hand.
        
        if (c == BjUtils::CARD_ACE) {
            mCards.push_back(c);
        } else {
            mCards.push_front(c);
        }
        
#ifdef PLAYER_CREATE_LOG
        mLog << BjUtils::getCardChar(c);
#endif
    }
    
    void clearCards() {
        mCards.clear();
    }

    // Should return stand for everything at or above 21.
    virtual BjUtils::ActionEnum getAction(BjUtils::CardValueEnum dealerCard) const = 0;
    
    BjUtils::CardValueEnum giveUpSplitCard() {
        // If this is called it should be a valid scenario to split, as
        // determined by the action strategy.
        BjUtils::CardValueEnum card = mCards.at(0);
        mCards.erase(mCards.begin());
        
#ifdef PLAYER_CREATE_LOG
        mLog << " Split\n" << BjUtils::getCardChar(card);
#endif
        
        return card;
    }
    
    bool haveBlackjack() const {
        return BjUtils::haveBlackjack(mCards);
    }
    
    int getHandValue() const {
        return BjUtils::getHandValue(mCards);
    }
    
    void reset() {
        mCards.clear();
        mWinCount = 0;
        mLossCount = 0;
        mPushCount = 0;
        mBlackjackCount = 0;
        mTotal = 0;
    }

    void pay(BjUtils::PayoutEnum p) {
        switch(p) {
            case BjUtils::PAY_LOSS:
                ++mLossCount;
                mTotal -= static_cast<int>(BET);
#ifdef PLAYER_CREATE_LOG
                mLog << " Loss: " << mTotal << "\n";
#endif
                break;
            case BjUtils::PAY_WIN:
                ++mWinCount;
                mTotal += static_cast<int>(BET);
#ifdef PLAYER_CREATE_LOG
                mLog << " Win: " << mTotal << "\n";
#endif
                break;
            case BjUtils::PAY_PUSH:
                ++mPushCount;
#ifdef PLAYER_CREATE_LOG
                mLog << " Push: " << mTotal << "\n";
#endif
                break;
            case BjUtils::PAY_BLACKJACK:
                ++mBlackjackCount;
                mTotal += static_cast<int>(BLACKJACK);
#ifdef PLAYER_CREATE_LOG
                mLog << " Blackjack: " << mTotal << "\n";
#endif
                break;
            case BjUtils::PAY_DOUBLE:
                ++mWinCount;
                mTotal += 2*static_cast<int>(BET);
#ifdef PLAYER_CREATE_LOG
                mLog << " Win Double: " << mTotal << "\n";
#endif
                break;
            case BjUtils::PAY_LOSS_DOUBLE:
                ++mLossCount;
                mTotal -= 2*static_cast<int>(BET);
#ifdef PLAYER_CREATE_LOG
                mLog << " Loss Double: " << mTotal << "\n";
#endif
                break;
            default:
                break;
        }
    }
    
    
    std::string getSplitActionTable() {
        using namespace BjUtils;
        
        std::string ret;
        
        for (int playerCard = CARD_TWO;
             playerCard <= CARD_ACE;
             ++playerCard) {
            mCards.clear();
            mCards.push_back(static_cast<CardValueEnum>(playerCard));
            mCards.push_back(static_cast<CardValueEnum>(playerCard));
            bool skip = false;
            for (int dealerCard = CARD_TWO;
                 dealerCard <= CARD_ACE;
                 ++dealerCard) {
                
                // Skip the redundant 10s
                if (playerCard == CARD_JACK ||
                    playerCard == CARD_QUEEN ||
                    playerCard == CARD_KING) {
                    skip = true;
                    continue;
                }
                
                if (dealerCard == CARD_JACK ||
                    dealerCard == CARD_QUEEN ||
                    dealerCard == CARD_KING) {
                    continue;
                }
                
                ActionEnum act = getAction(static_cast<CardValueEnum>(dealerCard));
                ret.append(getActionChar(act));
            }
            if (!skip) {
                ret.append("\n");
            }
        }
        return ret;
    }
    
    std::string getSoftActionTable() {
        using namespace BjUtils;
        
        std::string ret;
        
        for (int playerCard = CARD_TWO;
             playerCard <= CARD_NINE;
             ++playerCard) {
            mCards.clear();
            mCards.push_back(CARD_ACE);
            mCards.push_back(static_cast<CardValueEnum>(playerCard));
            for (int dealerCard = CARD_TWO;
                 dealerCard <= CARD_ACE;
                 ++dealerCard) {
                
                // Skip the redundant 10s
                if (dealerCard == CARD_JACK ||
                    dealerCard == CARD_QUEEN ||
                    dealerCard == CARD_KING) {
                    continue;
                }
                
                ActionEnum act = getAction(static_cast<CardValueEnum>(dealerCard));
                ret.append(getActionChar(act));
            }
            ret.append("\n");
        }
        return ret;
    }
    
    std::string getHardActionTable() {
        using namespace BjUtils;
        
        std::string ret;
        
        for (int total = 3; total < 21; ++total) {
            int rem = total;
            mCards.clear();
            if (rem % 2 == 1) {
                rem -= 3;
                mCards.push_back(CARD_THREE);
            }
            while(rem != 0) {
                mCards.push_back(CARD_TWO);
                rem -= 2;
            }
            
            for (int dealerCard = CARD_TWO;
                 dealerCard <= CARD_ACE;
                 ++dealerCard) {
                // Skip the redundant 10s
                if (dealerCard == CARD_JACK ||
                    dealerCard == CARD_QUEEN ||
                    dealerCard == CARD_KING) {
                    continue;
                }
                
                ActionEnum act = getAction(static_cast<CardValueEnum>(dealerCard));
                ret.append(getActionChar(act));
            }
            ret.append("\n");
        }
        return ret;
    }
    
    // Simple access functions
    int getWinCount() const { return mWinCount; }
    int getLossCount() const { return mLossCount; }
    int getPushCount() const { return mPushCount; }
    int getBlackjackCount() const { return mBlackjackCount; }
    int getTotal() const { return mTotal; }
    
#ifdef PLAYER_CREATE_LOG
    std::string getLog() const { return mLog.str(); }
    void clearLog() { mLog.str(""); mLog.clear(); }
#endif
    
protected:
    
    std::deque<BjUtils::CardValueEnum> mCards;
    int mWinCount;
    int mLossCount;
    int mPushCount;
    int mBlackjackCount;
    int mTotal;
    
#ifdef PLAYER_CREATE_LOG
    std::ostringstream mLog;
#endif
};

#endif // PLAYERBASE_H
