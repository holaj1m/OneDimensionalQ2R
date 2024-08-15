#include <cstdlib>
#include <vector>

// Compute neighbors index considering periodic boundary conditions
void idxPeriodicBoudaryCondition(size_t size, int *firstNeighborRightIdx, int *secondNeighborRightIdx, int *firstNeighborLeftIdx, int *secondNeighborLeftIdx){

    // Visit each cell on the state's array
    for(size_t idxCell{}; idxCell < size; idxCell++){
        // Mod operation to obtain neighbors
        firstNeighborRightIdx[idxCell]  =   (idxCell + 1) % size;
        secondNeighborRightIdx[idxCell] =   (idxCell + 2) % size;

        firstNeighborLeftIdx[idxCell]   =   (idxCell + size - 1) % size;
        secondNeighborLeftIdx[idxCell]  =   (idxCell + size - 2) % size;
    }
    

}

// Rule of evolution for the Q2R-Potts cellular automaton
int Q2RPottsRule(size_t idxCell, int *ptrStates, const int &firstNeighborRight, const int &secondNeighborRight, const int &firstNeighborLeft, const int &secondNeighborLeft){

    // Get the current state and its neighbors
    int currentState{ptrStates[idxCell]};

    // Create vector to save neighbors
    std::vector<int> neighborhood{firstNeighborRight, secondNeighborRight, firstNeighborLeft, secondNeighborLeft};

    // Create variables to save the states frequency over neighborhood and the evolution of the state
    int freqStateA{}, freqStateB{}, freqStateC{}, evolutionState{};

    // Counting the frequency of each state over neighborhood
    for(int neighbor:neighborhood){
        switch(neighbor){
            case -1:    freqStateA++; break;
            case 0:     freqStateB++; break;
            default:    freqStateC++; break;
        }
    }

    // Update the evolution state considering the different frequency among neighborhood
    if(freqStateA == 4 || freqStateB == 4 || freqStateC == 4){
        evolutionState = currentState;
    }

    else if(freqStateA < 3 && freqStateB < 3 && freqStateC < 3){

        if(freqStateA == 0 || freqStateB == 0 || freqStateC == 0){

            if(freqStateA == 2 && freqStateB == 2){
                switch(currentState){
                    case -1: evolutionState = 0; break;
                    case 0: evolutionState = -1; break;
                    default: evolutionState = 1; break;
                }
            }

            else if(freqStateA == 2 && freqStateC == 2){
                switch(currentState){
                    case -1: evolutionState = 1; break;
                    case 1: evolutionState = -1; break;
                    default: evolutionState = 0; 
                }
            }

            else if(freqStateB == 2 && freqStateC == 2){
                switch(currentState){
                    case 0: evolutionState = 1; break;
                    case 1: evolutionState = 0; break;
                    default: evolutionState = -1;
                }
            }

        }

        else{
            if(freqStateA == 2){
                switch(currentState){
                    case -1: evolutionState = -1; break;
                    case 0: evolutionState = 1; break;
                    default: evolutionState = 0;
                }
            }

            else if(freqStateB == 2){
                switch(currentState){
                    case 0: evolutionState = 0; break;
                    case 1: evolutionState = -1; break;
                    default: evolutionState = 1; 
                }
            }

            else if(freqStateC == 2){
                switch(currentState){
                    case 1: evolutionState = 1; break;
                    case -1: evolutionState = 0; break;
                    default: evolutionState = -1;
                }
            }

        }

    }
    else{evolutionState = currentState;}

    return evolutionState;
}

//Computation of energy on the system
void computeEnergy(size_t size, int &energy, int *ptrStates, int *ptrNeighbors, int *ptrFirstNeighborRightIdx, int *ptrSecondNeighborRightIdx, int *ptrFirstNeighborLeftIdx, int *ptrSecondNeighborLeftIdx){
    
    int currentState{};
    int firstNeighborRight{}, secondNeighborRight{}, firstNeighborLeft{}, secondNeighborLeft{};

    for(size_t idxCell{}; idxCell < size; idxCell++){
        
        // State in the current cell or index
        int currentState{ptrStates[idxCell]};

        // Neighbors of current cell
        firstNeighborRight  =   ptrNeighbors[ptrFirstNeighborRightIdx[idxCell]];
        secondNeighborRight =   ptrNeighbors[ptrSecondNeighborRightIdx[idxCell]];

        firstNeighborLeft   =   ptrNeighbors[ptrFirstNeighborLeftIdx[idxCell]];
        secondNeighborLeft  =   ptrNeighbors[ptrSecondNeighborLeftIdx[idxCell]];

        if(currentState == firstNeighborRight){energy++;}

        if(currentState == secondNeighborRight){energy++;}

        if(currentState == firstNeighborLeft){energy++;}

        if(currentState == secondNeighborLeft){energy++;}
    }

}


