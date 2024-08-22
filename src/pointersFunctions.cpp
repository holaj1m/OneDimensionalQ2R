#include <cstdlib>
#include <iostream>


// Create an int one-dimensional pointer
int *create1DPtr(size_t ptrSize){

    int *allocateMemory{nullptr};

    allocateMemory = new int[ptrSize];

    return allocateMemory;
}

// Transform from decimal number to ternary base
void decimalToTernary(size_t size, int decimalNumber, int *ptrConf){
    
    int sign = (decimalNumber < 0) ? -1 : 1;
    decimalNumber = std::abs(decimalNumber);

    for(size_t power{}; power < size; power++){

        int remainder = decimalNumber % 3;
        decimalNumber /= 3;

        // Balance the ternary base to -1, 0 and 1
        if (remainder == 2 || remainder == -2) {
            remainder = -1;
            decimalNumber++;
        }

        // Fill the conf. from right to left
        ptrConf[size - 1 - power] = remainder * sign;
    }

    
}

// Configure initial states for adjacency list and clusters
void initialStateAdjListCluster(const size_t &size, bool *ptrAdjList, int *ptrClusterA, int *ptrClusterB, int *ptrClusterC){
    
    for(size_t cellIdx{}; cellIdx < size; cellIdx++){

        ptrAdjList[cellIdx] = true;

        ptrClusterA[cellIdx] = 0;
        ptrClusterB[cellIdx] = 0;
        ptrClusterC[cellIdx] = 0;
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

