#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cstdio>

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

// Read parameters of the simulation from external file
int extractFrom(std::string parameters, size_t &sizeSystem, int &numConfStates, int &numConfNeighbors){
  std::ifstream in_file;
  std::string line{};
  std::vector <std::string> text_vec{};

  in_file.open(parameters);
  if(!in_file){
    std::cerr << "problem opening" << parameters << std::endl;
    return 1;
  }
  while(getline(in_file,line)){
    text_vec.push_back(line.substr(line.find("=")+2,line.length()-(line.find("=")+2)));
  }
  sizeSystem        = size_t(stoi(text_vec.at(0)));
  numConfStates     = stoi(text_vec.at(1));
  numConfNeighbors  = stoi(text_vec.at(2));
  return 0;
}
