// Comparer les methodes publiques de deux backends.
//
// Entree : deux listes, chacune introduite par son nombre de noms, les noms
//          etant separes par des espaces ou des sauts de ligne.
//          La liste A est le vrai casque, la liste B est le simulateur.
// Sortie : DEUX k, puis SEUL_A nom (ordre de A), puis SEUL_B nom (ordre de B),
//          puis DIFFERENTES d SUR u.

#include <cstddef>
#include <iostream>
#include <set>
#include <string>
#include <vector>

namespace {

// Lit un compte puis autant de noms, sans doublon, dans l'ordre de lecture.
std::vector<std::string> LireListe(std::istream& entree) {
    std::size_t n = 0;
    entree >> n;

    std::vector<std::string> noms;
    std::set<std::string> vus;
    std::string nom;
    for (std::size_t i = 0; i < n && entree >> nom; ++i) {
        if (vus.insert(nom).second) {
            noms.push_back(nom);
        }
    }
    return noms;
}

}  // namespace

int main() {
    const std::vector<std::string> a = LireListe(std::cin);
    const std::vector<std::string> b = LireListe(std::cin);

    const std::set<std::string> ensembleA(a.begin(), a.end());
    const std::set<std::string> ensembleB(b.begin(), b.end());

    std::size_t communs = 0;
    std::vector<std::string> seulA;
    for (const std::string& nom : a) {
        if (ensembleB.count(nom) != 0) {
            ++communs;
        } else {
            seulA.push_back(nom);
        }
    }

    std::vector<std::string> seulB;
    for (const std::string& nom : b) {
        if (ensembleA.count(nom) == 0) {
            seulB.push_back(nom);
        }
    }

    // d : noms presents dans une seule liste. u : noms differents des deux
    // listes reunies (les communs ne sont comptes qu'une fois).
    const std::size_t d = seulA.size() + seulB.size();
    const std::size_t u = communs + d;

    std::cout << "DEUX " << communs << '\n';
    for (const std::string& nom : seulA) {
        std::cout << "SEUL_A " << nom << '\n';
    }
    for (const std::string& nom : seulB) {
        std::cout << "SEUL_B " << nom << '\n';
    }
    std::cout << "DIFFERENTES " << d << " SUR " << u << '\n';
    return 0;
}
