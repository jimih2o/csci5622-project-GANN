#ifndef IDEALPLAYER_H
#define IDEALPLAYER_H

#include "playerbase.h"

#include <string>
#include <map>

// The 'ideal' strategy:
// http://www.blackjackapprenticeship.com/resources/blackjack-strategy-charts/
// 
// Not playing surrender.
// 
class IdealPlayer : public PlayerBase {
    
    std::map<char, std::string> mSplitHash;
    std::map<int, std::string> mSoftHash;
    std::map<int, std::string> mHardHash;
    
public:
    
    IdealPlayer() {
        // X = double, or stand
        // D = double, or hit
        // H = hit
        // S = stand
        
        mSplitHash['9'] = "YYYYYNYYNN";
        mSplitHash['7'] = "YYYYYYNNNN";
        mSplitHash['6'] = "NYYYYNNNNN";
        mSplitHash['3'] = "NNYYYYNNNN";
        mSplitHash['2'] = "NNYYYYNNNN";
        
        mSoftHash[9] = "SSSSSSSSSS";
        mSoftHash[8] = "SSSSXSSSSS";
        mSoftHash[7] = "XXXXXSSHHH";
        mSoftHash[6] = "HDDDDHHHHH";
        mSoftHash[5] = "HHDDDHHHHH";
        mSoftHash[4] = "HHDDDHHHHH";
        mSoftHash[3] = "HHHDDHHHHH";
        mSoftHash[2] = "HHHDDHHHHH";
        
        mHardHash[16] = "SSSSSHHHHH";
        mHardHash[15] = "SSSSSHHHHH";
        mHardHash[14] = "SSSSSHHHHH";
        mHardHash[13] = "SSSSSHHHHH";
        mHardHash[12] = "HHSSSHHHHH";
        mHardHash[11] = "DDDDDDDDDD";
        mHardHash[10] = "DDDDDDDDHH";
        mHardHash[9]  = "HDDDDHHHHH";
    }
    
    BjUtils::ActionEnum getAction(BjUtils::CardValueEnum dealerCard) const override {
        using namespace BjUtils;
        using namespace std;
        
        // Get the total
        bool isSoft = false;
        int total = BjUtils::getHandValue(mCards, isSoft);
        
        if (total >= 21)
            return ACTION_STAND;
        
        // Set the dealer card index for the tables.
        int di = 0; // dealer index
        if (dealerCard >= CARD_TWO && dealerCard <= CARD_NINE)
            di = static_cast<int>(dealerCard)-2;
        else if (dealerCard == CARD_ACE)
            di = 9;
        else if (dealerCard >= CARD_TEN && dealerCard <= CARD_KING)
            di = 8;
        else
            throw std::string("Invalid dealer card?");
        
        bool initialHand = (mCards.size() == 2);
        
        // Splits ****************************************
        if (initialHand && (mCards.at(0) == mCards.at(1))) {
            if (mCards.at(0) == CARD_ACE ||
                mCards.at(0) == CARD_EIGHT)
                return ACTION_SPLIT;
            // Use the hash table.
            char cardChar = getCardChar(mCards.at(0));
            if (mSplitHash.count(cardChar) > 0) {
                if (mSplitHash.at(cardChar).at(di) == 'Y')
                    return ACTION_SPLIT;
            }
        }
        
        // Soft totals ***********************************
        if (isSoft) {
            // Create soft hash table.
            int v = total - 11;
            if (mSoftHash.count(v) > 0) {
                char a = mSoftHash.at(v).at(di);
                if (a == 'S')
                    return ACTION_STAND;
                else if (a == 'H')
                    return ACTION_HIT;
                else if (a == 'D')
                    return (initialHand ? ACTION_DOUBLE : ACTION_HIT);
                else if (a == 'X')
                    return (initialHand ? ACTION_DOUBLE : ACTION_STAND);
                else
                    throw string("Invalid soft hash action");
            }
        }
        
        // Hard totals ***********************************
        if (total >= 17)
            return ACTION_STAND;
        char action = 'H';
        if (mHardHash.count(total) > 0) {
            action = mHardHash.at(total).at(di);
        }
        if (action == 'H')
            return ACTION_HIT;
        else if (action == 'S')
            return ACTION_STAND;
        else if (action == 'D')
            return (initialHand ? ACTION_DOUBLE : ACTION_HIT);
        else
            throw string("Invalid hard hash action");
        
        throw string("No action taken on hand");
        return ACTION_HIT;
    }
};

#endif // IDEALPLAYER_H
