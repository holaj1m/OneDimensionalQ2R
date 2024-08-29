% PROCEDURAL SCRIPT

% create a structure with folders
PS=dir('SPACE_*');
phaseSpace = struct();
for i=1:length(PS)
    % enter into the folder
    cd(PS(i).name);
    dimSystem = PS(i).name;
    dimSystem = str2double(dimSystem(end));
    % create a structure with the name of all configurations
    CNF=dir('cnf_*');

    % visit every configuration
    for j=1:length(CNF)
        % get into the folder of the curren configuration
        cd(CNF(j).name);

        % Save the data associated to the configuration
        % Period
        phaseSpace(i).(CNF(j).name).period = readBinState('period.bin', 'int');

        % Energy
        phaseSpace(i).(CNF(j).name).energy = readBinState('energy.bin', 'int');

        % Read the evolution of configuration
        confStates = readBinState('confNumberStates.bin','int');
        confNeighbors = readBinState('confNumberNeighbors.bin','int');
        % Save the configuration in the structure
        phaseSpace(i).(CNF(j).name).evolution = [confStates confNeighbors];
        clear confStates confNeighbors

        % Read the euclidean distance
        squareDistStates = readBinState('squareDistStates.bin','int');
        squareDistNeighbors = readBinState('squareDistNeigh.bin','int');
        % Save the square of distance in the structure
        phaseSpace(i).(CNF(j).name).squareDist = [squareDistStates squareDistNeighbors];
        clear squareDistStates squareDistNeighbors

        % Read clusters
        clA = readBinState('clusterStateA.bin', 'int');
        clB = readBinState('clusterStateB.bin', 'int');
        clC = readBinState('clusterStateC.bin', 'int');
        % Rearrenge data properly
        clA = reshape(clA,dimSystem,[])';
        clB = reshape(clB,dimSystem,[])';
        clC = reshape(clC,dimSystem,[])';
        % find the unique clusters on the evolution
        uniqueClA = unique(clA,"rows");
        uniqueClB = unique(clB,"rows");
        uniqueClC = unique(clC,"rows");

        % Save clusters in the structure
        phaseSpace(i).(CNF(j).name).clA = clA;
        phaseSpace(i).(CNF(j).name).uniqueClA = uniqueClA;
        
        phaseSpace(i).(CNF(j).name).clB = clB;
        phaseSpace(i).(CNF(j).name).uniqueClB = uniqueClB;

        phaseSpace(i).(CNF(j).name).clC = clC;
        phaseSpace(i).(CNF(j).name).uniqueClC = uniqueClC;


        
        cd ..
    end
    cd ..
end
%%
% PARALLEL SCRIPT

tic
cd SPACE_SIZE_006/
% Get the full path of the current folder
spaceFolderPath = fullfile(pwd);

% Get the configurations within the current folder
CNF = dir(fullfile(spaceFolderPath, 'cnf_*'));
numConfigurations = length(CNF);

% Dimension of the considered chain
dimSystem = 6;

% Preallocate a cell array to store the results of each iteration
phaseSpaceTemp = cell(1, numConfigurations);

parfor j = 1:numConfigurations
    % Get the full path of the current configuration
    configFolderPath = fullfile(spaceFolderPath, CNF(j).name);

    % Create a temporary structure to store the data of this configuration
    tempStruct = struct();

    % Read and save the data
    tempStruct.period = readBinState(fullfile(configFolderPath, 'period.bin'), 'int');
    tempStruct.energy = readBinState(fullfile(configFolderPath, 'energy.bin'), 'int');

    % Read the evolution of the configuration
    confStates = readBinState(fullfile(configFolderPath, 'confNumberStates.bin'), 'int');
    confNeighbors = readBinState(fullfile(configFolderPath, 'confNumberNeighbors.bin'), 'int');
    tempStruct.evolution = [confStates confNeighbors];

    % Read the Euclidean distance
    squareDistStates = readBinState(fullfile(configFolderPath, 'squareDistStates.bin'), 'int');
    squareDistNeighbors = readBinState(fullfile(configFolderPath, 'squareDistNeigh.bin'), 'int');
    tempStruct.squareDist = [squareDistStates squareDistNeighbors];

    % Read the clusters
    clA = readBinState(fullfile(configFolderPath, 'clusterStateA.bin'), 'int');
    clB = readBinState(fullfile(configFolderPath, 'clusterStateB.bin'), 'int');
    clC = readBinState(fullfile(configFolderPath, 'clusterStateC.bin'), 'int');

    % Rearrange the data properly
    clA = reshape(clA, dimSystem, [])';
    clB = reshape(clB, dimSystem, [])';
    clC = reshape(clC, dimSystem, [])';

    % Find the unique clusters in the evolution
    tempStruct.clA = clA;
    tempStruct.uniqueClA = unique(clA, "rows");
    tempStruct.clB = clB;
    tempStruct.uniqueClB = unique(clB, "rows");
    tempStruct.clC = clC;
    tempStruct.uniqueClC = unique(clC, "rows");

    % Save the temporary structure in the corresponding cell
    phaseSpaceTemp{j} = tempStruct;
end

% Combine all the temporary structures into the final structure
for j = 1:numConfigurations
    phaseSpace.(CNF(j).name) = phaseSpaceTemp{j};
end

cd ..
toc

%%
% Getting some info from the structure

names = fieldnames(phaseSpace);
totalConf = length(names);
energy = zeros(totalConf,1);
period = energy;
configuration = zeros(totalConf,2);
for i=1:totalConf
    energy(i,1) = phaseSpace.(names{i}).energy;
    period(i,1) = phaseSpace.(names{i}).period;
    tempEvo = phaseSpace.(names{i}).evolution;
    configuration(i,:) = tempEvo(1,:);
end

