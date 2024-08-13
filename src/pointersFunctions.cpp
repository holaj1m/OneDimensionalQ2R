#include <cstdlib>
#include <iostream>


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

// Permutation of matrices to evolve one step
void reArrangePtr(size_t ptrSize, int *statesPtr, int *neighborsPtr, int *evolutionPtr){

    for(size_t i{}; i < ptrSize; i++){
        statesPtr[i]    = neighborsPtr[i];
        neighborsPtr[i] = evolutionPtr[i];
    }
}

// Display pointer
void displayPtr(size_t ptrSize, int *ptr){
    std::cout << "[" << ptr[0] << ", ";
    for(size_t i{1}; i < ptrSize - 1; i++){
        std::cout << ptr[i] << ", ";
    }
    std::cout << ptr[ptrSize-1] << "]" << std::endl;
}

