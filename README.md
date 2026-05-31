# PP2I - Carcassonne

Ce projet GitLab est le pp2i du groupe "Les Fork Bombs".

# Prérequis

Une installation globale de sdl3, sdl3_image, sdl3_ttf, de cmocka et de libasan (pour le debug uniquement)

# Gestion du projet

La gestion du projet se fait sur [OpenProject](https://op.eu.mpgn.dev/), 
Les branches et MRs sont liées à des lots de travaux sur ce dernier

# Compilation

## Installation des dépendances

L'environment de compilation du projet nécesite; 

Les dépendances suivantes:

- SDL3
- SDL3_image
- SDL3_ttf
- cmocka

Chacunes compilées pour:

- WASM32
- Mingw64
- Linux

Un script utilitaire est fourni pour installer et
compiler les dépendances, vous pouvez l'appeler avec
`make req`.

En plus de celà, une version récente de:

- clang
- clang-format
- clang-tidy
- wine
- nodejs
- lcov_cobertura

Vous sera demandé.

## Commandes du projet

Voici les informations organisées sous forme de tableau pour plus de clarté, avec un rappel des variables de configuration disponibles :

### Variables de configuration

* **`TARGET`** : `x86_64-pc-linux-gnu`, `x86_64-w64-mingw32`, `wasm32-unknown-emscripten`
* **`PROFILE`** : `debug`, `release`

### Liste des commandes Make

| Commande | Description |
| --- | --- |
| `make build PROFILE={profile} TARGET={target}` | Compile uniquement les exécutables finaux (ex: Carcassonne). |
| `make all PROFILE={profile} TARGET={target}` | Compile tous les exécutables, y compris les fichiers de tests. |
| `make test PROFILE={profile} TARGET={target}` | Exécute l'ensemble des suites de tests. |
| `make coverage` | Exécute les tests et génère les rapports de couverture de code. |
| `make bear` | Compile le projet pour générer le fichier `compile_commands.json` (requis par `clangd`). |
| `make appimage PROFILE={profile}` | Crée un exécutable portable AppImage (nécessite un environnement privilégié). |
| `make clean PROFILE={profile} TARGET={target}` | Nettoie la solution en supprimant les fichiers de compilation. |
| `make format` | Formate automatiquement le code source (via `clang-format`). |
| `make tidy` | Linte et analyse statiquement le code (via `clang-tidy`). |
