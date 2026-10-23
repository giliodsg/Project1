#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <random>
#include <cctype>
#include <conio.h>
#include <chrono>
#include <thread>

struct Map {
    int ground = 0;
    int water = 1;
    int mountain = 2;
    int exit = 3;
};

struct hero {
    int hp = 1;
    int id = 0;
};

struct Position {
    int ligne = 0;
    int colonne = 0;

    bool operator==(const Position& other) const {
        return ligne == other.ligne && colonne == other.colonne;
    }
};

struct Ennemi {
    Position pos;
};

// Structure représentant un niveau complet 
struct Level {
    int grille[10][20]{};
    Position heroStart{};
    Position exitPos{};
    std::vector<Ennemi> ennemis;
};

// Fonction pour initialiser et charger tous les niveaux hardcodés
std::vector<Level> ChargerNiveaux(const Map& config) {
    std::vector<Level> niveaux;

    // --- NIVEAU 1 ---
    Level lvl1;
    lvl1.heroStart = { 4, 0 };
    lvl1.exitPos = { 9, 19 };
    lvl1.ennemis = {
        { { 2, 2 } },
        { { 4, 11 } },
        { { 6, 11 } },
        { { 8, 8 } },
        { { 8, 16 } }
    };

    for (int Ligne = 0; Ligne < 10; ++Ligne) {
        for (int Colonne = 0; Colonne < 20; ++Colonne) {
            lvl1.grille[Ligne][Colonne] = config.ground;
        }
    }
    // Eau
    for (int Ligne = 1; Ligne <= 3; ++Ligne) {
        for (int Colonne = 5; Colonne <= 8; ++Colonne) {
            lvl1.grille[Ligne][Colonne] = config.water;
        }
    }
    // Montagnes
    lvl1.grille[2][12] = config.mountain;
    lvl1.grille[2][13] = config.mountain;
    lvl1.grille[2][14] = config.mountain;
    lvl1.grille[6][2] = config.mountain;
    lvl1.grille[6][3] = config.mountain;
    lvl1.grille[6][4] = config.mountain;
    lvl1.grille[6][16] = config.water;
    lvl1.grille[6][17] = config.water;

    // Positionner la sortie dans la grille
    lvl1.grille[lvl1.exitPos.ligne][lvl1.exitPos.colonne] = config.exit;
    niveaux.push_back(lvl1);

    // --- NIVEAU 2 ---
    Level lvl2;
    lvl2.heroStart = { 0, 0 };
    lvl2.exitPos = { 5, 10 };
    lvl2.ennemis = {
        { { 1, 5 } },
        { { 3, 8 } },
        { { 7, 2 } }
    };

    for (int Ligne = 0; Ligne < 10; ++Ligne) {
        for (int Colonne = 0; Colonne < 20; ++Colonne) {
            lvl2.grille[Ligne][Colonne] = config.ground;
        }
    }
    // Murs / Montagnes pour former un couloir
    for (int c = 0; c < 15; ++c) lvl2.grille[4][c] = config.mountain;
    for (int c = 5; c < 20; ++c) lvl2.grille[6][c] = config.mountain;

    lvl2.grille[lvl2.exitPos.ligne][lvl2.exitPos.colonne] = config.exit;
    niveaux.push_back(lvl2);

    return niveaux;
}

bool EstTraversable(int grille[10][20], int ligne, int colonne, const Map& config) {
    if (ligne < 0 || ligne >= 10 || colonne < 0 || colonne >= 20) {
        return false;
    }

    int caseValeur = grille[ligne][colonne];
    if (caseValeur == config.water || caseValeur == config.mountain) {
        return false;
    }
    return true;
}

void DeplacerEnnemis(int grille[10][20], std::vector<Ennemi>& ennemis, const Map& config) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> dist(0, 3);

    for (auto& ennemi : ennemis) {
        int dir = dist(gen);
        int nLigne = ennemi.pos.ligne;
        int nColonne = ennemi.pos.colonne;

        if (dir == 0) nLigne--;
        else if (dir == 1) nLigne++;
        else if (dir == 2) nColonne--;
        else if (dir == 3) nColonne++;

        if (EstTraversable(grille, nLigne, nColonne, config) && grille[nLigne][nColonne] != config.exit) {
            ennemi.pos.ligne = nLigne;
            ennemi.pos.colonne = nColonne;
        }
    }
}

