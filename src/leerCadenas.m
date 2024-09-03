E5 = readBinState('energiesL5.bin', 'int');
T5 = readBinState('periodsL5.bin', 'int');

E6 = readBinState('energiesL6.bin', 'int');
T6 = readBinState('periodsL6.bin', 'int');

E7 = readBinState('energiesL7.bin', 'int');
T7 = readBinState('periodsL7.bin', 'int');

E8 = readBinState('energiesL8.bin', 'int');
T8 = readBinState('periodsL8.bin', 'int');

E9 = readBinState('energiesL9.bin', 'int');
T9 = readBinState('periodsL9.bin', 'int');

% concatenar elementos hasta L = 8
Et = [E5;E6;E7;E8];
Pt = [T5;T6;T7;T8];

% Obtener elementos unicos de energia y periodo de L = 5 a L = 8
e58 = unique(Et);
t58 = unique(Pt);

% Obtener elementos unicos en L = 9
e9 = unique(E9);
t9 = unique(T9);

% Concatenar todas las cadenas y obtener elementos unicos
energias = unique([e58;e9]);
periodos = unique([t58;t9]);

% Limpiar elementos anteriores
clear e58 e9 t58 t9 Et Pt

% Armar frecuencias de cada energia y periodo
[e5, ~, idxToE5] = unique(E5);
[t5, idxTot5, idxToT5] = unique(T5);

frecE5 = accumarray(idxToE5,1)';
e5 = e5';
energyFrec5 = [e5; frecE5];

periodT5 = accumarray(idxToT5,1)';
t5 = t5';
periodFrec5 = [t5; periodT5];
clear e5 frecE5 idxToE5 idxToT5

[e6, ~, idxToE6] = unique(E6);
[t6, ~, idxToT6] = unique(T6);

frecE6 = accumarray(idxToE6,1)';
e6 = e6';
energyFrec6 = [e6; frecE6];

periodT6 = accumarray(idxToT6,1)';
t6 = t6';
periodFrec6 = [t6; periodT6];
clear e6 t6 frecE6 idxToE6 idxToT6

[e7, ~, idxToE7] = unique(E7);
[t7, ~, idxToT7] = unique(T7);

frecE7 = accumarray(idxToE7,1)';
e7 = e7';
energyFrec7 = [e7; frecE7];

periodT7 = accumarray(idxToT7,1)';
t7 = t7';
periodFrec7 = [t7; periodT7];
clear e7 t7 frecE7 idxToE7 idxToT7 E7 T7

[e8, ~, idxToE8] = unique(E8);
[t8, ~, idxToT8] = unique(T8);

frecE8 = accumarray(idxToE8,1)';
e8 = e8';
energyFrec8 = [e8; frecE8];

periodT8 = accumarray(idxToT8,1)';
t8 = t8';
periodFrec8 = [t8; periodT8];
clear e8 t8 frecE8 idxToE8 idxToT8 E8 T8

[e9, ~, idxToE9] = unique(E9);
[t9, ~, idxToT9] = unique(T9);

frecE9 = accumarray(idxToE9,1)';
e9 = e9';
energyFrec9 = [e9; frecE9];

periodT9 = accumarray(idxToT9,1)';
t9 = t9';
periodFrec9 = [t9; periodT9];
clear e9 frecE9 idxToE9 idxToT9

% ENERGIAS
dataE = zeros(5+1,length(energias)+1);
dataE(1,1) = NaN;
dataE(2:end,1) = 5:9;
dataE(1,2:end) = energias;

[~,idx] = ismember(energyFrec5(1,:),dataE(1,:));
dataE(2,idx) = energyFrec5(2,:);

[~,idx] = ismember(energyFrec6(1,:),dataE(1,:));
dataE(3,idx) = energyFrec6(2,:);

[~,idx] = ismember(energyFrec7(1,:),dataE(1,:));
dataE(4,idx) = energyFrec7(2,:);

[~,idx] = ismember(energyFrec8(1,:),dataE(1,:));
dataE(5,idx) = energyFrec8(2,:);

[~,idx] = ismember(energyFrec9(1,:),dataE(1,:));
dataE(6,idx) = energyFrec9(2,:);

% PERIODOS
dataT = zeros(5+1,length(periodos)+1);
dataT(1,1) = NaN;
dataT(2:end,1) = 5:9;
dataT(1,2:end) = periodos;

[~,idx] = ismember(periodFrec5(1,:),dataT(1,:));
dataT(2,idx) = periodFrec5(2,:);

[~,idx] = ismember(periodFrec6(1,:),dataT(1,:));
dataT(3,idx) = periodFrec6(2,:);

[~,idx] = ismember(periodFrec7(1,:),dataT(1,:));
dataT(4,idx) = periodFrec7(2,:);

[~,idx] = ismember(periodFrec8(1,:),dataT(1,:));
dataT(5,idx) = periodFrec8(2,:);

[~,idx] = ismember(periodFrec9(1,:),dataT(1,:));
dataT(6,idx) = periodFrec9(2,:);
