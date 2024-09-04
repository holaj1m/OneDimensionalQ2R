#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cstdio>
#include <filesystem>

namespace fs = std::filesystem;


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

void crearDirectorio(int var1, int var2){
  std::string nombreDirectorio;

    if (var1 < 0 && var2 < 0) {
        nombreDirectorio = "cnf_m" + std::to_string(abs(var1)) + "_m" + std::to_string(abs(var2));
    } else if (var1 > 0 && var2 > 0) {
        nombreDirectorio = "cnf_p" + std::to_string(var1) + "_p" + std::to_string(var2);
    } else if (var1 == 0 && var2 == 0) {
        nombreDirectorio = "cnf_0_0";
    } else if (var1 == 0) {
        if (var2 < 0) {
            nombreDirectorio = "cnf_0_m" + std::to_string(abs(var2));
        } else {
            nombreDirectorio = "cnf_0_p" + std::to_string(var2);
        }
    } else if (var2 == 0) {
        if (var1 < 0) {
            nombreDirectorio = "cnf_m" + std::to_string(abs(var1)) + "_0";
        } else {
            nombreDirectorio = "cnf_p" + std::to_string(var1) + "_0";
        }
    } else {
        nombreDirectorio = "cnf_" + std::to_string(var1) + "_" + std::to_string(var2);
    }

    // Crear el directorio
    if (!fs::exists(nombreDirectorio)) {
        fs::create_directory(nombreDirectorio);
        std::cout << "Directorio creado: " << nombreDirectorio << std::endl;
    } else {
        std::cout << "El directorio ya existe: " << nombreDirectorio << std::endl;
    }

    // Cambiar al directorio recién creado
    fs::current_path(nombreDirectorio);
}
