#ifndef RANDOMPLAYER_H
#define RANDOMPLAYER_H

#include <stdlib.h>

#include "playerbase.h"

class RandomPlayer : public PlayerBase {
public:
    
    BjUtils::ActionEnum getAction(BjUtils::CardValueEnum dealerCard) const override {
        // Stand on everything above and including 17.
        if (getHandValue() >= 21) {
            return BjUtils::ACTION_STAND;
        } else {
            return static_cast<BjUtils::ActionEnum>(rand() % 2);
        }
    }
    
};

#endif // RANDOMPLAYER_H
