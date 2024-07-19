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
    FILE *countStateA = createBinOutput("countStateA.bin");
    FILE *countStateB = createBinOutput("countStateB.bin");
    FILE *countStateC = createBinOutput("countStateC.bin");

    // Verify if the files were opened properly
    verifyBinaryOutput(countStateA);
    verifyBinaryOutput(countStateB);
    verifyBinaryOutput(countStateC);

    //=============================================================================================================
    //===================================== E V O L V E  T H E  S Y S T E M ======================================= 

    // Define variable to save the position of the neighbors of a certain cell during evolution
    size_t firstNeighborRightIdx{}, secondNeighborRightIdx{};
    size_t firstNeighborLeftIdx{}, secondNeighborLeftIdx{};
    for(size_t t{}; t < time; t++){

        // Define variables to store the frequency of each state over simulation
        int totalStatesA{}, totalStatesB{}, totalStatesC{};

        // Access to each cell of the array to evolve it
        for(size_t cellIdx{}; cellIdx < dimension; cellIdx++){

            // Apply periodic boundary condition to the system
            periodicBoudaryCondition(cellIdx, dimension, firstNeighborRightIdx, secondNeighborRightIdx, firstNeighborLeftIdx, secondNeighborLeftIdx);

            // Evolve the state
            nextStates[cellIdx] = Q2RPottsRule(currentStates, neighbors, cellIdx, firstNeighborRightIdx, secondNeighborRightIdx, firstNeighborLeftIdx, secondNeighborLeftIdx);
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
