#ifndef BLACKJACKUTILS_H
#define BLACKJACKUTILS_H

#include <string>
#include <deque>
#include <algorithm>

namespace BjUtils {

    enum CardValueEnum
    {
        CARD_TWO = 2,
        CARD_THREE,
        CARD_FOUR,
        CARD_FIVE,
        CARD_SIX,
        CARD_SEVEN,
        CARD_EIGHT,
        CARD_NINE,
        CARD_TEN,
        CARD_JACK,
        CARD_QUEEN,
        CARD_KING,
        CARD_ACE
    };


    enum ActionEnum
    {
        ACTION_HIT = 0,
        ACTION_STAND,
        ACTION_DOUBLE,
        ACTION_SPLIT
    };
    
    enum PayoutEnum
    {
        PAY_LOSS = 0,
        PAY_PUSH,
        PAY_WIN,
        PAY_DOUBLE,
        PAY_LOSS_DOUBLE,
        PAY_BLACKJACK
    };

    std::string getActionChar(ActionEnum act)
    {
        switch (act) {
            case ACTION_HIT:
                return "H";
                break;
            case ACTION_STAND:
                return "S";
                break;
            case ACTION_DOUBLE:
                return "D";
                break;
            case ACTION_SPLIT:
                return "T";
                break;
            default:
                return "?";
                break;
        }
    }
    
    std::string getCardName(CardValueEnum c)
    {
        switch(c) {
            case CARD_TWO:
                return "two";
                break;
            case CARD_THREE:
                return "three";
                break;
            case CARD_FOUR:
                return "four";
                break;
            case CARD_FIVE:
                return "five";
                break;
            case CARD_SIX:
                return "six";
                break;
            case CARD_SEVEN:
                return "seven";
                break;
            case CARD_EIGHT:
                return "eight";
                break;
            case CARD_NINE:
                return "nine";
                break;
            case CARD_TEN:
                return "ten";
                break;
            case CARD_JACK:
                return "jack";
                break;
            case CARD_QUEEN:
                return "queen";
                break;
            case CARD_KING:
                return "king";
                break;
            case CARD_ACE:
                return "ace";
                break;
        }
        return "";
    }

    char getCardChar(CardValueEnum c)
    {
        switch(c) {
            case CARD_TWO:
                return '2';
                break;
            case CARD_THREE:
                return '3';
                break;
            case CARD_FOUR:
                return '4';
                break;
            case CARD_FIVE:
                return '5';
                break;
            case CARD_SIX:
                return '6';
                break;
            case CARD_SEVEN:
                return '7';
                break;
            case CARD_EIGHT:
                return '8';
                break;
            case CARD_NINE:
                return '9';
                break;
            case CARD_TEN:
                return 'T';
                break;
            case CARD_JACK:
                return 'J';
                break;
            case CARD_QUEEN:
                return 'Q';
                break;
            case CARD_KING:
                return 'K';
                break;
            case CARD_ACE:
                return 'A';
                break;
            default:
                break;
        }
        return '?';
    }
    
    // The Aces should be at the end of the deque for this to work
    // properly.
    int getHandValue(const std::deque<CardValueEnum> &cards, bool &soft)
    {
        int total = 0;
        soft = false;

        for (int i = 0; i < cards.size(); ++i) {
            if (cards.at(i) == CARD_ACE) {
                if (total + 11 > 21) {
                    total += 1;
                } else {
                    total += 11;
                    soft = true;
                }
            } else if (cards.at(i) > CARD_NINE) {
                total += 10;
            } else {
                total += static_cast<int>(cards.at(i));
            }
        }
        
        // It is possible that the first ace we encounter
        // would be registered as soft but more following aces
        // would be hard resulting in a total that is greater
        // than 21. In this case make the first ace soft. 
        // Obviously only one ace in a given hand can be 'soft'
        if (soft && total > 21) {
            total -= 10;
            soft = false;
        }
        
        return total;
    }
    
    int getHandValue(const std::deque<CardValueEnum> &cards) {
        bool temp;
        return getHandValue(cards, temp);
    }

    bool haveBlackjack(const std::deque<CardValueEnum> &cards) {
        return (cards.size() == 2) && (BjUtils::getHandValue(cards) == 21);
    }
    
}



#endif // BLACKJACKUTILS_H
