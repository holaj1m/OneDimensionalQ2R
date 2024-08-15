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
    double densityStatesAB{0.5};
    double densityStatesAC{0.8};

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

    

    for(size_t t{}; t < time; t++){
        // Variable to test energy conservation
        int energy{};

        // Define variables to store the frequency of each state over simulation
        int totalStatesA{}, totalStatesB{}, totalStatesC{};

        displayPtr(dimension, currentStates);
        displayPtr(dimension, neighbors);

        // Access to each cell of the array to evolve it
        for(size_t cellIdx{}; cellIdx < dimension; cellIdx++){

            // Neighbors of current cell
            firstNeighborRight  =   neighbors[ptrFirstNeighborRightIdx[cellIdx]];
            secondNeighborRight =   neighbors[ptrSecondNeighborRightIdx[cellIdx]];

            firstNeighborLeft   =   neighbors[ptrFirstNeighborLeftIdx[cellIdx]];
            secondNeighborLeft  =   neighbors[ptrSecondNeighborLeftIdx[cellIdx]];

            // Evolve the state
            nextStates[cellIdx] = Q2RPottsRule(cellIdx, currentStates, firstNeighborRight, secondNeighborRight, firstNeighborLeft, secondNeighborLeft);

        }
        displayPtr(dimension, nextStates);
        std::cout << "===============================================\n";

        // Permute currentStates with neighbors and neighbors with nextStates to evolve one step
        reArrangePtr(dimension, currentStates, neighbors, nextStates);

    }

    // Compute the energy
    int energy{};
    computeEnergy(dimension, energy, currentStates, neighbors, ptrFirstNeighborRightIdx, ptrSecondNeighborRightIdx, ptrFirstNeighborLeftIdx, ptrSecondNeighborLeftIdx);
    std::cout << energy << std::endl;

    // Clean the memory used by pointers
    delete[] currentStates; currentStates   = NULL;
    delete[] neighbors;     neighbors       = NULL;
    delete[] nextStates;    nextStates      = NULL;

    return 0;
}
