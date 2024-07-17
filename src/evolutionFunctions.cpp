#include <cstdlib>
#include <vector>

// Compute neighbors index considering periodic boundary conditions
void periodicBoudaryCondition(const size_t &idxCell, size_t size, size_t &firstNeighborRightIdx, size_t &secondNeighborRightIdx, size_t &firstNeighborLeftIdx, size_t &secondNeighborLeftIdx){

    // Mod operation to obtain neighbors
    firstNeighborRightIdx  =   (idxCell + 1) % size;
    secondNeighborRightIdx =   (idxCell + 2) % size;

    firstNeighborLeftIdx   =   (idxCell + size - 1) % size;
    secondNeighborLeftIdx  =   (idxCell + size - 2) % size;

}

// Rule of evolution for the Q2R-Potts cellular automaton
int Q2RPottsRule(int *ptrStates, int *ptrNeighbors, size_t idxCell, size_t firstNeighborRightIdx, size_t secondNeighborRightIdx, size_t firstNeighborLeftIdx, size_t secondNeighborLeftIdx){

    // Get the current state and its neighbors
    int currentState{ptrStates[idxCell]}, firstNeighborRight{ptrNeighbors[firstNeighborRightIdx]}, secondNeighborRight{ptrNeighbors[secondNeighborRightIdx]};
    int firstNeighborLeft{ptrNeighbors[firstNeighborLeftIdx]}, secondNeighborLeft{ptrNeighbors[secondNeighborLeftIdx]};

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




