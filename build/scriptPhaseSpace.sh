#!/bin/bash
LANG=en_US
###############################################

# Minimum size of the chain for this process
MINSIZE=5

# Maximum size of the chain for this process
MAXSIZE=5

# Number of cores to use 
NUM_CORES=14

# Variable to handle concurrency
l=0

# Create different folders for each size of the system
for (( k=$MINSIZE; k <= $MAXSIZE; k++ )); do
    # Dimension of the system
    DIME=$((k))
    # Name of the folder
    nameDime='SPACE_SIZE_'
    direDime=$(printf "${nameDime}%03d" $k)
    # Create directory
    mkdir -p $direDime
    # Copy the executable program
    cp a.out ${direDime}
    # Copy the parameters for the simulation
    cp parameters.XX ${direDime}
    # Enter in the directory
    cd ${direDime}

    # Compute the half of the number of configurations
    maxConfig=$(( (3**k - 1)/2 ))
    minConfig=$((-maxConfig))

    # Create folders corresponding to each configuration
    for (( i=$minConfig; i <= $maxConfig; i++ )); do
        for (( j=$minConfig; j <= $maxConfig; j++ )); do

            # Negatives configurations will have a prefix of m and positive a prefix of p
            if [ $i -lt 0 ]; then
                i_prefix="m"
                i_value=$((-$i)) 
            elif [ $i -gt 0 ]; then
                i_prefix="p"
                i_value=$i
            else
                i_prefix=""
                i_value=0
            fi

            if [ $j -lt 0 ]; then
                j_prefix="m"
                j_value=$((-$j))  
            elif [ $j -gt 0 ]; then
                j_prefix="p"
                j_value=$j
            else
                j_prefix=""
                j_value=0
            fi

            # Crea el nombre de la carpeta de configuración
            nameConf='cnf_'
            dirFolder=$(printf "${nameConf}${i_prefix}${i_value}_${j_prefix}${j_value}")
            CONFI=$((i))
            CONFJ=$((j))
            mkdir -p $dirFolder
            # Copy the executable program
            cp a.out ${dirFolder}
            # Copy the parameters for the simulation
            cp parameters.XX ${dirFolder}
            sed -e "s/DIME/$DIME/g" -e "s/CONFI/$CONFI/g" -e "s/CONFJ/$CONFJ/g" parameters.XX > ${dirFolder}/PARAMETERSVAL.txt
            # Enter in the directory
            cd ${dirFolder}
            
            # Use only NUM_CORES to run the program
            ((l=l%NUM_CORES))
            ((l++==0)) && wait
            # Run program in parallel
            nohup ./a.out& > timecpu.txt
             
            cd ..
        done
    done
    cd ..
done
