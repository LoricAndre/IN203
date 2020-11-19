# TP2 de NOM Prénom

`pandoc -s --toc tp3.md --css=./github-pandoc.css -o tp3.html`

## 2.1
1. Si le processus 0 envoit avant le 1, tout va bien.
2. Si le processus 1 envoit avant le 0, le 2 va envoyer sa réponse au 1 avant d'écouter sa question ce qui va bloquer. 
3. 50%, on n'a aucun moyen de savoir lequel sera envoyé en premier.

## 2.2
On a $S(n) ~= 1/f = 1/0.1 = 10$

## Mandelbrot 


*Expliquer votre stratégie pour faire une partition équitable des lignes de l'image entre chaque processus*

           | Taille image : 800 x 600 | 
-----------+---------------------------
séquentiel |              
1          |              
2          |              
3          |              
4          |              
8          |              


*Discuter sur ce qu'on observe, la logique qui s'y cache.*

*Expliquer votre stratégie pour faire une partition dynamique des lignes de l'image entre chaque processus*

           | Taille image : 800 x 600 | 
-----------+---------------------------
séquentiel |              
1          |              
2          |              
3          |              
4          |              
8          |              



## Produit matrice-vecteur



*Expliquer la façon dont vous avez calculé la dimension locale sur chaque processus, en particulier quand le nombre de processus ne divise pas la dimension de la matrice.*
