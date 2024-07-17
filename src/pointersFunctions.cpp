#include <cstdlib>

// Create a one-dimensional pointer
int *create1DPtr(size_t ptrSize){

    int *allocateMemory{nullptr};

    allocateMemory = new int[ptrSize];

    return allocateMemory;
}

// Configure initial conditions over pointers
void configureInitialConditions(size_t ptrSize, double densityStatesAB, double densityStatesAC, int *statesPtr, int *neighborsPtr, int *evolutionPtr){
    
    // Create a variable that will determines if the state is or not zero
    int selectZero{};

    for(size_t i{} ; i < ptrSize ; i++){

        // Initialize ptr of states randomly considering states density

        selectZero = 1- int(drand48() + densityStatesAB);
        if(selectZero == 0){statesPtr[i] = 0;}
        else{statesPtr[i] = 1 - 2*int(drand48() + densityStatesAC);}

        // As initial condition we impose that neighbors are equal to states for the first step

        neighborsPtr[i] = statesPtr[i];

        // Finally replace the garbage on evolution buffer

        evolutionPtr[i] = statesPtr[i];
    }
}