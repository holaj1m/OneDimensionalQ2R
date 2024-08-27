#ifndef SAVE_DATA_FUNCTIONS_H
#define SAVE_DATA_FUNCTIONS_H

// Create a new binary file output
FILE* createBinOutput(const char *name);

// Verify if the file was open properly
void verifyBinaryOutput(FILE* outFile);
// Read parameters of the simulation from external file
int extractFrom(std::string parameters, size_t &sizeSystem, int &numConfStates, int &numConfNeighbors);

#endif