p = readBinState('period.bin', 'int');
e = readBinState('energy.bin', 'int');
dim = 6;

clA = readBinState('clusterStateA.bin', 'int');
clB = readBinState('clusterStateB.bin', 'int');
clC = readBinState('clusterStateC.bin', 'int');

clA = reshape(clA,dim,[])';
clB = reshape(clB,dim,[])';
clC = reshape(clC,dim,[])';

confStates = readBinState('confNumberStates.bin','int');
confNeighbors = readBinState('confNumberNeighbors.bin','int');
configurations = [confStates confNeighbors];
clear confStates confNeighbors

squareDistStates = readBinState('squareDistStates.bin','int');
squareDistNeighbors = readBinState('squareDistNeigh.bin','int');
squareDist = [squareDistStates squareDistNeighbors];
clear squareDistStates squareDistNeighbors
