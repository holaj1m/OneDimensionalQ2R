#include <iostream>
#include <cstdlib>

#include "../include/pointersFunctions.h"
#include "../include/saveDataFunctions.h"
#include "../include/evolutionFunctions.h"

//DEFINE drand48 TO OBTAIN A RANDOM NUMBER
#define drand48() ((float)rand()/(float)RAND_MAX)
int main(){

    // Size of the system and total numer of iterations
    size_t dimension{6}, time{5};

    // Pointers to allocate states of the system
    int *currentStates{nullptr}; // pointer to the current states
    int *neighbors{nullptr}; // pointer to the neighbours
    int *nextStates{nullptr}; // Buffer that point to the future states

    // Allocate memory on pointers
    currentStates   =   create1DPtr(dimension);
    neighbors       =   create1DPtr(dimension);
    nextStates      =   create1DPtr(dimension);

    // Density between states A and B, and between A and C.
    // in the distribution, or total configuration.
    double densityStatesAB{0.3};
    double densityStatesAC{0.15};

    // Set the initial conditions
    configureInitialConditions(dimension, densityStatesAB, densityStatesAC,currentStates, neighbors, nextStates);

    // Create output files to store the counts of each state
    /*FILE *countStateA = createBinOutput("countStateA.bin");
    FILE *countStateB = createBinOutput("countStateB.bin");
    FILE *countStateC = createBinOutput("countStateC.bin");

    // Verify if the files were opened properly
    verifyBinaryOutput(countStateA);
    verifyBinaryOutput(countStateB);
    verifyBinaryOutput(countStateC);*/

    //=============================================================================================================
    //===================================== E V O L V E  T H E  S Y S T E M ======================================= 
    
    // Pointers to allocate index of neighbors for each state
    int *firstNeighborRightIdx{nullptr}, *secondNeighborRightIdx{nullptr};
    int *firstNeighborLeftIdx{nullptr}, *secondNeighborLeftIdx{nullptr};

    // Allocate memory on index pointers
    firstNeighborRightIdx   =   create1DPtr(dimension);
    secondNeighborRightIdx  =   create1DPtr(dimension);

    firstNeighborLeftIdx    =   create1DPtr(dimension);
    secondNeighborLeftIdx   =   create1DPtr(dimension);

    // Compute the idx of the neighbor for each position in the array
    idxPeriodicBoudaryCondition(dimension, firstNeighborRightIdx, secondNeighborRightIdx, firstNeighborLeftIdx, secondNeighborLeftIdx);
    displayPtr(dimension,firstNeighborRightIdx);
    displayPtr(dimension,firstNeighborLeftIdx);
    // Define variables to save the current neighbors of a cell
    int firstNeighborRight{}, secondNeighborRight{};
    int firstNeighborLeft{}, secondNeighborLeft{};

    for(size_t t{}; t < time; t++){

        // Define variables to store the frequency of each state over simulation
        int totalStatesA{}, totalStatesB{}, totalStatesC{};

        // Access to each cell of the array to evolve it
        for(size_t cellIdx{}; cellIdx < dimension; cellIdx++){

            // Neighbors of current cell
            firstNeighborRight  =   neighbors[firstNeighborRightIdx[cellIdx]];
            secondNeighborRight =   neighbors[secondNeighborRightIdx[cellIdx]];

            firstNeighborLeft   =   neighbors[firstNeighborLeftIdx[cellIdx]];
            secondNeighborLeft  =   neighbors[secondNeighborLeftIdx[cellIdx]];

            // Evolve the state
            nextStates[cellIdx] = Q2RPottsRule(cellIdx, currentStates, firstNeighborRight, secondNeighborRight, firstNeighborLeft, secondNeighborLeft);
        }

        // Permute currentStates with neighbors and neighbors with nextStates to evolve one step
        reArrangePtr(dimension, currentStates, neighbors, nextStates);

    }

    // Clean the memory used by pointers
    delete[] currentStates; currentStates   = NULL;
    delete[] neighbors;     neighbors       = NULL;
    delete[] nextStates;    nextStates      = NULL;

    return 0;
}
