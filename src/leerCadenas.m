E = readBinState('energies.bin', 'int');
T = readBinState('periods.bin', 'int');

[e, idxToe, idxToE] = unique(E);

frecE = accumarray(idxToE,1)';
e = e';

energyFrec = [e; frecE];
