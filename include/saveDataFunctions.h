#ifndef SAVE_DATA_FUNCTIONS_H
#define SAVE_DATA_FUNCTIONS_H

// Create a new binary file output
FILE* createBinOutput(const char *name);

// Verify if the file was open properly
void verifyBinaryOutput(FILE* outFile);
#endif