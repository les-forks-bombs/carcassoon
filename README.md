# Modalités communes — Projets PPII

Ce document présente les règles générales communes aux deux sujets du projet PPII.

## Accès aux sujets

- Sujet 1 — Civilization : [README_CIV.md](README_CIV.md)
- Sujet 2 — Carcassonne  : [README_CARCASSONNE.md](README_CARCASSONNE.md)

---

## Objectifs communs

Les deux projets visent à développer une application de jeu en langage C avec :

- Une architecture modulaire et maintenable ;
- une logique de jeu correctement implémentée ;
- un mode d’exécution en ligne de commande entièrement dans le terminal (TUI - Text-based user interface) ; 
- un mode d’exécution graphique utilisant la librairie libSDL (https://www.libsdl.org/) ;
- des tests unitaires testant a minima le cœur de votre programme (il n'est pas attendu de tests de bout en bout depuis les interfaces utilisateurs) ; 
- une documentation technique claire.


---

## Exigences techniques communes

- Langage principal : **C**.
- Build : **Makefile** avec compilation séparée.
- Contrôle de version : **Git/GitLab**.
- Qualité de code : gestion des erreurs, lisibilité, séparation des responsabilités.

### Cibles Make minimales attendues

- `make` : compiler le projet
- `make cli` : lancer la version terminal
- `make sdl` : lancer la version graphique
- `make test` : exécuter les tests
- `make clean` : nettoyer les artefacts de compilation
- `make req` : installe les dépendances du projet

> Selon le sujet, d’autres cibles peuvent être demandées (`make reset`, etc.).

---

## Livrables communs

Le dépôt GitLab doit contenir a minima :

- un fichier README.md
- le code source complet ;
- un Makefile fonctionnel ;
- des instructions d’installation et d’exécution ;
- les tests unitaires ;
- une documentation technique ;
- les éléments de gestion de projet (planning, comptes-rendus, répartition des tâches, bilan).

---

## Évaluation (principes communs)

L’évaluation porte notamment sur :

- la qualité de la modélisation ;
- la conformité aux règles du sujet choisi ;
- la robustesse de l’application ;
- la qualité des interfaces (CLI + SDL) ;
- la qualité des tests et de la documentation ;
- les pratiques de développement collaboratif.

---

## Méthode de travail recommandée

- Développer par incréments (socle jouable → stabilisation → enrichissements).
- Valider régulièrement avec des tests.
- Garder la logique de jeu indépendante du rendu (CLI/SDL).
- Documenter les choix techniques au fil de l’avancement.

---

## Choix du sujet

Chaque groupe choisit un sujet principal à réaliser. Une justification du choix n'est pas forcément attendue, mais peut être un plus.

- Civilization : [README_CIV.md](README_CIV.md)
- Carcassonne : [README_CARCASSONNE.md](README_CARCASSONNE.md)

Une fois le sujet choisi, ce document peut être supprimé ou renomé pour faire place à votre propre fichier README.md décrivant votre projet.


---

## Date de rendu et Soutenance

Le projet est à rendre pour le **29 mai 2026 à 22 heures au plus tard**.

Des soutenances de groupes de projet seront organisées dans la foulée (début juin).

Votre projet fera l'objet d'une démonstration devant un jury composé d'au moins 2 membres de l’équipe pédagogique. Durant cette soutenance, vous serez jugés sur votre démonstration de l'application et votre capacité à expliquer votre projet et son fonctionnement. 
Chaque membre du groupe devra être présent lors de soutenance et participer activement. *Toute personne ne se présentant pas à la soutenance sera considérée comme démissionnaire de l'UE et en conséquence, ne pourra pas la valider pour l’année universitaire 2025-2026.*

Il est attendu que chaque membre du groupe ait contribué à plusieurs parties fonctionnelles du code (il ne s'agit pas d'avoir uniquement corrigé quelques lignes par ci et par là).


---

## Fraude, tricherie et plagiat

Ne trichez pas ! Ne copiez pas ! Ne plagiez pas ! Si vous le faites, vous serez lourdement sanctionnés. Nous ne ferons pas de distinction entre copieur et copié. Vous n'avez pas de (bonnes) raisons de copier. De même, vous ne devez pas utiliser de solution clé en main trouvée sur internet.

Par tricher, nous entendons notamment :
- Rendre le travail d’un collègue en y apposant votre nom ;
- Obtenir un code, une solution par un moteur de recherche (ou une IA) et la rendre sous votre nom ;
- Récupérer du code et ne changer que les noms de variables et fonctions ou leur ordre avant de les présenter sous votre nom 
- Autoriser consciemment ou inconsciemment un collègue à s'approprier votre travail personnel. Assurez-vous notamment que votre projet et ses différentes copies locales ne soient lisibles que par vous et les membres de votre groupe.

Nous encourageons les séances de *brainstorming* et de discussion entre les élèves sur le projet. C’est une démarche naturelle et saine comme vous la rencontrerez dans votre vie professionnelle. Si les réflexions communes sont fortement recommandées, vous ne pouvez rendre que du code et des documents écrits par vous-même. Vous indiquerez, en particulier, dans votre rapport toutes vos sources (comme les sites internet que vous auriez consultés), en indiquant brièvement ce que vous en avez retenu.
Il est quasi certain que nous détections les tricheries. En effet, les rapports et les codes sont systématiquement soumis à des outils de détection de plagiat et de copie. Il existe notamment des outils de détection de manipulation de code extraordinaire mis à disposition par l’Université de Stanford, tels que `MOSS` (https://theory.stanford.edu/~aiken/moss/) ou `compare50` (https://cs50.readthedocs.io/projects/compare50/). De plus, chacun a son propre style de programmation et personne ne développe la même chose de la même manière.

Puisqu'il s'agit d'un projet réalisé dans le cadre de cours avancés de programmation, nous nous attendons à ce que vous soyez capable d'apprendre à déboguer des programmes par vous-même. Par exemple, demander à un autre élève de regarder directement votre code et de donner des suggestions d'amélioration commence à devenir un peu délicat au niveau éthique.


Dans le cadre du présent projet, l’utilisation d’outils d’intelligence artificielle générative (dont les modèles de langage de type LLM, assistants de programmation, générateurs de code, de tests, de documentation ou de rapports) est strictement encadrée. 

Sont notamment interdits : la génération automatique, totale ou partielle, de code source, d’architectures, de jeux de tests, de rapports, de livrables ou de présentations destinés à être évalués ; la reformulation substantielle de travaux rédigés par les étudiants ; l’utilisation d’IA pour résoudre directement des problèmes techniques ou algorithmiques constitutifs des objectifs pédagogiques ; ainsi que toute utilisation visant à contourner l’évaluation des compétences individuelles. 


En revanche, l’usage d’outils d’IA est autorisé à des fins d’apprentissage et de documentation, par exemple pour obtenir des explications conceptuelles, clarifier des notions, explorer des pistes de compréhension ou identifier des ressources pertinentes, dès lors que le travail remis est intégralement produit, compris et maîtrisé par les étudiants. Les outils d’IA peuvent également être utilisés, sauf indication contraire explicite de l’équipe pédagogique, à des fins accessoires telles que la correction orthographique mineure ou la clarification linguistique, sans modification du contenu technique.

Toute utilisation autorisée doit être explicitement déclarée et documentée. Le non-respect de ces règles constitue un manquement à l’intégrité académique et pourra donner lieu à des sanctions.

**Si vous rencontrez des difficultés pour terminer une tâche, veuillez contacter l'un de vos enseignants afin que nous puissions vous aider. Nous préférons de loin passer du temps à vous aider plutôt que de traiter des cas de fraudes.**