void AfficherGrille(int grille[10][20], int heroLigne, int heroColonne, const std::vector<Ennemi>& ennemis, Map config, size_t numNiveau) {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif

    std::cout << "--- NIVEAU " << numNiveau + 1 << " ---\n\n";

    for (int Ligne = 0; Ligne < 10; ++Ligne) {
        for (int Colonne = 0; Colonne < 20; ++Colonne) {
            if (Ligne == heroLigne && Colonne == heroColonne) {
                std::cout << 'H';
            }
            else {
                bool estEnnemi = false;
                for (const auto& e : ennemis) {
                    if (e.pos.ligne == Ligne && e.pos.colonne == Colonne) {
                        estEnnemi = true;
                        break;
                    }
                }

                if (estEnnemi) {
                    std::cout << 'X';
                }
                else {
                    int caseValeur = grille[Ligne][Colonne];
                    if (caseValeur == config.ground) {
                        std::cout << '.';
                    }
                    else if (caseValeur == config.water) {
                        std::cout << '~';
                    }
                    else if (caseValeur == config.mountain) {
                        std::cout << 'A';
                    }
                    else if (caseValeur == config.exit) {
                        std::cout << 'E';
                    }
                }
            }
        }
        std::cout << '\n';
    }
}

bool VerificationCollision(int heroLigne, int heroColonne, const std::vector<Ennemi>& ennemis, hero& player) {
    for (const auto& e : ennemis) {
        if (e.pos.ligne == heroLigne && e.pos.colonne == heroColonne) {
            player.hp--;
            return true;
        }
    }
    return false;
}

void TraiterGameOver() {
    std::cout << "\n           GAME OVER           \n";
    std::cout << "   Va dormir sah c'est mieux pour toi \n\n";

    std::this_thread::sleep_for(std::chrono::seconds(2));

    while (_kbhit()) {
        (void)_getch();
    }

    std::cout << "Appuyez sur une touche pour quitter...";
    (void)_getch();
}

int main() {
    Map config;
    hero player;

    std::vector<Level> niveaux = ChargerNiveaux(config);
    size_t indexNiveau = 0;

    Level niveauActuel = niveaux[indexNiveau];
    int heroLigne = niveauActuel.heroStart.ligne;
    int heroColonne = niveauActuel.heroStart.colonne;

    bool jeuEnCours = true;

    while (jeuEnCours) {
        AfficherGrille(niveauActuel.grille, heroLigne, heroColonne, niveauActuel.ennemis, config, indexNiveau);

        std::cout << "\nDeplacement (Z-Haut, S-Bas, Q-Gauche, D-Droite, K-Quitter) : " << std::flush;

        char action = static_cast<char>(_getch());
        action = static_cast<char>(std::toupper(static_cast<unsigned char>(action)));

        int nouvelleLigne = heroLigne;
        int nouvelleColonne = heroColonne;

        if (action == 'Z') nouvelleLigne--;
        else if (action == 'S') nouvelleLigne++;
        else if (action == 'Q') nouvelleColonne--;
        else if (action == 'D') nouvelleColonne++;
        else if (action == 'K') {
            jeuEnCours = false;
            continue;
        }

        if (EstTraversable(niveauActuel.grille, nouvelleLigne, nouvelleColonne, config)) {
            heroLigne = nouvelleLigne;
            heroColonne = nouvelleColonne;
        }


        if (heroLigne == niveauActuel.exitPos.ligne && heroColonne == niveauActuel.exitPos.colonne) {
            indexNiveau++;
            if (indexNiveau < niveaux.size()) {
                std::cout << "\n\nPlease wait...\n";
                std::this_thread::sleep_for(std::chrono::seconds(2));

                niveauActuel = niveaux[indexNiveau];
                heroLigne = niveauActuel.heroStart.ligne;
                heroColonne = niveauActuel.heroStart.colonne;
                continue;
            }
            else {
                std::cout << "\n\nVictory\n";
                (void)_getch();
                jeuEnCours = false;
                continue;
            }
        }

        // Collision après déplacement du joueur
        if (VerificationCollision(heroLigne, heroColonne, niveauActuel.ennemis, player)) {
            if (player.hp <= 0) {
                AfficherGrille(niveauActuel.grille, heroLigne, heroColonne, niveauActuel.ennemis, config, indexNiveau);
                TraiterGameOver();
                jeuEnCours = false;
                continue;
            }
        }

        // Déplacement des ennemis
        DeplacerEnnemis(niveauActuel.grille, niveauActuel.ennemis, config);

        if (VerificationCollision(heroLigne, heroColonne, niveauActuel.ennemis, player)) {
            if (player.hp <= 0) {
                AfficherGrille(niveauActuel.grille, heroLigne, heroColonne, niveauActuel.ennemis, config, indexNiveau);
                TraiterGameOver();
                jeuEnCours = false;
            }
        }
    }

    return 0;
}