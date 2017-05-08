#include <iostream>

#include "doublefann.h"
#include "fann_cpp.h"

#include "evolutionstrategybase.h"
#include "nn_player.h"

#include <dealer.h>
#include <ndeckshoe.h>
#include <idealplayer.h>

#include "sync_functions.h"

using namespace std;

#include <ios>
#include <iostream>
#include <iomanip>
#include <vector>
#include <math.h>
#include <time.h>
#include <fstream>

// Callback function that simply prints the information to cout
int print_callback(FANN::neural_net &net, FANN::training_data &train,
    unsigned int max_epochs, unsigned int epochs_between_reports,
    float desired_error, unsigned int epochs, void *user_data)
{
    cout << "Epochs     " << setw(8) << epochs << ". "
         << "Current Error: " << left << net.get_MSE() << right << "   ";
    unsigned int numWeights = net.get_total_connections();
    vector<FANN::connection> weights;
    weights.resize(numWeights);
    net.get_connection_array(weights.data());
    cout << "Weights: ";
    for (int i = 0; i < weights.size(); ++i) {
        cout << weights.at(i).weight;
        if (i != weights.size() - 1) {
            cout << ", ";
        }
    }
    cout << endl;
    return 0;
}



void testXorEvolution() {
    typedef NN_Wrapper3<2, 3, 1> XOR_NN_t;
    
    vector<XOR_NN_t> xorNNs(319);
    
    ManualEvolution<XOR_NN_t> evolver;
    
    for (int i = 0; i < xorNNs.size(); ++i) {
        xorNNs[i].randomize();
    }
    
    array<double, 2> input1;
    array<double, 2> input2;
    array<double, 2> input3;
    array<double, 2> input4;
    input1[0] = -1.0; input1[1] = -1.0;
    input2[0] =  1.0; input2[1] = -1.0;
    input3[0] = -1.0; input3[1] =  1.0;
    input4[0] =  1.0; input4[1] =  1.0;
    double expected1 = -1.0;
    double expected2 =  1.0;
    double expected3 =  1.0;
    double expected4 = -1.0;
    
    array<double, 1> outputBuf1;
    array<double, 1> outputBuf2;
    array<double, 1> outputBuf3;
    array<double, 1> outputBuf4;
    
    ofstream file;
    file.open("XOR_results.csv");
    
    for (int gen_it = 0; gen_it < 100000; ++gen_it) {
        
        for (int i = 0; i < xorNNs.size(); ++i) {
            // Run each neural network and accumulate the 
            // margin.
            
            outputBuf1[0] = 0.0;
            outputBuf2[0] = 0.0;
            outputBuf3[0] = 0.0;
            outputBuf4[0] = 0.0;
            
            xorNNs[i].run(input1, outputBuf1);
            xorNNs[i].run(input2, outputBuf2);
            xorNNs[i].run(input3, outputBuf3);
            xorNNs[i].run(input4, outputBuf4);
            
            double err1 = fabs(outputBuf1[0]*100 - expected1*100);
            double err2 = fabs(outputBuf2[0]*100 - expected2*100);
            double err3 = fabs(outputBuf3[0]*100 - expected3*100);
            double err4 = fabs(outputBuf4[0]*100 - expected4*100);
            
            double err = err1 + err2 + err3 + err4;
            double score = (800.0 - err);
            xorNNs[i].setScore(score);
        }
        
        evolver.EvolveGeneration(xorNNs);
        
        if (gen_it % 100 == 0) {
            cout << "Generation " << gen_it << " best score = "
                 << xorNNs[0].score() << endl;
            file << gen_it << ", " << xorNNs[0].score() << "\n";
        }
    }
}



