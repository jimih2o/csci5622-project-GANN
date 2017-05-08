#ifndef NN_PLAYER_H
#define NN_PLAYER_H

#include "fann_cpp.h"

#include "playerbase.h"
#include "evolutionstrategybase.h"

#include <algorithm>
#include <random>
#include <chrono>

class NNPlayer : public PlayerBase {
public:
    
    typedef NN_Wrapper4<4, 80, 20, 2> NET_TYPE;
    
    enum {
        MAX_NUM_PLAYER_CARDS = 11
    };
    
    NNPlayer()
        : mWorldRank(1),
          mWorldSize(1),
          mScoreRank(0),
          mSmallDist(-0.01, 0.01),
          mLargeDist(-0.05, 0.05),
          mSeed(std::chrono::system_clock::now().time_since_epoch().count()),
          mGenerator(mSeed)
    {
        mConnections.resize(mNet.getNumConnections());
    }
    
    NNPlayer(int worldRank, int worldSize)
        : mWorldRank(worldRank),
          mWorldSize(worldSize),
          mScoreRank(0),
          mSmallDist(-0.01, 0.01),
          mLargeDist(-0.05, 0.05),
          mSeed(std::chrono::system_clock::now().time_since_epoch().count()),
          mGenerator(mSeed)
    {
        mConnections.resize(mNet.getNumConnections());
    }
    
    static double GetCardInputVal(BjUtils::CardValueEnum card) {
        switch(card) {
            case BjUtils::CARD_TWO:
                return -0.8;
                break;
            case BjUtils::CARD_THREE:
                return -0.6;
                break;
            case BjUtils::CARD_FOUR:
                return -0.4;
                break;
            case BjUtils::CARD_FIVE:
                return -0.2;
                break;
            case BjUtils::CARD_SIX:
                return 0.0;
                break;
            case BjUtils::CARD_SEVEN:
                return 0.2;
                break;
            case BjUtils::CARD_EIGHT:
                return 0.4;
                break;
            case BjUtils::CARD_NINE:
                return 0.6;
                break;
            case BjUtils::CARD_TEN:
            case BjUtils::CARD_JACK:
            case BjUtils::CARD_QUEEN:
            case BjUtils::CARD_KING:
                return 0.8;
                break;
            case BjUtils::CARD_ACE:
                return 1.0;
                break;
            default:
                return -1.0;
                break;
        }
    }
    
    BjUtils::ActionEnum getAction(BjUtils::CardValueEnum dealerCard) const override {
        
        bool soft = false;
        int total = BjUtils::getHandValue(mCards, soft);
        
        if (total >= 21)
            return BjUtils::ACTION_STAND;
        
        mInputValues[0] = GetCardInputVal(dealerCard);
        
        // Hard totals are mapped the following way:
        // Minimum (2) = -1, Maximum (20) = 1
        mInputValues[1] = (((double)total) / 10.0) - 1.0;
        if (soft) {
            mInputValues[2] = 1.0;
        } else {
            mInputValues[2] = -1.0;
        }
        
        //for (int i = 0; i < MAX_NUM_PLAYER_CARDS; ++i) {
        //    if (mCards.size() > i) {
        //        mInputValues[i+1] = GetCardInputVal(mCards.at(i));
        //    } else {
        //        mInputValues[i+1] = -1.0;
        //    }
        //}
        
        bool initialHand = (mCards.size() == 2);
        
        // Splitting
        if (initialHand && (mCards.at(0) == mCards.at(1))) {
           mInputValues[3] = 1.0; 
        } else {
            mInputValues[3] = -1.0;
        }
        
        mNet.run(mInputValues, mOutputValues);
        
        if (initialHand && (mCards.at(0) == mCards.at(1))) {
            // Allowed to split. Check the split output of the NN
            if (mOutputValues[0] > 0.0) {
                return BjUtils::ACTION_SPLIT;
            }
        }
        
        // Double, Hit, or Stand:
        if (mOutputValues[1] < -0.6) {
            return BjUtils::ACTION_DOUBLE;
        } else if (mOutputValues[1] < 0.2) {
            return BjUtils::ACTION_HIT;
        } else {
            return BjUtils::ACTION_STAND;
        }
    }
    
    double getSmall() {
        return mSmallDist(mGenerator);
    }
    
    double getLarge() {
        return mLargeDist(mGenerator);
    }
    
    void setWorldRank(int worldRank) {
        mWorldRank = worldRank;
    }
    
    int getWorldRank() const {
        return mWorldRank;
    }
    
    void setWorldSize(int worldSize) {
        mWorldSize = worldSize;
    }
    
    void mutate() {
        if (mWorldRank < 10) {
            // Do nothing
        } else if (mWorldRank >= 10 && mWorldRank < (mWorldSize / 2)) {
            // Small modifications
            mNet.extractWeights(mConnections);
            for (int c = 0; c < mConnections.size(); ++c) {
                mConnections[c].weight += getSmall();
            }
            mNet.setWeights(mConnections);
        } else if (mWorldRank >= (mWorldSize / 2) &&
                   mWorldRank < (3*(mWorldSize / 4))) {
            // Large modifications
            mNet.extractWeights(mConnections);
            for (int c = 0; c < mConnections.size(); ++c) {
                mConnections[c].weight += getLarge();
            }
            mNet.setWeights(mConnections);
        } else {
            // Randomize all the rest from scratch
            mNet.randomize();
        }
    }
    
    NET_TYPE &net() {
        return mNet;
    }
    
    
    
    
private:
    int mWorldRank;
    int mWorldSize;
    int mScoreRank;
    
    std::uniform_real_distribution<double> mSmallDist;
    std::uniform_real_distribution<double> mLargeDist;
    unsigned mSeed;
    std::default_random_engine mGenerator;
    
    std::vector<FANN::connection> mConnections;
    mutable NET_TYPE mNet;
    
    mutable std::array<double, 4> mInputValues;
    mutable std::array<double, 2> mOutputValues;
};

#endif // NN_PLAYER_H
