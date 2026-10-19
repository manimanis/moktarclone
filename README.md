# Réplique Cross-Platform (Windows & Linux) en C++ du jeu MOKTAR (1991)
### *Lagaf': Les Aventures de Moktar - Vol 1: La Zoubida* (Titus Interactive)

Cette réplique native en C++ est un clone fidèle du jeu DOS culte **Moktar** (1991) créé par rétro-ingénierie (*reverse engineering*) des fichiers originaux, compatible et compilable à la fois sur **Windows** (MSYS2 / UCRT64 ou MinGW64) et **Linux**.

---

## 🎮 Lancement rapide

### Sur Windows :
Double-cliquez ou lancez simplement dans votre invite de commande (CMD ou PowerShell) :
```cmd
run_moktar.bat
```
*(Si le jeu n'est pas encore compilé, il lance automatiquement la compilation via MSYS2).*

### Sur Linux / MSYS2 Bash :
```bash
./run_moktar.sh
```

### Options en ligne de commande :
```bash
# Exemple Windows (run_moktar.bat ou .\bin\moktar.exe) / Linux (./run_moktar.sh)
./run_moktar.sh --help            # Affiche l'aide complète
./run_moktar.sh --info            # Affiche le rapport de reverse engineering
./run_moktar.sh --level <1-16>    # Démarre directement à un niveau (1 à 16)
./run_moktar.sh --scale <1-4>     # Échelle de la fenêtre (1x, 2x, 3x=960x600, 4x)
./run_moktar.sh --fullscreen      # Mode plein écran (ratio 4:3 conservé)
./run_moktar.sh --godmode         # Trainer : santé / PV illimités
./run_moktar.sh --noclip          # Traversée des murs
./run_moktar.sh --skip-intro      # Passer directement au jeu
./run_moktar.sh --assets <DIR>    # Spécifier un autre dossier de données
```

---

## ⌨️ Commandes et Touches Raccourci en Jeu

| Action | Clavier | Manette de jeu |
|---|---|---|
| **Déplacement / Accroupi / Viser** | Flèches directionnelles, `W, A, S, D` ou `Z, Q, S, D` (AZERTY) | Croix directionnelle / Stick gauche |
| **Sauter** | `Espace`, `Ctrl`, ou `Flèche Haut` / `W` / `Z` | Bouton A / Croix |
| **Ramasser / Poser / Jeter un objet** | `C`, `X`, `Entrée` ou `Espace` (accroupi/porté) | Bouton B / X |
| **Pause** | `P` | Bouton Start |
| **Plein écran (Toggle)** | `F11` ou `Alt + Entrée` | - |
| **Musique On / Off** | `M` ou `F3` | - |
| **Trainer : PV illimités (Godmode)** | `F5` ou `G` | - |
| **Trainer : Vies infinies** | `F6` | - |
| **Trainer : Mode Noclip (Traversée)** | `F7` ou `N` | - |
| **Trainer : Sauter au niveau suivant** | `F8` ou `PageDown` | - |
| **Suicide / Recommencer au checkpoint** | `F1` | - |
| **Game Over immédiat** | `F2` | - |
| **Afficher page de statut (Vies/Étape)** | `F4` | - |
| **Afficher jauge d'énergie / PV** | `E` | - |
| **Quitter / Menu** | `Échap` | Bouton Back / Select |

---

## 🔍 Analyse de Reverse Engineering

### 1. Décompression des Archives `.SQZ`
Le jeu utilise un conteneur propriétaire identifié par l'entête de 4 octets :
`[b1, comp_type, b3, b4]`
- **Taille décompressée** : `out_len = (b1 & 0x0F) << 16 | (b4 & 0xFF) << 8 | (b3 & 0xFF)`
- **Type `0x10` (LZW)** : Algorithme Lempel-Ziv-Welch adaptatif avec codes d'effacement de dictionnaire (`0x100`), fin de flux (`0x101`), et table jusqu'à 4096 entrées (largeur de 9 à 12 bits).
- **Autres types (Huffman + RLE)** : Arbre binaire préfixé suivi d'un flux RLE.

### 2. Formats des Niveaux (`LEVEL0.SQZ` à `LEVELG.SQZ`)
Moktar comprend **16 niveaux complets** (contre 15 dans sa version internationale *Titus the Fox*) :
- **Hauteur du niveau** : `height = (taille_décompressée - 35828) / 256 tuiles`
- **Grille de tuiles** : 256 colonnes x `height` lignes (1 octet par tuile, indices 0 à 255).
- **Banque de tuiles** : 256 tuiles 16x16 pixels stockées en 4 plans de bits EGA/VGA (128 octets par tuile = 32 768 octets au total).
- **Drapeaux physiques** :
  - `offset + 32768` (256 octets) : Collisions horizontales (murs pleins).
  - `offset + 32768 + 256` (256 octets) : Collisions de sol et pentes.
  - `offset + 32768 + 512` (256 octets) : Plafond et permutation cyclique des animations de tuiles (eau, torches, etc.).
- **Table d'objets interactifs** (40 entrées x 6 octets) :
  - Caisses, poubelles, pots, trampolines, enclumes, ascenseurs, bonus.
- **Table d'ennemis et pièges** (50 entrées x 25 octets) :
  - 19 types d'IA différents (rondes, tirs, sauts, chutes du plafond, et le **Boss exclusif du Niveau 15 sur Moktar**).
- **Positions joueur et sortie** :
  - Spawn du joueur : X (`offset + 33778`), Y (`offset + 33780`).
  - Porte de fin de niveau : X (`offset + 35824`), Y (`offset + 35826`).

### 3. Sprites (`SPRITES.SQZ`)
- Graphismes des personnages et projectiles au format planaire 4 bits (16 couleurs).
- `groupsize = (largeur * hauteur) / 8` octets par plan.
- Décodage des 4 plans de bits (plans 0 à 3) pour obtenir l'index de palette de chaque pixel.
- Métadonnées spécifiques à Moktar (largeurs, hauteurs, hitboxes et point de portée au-dessus de la tête).

### 4. Synthèse Sonore FM AdLib (Yamaha YM3812 / OPL2)
- Les données sonores sont logées directement dans `moktar.exe` à l'offset **28817** pour une longueur de **18 184 octets**.
- Elles sont automatiquement extraites au premier lancement dans `music_mok.bin`.
- Émulation matérielle via **DOSBox DBOPL** (Woody's OPL2 engine) à 44 100 Hz stéréo.
- Émulation de la cadence du timer DOS d'origine (~72.7 Hz / délai 13.75 ms) dans le callback audio SDL pour un rendu sample-accurate sans latence ni dérive de tempo.

### 5. Table des Niveaux et Mots de Passe Authentiques

| Niveau | Titre Original | Fichier | Code d'accès |
|---|---|---|---|
| **1** | A LA RECHERCHE DE LA ZOUBIDA | `level0.sqz` | `EFE8` |
| **2** | LES QUARTIERS CHICS | `levelj.sqz` | `5165` |
| **3** | ATTENTION TRAVAUX | `level1.sqz` | `67D4` |
| **4** | LES COULOIRS DU METRO | `level2.sqz` | `2BDA` |
| **5** | LES CATACOMBES INFERNALES | `level3.sqz` | `11E5` |
| **6** | ARRIVEE DANS LA CITE | `level4.sqz` | `86EE` |
| **7** | L IMMEUBLE DE LA ZOUBIDA | `level5.sqz` | `4275` |
| **8** | SOUS LE CHEMIN DE MARRAKECH | `level6.sqz` | `A0B9` |
| **9** | LA CITE ENFOUIE | `level7.sqz` | `501C` |
| **10** | DESERT PRIVE | `level8.sqz` | `E9ED` |
| **11** | LA VILLE DES SABLES | `level9.sqz` | `D4E6` |
| **12** | LE PHARE OUEST | `levelb.sqz` | `A531` |
| **13** | UN BON TUYAU | `levelc.sqz` | `CE96` |
| **14** | DE RETOUR AU PAYS | `levele.sqz` | `B1A4` |
| **15** | DIRECTION BARBES (Boss Moktar) | `levelf.sqz` | `EBEA` |
| **16** | BIG BISOUS | `levelg.sqz` | `3B9C` |

---

## 🛠️ Recompilation

### 1. Sous Windows (via MSYS2)

Deux méthodes simples sont disponibles :

#### Méthode A : Script automatique en 1 clic
Exécutez simplement :
```cmd
build_windows.bat
```
Ce script détecte automatiquement MSYS2 (`C:\MSYS2` ou `C:\msys64`), configure l'environnement UCRT64, vérifie les dépendances et lance la compilation.
Le binaire généré sera `bin\moktar.exe`.

#### Méthode B : Depuis le terminal MSYS2 (UCRT64 ou MINGW64)
Ouvrez le terminal UCRT64 (ou exécutez `C:\MSYS2\ucrt64.exe`) :
```bash
# Vérification des dépendances (à faire une seule fois) :
pacman -S --needed make mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-make mingw-w64-ucrt-x86_64-sdl12-compat mingw-w64-ucrt-x86_64-SDL2

# Compilation :
make clean
make -j$(nproc)
```

---

### 2. Sous Linux

```bash
make clean
make -j$(nproc)
```

Le binaire exécutable sera généré dans `bin/moktar`.