void testNNCopies() {
    typedef NN_Wrapper3<2, 3, 1> XOR_NN_t;
    XOR_NN_t one;
    XOR_NN_t two;
    
    array<double, 2> input1;
    array<double, 2> input2;
    array<double, 2> input3;
    array<double, 2> input4;
    input1[0] = -1.0; input1[1] = -1.0;
    input2[0] =  1.0; input2[1] = -1.0;
    input3[0] = -1.0; input3[1] =  1.0;
    input4[0] =  1.0; input4[1] =  1.0;
    
    array<double, 1> outputBuf11;
    array<double, 1> outputBuf12;
    array<double, 1> outputBuf13;
    array<double, 1> outputBuf14;
    
    one.run(input1, outputBuf11);
    one.run(input2, outputBuf12);
    one.run(input3, outputBuf13);
    one.run(input4, outputBuf14);
    
    array<double, 1> outputBuf21;
    array<double, 1> outputBuf22;
    array<double, 1> outputBuf23;
    array<double, 1> outputBuf24;
    
    two.run(input1, outputBuf21);
    two.run(input2, outputBuf22);
    two.run(input3, outputBuf23);
    two.run(input4, outputBuf24);
    
    bool eq1 = outputBuf11[0] == outputBuf21[0];
    bool eq2 = outputBuf12[0] == outputBuf22[0];
    bool eq3 = outputBuf13[0] == outputBuf23[0];
    bool eq4 = outputBuf14[0] == outputBuf24[0];
    
    if (eq1 && eq2 && eq3 && eq4) {
        cout << "NN_ONE equals NN_TWO\n";
    } else {
        cout << "NN_ONE does not equal NN_TWO?!?!?\n";
    }
    
    one.randomize();
    two.randomize();
    
    one.run(input1, outputBuf11);
    one.run(input2, outputBuf12);
    one.run(input3, outputBuf13);
    one.run(input4, outputBuf14);
    two.run(input1, outputBuf21);
    two.run(input2, outputBuf22);
    two.run(input3, outputBuf23);
    two.run(input4, outputBuf24);
    eq1 = outputBuf11[0] == outputBuf21[0];
    eq2 = outputBuf12[0] == outputBuf22[0];
    eq3 = outputBuf13[0] == outputBuf23[0];
    eq4 = outputBuf14[0] == outputBuf24[0];
    
    if (eq1 && eq2 && eq3 && eq4) {
        cout << "NN_ONE equals NN_TWO?!?!?\n";
    } else {
        cout << "NN_ONE does not equal NN_TWO.\n";
    }
    
    two = one;
    
    one.run(input1, outputBuf11);
    one.run(input2, outputBuf12);
    one.run(input3, outputBuf13);
    one.run(input4, outputBuf14);
    two.run(input1, outputBuf21);
    two.run(input2, outputBuf22);
    two.run(input3, outputBuf23);
    two.run(input4, outputBuf24);
    eq1 = outputBuf11[0] == outputBuf21[0];
    eq2 = outputBuf12[0] == outputBuf22[0];
    eq3 = outputBuf13[0] == outputBuf23[0];
    eq4 = outputBuf14[0] == outputBuf24[0];
    
    if (eq1 && eq2 && eq3 && eq4) {
        cout << "NN_ONE equals NN_TWO.\n";
    } else {
        cout << "NN_ONE does not equal NN_TWO?!?!?\n";
    }
}


