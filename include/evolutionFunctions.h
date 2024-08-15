#ifndef EVOLUTION_FUNCTIONS_H
#define EVOLUTION_FUNCTIONS_H

#include <cstdlib>

// Compute neighbors considering periodic boundary conditions
void idxPeriodicBoudaryCondition(size_t size, int *firstNeighborRightIdx, int *secondNeighborRightIdx, int *firstNeighborLeftIdx, int *secondNeighborLeftIdx);

// Rule of evolution for the Q2R-Potts cellular automaton
int Q2RPottsRule(size_t idxCell, int *ptrStates, const int &firstNeighborRight, const int &secondNeighborRight, const int &firstNeighborLeft, const int &secondNeighborLeft);

// Computation of the energy on the system
void computeEnergy(size_t size, int &energy, int *ptrStates, int *ptrNeighbors, int *ptrFirstNeighborRightIdx, int *ptrSecondNeighborRightIdx, int *ptrFirstNeighborLeftIdx, int *ptrSecondNeighborLeftIdx);

#endif