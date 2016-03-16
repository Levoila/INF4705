greedy = 0;
dynamic = 1;
local = 2;

%Useful indices
nbLocations = 2;
maxRevenu = 3;
maxQuantity = 3;
supplierQuantity = 4;
time = 5;
revenu = 6;

data = csvread('results.csv');
greedyData = data(data(:,1) == greedy,:);
dynamicData = data(data(:,1) == dynamic,:);
localData = data(data(:,1) == local,:);

% Greedy algorithm
Y = greedyData(:,time);
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
Y = dynamicData(:,time);
hyp = dynamicData(:,nbLocations) .* dynamicData(:,supplierQuantity);
[c, r2] = regression(hyp, Y);

figure();
titre = sprintf('Test des constantes pour confirmer l''hypothese sur l''algorithme\nde programmation dynamique probabiliste (R2 = %f)', r2);
title(titre);
xlabel('Hypothèse');
ylabel('Temps d''exécution (s)');
hold on;
plot(hyp, Y, 'x');
plot(hyp, c(1)*hyp + c(2));
hold off;