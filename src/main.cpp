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

    // Variables to store square of distances between configurations
    int squareStateDist{}, squareNeighDist{};

    // Variables to store the transformation of the conf. from ternary to decimal
    int decStates{}, decNeighbors{};

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

    //===========================================================================================
    //===================================== C L U S T E R ======================================= 

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

    //============================================================================================
    //================================ F I L E S ===========================
    
    // Binary files to save the distances
    FILE* distanceStatesFile    = createBinOutput("squareDistStates.bin");
    FILE* distanceNeighborsFile = createBinOutput("squareDistNeigh.bin");

    // Binary files to save the evolution of configuration in decimal numbers
    FILE* confNumberStatesFile      = createBinOutput("confNumberStates.bin");
    FILE* confNumberNeighborsFile   = createBinOutput("confNumberNeighbors.bin");

    // Binary files to save the evolution of clusters in time
    FILE* clusterStateAFile = createBinOutput("clusterStateA.bin");
    FILE* clusterStateBFile = createBinOutput("clusterStateB.bin");
    FILE* clusterStateCFile = createBinOutput("clusterStateC.bin");

    // Verify outputs
    verifyBinaryOutput(distanceStatesFile);
    verifyBinaryOutput(distanceNeighborsFile);
    verifyBinaryOutput(confNumberStatesFile);
    verifyBinaryOutput(confNumberNeighborsFile);
    verifyBinaryOutput(clusterStateAFile);
    verifyBinaryOutput(clusterStateBFile);
    verifyBinaryOutput(clusterStateCFile);


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
            squareStateDist += statesDist[cellIdx] * statesDist[cellIdx];
            squareNeighDist += neighborsDist[cellIdx] * neighborsDist[cellIdx];

            // Get the configuration in decimal form
            decStates       += currentStates[cellIdx] * pow(3,dimension - 1 - cellIdx);
            decNeighbors    += neighbors[cellIdx] * pow(3,dimension - 1 - cellIdx);

        }

        // Permute currentStates with neighbors and neighbors with nextStates to evolve one step
        reArrangePtr(dimension, currentStates, neighbors, nextStates);

        // ============ S A V I N G  D A T A ==============
        // Save the number of configuration in files
        fwrite(&decStates, sizeof(int), 1, confNumberStatesFile);
        fwrite(&decNeighbors, sizeof(int), 1, confNumberNeighborsFile);

        // Distances of between configurations
        fwrite(&squareStateDist, sizeof(int), 1, distanceStatesFile);
        fwrite(&squareNeighDist, sizeof(int), 1, distanceNeighborsFile);

        // Clusters
        fwrite(clusterStateA, sizeof(int), dimension, clusterStateAFile);
        fwrite(clusterStateB, sizeof(int), dimension, clusterStateBFile);
        fwrite(clusterStateC, sizeof(int), dimension, clusterStateCFile);

        // Verify if the cycle was closed
        if(comparePtrs(dimension, initialCondStates, currentStates, initialCondNeigh, neighbors)){
            // Close the files
            fclose(confNumberStatesFile);
            fclose(confNumberNeighborsFile);
            fclose(distanceStatesFile);
            fclose(distanceNeighborsFile);
            fclose(clusterStateAFile);
            fclose(clusterStateBFile);
            fclose(clusterStateCFile);

            // Break the loop
            break;
        }

        // Set the conditions properly for another iteration
        squareStateDist = 0; squareNeighDist = 0;
        decStates = 0; decNeighbors = 0;
        initialStateAdjListCluster(dimension, adjListStates, clusterStateA, clusterStateB, clusterStateC);
    }

    // Compute the energy
    int energy{};
    computeEnergy(dimension, energy, currentStates, neighbors, ptrFirstNeighborRightIdx, ptrSecondNeighborRightIdx, ptrFirstNeighborLeftIdx, ptrSecondNeighborLeftIdx);

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
