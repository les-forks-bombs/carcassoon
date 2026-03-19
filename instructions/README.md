# Modalités communes — Projets PPII

Ce document présente les règles générales communes aux deux sujets du projet PPII. Il doit être lu avec le sujet choisi par votre groupe.

## Accès aux sujets

- Sujet 1 — Civilization : [README_CIV.md](README_CIV.md)
- Sujet 2 — Carcassonne : [README_CARCASSONNE.md](README_CARCASSONNE.md)

## Questions 

- Un salon Discord a été créé pour y poser vos questions sur le/les sujets : [https://discord.gg/YpqZ3ESC]
- Ces sujets sont suceptibles d'être remis à jour, en particulier sur les livrables de gestion de projets. Voici [le lien du dépôt sujet](https://gibson.telecomnancy.univ-lorraine.fr/projets/2526/PPII/ppii-s6/template/sujet). Une notification Discord sera envoyée à chaque modification.

---

## Cadre du module

Le projet s’inscrit dans l’UE **Projet Pluridisciplinaire d’Informatique Intégrative 2**.

- Travail en équipe de **3 à 4 étudiants**.
- Charge de travail indicative : **100 h projet** par équipe.
- Développement attendu par **itérations** : socle jouable, stabilisation, enrichissements, finalisation.

Le sujet évalue autant la qualité du logiciel produit que la démarche d’ingénierie adoptée pendant le semestre.

---

## Objectifs communs

Les deux projets visent à développer une application de jeu en langage C avec :

- une architecture modulaire et maintenable ;
- une logique de jeu correctement implémentée ;
- un mode d’exécution en ligne de commande entièrement dans le terminal (TUI - Text-based user interface) ;
- un mode d’exécution graphique utilisant la bibliothèque [libSDL](https://www.libsdl.org/) ;
- des tests unitaires couvrant a minima le cœur du programme ;
- une documentation technique claire.

---

## Exigences techniques communes

- Langage principal : **C**.
- Build : **Makefile** avec compilation séparée.
- Contrôle de version : **Git/GitLab**.
- Qualité de code : gestion des erreurs, lisibilité, séparation des responsabilités.
- Architecture : la logique métier du jeu doit être **indépendante** des interfaces CLI et SDL.
- Robustesse : les entrées invalides doivent être gérées proprement ; les scénarios normaux d’utilisation ne doivent pas conduire à des plantages répétés ou à des fuites mémoire manifestes.

### Cibles Make minimales attendues

- `make` : compiler le projet
- `make cli` : lancer la version terminal
- `make sdl` : lancer la version graphique
- `make test` : exécuter les tests
- `make clean` : nettoyer les artefacts de compilation
- `make req` : installer les dépendances du projet

> Selon le sujet, d’autres cibles peuvent être demandées (`make reset`, etc.).

Les cibles `make cli` et `make sdl` doivent lancer une partie avec des paramètres par défaut raisonnables. Les exécutables doivent aussi pouvoir être lancés directement avec des arguments personnalisés.

---

## Livrables communs

Le dépôt GitLab doit contenir a minima :

- un fichier `README.md` décrivant le projet rendu ;
- le code source complet ;
- un Makefile fonctionnel ;
- des instructions d’installation et d’exécution ;
- les tests unitaires ;
- une documentation technique ;
- les éléments de gestion de projet (planning, comptes-rendus, répartition des tâches, bilan).

Le `README.md` du dépôt de rendu doit préciser au minimum :

- les prénoms et noms des membres du groupe ;
- le sujet choisi et, le cas échéant, les extensions implémentées ;
- les dépendances logicielles ;
- les commandes de compilation, de lancement et de test ;
- les paramètres de lancement pris en charge ;
- les limites connues ou fonctionnalités non terminées.

---

## Évaluation (principes communs)

L’évaluation porte notamment sur :

- la qualité de la modélisation ;
- la conformité aux règles du sujet choisi ;
- la robustesse de l’application ;
- la qualité des interfaces (CLI + SDL) ;
- la qualité des tests et de la documentation ;
- les pratiques de développement collaboratif.

Une fonctionnalité partiellement implémentée mais documentée honnêtement sera mieux évaluée qu’une fonctionnalité annoncée comme terminée alors qu’elle est instable ou non démontrable.

---

## Méthode de travail recommandée

- développer par incréments (socle jouable → stabilisation → enrichissements) ;
- valider régulièrement avec des tests ;
- garder la logique de jeu indépendante du rendu (CLI/SDL) ;
- documenter les choix techniques au fil de l’avancement ;
- tracer et conserver les décisions importantes : structures de données, arbitrages fonctionnels, limites connues ;
- s’appuyer sur des outils de diagnostic adaptés (`gdb`, sanitizers, `valgrind`, etc.).

---

## Choix du sujet

Chaque groupe choisit un sujet principal à réaliser. Une justification du choix n'est pas forcément attendue, mais peut constituer un plus.

- Civilization : [README_CIV.md](README_CIV.md)
- Carcassonne : [README_CARCASSONNE.md](README_CARCASSONNE.md)

Dans votre dépôt de rendu, ce document pourra être remplacé ou renommé afin de laisser la place à votre propre `README.md` décrivant le projet effectivement réalisé.

---

## Clarifications utiles

- sauf mention contraire dans le sujet choisi, une règle simplifiée est acceptable si elle est **cohérente, stable et documentée** ;
- si vous modifiez des constantes de jeu ou simplifiez une mécanique, vous devez l’indiquer explicitement dans la documentation technique ;
- quand le sujet propose une graine (`seed`) ou des paramètres de lancement, ils doivent être réellement exploités par le programme afin de rendre les parties reproductibles ;
- les interfaces CLI et SDL peuvent différer visuellement, mais elles doivent piloter le **même moteur de jeu** et les **mêmes règles**.

---

## Date de rendu et soutenance

Le projet est à rendre pour le **29 mai 2026 à 22 heures au plus tard**.

Des soutenances de groupes de projet seront organisées dans la foulée, début juin 2026.

Votre projet fera l'objet d'une démonstration devant un jury composé d'au moins 2 membres de l’équipe pédagogique. Durant cette soutenance, vous serez jugés sur la démonstration de l'application et sur votre capacité à expliquer votre projet, son architecture et les choix effectués.

Chaque membre du groupe devra être présent lors de la soutenance et participer activement. *Toute personne ne se présentant pas à la soutenance sera considérée comme démissionnaire de l'UE et, en conséquence, ne pourra pas la valider pour l’année universitaire 2025-2026.*

Il est attendu que chaque membre du groupe ait contribué à plusieurs parties fonctionnelles du code. Il ne s'agit pas d'avoir uniquement corrigé quelques lignes de manière marginale.

---

## Fraude, tricherie et plagiat

Ne trichez pas. Ne copiez pas. Ne plagiez pas. Si vous le faites, vous serez lourdement sanctionnés. Nous ne ferons pas de distinction entre copieur et copié. Vous ne devez pas utiliser de solution clé en main trouvée sur internet.

Par tricher, nous entendons notamment :

- rendre le travail d’un collègue en y apposant votre nom ;
- obtenir un code ou une solution par un moteur de recherche ou une IA, puis le rendre sous votre nom ;
- récupérer du code et ne changer que les noms de variables et fonctions ou leur ordre avant de le présenter sous votre nom ;
- autoriser consciemment ou inconsciemment un collègue à s'approprier votre travail personnel. Assurez-vous notamment que votre projet et ses différentes copies locales ne soient lisibles que par vous et les membres de votre groupe.

Nous encourageons les séances de *brainstorming* et de discussion entre les élèves sur le projet. C’est une démarche naturelle et saine, comme vous la rencontrerez dans votre vie professionnelle. Si les réflexions communes sont fortement recommandées, vous ne pouvez rendre que du code et des documents écrits par vous-même. Vous indiquerez en particulier, dans votre rapport, toutes vos sources (comme les sites internet consultés), en précisant brièvement ce que vous en avez retenu.

Il est quasi certain que nous détecterons les tricheries. En effet, les rapports et les codes sont systématiquement soumis à des outils de détection de plagiat et de copie. Il existe notamment des outils de détection de manipulation de code mis à disposition par l’Université de Stanford, tels que `MOSS` (https://theory.stanford.edu/~aiken/moss/) ou `compare50` (https://cs50.readthedocs.io/projects/compare50/). De plus, chacun a son propre style de programmation et personne ne développe exactement la même chose de la même manière.

Puisqu'il s'agit d'un projet réalisé dans le cadre de cours avancés de programmation, nous nous attendons à ce que vous soyez capables d'apprendre à déboguer des programmes par vous-mêmes. Par exemple, demander à un autre élève de regarder directement votre code et de donner des suggestions d'amélioration commence à devenir délicat au niveau éthique.

Dans le cadre du présent projet, l’utilisation d’outils d’intelligence artificielle générative, dont les modèles de langage de type LLM, assistants de programmation, générateurs de code, de tests, de documentation ou de rapports, est strictement encadrée.

Sont notamment interdits : la génération automatique, totale ou partielle, de code source, d’architectures, de jeux de tests, de rapports, de livrables ou de présentations destinés à être évalués ; la reformulation substantielle de travaux rédigés par les étudiants ; l’utilisation d’IA pour résoudre directement des problèmes techniques ou algorithmiques constitutifs des objectifs pédagogiques ; ainsi que toute utilisation visant à contourner l’évaluation des compétences individuelles.

En revanche, l’usage d’outils d’IA est autorisé à des fins d’apprentissage et de documentation, par exemple pour obtenir des explications conceptuelles, clarifier des notions, explorer des pistes de compréhension ou identifier des ressources pertinentes, dès lors que le travail remis est intégralement produit, compris et maîtrisé par les étudiants. Les outils d’IA peuvent également être utilisés, sauf indication contraire explicite de l’équipe pédagogique, à des fins accessoires telles que la correction orthographique mineure ou la clarification linguistique, sans modification du contenu technique.

Toute utilisation autorisée doit être explicitement déclarée et documentée. Le non-respect de ces règles constitue un manquement à l’intégrité académique et pourra donner lieu à des sanctions.

**Si vous rencontrez des difficultés pour terminer une tâche, veuillez contacter l'un de vos enseignants afin que nous puissions vous aider. Nous préférons de loin passer du temps à vous aider plutôt que de traiter des cas de fraude.**
