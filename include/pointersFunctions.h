#ifndef POINTERS_FUNCTIONS_H
#define POINTERS_FUNCTIONS_H

#include <cstdlib>

// Create a one-dimensional pointer
int *create1DPtr(size_t ptrSize);

// Configure initial conditions over pointers
void configureInitialConditions(size_t ptrSize, double densityStatesAB, double densityStatesAC, int *statesPtr, int *neighborsPtr, int *evolutionPtr);
#endif