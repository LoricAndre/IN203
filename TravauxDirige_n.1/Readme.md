# Initiation à MPI

Ce TP permet une initiation aux commandes de base de MPI.

##Rappels essentiels :
1. Pour lancer une exécution parallèle avec MPI :

mpiexec -np xx ./nom_executable

où xx est le nombre de processus qui lanceront simultanément l'exécutable.

2. Utiliser le Makefile fourni avec le TP, cela vous évitera (principalement pour ceux utilisant MSYS 2) une fastidieuse ligne de commande.

3. Pour ceux ayant installé MSYS 2, bien penser à commenter dans le Makefile la ligne

include Make.inc

et décommenter la ligne

include Make_msys2.inc

Pour ceux ayant un Mac, penser également à commenter la ligne

include Make.inc

et décommenter la ligne

include Make_osx.inc

4. Utiliser les fichiers SkeletonMPIProgram.cpp ou SkeletonMPIProgramWithFilesOutput.cpp comme canevas pour commencer votre programme MPI (conserver ces squelettes pour les prochaines sessions, ils seront utiles).

5. Lorsque vous mettez en oeuvre un algorithme en parallèle, ayez une vision locale du problème :

- Si je suis le processus de rang x, que dois-je faire ?
- J'envoie des messages à qui ?
- Je reçois des messages de qui ?

Par contre, lors de la mise au point de l'algorithme parallèle, il est nécessaire d'avoir une vision globale
de ce que font tous les processus !

## Envoi bloquant et non bloquant
 - Envoi bloquant : Sûr car puisque tout est bloquant, on connait la valeur de tous nos paramètres lorsque nos tâches sont exécutées
 - Envoi non bloquant : On ne connaît pas l'état de nos paramètres. Il suffit de remonter l'appel à MPI_Wait au dessus de nos tâches.

## Jeton
 Pas de remarque particulière.

## Pi par lancer de fléchettes
 - Bloquant : 5s pour 16 threads chacun calculant 10 millions de points (précision aux alentours de $10^-4$)
 - Non bloquant : à peine plus long (dû à oversubscribe, je n'ai que 2 coeurs ?)
