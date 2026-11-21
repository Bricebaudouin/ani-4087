// Le type opaque : quels fichiers .cpp ont besoin d'un en-tete externe ?
//
// Entree : E puis E noms d'en-tetes externes, puis F, puis F lignes
//          "nom k i1 ... ik" (un fichier du projet et ce qu'il inclut).
// Sortie : une ligne par unite .cpp ("nom OUI chemin" ou "nom NON"),
//          puis "TOUCHEES t SUR u".

#include <cstddef>
#include <iostream>
#include <map>
#include <optional>
#include <queue>
#include <set>
#include <string>
#include <vector>

namespace {

struct Projet {
    std::set<std::string> externes;
    std::map<std::string, std::vector<std::string>> inclusions;
};

bool FinitParCpp(const std::string& nom) {
    const std::string suffixe = ".cpp";
    return nom.size() >= suffixe.size() &&
           nom.compare(nom.size() - suffixe.size(), suffixe.size(), suffixe) == 0;
}

// Remonte de l'en-tete externe jusqu'a l'unite, puis rend le chemin a l'endroit.
std::string ConstruireChemin(const std::map<std::string, std::string>& parent,
                             const std::string& unite, const std::string& externe) {
    std::vector<std::string> noms;
    std::string courant = externe;
    while (courant != unite) {
        noms.push_back(courant);
        courant = parent.at(courant);
    }
    noms.push_back(unite);

    std::string chemin;
    for (std::size_t i = noms.size(); i > 0; --i) {
        chemin += noms[i - 1];
        if (i > 1) {
            chemin += '>';
        }
    }
    return chemin;
}

// Parcours par niveaux depuis l'unite. Un nom deja rencontre (unite comprise)
// n'est jamais repris. Rend le chemin jusqu'au premier en-tete externe
// rencontre, ou rien si aucun n'est atteint.
std::optional<std::string> ChercherExterne(const Projet& projet, const std::string& unite) {
    std::map<std::string, std::string> parent;  // nom -> fichier qui l'a fait rencontrer
    std::set<std::string> vus;
    std::queue<std::string> file;

    vus.insert(unite);
    file.push(unite);

    while (!file.empty()) {
        const std::string courant = file.front();
        file.pop();

        const auto trouve = projet.inclusions.find(courant);
        if (trouve == projet.inclusions.end()) {
            continue;  // en-tete du systeme : il n'inclut rien
        }
        for (const std::string& nom : trouve->second) {
            if (!vus.insert(nom).second) {
                continue;
            }
            parent[nom] = courant;
            if (projet.externes.count(nom) != 0) {
                return ConstruireChemin(parent, unite, nom);  // un externe n'inclut rien
            }
            file.push(nom);
        }
    }
    return std::nullopt;
}

}  // namespace

int main() {
    Projet projet;

    std::size_t e = 0;
    std::cin >> e;
    for (std::size_t i = 0; i < e; ++i) {
        std::string nom;
        std::cin >> nom;
        projet.externes.insert(nom);
    }

    std::size_t f = 0;
    std::cin >> f;
    std::vector<std::string> fichiers;  // ordre de l'entree
    for (std::size_t i = 0; i < f; ++i) {
        std::string nom;
        std::size_t k = 0;
        std::cin >> nom >> k;
        std::vector<std::string> inclus;
        for (std::size_t j = 0; j < k; ++j) {
            std::string cible;
            std::cin >> cible;
            inclus.push_back(cible);
        }
        fichiers.push_back(nom);
        projet.inclusions[nom] = inclus;
    }

    std::size_t unites = 0;
    std::size_t touchees = 0;
    for (const std::string& nom : fichiers) {
        if (!FinitParCpp(nom)) {
            continue;
        }
        ++unites;
        const std::optional<std::string> chemin = ChercherExterne(projet, nom);
        if (chemin) {
            ++touchees;
            std::cout << nom << " OUI " << *chemin << '\n';
        } else {
            std::cout << nom << " NON" << '\n';
        }
    }
    std::cout << "TOUCHEES " << touchees << " SUR " << unites << '\n';
    return 0;
}
