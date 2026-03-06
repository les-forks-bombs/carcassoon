# ABSO2I2

*Par Mathurin M. & Sylvain W.*
*Pour le projet PPII2 de 1A (promo 2028).*

*Relu par Théo COURTADE*

# Développement d'une application de jeu **Carcassonne** avec IA

[[_TOC_]]

---

## Objectif du projet

Ce projet vise à mettre en œuvre des compétences en développement C, en modélisation de jeu, en implémentation de structures de données, en utilisation d’une bibliothèque graphique externe, en intelligence artificielle et en gestion de projet, tout en étant un défi ludique et pratique avec la conception d'un jeu.

L'objectif est de concevoir et développer une application jouable du jeu de société **Carcassonne**.
L'application doit permettre, dans un premier temps, à des joueurs humains de jouer en local. Dans un deuxième temps, certains joueurs pourront être remplacés par une IA.

Les élèves devront réaliser une application complète en langage C, comprenant :

- une **version CLI/ASCII** (affichage dans le terminal + interaction clavier),
- une **version graphique** utilisant la bibliothèque [libSDL](https://www.libsdl.org/),
- un **joueur virtuel** capable de prendre des décisions cohérentes.

---

## Objectifs pédagogiques

- Élaborer un cahier des charges.
- Modéliser un jeu de société en structures de données C.
- Implémenter des algorithmes avancés en langage C.
- Développer des tests unitaires et fonctionnels pour garantir la fiabilité du code.
- Expérimenter la gestion de projet en équipe (planification, suivi, comptes-rendus).
- Mettre en pratique la gestion de versions avec Git/GitLab.

---

## Cahier des charges

### Fonctionnalités obligatoires

- **Partie en mode graphique** : lancer une partie jouable en SDL, avec au moins deux joueurs locaux.
- **Partie en mode texte** : fournir un mode CLI jouable pour les tests et le débogage.
- **Joueur virtuel (IA)** : permettre le lancement d’une partie avec au moins un joueur virtuel.
- **Règles de base de Carcassonne** : placement de tuiles, pose de meeples, décompte de points en cours de partie et calcul de score final (voir [Règles](#règles)).
- **Socle commun de règles** : routes, villes et monastères/abbayes font partie du périmètre obligatoire ; les paysans peuvent rester optionnels si ce choix est clairement documenté.

### Fonctionnalités optionnelles

- Implémenter une ou plusieurs variantes/mini-extensions.
- Ajouter des extensions officielles si le socle de base est stable.

(voir [Extensions & Mini-extensions](#extensions--mini-extensions))

### Technologies

- **C** : utilisation exclusive pour la logique principale.
- **libSDL** : interface graphique.
- **Makefile** : compilation séparée et cibles dédiées.
- *(optionnel)* **ncurses** pour le mode CLI.

### Cibles Makefile minimales

Le `Makefile` doit contenir a minima :

| Cible        | Description |
|--------------|-------------|
| `make req`   | Installe les dépendances du projet |
| `make`       | Compile le projet |
| `make cli`   | Lance la version CLI |
| `make sdl`   | Lance la version SDL |
| `make test`  | Exécute les tests unitaires |
| `make clean` | Supprime les artefacts de compilation |
| `make reset` | Réinitialise les données de partie si nécessaire |

### Paramètres de lancement

Le programme doit proposer un mode d’aide (`--help`) et gérer proprement les arguments invalides.

> Pour homogénéiser les rendus, l’exécutable principal doit s’appeler `carcassonne`.

| Paramètre long   | Forme courte | Valeur par défaut | Description |
|------------------|:------------:|:-----------------:|-------------|
| `--mode MODE`    | `-m MODE`    | `sdl`             | Mode d’affichage : `cli` ou `sdl` |
| `--players N`    | `-p N`       | 2                 | Nombre de joueurs |
| `--seed N`       | `-s N`       | aléatoire         | Graine de la partie |
| `--ai N`         | `-a N`       | 0                 | Nombre de joueurs virtuels parmi les joueurs |
| `--max-turns N`  | `-t N`       | illimité          | Limite de tours optionnelle |

Exemples :

```bash
./carcassonne --mode sdl --players 2 --ai 1 --seed 42
./carcassonne -m cli -p 3 -a 1
```

Contraintes minimales attendues :

- `2 <= players <= 5` ;
- `0 <= ai <= players` ;
- la même graine doit reproduire le même ordre de pioche et donc la même partie, à règles égales ;
- les versions CLI et SDL doivent utiliser le **même moteur de jeu**.
- un joueur IA doit au minimum être capable de produire un **coup légal** dans un temps raisonnable ; sa sophistication est un critère d’évaluation, pas un prérequis bloquant pour le socle.

---

## Étapes du projet

### 1. Étude préalable

- Étudier les règles de Carcassonne, surtout les placements et connexions de zones (voir [Règles](#règles)).
- Réaliser un état des lieux des variantes/extensions potentielles (voir [Extensions & Mini-extensions](#extensions--mini-extensions)).
- Produire un **état de l’art** des algorithmes d’IA utilisables (Minimax, Alpha-Beta, heuristiques, etc.).

### 2. Conception de l’application

- Définir les structures de données du jeu de base.
- Prévoir la séparation entre logique de jeu et affichage. Le moteur principal du jeu (dont les structures de données et l'implémentation des règles de jeu) doit être partagé entre la version CLI et la version graphique.
- Concevoir dès le départ une API interne compatible avec un joueur IA.

### 3. Développement

- **Core** : règles du jeu, validation des coups, scoring. Il est possible d'imaginer que le moteur de jeu indique ou non (cela peut être une option de jeu) quelles sont les tuiles qu'il peut placer (ou non).
- **IA** : stratégie de sélection des coups et réglage de difficulté/temps de calcul. Vous pouvez proposer plusieurs moteurs d'intelligence artificielle et comparer leur performance/précision respective.
- **UI** : affichage du plateau, des scores, des informations de tour et des interactions. Libre à vous de choisir l'interface graphique la plus adéquate (vue du plateau du dessus sans ou avec défilement (*scrolling*) / zoom, vue 3D isométrique, moteur 3D), interaction uniquement au clavier ou via la souris.

### 4. Documentation et tests

- Documenter les choix d’architecture et d’algorithmes. Pour les algorithmes d’IA, il est attendu que vous réalisiez un état de l’art **ciblé et utile au projet**.
> Un état de l’art est ici une synthèse comparative des méthodes réellement pertinentes pour votre contexte (par exemple heuristiques, recherche locale, Minimax limité, Monte-Carlo, etc.), en explicitant leurs forces, leurs limites et les raisons de votre choix au regard de vos capacités de mise en œuvre.

- Produire des tests unitaires et des tests fonctionnels sur les cas critiques.

---

## Exigences du projet et points évalués

Les critères d’évaluation principaux seront les suivants :

- Qualité de la **modélisation** et des **structures de données**.
- Respect des **règles du jeu** et cohérence des validations.
- Qualité de l’**IA** (pertinence, robustesse, performance).
- Qualité de l’**interface utilisateur** (CLI et/ou SDL selon mode lancé).
- Respect des **bonnes pratiques** de développement (compilation séparée, organisation, Makefile, lisibilité) (voir [Cibles Makefile minimales](#cibles-makefile-minimales)).
- **Tests** et gestion des erreurs (robustesse).
- Clarté du périmètre choisi : variantes activées, simplifications assumées, limites connues.

---

## Conseils

### Démarrer par une mini-version

Une première version proche de *Mon premier Carcassonne* peut aider à stabiliser rapidement :
plateau affiché, placements simples, scoring partiel.

### Ressources tuiles

Textures et références utiles :
<https://wikicarpedia.com/car/Carcassonne_Tile_List/en>

### Variantes et extensions

Les variantes peuvent servir de simplification temporaire pendant le développement.
Les extensions doivent rester secondaires par rapport au socle du jeu de base.

---

## Rendu final

Le dépôt GitLab devra contenir au minimum :

- **Code source** complet + instructions d’installation/exécution.
- **État de l’art IA** (algorithmes étudiés et choix retenus).
- **Documentation technique** (structures, modules, architecture, arbitrages).
- **Tests unitaires** intégrés au dépôt et exécutables via `make test` (voir [Cibles Makefile minimales](#cibles-makefile-minimales)).
- **Documents de gestion de projet** : planification, comptes-rendus, répartition, post-mortem.
- **Limites connues** : règles simplifiées, variantes non prises en charge, comportements IA encore rudimentaires.

---

## Règles

Les règles présentées ici sont un résumé des règles complètes : <https://wikicarpedia.com/car/Base_game/fr>

### 0. But du jeu

Dans Carcassonne, les joueurs posent des tuiles tour après tour pour créer routes, villes, abbayes et prés.
Les meeples y sont placés pour marquer des points pendant la partie et au décompte final.
Le joueur avec le score le plus élevé remporte la partie.

### 1. Matériel et mise en place

- **Tuiles** : 72 tuiles de paysage (dont une tuile de départ).
- **Meeples** : 8 meeples par joueur (1 pour le score, 7 en réserve).
- **Piste de score**.

**Installation** : placer la tuile de départ au centre, mélanger les autres tuiles et former une pioche face cachée.

Pour l’implémentation, vous pouvez considérer le plateau comme **non borné logiquement**. En pratique, l’affichage peut utiliser une fenêtre, un défilement ou un zoom, tant que toutes les positions légales restent accessibles.

### 2. Déroulement d’un tour

En commençant par un premier joueur choisi aléatoirement, chaque joueur effectue :

#### Étape 1 : piocher et placer une tuile

Le joueur **doit** piocher puis poser une tuile adjacente à une tuile déjà placée, en respectant la compatibilité des paysages (pré/pré, chemin/chemin, ville/ville).
La tuile peut être tournée avant la pose.
Si aucune pose n’est possible, la tuile est retirée de la partie et le tour du joueur s’arrête.

#### Étape 2 : poser un meeple (optionnel)

Le joueur **peut** placer un meeple de sa réserve sur la tuile qu’il vient de poser.

- **Emplacements** : chemin (voleur), ville (chevalier), abbaye (moine), pré (paysan).
- **Exclusion** : impossible de placer un meeple sur une zone déjà reliée à une zone occupée.
- Le meeple, s’il est posé, doit obligatoirement être placé sur **la tuile qui vient d’être jouée**.

#### Étape 3 : évaluer les zones complétées

Si le placement termine une construction (route, ville, abbaye), les points sont attribués immédiatement,
et les meeples concernés sont récupérés.

En cas de majorité partagée, tous les joueurs à égalité marquent la totalité des points correspondants.

### 3. Décompte des points (en cours de partie)

| Élément | Condition de complétion | Points |
| --- | --- | --- |
| **Chemin** | Fermé aux deux extrémités | **1 point** par tuile |
| **Ville** | Entièrement fermée | **2 points** par tuile + **2 points** par blason |
| **Abbaye** | Entourée de 8 tuiles | **9 points** |

> **Majorité** : lorsqu’une zone complétée comporte des meeples de plusieurs joueurs, seul le ou les joueurs majoritaires marquent.
> En cas d’égalité de majorité, tous les joueurs concernés marquent la totalité des points.

### 4. Les paysans (règle optionnelle)

Cette règle peut être laissée **optionnelle** dans le cadre du socle minimal.

- Placés allongés dans les prés.
- Non récupérés durant la partie.
- Comptabilisés uniquement au score final.
- Si vous décidez de ne pas l’implémenter, votre interface et votre documentation doivent l’indiquer explicitement.

### 5. Fin de partie et score final

La partie s’arrête lorsque la dernière tuile est posée, ou plus tôt si une limite `--max-turns` a été fixée et atteinte. On compte alors :

* **Villes inachevées :** 1 point par tuile et 1 point par blason (moitié moins que si elle était terminée).
* **Chemins, Abbayes inachevés :** 1 point par tuile (comme si elles étaient terminées).
* **Paysans (Prés) :** si cette règle optionnelle est activée, chaque pré rapporte **3 points par ville terminée** qu'il touche. Comme pour les villes, c'est la majorité de meeples dans le pré qui détermine qui marque.


---

### Extensions & Mini-extensions

#### 1. Les Extensions Principales (Grandes boîtes)

Ces boîtes contiennent généralement une douzaine de tuiles, de nouveaux meeples et des mécaniques qui changent la dynamique du jeu.

* Auberges et Cathédrales (Ext. 1) : Ajoute le "Grand Meeple" et double les points des chemins/villes (mais attention au score nul si inachevé).

* Marchands et Bâtisseurs (Ext. 2) : Introduit les ressources (vin, blé, tissu), le Cochon (bonus de pré) et le Bâtisseur (rejouer un tour).

* Maire et Monastères (Ext. 3) : Ajoute le Maire, la Grange et les voitures à chevaux.

* La Tour (Ext. 4) : Permet de construire des tours en hauteur pour capturer les meeples adverses.

* Maires et Abbayes (Ext. 5) : (Souvent confondue avec la 3). Elle introduit des tuiles spéciales pour boucher les trous impossibles.

* Le Comte, le Roi et le Consort (Ext. 6) : Regroupe plusieurs mini-extensions dont la grande cité de Carcassonne.

* La Catapulte (Ext. 7) : Note : C'est l'extension la moins aimée, car elle utilise un accessoire physique pour lancer des jetons.

* Bazar, Ponts et Châteaux (Ext. 8) : Ajoute des enchères de tuiles et des ponts pour enjamber les prés.

* Moutons et Collines (Ext. 9) : Introduit un aspect "stop ou encore" avec le berger et ses moutons.

* Miss Pique et le Cirque (Ext. 10) : Un chapiteau de cirque qui donne des points aux meeples alentour.

#### 2. Les Mini-Extensions

Elles sont souvent vendues en petits sachets ou incluses dans des éditions spéciales (comme la Big Box).

* La Rivière : (Souvent incluse dans le jeu de base désormais) Remplace la tuile de départ par un cours d'eau.

* L’Abbé : Un meeple spécial qui peut être retiré d'une abbaye avant qu'elle ne soit finie.

* Les Mines d'Or : Ajoute des lingots qui multiplient les points en fin de partie.

* Mages et Sorcières : Des figurines qui augmentent ou diminuent la valeur d'une zone.

* Les Messagers : Des tuiles de score bonus sur la piste de score.

* Les Cercles de Culture : Permettent de rajouter ou retirer des meeples sur le plateau.

* Les Aéronefs (Vols de reconnaissance) : Permettent de placer des meeples de façon aléatoire sur d'autres tuiles.
    

---