void runNNplayers(int rank, int size) {
    //cout << "Running NN Player: \n";
    
    typedef Dealer<NDeckShoe<6>, NNPlayer> NNGame;
    
    const int NDEALERS = size;
    
    vector<NNGame> dealers(NDEALERS);
    
    typedef pair<int, int> IndScorePair;
    vector<IndScorePair> scores(NDEALERS-1);
    
    ofstream file;
    if (rank == 0) {
        file.open("results/NN_results.csv");
    }
    
    for (int i = 0; i < scores.size(); ++i) {
        scores[i].second = i+1;
    }
    
    int numHands = 1500;
    //int numHands = 15000;
    //int numHands = 150000;
    
    
    for (int gen_i = 0; gen_i < 1000; ++gen_i) {
        
        // Play blackjack
        if (rank != 0) {
            for (int i = 0; i < numHands; ++i) {
                dealers[rank].playHand();
            }
            // Send our scores back to zero
            int myTotal = dealers[rank].getPlayerTotal();
            SendToZero(&myTotal, 1, rank);
        } else {
            // If zero, receive all the scores
            for (int i = 0; i < scores.size(); ++i) {
                //scores[i].first = dealers[scores.at(i).second].getPlayerTotal();
                ZeroRecv(&scores[i].first, 1, scores.at(i).second);
            }
            
            sort(scores.begin(), scores.end(),
                 [](const pair<int,int> &one,
                 const pair<int,int> &two) {
                return one.first > two.first;
            });
        }
        
        
        
        // If zero, send all the scores back out
        if (rank == 0) {
            for (int i = 0; i < scores.size(); ++i) {
                ZeroSendTo(&i, 1, scores.at(i).second);
            }
            double bestRate = ((10.0*static_cast<double>(numHands))+scores.front().first) /
                              (10.0*static_cast<double>(numHands));
            cout << "Generation " << gen_i << " best score "
                 << bestRate << endl;
            file << bestRate << "\n";
        } else {
            // Receive our score
            int myWorldRank = -1;
            RecvFromZero(&myWorldRank, 1, rank);
            dealers[rank].player().setWorldRank(myWorldRank);
            
            // Standard mutation
            dealers[rank].player().mutate();
            dealers[rank].reset();
        }
        
    }
    
}


void runNNPairBonding(int rank, int size) {
    //cout << "Running NN Player: \n";
    
    typedef Dealer<NDeckShoe<6>, NNPlayer> NNGame;
    
    const int NDEALERS = size;
    
    vector<NNGame> dealers(NDEALERS);
    
    typedef pair<int, int> IndScorePair;
    vector<IndScorePair> scores(NDEALERS-1);
    
    ofstream file;
    if (rank == 0) {
        file.open("results/NN_results.csv");
    }
    
    for (int i = 0; i < scores.size(); ++i) {
        scores[i].second = i+1;
    }
    
    int numHands = 1500;
    //int numHands = 15000;
    //int numHands = 150000;
    
    vector<vector<FANN::connection>> connections(NDEALERS);
    vector<vector<double>> weights(NDEALERS);
    
    int numConnections = dealers[0].player().net().getNumConnections();
    for (int i = 0; i < NDEALERS; ++i) {
        connections[i].resize(numConnections);
        weights[i].resize(numConnections);
    }
    
    
    for (int gen_i = 0; gen_i < 1000; ++gen_i) {
        
        // Synchronize all the coefficients from zero to the rest of the world
        // Extract the weights
        for (int i = 0; i < NDEALERS; ++i) {
            dealers[i].player().net().extractWeights(connections[i]);
            for (int c = 0; c < numConnections; ++c) {
                weights[i][c] = connections[i][c].weight;
            }
        }
        
        // Send the weights out if zero node
        if (rank == 0) {
            for (int i = 0; i < NDEALERS; ++i) {
                ZeroSendTo(weights[i].data(), numConnections, i);
            }
        }
        
        // Receive the weights
        RecvFromZero(weights[rank].data(), numConnections, rank);
        
        // Set the weights in my local dealer
        for (int c = 0; c < numConnections; ++c) {
            connections[rank][c].weight = weights[rank][c];
        }
        
        dealers[rank].player().net().setWeights(connections[rank]);
        
        
        // Play blackjack
        if (rank != 0) {
            for (int i = 0; i < numHands; ++i) {
                dealers[rank].playHand();
            }
            // Send our scores back to zero
            int myTotal = dealers[rank].getPlayerTotal();
            SendToZero(&myTotal, 1, rank);
        } else {
            // If zero, receive all the scores
            for (int i = 0; i < scores.size(); ++i) {
                //scores[i].first = dealers[scores.at(i).second].getPlayerTotal();
                ZeroRecv(&scores[i].first, 1, scores.at(i).second);
            }
            
            sort(scores.begin(), scores.end(),
                 [](const pair<int,int> &one,
                 const pair<int,int> &two) {
                return one.first > two.first;
            });
        }
        
        
        
        if (rank == 0) {
            double bestRate = ((10.0*static_cast<double>(numHands))+scores.front().first) /
                              (10.0*static_cast<double>(numHands));
            cout << "Generation " << gen_i << " best score "
                 << bestRate << endl;
                 
            vector<vector<FANN::connection>> newConnections;
            
            const int keep = 5; // 5 choose 2 = 10
            //const int keep = 21; // // 21 choose 2 = 210
            
            for (int i = 0; i < keep; ++i) {
                for (int c = 0; c < i; ++c) {
                    if (c == i) continue;
                    
                    vector<FANN::connection> connections1 = connections[i];
                    
                    vector<FANN::connection> connections2 = connections[c];
                    
                    double wr1 = ((10.0*static_cast<double>(numHands))+scores[i].first) /
                                 (10.0*static_cast<double>(numHands));
                    
                    double wr2 = ((10.0*static_cast<double>(numHands))+scores[c].first) /
                                 (10.0*static_cast<double>(numHands));
                    
                    const int N = 3;
                    double mut1 = (powf(wr1, N) / (powf(wr1, N) + powf(wr2, N)));
                    
                    for (int x = 0; x < connections1.size(); ++x) {
                        connections1[x].weight = (connections1[x].weight * mut1) +
                                                 (connections2[x].weight * (1.0 - mut1));
                    }
                    
                    newConnections.push_back(connections1);
                }
            }
            
            int count = 0;
            for (int i = 0; i < newConnections.size(); ++i) {
                // Keep the first n since we used those to generate
                // the new coefficients
                dealers[scores[i+keep].second].player().net().setWeights(newConnections[i]);
                ++count;
            }
            while(count < NDEALERS) {
                dealers[scores[count].second].player().net().randomize();
                ++count;
            }
            
        } else {
            dealers[rank].reset();
        }
        
        
        
    }
    
}


