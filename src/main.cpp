#include <iostream>
#include <cstdlib>
#include <cmath> 
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

#include "../include/pointersFunctions.h"
#include "../include/saveDataFunctions.h"
#include "../include/evolutionFunctions.h"

int main(){
    //===================================================================================================
    //===================================== S T A T E  P O I N T E R S ==================================
    // Size of the system
    size_t dimension{};
    
    // The number of the current configurations
    int numberStates{}, numberNeighbors{};

    // Name of external file with size of the sys and configurations
    std::string extFileParam{"PARAMETERSVAL.txt"};

    // Get size of the system and configuration
    extractFrom(extFileParam, dimension, numberStates, numberNeighbors);

    // Pointers to allocate states of the system
    int *currentStates{nullptr}; // pointer to the current states
    int *neighbors{nullptr}; // pointer to the neighbours
    int *nextStates{nullptr}; // Buffer that point to the future states

    int *initialCondStates{nullptr}; // Initial states of the system
    int *initialCondNeigh{nullptr}; // Initial neighbors of the system

    // Allocate memory on pointers
    currentStates   =   create1DPtr(dimension);
    neighbors       =   create1DPtr(dimension);
    nextStates      =   create1DPtr(dimension);

    initialCondStates = create1DPtr(dimension);
    initialCondNeigh  = create1DPtr(dimension);

    //===================================================================================
    //===================================== P B C ======================================= 

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

    //============================================================================================
    //================================ E V O L V E  T H E  S Y S T E M =========================== 
    
    // Set the initial condition for the system
    decimalToTernary(dimension, numberStates, currentStates);
    decimalToTernary(dimension, numberNeighbors, neighbors);

    // Store the initial condition in the system
    decimalToTernary(dimension, numberStates, initialCondStates);
    decimalToTernary(dimension, numberNeighbors, initialCondNeigh);

    // Variable that count the period of the configuration
    int period{};

    // Compute the energy
    int energy{};
    computeEnergy(dimension, energy, currentStates, neighbors, ptrFirstNeighborRightIdx, ptrSecondNeighborRightIdx, ptrFirstNeighborLeftIdx, ptrSecondNeighborLeftIdx);


    // EVOLUTION OF THE SYSTEM
    
    while(true){
        // Add a period
        period++;

        // Access to each cell of the array to evolve it
        for(size_t cellIdx{}; cellIdx < dimension; cellIdx++){

            // Neighbors of current cell
            firstNeighborRight  =   neighbors[ptrFirstNeighborRightIdx[cellIdx]];
            secondNeighborRight =   neighbors[ptrSecondNeighborRightIdx[cellIdx]];

            firstNeighborLeft   =   neighbors[ptrFirstNeighborLeftIdx[cellIdx]];
            secondNeighborLeft  =   neighbors[ptrSecondNeighborLeftIdx[cellIdx]];

            // Evolve cell 
            nextStates[cellIdx] = Q2RPottsRule(cellIdx, currentStates, firstNeighborRight, secondNeighborRight, firstNeighborLeft, secondNeighborLeft);

        }

        // Permute currentStates with neighbors and neighbors with nextStates to evolve one step
        reArrangePtr(dimension, currentStates, neighbors, nextStates);

        // Verify if the cycle was closed
        if(comparePtrs(dimension, initialCondStates, currentStates, initialCondNeigh, neighbors)){

            if(period == 1){
                crearDirectorio(numberStates,numberNeighbors);
                // Binary file to store the energy of the configuration
                FILE* energyFile = createBinOutput("energy.bin");
                verifyBinaryOutput(energyFile);

                // Binary File to store the total period
                FILE* periodFile = createBinOutput("period.bin");
                verifyBinaryOutput(periodFile);

                // Write the period and energy on the files
                fwrite(&energy, sizeof(int),1, energyFile);
                fwrite(&period, sizeof(int), 1, periodFile);

                // Close the files
                fclose(energyFile);
                fclose(periodFile);

                // Binary files to save the evolution of configuration in ternary form
                FILE* confStatesFile      = createBinOutput("statesConf.bin");
                FILE* confNeighborsFile   = createBinOutput("neighborsConf.bin");

                verifyBinaryOutput(confStatesFile);
                verifyBinaryOutput(confNeighborsFile);
                fwrite(initialCondStates, sizeof(int), dimension, confStatesFile);
                fwrite(initialCondNeigh, sizeof(int), dimension, confNeighborsFile);
                fclose(confStatesFile);
                fclose(confNeighborsFile);
                
                fs::current_path("..");

            }

            // Break the loop
            break;
        }

    }

    
    

    // Clean the memory used by pointers
    delete[] currentStates; currentStates   = NULL;
    delete[] neighbors;     neighbors       = NULL;
    delete[] nextStates;    nextStates      = NULL;

    delete[] ptrFirstNeighborRightIdx;  ptrFirstNeighborRightIdx    = NULL;
    delete[] ptrFirstNeighborLeftIdx;   ptrFirstNeighborLeftIdx     = NULL;
    delete[] ptrSecondNeighborRightIdx; ptrSecondNeighborRightIdx   = NULL;
    delete[] ptrSecondNeighborLeftIdx;  ptrSecondNeighborLeftIdx    = NULL;

    return 0;
}
