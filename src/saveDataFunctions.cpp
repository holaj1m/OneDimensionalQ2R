#include<iostream>
#include<fstream>

#include "../include/saveDataFunctions.h"

// Create a new binary file output
FILE* createBinOutput(const char *name){

    FILE* newFile = fopen(name, "wb");
    return newFile;
}

// Verify if the file was open properly
void verifyBinaryOutput(FILE* outFile){

    if(outFile == NULL){
        std::cerr << "Error creating binary output" << std::endl;
    }
}


