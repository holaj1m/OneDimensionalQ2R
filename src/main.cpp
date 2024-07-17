#include <iostream>
#include <cstdlib>

#include "../include/pointersFunctions.h"

//DEFINE drand48 TO OBTAIN A RANDOM NUMBER
#define drand48() ((float)rand()/(float)RAND_MAX)
int main(){

    // Size of the system and total numer of iterations
    size_t dimension{}, time{};

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
    double densityStatesAB{};
    double densityStatesAC{};

    // Set the initial conditions
    configureInitialConditions(dimension, densityStatesAB, densityStatesAC,currentStates, neighbors, nextStates);

    return 0;
}
