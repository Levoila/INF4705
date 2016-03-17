greedy = 0;
dynamic = 1;
local = 2;

%Useful indices
nbLocations = 2;
maxQuantity = 3;
supplierQuantity = 4;
time = 5;
revenu = 6;

data = csvread('results.csv');
greedyData = data(data(:,1) == greedy,:);
dynamicData = data(data(:,1) == dynamic,:);
localData = data(data(:,1) == local,:);

% Greedy algorithm
disp('Greedy')
R = greedyData(:, revenu);
Y = greedyData(:,time);

averageRevenu = sum(R) / size(R,1);
averageTime = sum(Y) / size(Y,1);

fprintf('Average revenu : %f\n', averageRevenu);
fprintf('Average time : %fs\n\n', averageTime);

hyp = greedyData(:, nbLocations) .* greedyData(:, supplierQuantity) ./ greedyData(:, maxQuantity);
[c, r2] = regression(hyp, Y);

figure();
titre = sprintf('Test des constantes pour confirmer l''hypothese\nsur l''algorithme vorace probabiliste (R2 = %f)', r2);
title(titre);
xlabel('Hypothèse');
ylabel('Temps d''exécution (s)');
hold on;
plot(hyp, Y, 'x');
plot(hyp, c(1)*hyp + c(2));
hold off;

% Dynamic programming algorithm
disp('Dynamic')
R = dynamicData(:, revenu);
Y = dynamicData(:,time);
hyp = dynamicData(:,nbLocations) .* dynamicData(:,supplierQuantity);
[c, r2] = regression(hyp, Y);

averageRevenu = sum(R) / size(R,1);
averageTime = sum(Y) / size(Y,1);

fprintf('Average revenu : %f\n', averageRevenu);
fprintf('Average time : %fs\n\n', averageTime);

figure();
titre = sprintf('Test des constantes pour confirmer l''hypothese sur l''algorithme\nde programmation dynamique (R2 = %f)', r2);
title(titre);
xlabel('Hypothèse');
ylabel('Temps d''exécution (s)');
hold on;
plot(hyp, Y, 'x');
plot(hyp, c(1)*hyp + c(2));
hold off;

% Local optimizations algorithm
disp('Local')
R = localData(:,revenu);
Y = localData(:,time);
P = localData(:,supplierQuantity) ./ localData(:, maxQuantity);
N = localData(:,nbLocations);
hyp = P .* N .* N;
[c, r2] = regression(hyp, Y);

averageRevenu = sum(R) / size(R,1);
averageTime = sum(Y) / size(Y,1);

fprintf('Average revenu : %f\n', averageRevenu);
fprintf('Average time : %fs\n', averageTime);

figure();
titre = sprintf('Test des constantes pour confirmer l''hypothese sur l''algorithme\nd''optimisations locales (R2 = %f)', r2);
title(titre);
xlabel('Hypothèse');
ylabel('Temps d''exécution (s)');
hold on;
plot(hyp, Y, 'x');
plot(hyp, c(1)*hyp + c(2));
hold off;