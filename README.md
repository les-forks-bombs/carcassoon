# Modalités communes — Projets PPII

Ce document présente les règles générales communes aux deux sujets du projet PPII.

## Accès aux sujets

- Sujet 1 — Civilization : [README_CIV.md](README_CIV.md)
- Sujet 2 — Carcassonne  : [README_CARCASSONNE.md](README_CARCASSONNE.md)

---

## Objectifs communs

Les deux projets visent à développer une application de jeu en langage C avec :

- une architecture modulaire et maintenable ;
- une logique de jeu correctement implémentée ;
- un mode d’exécution en ligne de commande ;
- une interface graphique via SDL ;
- des tests unitaires ;
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

Chaque groupe choisit un sujet principal à réaliser. UNe justification du choix n'est pas forcément attendue, mais peut être un plus.

- Civilization : [README_CIV.md](README_CIV.md)
- Carcassonne : [README_CARCASSONNE.md](README_CARCASSONNE.md)

Une fois le sujet choisit, ce document peut être supprimé ou renomé.