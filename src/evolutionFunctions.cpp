#include <cstdlib>
#include <vector>

// Compute neighbors index considering periodic boundary conditions
void idxPeriodicBoudaryCondition(const size_t &size, int *neighborRightIdx, int *neighborLeftIdx, int *neighborUpIdx, int *neighborDownIdx){

    // Visit each cell on the state's array
    for(size_t row{}; row < size; row++){

        for(size_t column{}; column < size; column++){
            // Linearize the cell from 2D to 1D
            size_t idxCell = row * size + column;

            // Mod operation to obtain neighbors
            neighborRightIdx[idxCell] = (column + 1) % size;
            neighborLeftIdx[idxCell] = (column + size - 1) % size;

            neighborUpIdx[idxCell] = (row + size - 1) % size;
            neighborDownIdx[idxCell] = (row + 1) % size;
        }
        
    }

}

// Rule of evolution for the Q2R-Potts cellular automaton
int Q2RPottsRule(const size_t &idxCell, int *ptrStates, const int &firstNeighborRight, const int &secondNeighborRight, const int &firstNeighborLeft, const int &secondNeighborLeft){

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
void computeEnergy(const size_t &size, int &energy, int *ptrStates, int *ptrNeighbors, int *ptrneighborRightIdx, int *ptrneighborLeftIdx, int *ptrFirstNeighborLeftIdx, int *ptrSecondNeighborLeftIdx){
    
    int currentState{};
    int firstNeighborRight{}, secondNeighborRight{}, firstNeighborLeft{}, secondNeighborLeft{};

    for(size_t idxCell{}; idxCell < size; idxCell++){
        
        // State in the current cell or index
        currentState = ptrStates[idxCell];

        // Neighbors of current cell
        firstNeighborRight  =   ptrNeighbors[ptrneighborRightIdx[idxCell]];
        secondNeighborRight =   ptrNeighbors[ptrneighborLeftIdx[idxCell]];

        firstNeighborLeft   =   ptrNeighbors[ptrFirstNeighborLeftIdx[idxCell]];
        secondNeighborLeft  =   ptrNeighbors[ptrSecondNeighborLeftIdx[idxCell]];

        if(currentState == firstNeighborRight){energy++;}

        if(currentState == secondNeighborRight){energy++;}

        if(currentState == firstNeighborLeft){energy++;}

        if(currentState == secondNeighborLeft){energy++;}
    }

}

// Add visited idx to the adjacency list
void addAdjacencyList(const size_t &idxCell, const size_t idxVisited, int &clusterDirection, bool *adjListStates){
    
    // Add the cluster in the count on its specific direction
    clusterDirection++;

    // Add the index to the adjacency list
    adjListStates[idxVisited] = false;
}

// Clusters algorithm
void clustering(const size_t &size, const size_t &idxCell, int *ptrStates, int *ptrneighborRightIdx, int *ptrFirstNeighborLeftIdx, bool *adjListStates, int &clusterRight, int &clusterLeft){
    
    // Get the current state
    int currentState = ptrStates[idxCell];
    // Get the first adjacent states in the configuration of states
    int currentStateRight = ptrStates[ptrneighborRightIdx[idxCell]];
    int currentStateLeft  = ptrStates[ptrFirstNeighborLeftIdx[idxCell]];

    // The algorithm consists of checking the different possibilities depending on the current index
    // The special cases are on the borders

    if(idxCell == 0){

        // If the cluster is on the left side, we have to count backwards in the array
        if(currentState == currentStateLeft){

            for(size_t idxCluster{static_cast<size_t>(ptrFirstNeighborLeftIdx[idxCell])}; (idxCluster != idxCell && idxCluster >= 0); idxCluster--){

                if(currentState == ptrStates[idxCluster]){
                    addAdjacencyList(idxCell, idxCluster, clusterLeft, adjListStates);
                }
                // If the state visited is not equal to current state we stop the iteration
                else{break;}
            }
        }

        // If cluster is on the right side we count from the position to the end of the array
        if(currentState == currentStateRight){

            for(size_t idxCluster{static_cast<size_t>(ptrneighborRightIdx[idxCell])}; idxCluster < size; idxCluster++){

                if(currentState == ptrStates[idxCluster]){
                    addAdjacencyList(idxCell, idxCluster, clusterRight, adjListStates);
                }
                // If the state visited is not equal to current state we stop the iteration
                else{break;}
            }
        }
    }

    else{

        // If the cluster is on the left side, we have to count backwards in the array
        if(currentState == currentStateLeft){

            for(size_t idxCluster{static_cast<size_t>(ptrFirstNeighborLeftIdx[idxCell])}; (idxCluster != idxCell && idxCluster >= 0); idxCluster--){

                if(currentState == ptrStates[idxCluster]){
                    addAdjacencyList(idxCell, idxCluster, clusterLeft, adjListStates);
                }
                // If the state visited is not equal to current state we stop the iteration
                else{break;}
            }
        }

        // If cluster is on the right side we count from the position to the end of the array
        if(currentState == currentStateRight){

            for(size_t idxCluster{static_cast<size_t>(ptrneighborRightIdx[idxCell])}; idxCluster < size; idxCluster++){

                if(currentState == ptrStates[idxCluster]){
                    addAdjacencyList(idxCell, idxCluster, clusterRight, adjListStates);
                }
                // If the state visited is not equal to current state we stop the iteration
                else{break;}
            }
        }
    }

}

// Associate the cluster size to a position in an array
void clusterToArray(const size_t &idxCell, int *ptrStates, int *ptrClusterA, int *ptrClusterB, int *ptrClusterC, int &clusterSize){
    switch(ptrStates[idxCell]){
        case -1: ptrClusterA[clusterSize] += 1; break;
        case  0: ptrClusterB[clusterSize] += 1; break;
        default: ptrClusterC[clusterSize] += 1;
    }
}

// Save the total cluster of the state
void addTotalCluster(const size_t &size, const size_t &idxCell, int *ptrStates, int *ptrClusterA, int *ptrClusterB, int *ptrClusterC, int &clusterRight, int &clusterLeft){
    // The cluster's size will indicate the position on the array of clusters
    int clusterSize{};
    if( ((clusterRight != 0) || (clusterLeft != 0)) && (clusterLeft + clusterRight < static_cast<int>(size)) ){
        clusterSize = clusterRight + clusterLeft;
        clusterToArray(idxCell, ptrStates, ptrClusterA, ptrClusterB, ptrClusterC, clusterSize);
    }

    else if( clusterRight == 0 && clusterLeft == 0){
        clusterSize = 0;
        clusterToArray(idxCell, ptrStates, ptrClusterA, ptrClusterB, ptrClusterC, clusterSize);
    }

    else{
        clusterSize = size - 1;
        clusterToArray(idxCell, ptrStates, ptrClusterA, ptrClusterB, ptrClusterC, clusterSize);
    }

    // After setting the cluster size the values of clusters must return to 0
    clusterLeft = 0; clusterRight = 0;
}

