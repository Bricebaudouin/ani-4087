#include <iostream>
#include <string>
#include <vector>

namespace {

// Une application, telle qu'elle est decrite sur l'entree.
struct Application {
    std::string fin;           // TOUJOURS, SI_RENDU ou JAMAIS : quand la boucle appelle EndFrame
    int chaineGauche = 1;      // la chaine de l'oeil gauche est-elle dans la projection ?
    int chaineDroite = 1;      // la chaine de l'oeil droit est-elle dans la projection ?
    char instant = 'P';        // P : instant predit, N : instant courant
    char pose = 'R';           // R : pose du rendu, F : pose relue
    char espace = 'S';         // S : le bon espace, L : le mauvais
};

// Les symptomes que l'application donnerait sur un casque, dans cet ordre.
std::vector<std::string> SymptomesSurCasque(const Application &app) {
    std::vector<std::string> symptomes;

    if (app.fin == "JAMAIS") {
        symptomes.push_back("FIGE");
    } else if (app.fin == "SI_RENDU") {
        symptomes.push_back("NE_REPART_PAS");
    }
    if (app.chaineGauche == 0) {
        symptomes.push_back("OEIL_NOIR_G");
    }
    if (app.chaineDroite == 0) {
        symptomes.push_back("OEIL_NOIR_D");
    }
    if (app.instant == 'N') {
        symptomes.push_back("TREMBLE");
    }
    if (app.pose == 'F') {
        symptomes.push_back("DE_TRAVERS");
    }
    if (app.espace == 'L') {
        symptomes.push_back("DECALE");
    }
    return symptomes;
}

// Le simulateur n'ecrit que pour une fin de trame manquante ou une chaine absente.
// Les fautes d'instant, de pose et d'espace lui echappent.
bool SimulateurDit(const Application &app) {
    return app.fin != "TOUJOURS" || app.chaineGauche == 0 || app.chaineDroite == 0;
}

} // namespace

int main() {
    int nbApplications = 0;
    std::cin >> nbApplications;

    int nbMuettes = 0;

    for (int numero = 1; numero <= nbApplications; ++numero) {
        Application app;
        std::cin >> app.fin >> app.chaineGauche >> app.chaineDroite
                 >> app.instant >> app.pose >> app.espace;

        const std::vector<std::string> symptomes = SymptomesSurCasque(app);
        const bool dit = SimulateurDit(app);

        // Ligne du casque.
        std::cout << numero << " CASQUE";
        if (symptomes.empty()) {
            std::cout << " AUCUN";
        }
        for (const std::string &symptome : symptomes) {
            std::cout << " " << symptome;
        }
        std::cout << "\n";

        // Ligne du simulateur.
        std::cout << numero << " SIMULATEUR " << (dit ? "DIT" : "MUET") << "\n";

        // Muette : le casque montre un symptome, mais le simulateur ne dit rien.
        if (!symptomes.empty() && !dit) {
            ++nbMuettes;
        }
    }

    std::cout << "MUETTES " << nbMuettes << "\n";
    return 0;
}
