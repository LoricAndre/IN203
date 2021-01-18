# Rapport IN203 Loric ANDRE

## 1. OMP
Cette version est accessible en changeant `USE_OMP` à 1 dans `colonisation.cpp`
 - On utilise les générateurs de nombre aléatoires du C++
 - On parallélise ensuite le code à l'aide d'un `#pragma omp for`:
   - Problème rencontré :  les générateurs aléatoires sont alors partagés et sources de _data races_ entre les threads.
   - Ces data races sont alors un compromis entre :
     - Créer un générateur par thread et le passer en argument à chaque exécution de `mise_a_jour` et encerclant la boucle `while` du `main` dans un `#pragma omp parallel` -> Solution la plus prometteuse mais aussi la plus lente. Le passage de ces fonctions en argument génère manifestement des problèmes.
     - Créer un générateur par thread et par itération -> Solution visiblement la plu côuteuse, ces générateurs ne sont pas simples à initaliser
     - Créer un générateur global et le partager -> Solution adoptée car la plus rapide.
 - Finalement, cette solution est environ 2 fois plus lente que la version linéaire sur mon processeur (Celeron 2 coeurs).

## 2. MPI
Cette version est accessible en décommentant la ligne `#define USE_MPI` dans `parametres.h`
 - On va dans ce cas assigner des tâches à chaque thread : 
   - Le thread `(nbp - 1)` va être le thread maître et rassembler les données ainsi que les assembler et gérer l'affichager
   - Les autres vont effectuer les calculs.
 - Problème rencontré : La galaxie n'est pas mise à jour, une erreur `[archtop][[35216,1],1][btl_tcp.c:559:mca_btl_tcp_recv_blocking] recv(23) failed: Connection reset by peer (104)[archtop][[35216,1],0][btl_tcp.c:559:mca_btl_tcp_recv_blocking] recv(23) failed: Connection reset by peer (104)` apparaît régulièrement. Après vérification, les données envoyées et réceptionnées par les différents threads sont de la bonne taille et identique, lorsque le transport réussit.

## Conclusion
Aucune des méthodes de parallélisation n'a abouti par manque de temps (environ 3 heures ont été consacrées à la version OMP et plus de 10 à la version MPI sans progrès après la première demi-heure).
