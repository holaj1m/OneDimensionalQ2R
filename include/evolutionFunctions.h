#ifndef EVOLUTION_FUNCTIONS_H
#define EVOLUTION_FUNCTIONS_H

#include <cstdlib>

// Compute neighbors considering periodic boundary conditions
void idxPeriodicBoudaryCondition(const size_t &size, int *firstNeighborRightIdx, int *secondNeighborRightIdx, int *firstNeighborLeftIdx, int *secondNeighborLeftIdx);

// Rule of evolution for the Q2R-Potts cellular automaton
int Q2RPottsRule(const size_t &idxCell, int *ptrStates, const int &firstNeighborRight, const int &secondNeighborRight, const int &firstNeighborLeft, const int &secondNeighborLeft);

// Computation of the energy on the system
void computeEnergy(const size_t &size, int &energy, int *ptrStates, int *ptrNeighbors, int *ptrFirstNeighborRightIdx, int *ptrSecondNeighborRightIdx, int *ptrFirstNeighborLeftIdx, int *ptrSecondNeighborLeftIdx);

// Add visited idx to the adjacency list
void addAdjacencyList(const size_t &idxCell, const size_t idxVisited, int &clusterDirection, bool *adjListStates);

// Clusters algorithm
void clustering(const size_t &size, const size_t &idxCell, int *ptrStates, int *ptrFirstNeighborRightIdx, int *ptrFirstNeighborLeftIdx, bool *adjListStates, int &clusterRight, int &clusterLeft);

// Associate the cluster size to a position in an array
void clusterToArray(const size_t &idxCell, int *ptrStates, int *ptrClusterA, int *ptrClusterB, int *ptrClusterC, int &clusterSize);

// Save the total cluster of the state
void addTotalCluster(const size_t &size, const size_t &idxCell, int *ptrStates, int *ptrClusterA, int *ptrClusterB, int *ptrClusterC, int &clusterRight, int &clusterLeft);



#endif