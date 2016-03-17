vorace, dynamique et local:

-p		: 	Si ce paramètre est présent, le programme va afficher la liste des emplacements choisis
			et le revenu total, en plus du temps d'exécution qui est toujours affiché.
	 
-f path : 	Ce paramètre est nécessaire. Il indique le chemin du fichier de données à utiliser.

-b 		:	Si ce paramètre est présent, les informations sont affichés d'une manière plus simple
			(utile pour lire les données à partir d'un script)
			
-a algo	:	Sert à spécifier un algorithme à utiliser. La valeur par défaut dépend du programme. 


benchmark.py

Comme son nom l'indique, ce script exécute un benchmark sur tous les algorithmes et génère un fichier
de résultat results.csv. Si on ne modifie pas les paramètre à l'intérieur du script, il doit être
exécuté dans un répertoire contenent les programme vorace, dynamique et local et le dossier BTS doit 
également être présent.


process_data.m

Ce script Matlab requiert que le fichier results.csv soit présent dans le même répertoire.
Il génère 3 graphiques qui représentent le test des constantes pour chaque algorithme. Il
calcul aussi le temps et le revenu moyen de chaque algorithme.


regression.m

Fonction Matlab utilisée par le script process_data.m. Elle effectue une régression linéaire
sur les données en entrée.