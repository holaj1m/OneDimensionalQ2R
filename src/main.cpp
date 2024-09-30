#include <iostream>
#include <cstdlib>
#include <cmath> 
#include <string>

#include "../include/pointersFunctions.h"
#include "../include/saveDataFunctions.h"
#include "../include/evolutionFunctions.h"

int main(){
    //===================================================================================================
    //===================================== S T A T E  P O I N T E R S ==================================
    // Size of the square matrix
    size_t matrixSize{3};

    // Number of the configurations
    int numberStates{}, numberNeighbors{};
    // Name of external file with size of the sys and configurations
    std::string extFileParam{"PARAMETERSVAL.txt"};

    // Get size of the system and configuration
    extractFrom(extFileParam, matrixSize, numberStates, numberNeighbors);

    // Size of the 1D system
    size_t dimension{matrixSize * matrixSize};

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
    int *ptrNeighborRight{nullptr}, *ptrNeighborLeft{nullptr};
    int *ptrNeighborUp{nullptr}, *ptrNeighborDown{nullptr};

    // Allocate memory on index pointers
    ptrNeighborRight    =   create1DPtr(dimension);
    ptrNeighborLeft     =   create1DPtr(dimension);

    ptrNeighborUp       =   create1DPtr(dimension);
    ptrNeighborDown     =   create1DPtr(dimension);

    // Compute the idx of the neighbor for each position in the array
    // FUNCTION DESIGNED FOR A 3 X 3 MATRIX
    idxPeriodicBoudaryCondition(matrixSize, ptrNeighborRight, ptrNeighborLeft, ptrNeighborUp, ptrNeighborDown);

    // Define variables to save the current neighbors of a cell
    int neighborRight{}, neighborLeft{};
    int neighborUp{}, neighborDown{};


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

    // EVOLUTION OF THE SYSTEM
    
    while(true){
        // Add a period
        period++;

        // Access to each cell of the array to evolve it
        for(size_t cellIdx{}; cellIdx < dimension; cellIdx++){

            // Neighbors of current cell
            neighborRight  =   neighbors[ptrNeighborRight[cellIdx]];
            neighborLeft =   neighbors[ptrNeighborLeft[cellIdx]];

            neighborUp   =   neighbors[ptrNeighborUp[cellIdx]];
            neighborDown  =   neighbors[ptrNeighborDown[cellIdx]];

            // Evolve cell 
            nextStates[cellIdx] = Q2RPottsRule(cellIdx, currentStates, neighborRight, neighborLeft, neighborUp, neighborDown);

        }

        // Mostrar configuraciones de estados, vecinos y evolucion

        std::cout << "--------------- I T E R A C I O N "<< period << " ---------------" << std:: endl;
        std::cout << "Estados ";
        displayPtr(dimension, currentStates);

        std::cout <<"\nVecinos ";
        displayPtr(dimension, neighbors);

        std::cout <<"\nEvolución ";
        displayPtr(dimension, nextStates);


        // Permute currentStates with neighbors and neighbors with nextStates to evolve one step
        reArrangePtr(dimension, currentStates, neighbors, nextStates);

        // Verify if the cycle was closed
        if(comparePtrs(dimension, initialCondStates, currentStates, initialCondNeigh, neighbors)){
            // Break the loop
            break;
        }

    }

    // Compute the energy
    int energy{};
    computeEnergy(dimension, energy, currentStates, neighbors, ptrNeighborRight, ptrNeighborLeft, ptrNeighborUp, ptrNeighborDown);

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

    // Clean the memory used by pointers
    delete[] currentStates; currentStates   = NULL;
    delete[] neighbors;     neighbors       = NULL;
    delete[] nextStates;    nextStates      = NULL;

    delete[] ptrNeighborRight;  ptrNeighborRight    = NULL;
    delete[] ptrNeighborUp;   ptrNeighborUp     = NULL;
    delete[] ptrNeighborLeft; ptrNeighborLeft   = NULL;
    delete[] ptrNeighborDown;  ptrNeighborDown    = NULL;

    return 0;
}
