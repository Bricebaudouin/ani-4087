// Tous vos reglages : lesquels un menu peut-il vraiment changer ?
//
// Entree : un entier R, puis R lignes "nom action" avec action parmi
//          DECLARE, ECRIT, ENV, LIT, GETENV.
// Sortie : "nom CLASSE" par reglage, dans l'ordre de sa premiere apparition,
//          puis "MENU m SUR n".

#include <cstddef>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace {

struct Usage {
    bool ecrit = false;   // le code de l'application le pose
    bool env = false;     // la fonction des variables d'environnement le pose
    bool lit = false;     // un backend ou l'application s'en sert
    bool getenv = false;  // un fichier lit lui-meme la variable, sans le champ
};

// Premiere regle qui s'applique. DECLARE ne change rien : ce n'est pas une
// ecriture, la valeur de depart ne se change pas par la.
std::string Classer(const Usage& u) {
    if (u.getenv) {
        return "CACHE";
    }
    if (!u.lit) {
        return "FANTOME";
    }
    if (!u.ecrit && !u.env) {
        return "CONSTANTE";
    }
    return "MENU";
}

}  // namespace

int main() {
    std::size_t r = 0;
    std::cin >> r;

    std::vector<std::string> ordre;  // ordre de premiere apparition
    std::map<std::string, Usage> usages;

    for (std::size_t i = 0; i < r; ++i) {
        std::string nom;
        std::string action;
        if (!(std::cin >> nom >> action)) {
            break;
        }
        if (usages.find(nom) == usages.end()) {
            ordre.push_back(nom);
        }
        Usage& u = usages[nom];
        if (action == "ECRIT") {
            u.ecrit = true;
        } else if (action == "ENV") {
            u.env = true;
        } else if (action == "LIT") {
            u.lit = true;
        } else if (action == "GETENV") {
            u.getenv = true;
        }
        // DECLARE : le reglage existe, rien d'autre.
    }

    std::size_t menus = 0;
    for (const std::string& nom : ordre) {
        const std::string classe = Classer(usages[nom]);
        if (classe == "MENU") {
            ++menus;
        }
        std::cout << nom << ' ' << classe << '\n';
    }
    std::cout << "MENU " << menus << " SUR " << ordre.size() << '\n';
    return 0;
}
