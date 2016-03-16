% Greedy algorithm
data = csvread('results_greedy_time.csv');
Y = data(:,5);
hyp = data(:, 2) .* data(:, 4) ./ data(:, 3);
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
data = csvread('results_dynamic_time.csv');
Y = data(:,5);
hyp = data(:,2) .* data(:,4);
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