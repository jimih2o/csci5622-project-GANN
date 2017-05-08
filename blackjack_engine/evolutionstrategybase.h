#ifndef MANUALEVOLUTION_H
#define MANUALEVOLUTION_H

#include "fann_cpp.h"

#include <vector>
#include <array>
#include <utility>
#include <algorithm>
#include <random>
#include <chrono>
#include <functional>
#include <memory>
#include <cstring>



// No C++14 on the cluster, so we need our own
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}


template<const unsigned int NUM_INPUTS,
         const unsigned int NUM_L1_NODES,
         const unsigned int NUM_L2_NODES,
         const unsigned int NUM_OUTPUTS>
class NN_Wrapper4 {
public:
    
    typedef NN_Wrapper4<NUM_INPUTS,
                        NUM_L1_NODES,
                        NUM_L2_NODES,
                        NUM_OUTPUTS> ThisType;
    
    NN_Wrapper4()
        : mpNet(make_unique<FANN::neural_net>(FANN::LAYER,
                                              3,
                                              NUM_INPUTS,
                                              NUM_L1_NODES,
                                              NUM_L2_NODES,
                                              NUM_OUTPUTS)),
          mScore(0)
    {
        mpNet->set_learning_rate(0.7f);
    
        mpNet->set_activation_steepness_hidden(1.0);
        mpNet->set_activation_steepness_output(1.0);
        
        mpNet->set_activation_function_hidden(FANN::SIGMOID_SYMMETRIC_STEPWISE);
        mpNet->set_activation_function_output(FANN::SIGMOID_SYMMETRIC_STEPWISE);
        
        randomize();
    }
    
    void extractWeights(std::vector<FANN::connection> &connectionBuffer) const {
        unsigned int numConnections = mpNet->get_total_connections();
        if (connectionBuffer.size() < numConnections) {
            connectionBuffer.resize(numConnections);
        }
        mpNet->get_connection_array(connectionBuffer.data());
    }
    
    void setWeights(std::vector<FANN::connection> &connectionBuffer) {
        unsigned int numConnections = mpNet->get_total_connections();
        if (numConnections == connectionBuffer.size()) {
            mpNet->set_weight_array(connectionBuffer.data(), numConnections);
        }
    }
    
    unsigned int getNumConnections() {
        return mpNet->get_total_connections();
    }
    
    void randomize() {
        mpNet->randomize_weights(-0.1f, 0.1f);
    }
    
    void operator=(const ThisType &other)
    {
        if (this == &other) return;
        if (mpNet.get() == nullptr) {
            mpNet = make_unique<FANN::neural_net>(FANN::LAYER,
                                                  3,
                                                  NUM_INPUTS,
                                                  NUM_L1_NODES,
                                                  NUM_OUTPUTS);
        }
        std::vector<FANN::connection> connections;
        connections.resize(getNumConnections());
        other.extractWeights(connections);
        setWeights(connections);
        mScore = other.mScore;
    }
    
    NN_Wrapper4(const ThisType &other)
    {
        this->operator=(other);
    }
    
    double score() const {
        return mScore;
    }
    
    void setScore(double score) {
        mScore = score;
    }
    
    bool operator<(const ThisType &other) const {
        return mScore < other.mScore;
    }
    
    bool operator>(const ThisType &other) const {
        return mScore > other.mScore;
    }
    
    bool operator==(const ThisType &other) const {
        return mScore == other.mScore;
    }
    
    void run(std::array<fann_type, NUM_INPUTS> inputs,
             std::array<fann_type, NUM_OUTPUTS> &outputBuf) {
        fann_type *pResults = mpNet->run(inputs.data());
        memcpy(outputBuf.data(), pResults, sizeof(fann_type)*NUM_OUTPUTS);
    }
    
private:
    
    mutable std::unique_ptr<FANN::neural_net> mpNet;
    double mScore;
};




template<const unsigned int NUM_INPUTS,
         const unsigned int NUM_L1_NODES,
         const unsigned int NUM_OUTPUTS>
class NN_Wrapper3 {
public:
    
    typedef NN_Wrapper3<NUM_INPUTS,
                        NUM_L1_NODES,
                        NUM_OUTPUTS> ThisType;
    
    NN_Wrapper3()
        : mpNet(make_unique<FANN::neural_net>(FANN::LAYER,
                                              3,
                                              NUM_INPUTS,
                                              NUM_L1_NODES,
                                              NUM_OUTPUTS)),
          mScore(0)
    {
        mpNet->set_learning_rate(0.7f);
    
        mpNet->set_activation_steepness_hidden(1.0);
        mpNet->set_activation_steepness_output(1.0);
        
        mpNet->set_activation_function_hidden(FANN::SIGMOID_SYMMETRIC_STEPWISE);
        mpNet->set_activation_function_output(FANN::SIGMOID_SYMMETRIC_STEPWISE);
        
        randomize();
    }
    