void testIdeal() {
    cout << "Playing 100000 hands through 'ideal' player:\n";
    Dealer<NDeckShoe<6>, IdealPlayer> dealer1;
    
    for (int i = 0; i < 100000; ++i) {
        dealer1.playHand();
    }
    double ret = (10.0*100000.0 + ((double)dealer1.getPlayerTotal()))/(10.0*100000.0);
    cout << "Ideal player return rate: " << ret << endl;
}


void testMpi(int rank, int size) {
    int arr[320] = {0};
    arr[rank] = rank;
    
    SendToZero(&arr[rank], 1, rank);
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    if (rank == 0) {
        for (int i = 0; i < size; ++i) {
            ZeroRecv(&arr[i], 1, i);
        }
        for (int i = 0; i < size; ++i) {
            arr[i] = arr[i] * 2;
            ZeroSendTo(&arr[i], 1, i);
        }
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    RecvFromZero(&arr[rank], 1, rank);
    
    cout << "Rank " << rank << ":";
    for (int i = 0; i < size; ++i) {
        cout << " " << arr[i];
    }
    cout << "\n";
}


void sleep_ms(int milliseconds)
{
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}


/* Startup function. Syncronizes C and C++ output, calls the test function
   and reports any exceptions */
int main(int argc, char *argv[])
{
    int rank = 0, size = 1, len;
    char version[MPI_MAX_LIBRARY_VERSION_STRING];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_library_version(version, &len);
    
    sleep_ms(rank * 100);
    
    srand (time(NULL));
    
    //cout << "\n\nTesting XOR Evolution: \n";
    
    //testNNCopies();
    
    //testXorEvolution();
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    //runNNplayers(rank, size);
    
    runNNPairBonding(rank, size);
    
    //testIdeal();
    
    //testMpi(rank, size);
    
    MPI_Finalize();
    
    return 0;
}
