#ifndef DEALERPLAYER_H
#define DEALERPLAYER_H

#include "playerbase.h"

class DealerPlayer : public PlayerBase {
public:
    
    BjUtils::ActionEnum getAction(BjUtils::CardValueEnum dealerCard) const override {
        // Stand on everything above and including 17.
        if (getHandValue() >= 17) {
            return BjUtils::ACTION_STAND;
        } else {
            return BjUtils::ACTION_HIT;
        }
    }
    
};

#endif // DEALERPLAYER_H
