vorace, dynamique et local:

-p		: 	Si ce param�tre est pr�sent, le programme va afficher la liste des emplacements choisis
			et le revenu total, en plus du temps d'ex�cution qui est toujours affich�.
	 
-f path : 	Ce param�tre est n�cessaire. Il indique le chemin du fichier de donn�es � utiliser.

-b 		:	Si ce param�tre est pr�sent, les informations sont affich�s d'une mani�re plus simple
			(utile pour lire les donn�es � partir d'un script)
			
-a algo	:	Sert � sp�cifier un algorithme � utiliser. La valeur par d�faut d�pend du programme. 


benchmark.py

Comme son nom l'indique, ce script ex�cute un benchmark sur tous les algorithmes et g�n�re un fichier
de r�sultat results.csv. Si on ne modifie pas les param�tre � l'int�rieur du script, il doit �tre
ex�cut� dans un r�pertoire contenent les programme vorace, dynamique et local et le dossier BTS doit 
�galement �tre pr�sent.


process_data.m

Ce script Matlab requiert que le fichier results.csv soit pr�sent dans le m�me r�pertoire.
Il g�n�re 3 graphiques qui repr�sentent le test des constantes pour chaque algorithme. Il
calcul aussi le temps et le revenu moyen de chaque algorithme.


regression.m

Fonction Matlab utilis�e par le script process_data.m. Elle effectue une r�gression lin�aire
sur les donn�es en entr�e.