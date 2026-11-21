// Premiere passe : lire l'en-tete d'un fichier source et dire ce qu'il annonce.
//
// Entree  : une ligne avec N, puis N lignes du fichier, telles quelles.
// Sortie  : ENTETE e, IGNOREES i, une ligne VARIABLE par variable annoncee
//           (ou VARIABLE AUCUNE), puis PART p (pourcentage lu, arrondi bas).

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {

// Un signe de nom de variable : majuscule, chiffre ou '_'.
bool EstSigneDeNom(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
}

// Lettre ou chiffre (hors '_'), pour tester le caractere qui precede NK_.
bool EstLettreOuChiffre(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
}

// Vrai si la ligne, une fois ses espaces et tabulations de debut otes,
// commence par "//". Une ligne vide (ou blanche) n'est pas un commentaire.
bool EstCommentaire(const std::string& ligne) {
    const std::size_t debut = ligne.find_first_not_of(" \t");
    if (debut == std::string::npos) {
        return false;
    }
    return ligne.compare(debut, 2, "//") == 0;
}

// Ajoute a 'noms' chaque variable NK_xxx= trouvee dans la ligne,
// si elle n'y figure pas deja (ordre de premiere apparition conserve).
void CollecterVariables(const std::string& ligne, std::vector<std::string>& noms) {
    const std::string marque = "NK_";
    std::size_t pos = ligne.find(marque);
    while (pos != std::string::npos) {
        const bool avantValide =
            (pos == 0) || (!EstLettreOuChiffre(ligne[pos - 1]) && ligne[pos - 1] != '_');

        std::size_t fin = pos + marque.size();
        while (fin < ligne.size() && EstSigneDeNom(ligne[fin])) {
            ++fin;
        }
        const bool aDesSignes = fin > pos + marque.size();
        const bool suiviDeEgal = fin < ligne.size() && ligne[fin] == '=';

        if (avantValide && aDesSignes && suiviDeEgal) {
            const std::string nom = ligne.substr(pos, fin - pos);
            if (std::find(noms.begin(), noms.end(), nom) == noms.end()) {
                noms.push_back(nom);
            }
        }
        pos = ligne.find(marque, pos + 1);
    }
}

}  // namespace

int main() {
    // Lecture de N.
    long long n = 0;
    std::string premiere;
    if (std::getline(std::cin, premiere)) {
        std::istringstream flux(premiere);
        if (!(flux >> n) || n < 0) {
            n = 0;
        }
    }

    // Lecture des N lignes, telles quelles.
    std::vector<std::string> lignes;
    std::string ligne;
    while (static_cast<long long>(lignes.size()) < n && std::getline(std::cin, ligne)) {
        lignes.push_back(ligne);
    }

    // En-tete : bloc de commentaires // du haut du fichier.
    std::size_t entete = 0;
    std::vector<std::string> noms;
    while (entete < lignes.size() && EstCommentaire(lignes[entete])) {
        CollecterVariables(lignes[entete], noms);
        ++entete;
    }

    const long long e = static_cast<long long>(entete);
    const long long ignorees = n - e;
    const long long part = (n == 0) ? 0 : (100 * e) / n;

    std::cout << "ENTETE " << e << '\n';
    std::cout << "IGNOREES " << ignorees << '\n';
    if (noms.empty()) {
        std::cout << "VARIABLE AUCUNE" << '\n';
    } else {
        for (const std::string& nom : noms) {
            std::cout << "VARIABLE " << nom << '\n';
        }
    }
    std::cout << "PART " << part << '\n';
    return 0;
}
