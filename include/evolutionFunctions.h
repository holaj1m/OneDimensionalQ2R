#ifndef EVOLUTION_FUNCTIONS_H
#define EVOLUTION_FUNCTIONS_H

#include <cstdlib>

// Compute neighbors considering periodic boundary conditions
void periodicBoudaryCondition(const size_t &idxCell, size_t size, size_t &firstNeighborRightIdx, size_t &secondNeighborRightIdx, size_t &firstNeighborLeftIdx, size_t &secondNeighborLeftIdx);

// Rule of evolution for the Q2R-Potts cellular automaton
int Q2RPottsRule(int *ptrStates, int *ptrNeighbors, size_t idxCell, size_t firstNeighborRightIdx, size_t secondNeighborRightIdx, size_t firstNeighborLeftIdx, size_t secondNeighborLeftIdx);

#endif