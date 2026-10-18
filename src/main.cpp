/*
 * =====================================================================================
 *  MOKTAR (Lagaf': Les Aventures de Moktar - Vol 1: La Zoubida, Titus Software 1991)
 *  Réplique C++ Linux native par Reverse Engineering
 * =====================================================================================
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

namespace fs = std::filesystem;

extern "C" {
    #include "settings.h"
    #include "globals.h"
    #include "audio.h"

    int init_game();
    int playtitus(int firstlevel);
    void freefonts();
    int freeaudio();
    void checkerror();
    int readconfig(char *configfile);
    int viewintrotext();
    int viewimage(char * imagefile, int imageformat, int displayformat, int delay);
    int viewmenu(char * menufile, int menuformat);
    int initoriginal();
    int initcodes();
    int initleveltitles();
    int loadfonts();
}

struct GameOptions {
    std::string assetDir = "";
    int directLevel = 0;     // 0 = intro/menu, 1 to 16 = start level directly
    int scale = 3;           // 1x (320x200), 2x (640x400), 3x (960x600), 4x (1280x800)
    bool fullscreen = false;
    bool godmode = false;
    bool noclip = false;
    bool enableMusic = true;
    bool skipIntro = false;
    bool showInfo = false;
};

// Case-insensitive file finder
std::string findAssetFile(const std::string& directory, const std::string& targetName) {
    if (!fs::exists(directory)) return "";

    std::string targetLower = targetName;
    for (auto &c : targetLower) c = std::tolower(c);

    try {
        for (const auto& entry : fs::directory_iterator(directory)) {
            std::string filename = entry.path().filename().string();
            std::string filenameLower = filename;
            for (auto &c : filenameLower) c = std::tolower(c);

            if (filenameLower == targetLower) {
                return entry.path().string();
            }
        }
    } catch (...) {}

    // Fallbacks
    std::string directPath = directory + "/" + targetName;
    if (fs::exists(directPath)) return directPath;

    std::string lowerPath = directory + "/" + targetLower;
    if (fs::exists(lowerPath)) return lowerPath;

    std::string upperTarget = targetName;
    for (auto &c : upperTarget) c = std::toupper(c);
    std::string upperPath = directory + "/" + upperTarget;
    if (fs::exists(upperPath)) return upperPath;

    return "";
}

// Extract Moktar AdLib OPL2 music from MOKTAR.EXE (Offset: 28817, Size: 18184 bytes)
bool extractMusicFromExe(const std::string& exePath, const std::string& destPath) {
    std::ifstream exeFile(exePath, std::ios::binary);
    if (!exeFile.is_open()) {
        return false;
    }

    const uint32_t MUSIC_OFFSET = 28817;
    const uint32_t MUSIC_SIZE = 18184;

    exeFile.seekg(0, std::ios::end);
    size_t fileSize = exeFile.tellg();
    if (fileSize < MUSIC_OFFSET + MUSIC_SIZE) {
        std::cerr << "[!] moktar.exe trop court pour contenir la banque sonore AdLib (" << fileSize << " octets)\n";
        return false;
    }

    exeFile.seekg(MUSIC_OFFSET, std::ios::beg);
    std::vector<char> buffer(MUSIC_SIZE);
    exeFile.read(buffer.data(), MUSIC_SIZE);

    std::ofstream outFile(destPath, std::ios::binary);
    if (!outFile.is_open()) {
        return false;
    }
    outFile.write(buffer.data(), MUSIC_SIZE);
    std::cout << "[+] Musique AdLib (OPL2) extraite avec succès depuis " << exePath << " -> " << destPath << " (18 184 octets)\n";
    return true;
}

// Locate candidate asset directories
std::string resolveAssetDirectory(const std::string& userSpecified) {
    std::vector<std::string> candidates;
    if (!userSpecified.empty()) {
        candidates.push_back(userSpecified);
    }
    // Specific path requested by user:
    candidates.push_back("/home/manianis/snap/dosbox-x/1598/dosbox/Moktar");
    candidates.push_back("/home/manianis/snap/dosbox-x/1598/dosbox");
    // Local workspace paths:
    candidates.push_back("./original_assets");
    candidates.push_back("./assets");
    candidates.push_back(".");

    for (const auto& path : candidates) {
        if (fs::exists(path) && fs::is_directory(path)) {
            // Check if level0.sqz or sprites.sqz exists here
            if (!findAssetFile(path, "level0.sqz").empty() && !findAssetFile(path, "sprites.sqz").empty()) {
                return fs::canonical(path).string();
            }
        }
    }
    return "";
}

void printBanner() {
    std::cout << "\033[1;36m";
    std::cout << R"(
  __  __       _    _              _     _                       
 |  \/  |     | |  | |            | |   (_)                      
 | \  / | ___ | | _| |_ __ _ _ __ | |    _ _ __  _   ___  __     
 | |\/| |/ _ \| |/ / __/ _` | '__|| |   | | '_ \| | | \ \/ /     
 | |  | | (_) |   <| || (_| | |   | |___| | | | | |_| |>  <      
 |_|  |_|\___/|_|\_\\__\__,_|_|   |______|_|_| |_|\__,_/_/\_\     
    Lagaf': Les Aventures de Moktar - Vol 1: La Zoubida (1991)
    Réplique C++ Linux Native avec émulation sonore AdLib OPL2
)" << "\033[0m\n";
}

void printHelp(const char* progName) {
    printBanner();
    std::cout << "Utilisation: " << progName << " [OPTIONS]\n\n"
              << "Options disponibles:\n"
              << "  -h, --help           Affiche cette aide et quitte\n"
              << "  -a, --assets <DIR>   Spécifie le dossier contenant les fichiers originaux du jeu\n"
              << "  -l, --level <1-16>   Démarre directement au niveau spécifié (1 à 16)\n"
              << "  -s, --scale <1-4>    Facteur d'échelle de la fenêtre (1x, 2x, 3x=960x600, 4x)\n"
              << "  -f, --fullscreen     Lance le jeu en plein écran avec maintien du ratio 4:3\n"
              << "  -g, --godmode        Active l'invulnérabilité (Trainer PV illimités)\n"
              << "      --noclip         Active le mode traversée des murs\n"
              << "      --skip-intro     Passe directement au menu principal\n"
              << "      --nomusic        Désactive la musique AdLib FM\n"
              << "  -i, --info           Affiche les informations d'analyse reverse engineering\n\n"
              << "Commandes en jeu:\n"
              << "  Flèches / WASD / ZQSD Déplacement, accroupi, ramper, viser\n"
              << "  Espace / Ctrl / Haut  Sauter\n"
              << "  C / X / Entrée / Espace Ramasser / Poser / Jeter un objet\n"
              << "  P                     Pause\n"
              << "  M / F3                Activer / Désactiver la musique AdLib\n"
              << "  F11 / Alt + Entrée    Plein écran (Toggle)\n"
              << "  F5                    Trainer : PV illimités (Godmode)\n"
              << "  F6                    Trainer : Vies infinies\n"
              << "  F7                    Trainer : Mode Traversée (Noclip)\n"
              << "  F8 / PageDown         Trainer : Saut au niveau suivant\n"
              << "  F1                    Suicide / Perdre une vie\n"
              << "  F4                    Afficher l'écran de statut\n"
              << "  E                     Afficher la jauge d'énergie\n"
              << "  Échap                 Quitter / Retour menu\n\n";
}

void printReverseEngineeringInfo() {
    printBanner();
    std::cout << "\033[1;33m--- RAPPORT DE REVERSE ENGINEERING DU JEU MOKTAR (1991) ---\033[0m\n\n"
              << "1. Structure du conteneur d'archives (.SQZ):\n"
              << "   - Entête de 4 octets: [b1, comp_type, b3, b4]\n"
              << "   - Taille décompressée: (b1 & 0x0F) << 16 | (b4 & 0xFF) << 8 | (b3 & 0xFF)\n"
              << "   - Type 0x10: Décompression LZW (dictionnaire adaptatif 9 à 12 bits, codes 0x100=reset, 0x101=end)\n"
              << "   - Autre type: Arbre binaire de Huffman avec décodage RLE par paquets\n\n"
              << "2. Format des niveaux (LEVELx.SQZ):\n"
              << "   - Hauteur calculée: (taille_décompressée - 35828) / 256 tuiles\n"
              << "   - Grille de tuiles: 256 colonnes x Hauteur lignes (1 octet par index de tuile)\n"
              << "   - Banque de 256 tuiles de 16x16 pixels stockées en 4 plans de bits EGA/VGA (128 octets/tuile)\n"
              << "   - Masques de collisions: horizontal, sol/pente et plafond/permutation d'animation\n"
              << "   - Table d'objets interactifs (40 objets x 6 octets: type, coordonnées X,Y, état)\n"
              << "   - Table des ennemis et pièges (50 entités x 25 octets: 19 comportements IA)\n\n"
              << "3. Format des sprites (SPRITES.SQZ):\n"
              << "   - Planar 4 bits (16 couleurs): groupsize = (largeur * hauteur) / 8\n"
              << "   - 4 plans entrelacés (Plan 0, 1, 2, 3) pour reconstituer l'index de couleur de chaque pixel\n"
              << "   - Table de métadonnées pour Moktar: largeur, hauteur, hitbox et points d'accroche\n\n"
              << "4. Synthèse audio AdLib / OPL2:\n"
              << "   - Emplacement dans MOKTAR.EXE: offset 28817, longueur 18184 octets\n"
              << "   - Structure: pistes mélodiques, définitions des 19 instruments FM (opérateurs, attack, decay,\n"
              << "     sustain, release, feedback) et 14 effets sonores (SFX)\n"
              << "   - Pilote OPL2: cadence d'horloge DOS ~72.7 Hz (timer 13.75 ms) émulé via DBOPL\n\n"
              << "5. Niveaux et mots de passe officiels de Moktar (16 niveaux):\n"
              << "   [Niv 01] A LA RECHERCHE DE LA ZOUBIDA  ->  EFE8\n"
              << "   [Niv 02] LES QUARTIERS CHICS           ->  5165\n"
              << "   [Niv 03] ATTENTION TRAVAUX             ->  67D4\n"
              << "   [Niv 04] LES COULOIRS DU METRO         ->  2BDA\n"
              << "   [Niv 05] LES CATACOMBES INFERNALES     ->  11E5\n"
              << "   [Niv 06] ARRIVEE DANS LA CITE          ->  86EE\n"
              << "   [Niv 07] L IMMEUBLE DE LA ZOUBIDA      ->  4275\n"
              << "   [Niv 08] SOUS LE CHEMIN DE MARRAKECH   ->  A0B9\n"
              << "   [Niv 09] LA CITE ENFOUIE               ->  501C\n"
              << "   [Niv 10] DESERT PRIVE                  ->  E9ED\n"
              << "   [Niv 11] LA VILLE DES SABLES           ->  D4E6\n"
              << "   [Niv 12] LE PHARE OUEST                ->  A531\n"
              << "   [Niv 13] UN BON TUYAU                  ->  CE96\n"
              << "   [Niv 14] DE RETOUR AU PAYS             ->  B1A4\n"
              << "   [Niv 15] DIRECTION BARBES              ->  EBEA (Boss Moktar)\n"
              << "   [Niv 16] BIG BISOUS                    ->  3B9C\n\n";
}

int main(int argc, char* argv[]) {
    GameOptions opt;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printHelp(argv[0]);
            return 0;
        } else if (arg == "-i" || arg == "--info") {
            printReverseEngineeringInfo();
            return 0;
        } else if (arg == "-a" || arg == "--assets") {
            if (i + 1 < argc) opt.assetDir = argv[++i];
        } else if (arg == "-l" || arg == "--level") {
            if (i + 1 < argc) opt.directLevel = std::max(1, std::min(16, std::stoi(argv[++i])));
        } else if (arg == "-s" || arg == "--scale") {
            if (i + 1 < argc) opt.scale = std::max(1, std::min(4, std::stoi(argv[++i])));
        } else if (arg == "-f" || arg == "--fullscreen") {
            opt.fullscreen = true;
        } else if (arg == "-g" || arg == "--godmode") {
            opt.godmode = true;
        } else if (arg == "--noclip") {
            opt.noclip = true;
        } else if (arg == "--skip-intro") {
            opt.skipIntro = true;
        } else if (arg == "--nomusic") {
            opt.enableMusic = false;
        }
    }

    printBanner();

    // 1. Resolve assets directory
    std::string assetsDir = resolveAssetDirectory(opt.assetDir);
    if (assetsDir.empty()) {
        std::cerr << "\033[1;31m[ERREUR] Impossible de trouver les fichiers originaux du jeu Moktar !\033[0m\n"
                  << "Veuillez spécifier le dossier avec l'option --assets <DOSSIER>\n"
                  << "Exemple: " << argv[0] << " --assets /home/manianis/snap/dosbox-x/1598/dosbox/Moktar\n";
        return 1;
    }

    std::cout << "[*] Dossier de jeu détecté: \033[1;32m" << assetsDir << "\033[0m\n";

    // 2. Check for music_mok.bin in candidate directories
    std::vector<std::string> musicCandidates = {
        findAssetFile(assetsDir, "music_mok.bin"),
        findAssetFile(".", "music_mok.bin"),
        findAssetFile("./original_assets", "music_mok.bin"),
        findAssetFile("./opentitus_ref/bin", "music_mok.bin")
    };
    std::string musicPath = "";
    for (const auto& cand : musicCandidates) {
        if (!cand.empty() && fs::exists(cand)) {
            try {
                if (fs::file_size(cand) == 18184) {
                    musicPath = cand;
                    break;
                }
            } catch (...) {}
        }
    }

    if (musicPath.empty()) {
        std::cerr << "[!] Avertissement: Musique AdLib (music_mok.bin) introuvable. Audio FM désactivé.\n";
        opt.enableMusic = false;
    } else {
        std::cout << "[*] Fichier musique AdLib (OPL2): " << musicPath << "\n";
        // Ensure music_mok.bin is in current directory for engine loader
        if (musicPath != "./music_mok.bin" && musicPath != "music_mok.bin") {
            try {
                fs::copy_file(musicPath, "./music_mok.bin", fs::copy_options::overwrite_existing);
            } catch (...) {}
        }
    }

    // 3. Map level files
    const char* levelFileNames[16] = {
        "level0.sqz", "levelj.sqz", "level1.sqz", "level2.sqz",
        "level3.sqz", "level4.sqz", "level5.sqz", "level6.sqz",
        "level7.sqz", "level8.sqz", "level9.sqz", "levelb.sqz",
        "levelc.sqz", "levele.sqz", "levelf.sqz", "levelg.sqz"
    };

    // 4. Generate dynamic titus.conf
    std::ofstream cfg("titus.conf");
    if (!cfg.is_open()) {
        std::cerr << "[!] Impossible de générer la configuration titus.conf\n";
        return 1;
    }

    int windowWidth = 320 * opt.scale;
    int windowHeight = 200 * opt.scale;

    cfg << "# Fichier généré automatiquement pour Moktar Linux C++\n"
        << "reswidth 320\n"
        << "resheight 200\n"
        << "windowwidth " << windowWidth << "\n"
        << "windowheight " << windowHeight << "\n"
        << "devmode " << (opt.godmode ? 1 : 0) << "\n"
        << "bitdepth 32\n"
        << "ingamewidth 320\n"
        << "ingameheight 192\n"
        << "videomode " << (opt.fullscreen ? 1 : 0) << "\n"
        << "game 1\n"
        << "levelcount 16\n\n";

    for (int i = 0; i < 16; ++i) {
        std::string lpath = findAssetFile(assetsDir, levelFileNames[i]);
        if (lpath.empty()) {
            std::cerr << "[!] Fichier de niveau manquant: " << levelFileNames[i] << "\n";
        }
        cfg << "level " << (i + 1) << " " << (lpath.empty() ? levelFileNames[i] : lpath) << "\n";
    }

    std::string spritesPath = findAssetFile(assetsDir, "sprites.sqz");
    std::string logoPath = findAssetFile(assetsDir, "titus.sqz");
    std::string introPath = findAssetFile(assetsDir, "titre.sqz");
    std::string menuPath = findAssetFile(assetsDir, "menu.sqz");
    std::string fontPath = findAssetFile(assetsDir, "fonts.sqz");

    cfg << "\nsprites " << (spritesPath.empty() ? "sprites.sqz" : spritesPath) << "\n"
        << "logo " << (logoPath.empty() ? "titus.sqz" : logoPath) << "\n"
        << "logoformat 2\n"
        << "intro " << (introPath.empty() ? "titre.sqz" : introPath) << "\n"
        << "introformat 2\n"
        << "menu " << (menuPath.empty() ? "menu.sqz" : menuPath) << "\n"
        << "menuformat 2\n"
        << "font " << (fontPath.empty() ? "fonts.sqz" : fontPath) << "\n";

    cfg.close();

    // 5. Configure environment variables for optimal SDL scaling and controller support
    std::string scaleStr = std::to_string(opt.scale);
    setenv("SDL12COMPAT_WINDOW_SCALING", scaleStr.c_str(), 1);
    setenv("SDL12COMPAT_SCALE_METHOD", "nearest", 1);
    setenv("SDL12COMPAT_USE_GAME_CONTROLLERS", "1", 1);
    setenv("SDL12COMPAT_SYNC_TO_VBLANK", "1", 1);

    std::cout << "[*] Initialisation du moteur Moktar C++ (Résolution: "
              << windowWidth << "x" << windowHeight 
              << ", Mode: " << (opt.fullscreen ? "Plein écran" : "Fenêtré")
              << ", Audio OPL: " << (opt.enableMusic ? "Activé" : "Désactivé") << ")...\n";

    // 6. Initialize game engine
    int initStatus = init_game();
    if (initStatus < 0) {
        std::cerr << "\033[1;31m[ERREUR] Échec de l'initialisation du moteur graphique/audio.\033[0m\n";
        checkerror();
        return 1;
    }

    // Apply trainer options if requested
    if (opt.godmode) {
        GODMODE = true;
        std::cout << "[*] Mode Invulnérabilité (Godmode) activé !\n";
    }
    if (opt.noclip) {
        NOCLIP = true;
        GODMODE = true;
        std::cout << "[*] Mode Noclip (Traversée des murs) activé !\n";
    }

    int state = 1;
    int retval = 0;

    // 7. Intro & Menu sequences
    if (!opt.skipIntro) {
        retval = viewintrotext();
        if (retval < 0) state = 0;

        if (state) {
            retval = viewimage(tituslogofile, tituslogoformat, 0, 3000);
            if (retval < 0) state = 0;
        }

        if (state && opt.enableMusic) {
            SELECT_MUSIC(15); // "La Zoubida" intro theme
        }

        if (state) {
            retval = viewimage(titusintrofile, titusintroformat, 0, 5000);
            if (retval < 0) state = 0;
        }
    }

    // 8. Main game loop
    if (opt.directLevel >= 1) {
        std::cout << "[*] Lancement direct du Niveau " << opt.directLevel << " (" << leveltitle[opt.directLevel - 1] << ")...\n";
        playtitus(opt.directLevel - 1);
    } else {
        while (state) {
            if (opt.enableMusic && (last_song != 15 || is_music_finished())) {
                SELECT_MUSIC(15); // Thème menu "La Zoubida"
            }
            retval = viewmenu(titusmenufile, titusmenuformat);
            if (retval <= 0) {
                state = 0;
            }
            if (state && retval <= levelcount) {
                std::cout << "[*] Lancement du Niveau " << retval << "...\n";
                retval = playtitus(retval - 1);
                if (retval < 0) state = 0;
            }
        }
    }

    // 9. Clean shutdown
    freefonts();
    if (opt.enableMusic) {
        freeaudio();
    }

    SDL_Quit();
    std::cout << "[*] Merci d'avoir joué à Moktar ! À bientôt.\n";
    return 0;
}
