#ifndef POINTERS_FUNCTIONS_H
#define POINTERS_FUNCTIONS_H

#include <cstdlib>

// Create a one-dimensional pointer
int *create1DPtr(size_t ptrSize);

// Transform from decimal number to ternary base
void decimalToTernary(size_t size, int decimalNumber, int *ptrConf);

// Configure initial states for adjacency list and clusters
void initialStateAdjListCluster(const size_t &size, bool *ptrAdjList, int *ptrClusterA, int *ptrClusterB, int *ptrClusterC);

// Permutation of pointers to evolve one step
void reArrangePtr(size_t ptrSize, int *statesPtr, int *neighborsPtr, int *evolutionPtr);

// Display pointer
void displayPtr(size_t ptrSize, int *ptr);

// Compare the elements of two pointers
bool comparePtrs(size_t size, int *ptr1, int *ptr2, int *ptr3, int *ptr4);

#endif