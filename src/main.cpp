#include <iostream>
#include <cstdlib>
#include <cmath> 

#include "../include/pointersFunctions.h"
#include "../include/saveDataFunctions.h"
#include "../include/evolutionFunctions.h"

//DEFINE drand48 TO OBTAIN A RANDOM NUMBER
#define drand48() ((float)rand()/(float)RAND_MAX)
int main(){

    // Size of the system
    size_t dimension{6};

    // Pointers to allocate states of the system
    int *currentStates{nullptr}; // pointer to the current states
    int *neighbors{nullptr}; // pointer to the neighbours
    int *nextStates{nullptr}; // Buffer that point to the future states

    // Allocate memory on pointers
    currentStates   =   create1DPtr(dimension);
    neighbors       =   create1DPtr(dimension);
    nextStates      =   create1DPtr(dimension);

    // Pointers to store distance components
    int *statesDist{nullptr}, *neighborsDist{nullptr};

    // Allocate memory to save components of distance
    statesDist      = create1DPtr(dimension);
    neighborsDist   = create1DPtr(dimension);

    // Set distances to zero for safety
    for(size_t cellIdx{}; cellIdx < dimension; cellIdx++){
        statesDist[cellIdx]     = 0;
        neighborsDist[cellIdx]  = 0;
    }

    //=============================================================================================================
    //===================================== E V O L V E  T H E  S Y S T E M ======================================= 
    
    // Pointers to allocate index of neighbors for each state
    int *ptrFirstNeighborRightIdx{nullptr}, *ptrSecondNeighborRightIdx{nullptr};
    int *ptrFirstNeighborLeftIdx{nullptr}, *ptrSecondNeighborLeftIdx{nullptr};

    // Allocate memory on index pointers
    ptrFirstNeighborRightIdx   =   create1DPtr(dimension);
    ptrSecondNeighborRightIdx  =   create1DPtr(dimension);

    ptrFirstNeighborLeftIdx    =   create1DPtr(dimension);
    ptrSecondNeighborLeftIdx   =   create1DPtr(dimension);

    // Compute the idx of the neighbor for each position in the array
    idxPeriodicBoudaryCondition(dimension, ptrFirstNeighborRightIdx, ptrSecondNeighborRightIdx, ptrFirstNeighborLeftIdx, ptrSecondNeighborLeftIdx);

    // Define variables to save the current neighbors of a cell
    int firstNeighborRight{}, secondNeighborRight{};
    int firstNeighborLeft{}, secondNeighborLeft{};

    // Pointer to allocate index of visited states during the computation of clusters
    bool *adjListStates{nullptr};

    // Allocate memory for adjacency list pointers
    adjListStates = new bool[dimension];

    // Pointers to allocate clusters of each state
    int *clusterStateA{nullptr}, *clusterStateB{nullptr}, *clusterStateC{nullptr};

    // Allocate memory for clusters ptrs
    clusterStateA = create1DPtr(dimension);
    clusterStateB = create1DPtr(dimension);
    clusterStateC = create1DPtr(dimension);

    // Set all clusters to cero and adj List to true
    // I'm going to set true as "idx that yo have to visit"
    initialStateAdjListCluster(dimension, adjListStates, clusterStateA, clusterStateB, clusterStateC);
    // Thus, if a certain index is true we have to compute its clusters

    // Variables to store clusters on a specific direction as a buffer
    int clusterRight{}, clusterLeft{};

    // Variables to store square of distances between configurations
    int squareStateDist{}, squareNeighDist{};

    // Variables to store the transformation of the conf. from ternary to decimal
    int decStates{}, decNeighbors{};

    

    decimalToTernary(dimension, 25, currentStates);
    decimalToTernary(dimension, -101, neighbors);

    

    
    
    // EVOLUTION OF THE SYSTEM
    
    for(int i{}; i < 10;i++){

        // Access to each cell of the array to evolve it
        for(size_t cellIdx{}; cellIdx < dimension; cellIdx++){

            if(adjListStates[cellIdx]){
                clustering(dimension, cellIdx, currentStates, ptrFirstNeighborRightIdx, ptrFirstNeighborLeftIdx, adjListStates, clusterRight, clusterLeft);
                addTotalCluster(dimension, cellIdx, currentStates, clusterStateA, clusterStateB, clusterStateC, clusterRight, clusterLeft);
            }

            // Neighbors of current cell
            firstNeighborRight  =   neighbors[ptrFirstNeighborRightIdx[cellIdx]];
            secondNeighborRight =   neighbors[ptrSecondNeighborRightIdx[cellIdx]];

            firstNeighborLeft   =   neighbors[ptrFirstNeighborLeftIdx[cellIdx]];
            secondNeighborLeft  =   neighbors[ptrSecondNeighborLeftIdx[cellIdx]];

            // Evolve cell 
            nextStates[cellIdx] = Q2RPottsRule(cellIdx, currentStates, firstNeighborRight, secondNeighborRight, firstNeighborLeft, secondNeighborLeft);

            // Compute relative distance between components
            statesDist[cellIdx]     = neighbors[cellIdx] - currentStates[cellIdx];
            neighborsDist[cellIdx]  = nextStates[cellIdx] - neighbors[cellIdx];

            // Compute the square of the euclidean distance
            squareStateDist = statesDist[cellIdx] * statesDist[cellIdx];
            squareNeighDist = neighborsDist[cellIdx] * neighborsDist[cellIdx];

            // Get the configuration in decimal form
            decStates       += currentStates[cellIdx] * pow(3,dimension - 1 - cellIdx);
            decNeighbors    += neighbors[cellIdx] * pow(3,dimension - 1 - cellIdx);

        }

        // Permute currentStates with neighbors and neighbors with nextStates to evolve one step
        reArrangePtr(dimension, currentStates, neighbors, nextStates);

        // Set the conditions properly for counting another iteration
        decStates = 0; decNeighbors = 0;
        initialStateAdjListCluster(dimension, adjListStates, clusterStateA, clusterStateB, clusterStateC);
    }

    // Compute the energy
    int energy{};
    computeEnergy(dimension, energy, currentStates, neighbors, ptrFirstNeighborRightIdx, ptrSecondNeighborRightIdx, ptrFirstNeighborLeftIdx, ptrSecondNeighborLeftIdx);
    std::cout << energy << std::endl;
    std:: cout << "===================================================================" << std::endl;

    // Clean the memory used by pointers
    delete[] currentStates; currentStates   = NULL;
    delete[] neighbors;     neighbors       = NULL;
    delete[] nextStates;    nextStates      = NULL;

    delete[] ptrFirstNeighborRightIdx;  ptrFirstNeighborRightIdx    = NULL;
    delete[] ptrFirstNeighborLeftIdx;   ptrFirstNeighborLeftIdx     = NULL;
    delete[] ptrSecondNeighborRightIdx; ptrSecondNeighborRightIdx   = NULL;
    delete[] ptrSecondNeighborLeftIdx;  ptrSecondNeighborLeftIdx    = NULL;

    delete[] adjListStates; adjListStates = NULL;

    delete[] clusterStateA; clusterStateA = NULL;
    delete[] clusterStateB; clusterStateB = NULL;
    delete[] clusterStateC; clusterStateC = NULL;

    return 0;
}