    void extractWeights(std::vector<FANN::connection> &connectionBuffer) const {
        unsigned int numConnections = mpNet->get_total_connections();
        if (connectionBuffer.size() < numConnections) {
            connectionBuffer.resize(numConnections);
        }
        mpNet->get_connection_array(connectionBuffer.data());
    }
    
    void setWeights(std::vector<FANN::connection> &connectionBuffer) {
        unsigned int numConnections = mpNet->get_total_connections();
        if (numConnections == connectionBuffer.size()) {
            mpNet->set_weight_array(connectionBuffer.data(), numConnections);
        }
    }
    
    unsigned int getNumConnections() {
        return mpNet->get_total_connections();
    }
    
    void randomize() {
        mpNet->randomize_weights(-0.1f, 0.1f);
    }
    
    void operator=(const ThisType &other)
    {
        if (this == &other) return;
        if (mpNet.get() == nullptr) {
            mpNet = make_unique<FANN::neural_net>(FANN::LAYER,
                                                  3,
                                                  NUM_INPUTS,
                                                  NUM_L1_NODES,
                                                  NUM_OUTPUTS);
        }
        std::vector<FANN::connection> connections;
        connections.resize(getNumConnections());
        other.extractWeights(connections);
        setWeights(connections);
        mScore = other.mScore;
    }
    
    NN_Wrapper3(const ThisType &other)
    {
        this->operator=(other);
    }
    
    double score() const {
        return mScore;
    }
    
    void setScore(double score) {
        mScore = score;
    }
    
    bool operator<(const ThisType &other) const {
        return mScore < other.mScore;
    }
    
    bool operator>(const ThisType &other) const {
        return mScore > other.mScore;
    }
    
    bool operator==(const ThisType &other) const {
        return mScore == other.mScore;
    }
    
    void run(std::array<fann_type, NUM_INPUTS> inputs,
             std::array<fann_type, NUM_OUTPUTS> &outputBuf) {
        fann_type *pResults = mpNet->run(inputs.data());
        memcpy(outputBuf.data(), pResults, sizeof(fann_type)*NUM_OUTPUTS);
    }
    
private:
    
    mutable std::unique_ptr<FANN::neural_net> mpNet;
    double mScore;
};




template<typename NN_WRAPPER_TYPE>
class ManualEvolution {

public:
    // mutateFactor is a four-digit 
    ManualEvolution()
        : mSmallDist(-0.2, 0.2),
          mLargeDist(-1.0, 1.0),
          mSeed(std::chrono::system_clock::now().time_since_epoch().count()),
          mGenerator(mSeed) {} // NOOP
    
    float getSmall() {
        return mSmallDist(mGenerator);
    }
    
    float getLarge() {
        return mLargeDist(mGenerator);
    }
    
    virtual void EvolveGeneration(std::vector<NN_WRAPPER_TYPE> &newGen) {
        // Copy the old generation;
        size_t size = newGen.size();
        
        // Sort the new/old generation in descending order - higher score is better
        std::sort(newGen.begin(), newGen.end(), std::greater<NN_WRAPPER_TYPE>());
        
        std::vector<FANN::connection> connections;
        connections.resize(newGen[0].getNumConnections());
        
        for (int i = 0; i < newGen.size(); ++i) {
            if (i < 10) {
                // Do nothing
            } else if (i >= 10 && i < (size / 2)) {
                // Small modifications
                newGen[i].extractWeights(connections);
                for (int c = 0; c < connections.size(); ++c) {
                    connections[c].weight += getSmall();
                }
                newGen[i].setWeights(connections);
            } else if (i >= (size / 2) &&
                       i < (3*(size / 4))) {
                // Large modifications
                newGen[i].extractWeights(connections);
                for (int c = 0; c < connections.size(); ++c) {
                    connections[c].weight += getLarge();
                }
                newGen[i].setWeights(connections);
            } else {
                // Randomize all the rest from scratch
                newGen[i].randomize();
            }
        }
        
        
    }
    
private:
    std::uniform_real_distribution<double> mSmallDist;
    std::uniform_real_distribution<double> mLargeDist;
    unsigned mSeed;
    std::default_random_engine mGenerator;
    
};

#endif // MANUALEVOLUTION_H

