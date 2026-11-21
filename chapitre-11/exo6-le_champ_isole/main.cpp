// Le champ isole : trouver les lignes qui savent quel backend tourne.
//
// Entree : "C k n1..nk" (noms qui disent quel backend tourne),
//          "D m d1..dm" (noms des definis de disponibilite),
//          N, puis N lignes "ZONE texte" (INIT, FABRIQUE, BOUCLE, SCENE).
// Sortie : FUITE l ZONE / DEFINI l ZONE pour chaque ligne fautive, puis
//          DECISIONS, FUITES, DEFINIS et le verdict sur l'abstraction.

#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {

// Lit "X n n1 ... nn" : une etiquette, un compte, puis les noms.
std::vector<std::string> LireNoms(const std::string& ligne) {
    std::istringstream flux(ligne);
    std::string etiquette;
    std::size_t n = 0;
    flux >> etiquette >> n;

    std::vector<std::string> noms;
    std::string nom;
    for (std::size_t i = 0; i < n && flux >> nom; ++i) {
        noms.push_back(nom);
    }
    return noms;
}

void RetirerRetourChariot(std::string& texte) {
    if (!texte.empty() && texte.back() == '\r') {
        texte.pop_back();
    }
}

// Lettre, chiffre ou '_' : un signe qui prolongerait un nom.
bool EstSigneDeMot(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') ||
           c == '_';
}

// Le nom est ecrit tel quel, entier : ni lettre, ni chiffre, ni '_' juste
// avant ou juste apres. Toutes les occurrences sont essayees.
bool ContientNom(const std::string& texte, const std::string& nom) {
    if (nom.empty()) {
        return false;
    }
    std::size_t pos = texte.find(nom);
    while (pos != std::string::npos) {
        const std::size_t fin = pos + nom.size();
        const bool avantValide = (pos == 0) || !EstSigneDeMot(texte[pos - 1]);
        const bool apresValide = (fin >= texte.size()) || !EstSigneDeMot(texte[fin]);
        if (avantValide && apresValide) {
            return true;
        }
        pos = texte.find(nom, pos + 1);
    }
    return false;
}

bool ContientUnDes(const std::string& texte, const std::vector<std::string>& noms) {
    for (const std::string& nom : noms) {
        if (ContientNom(texte, nom)) {
            return true;
        }
    }
    return false;
}

// Un commentaire commence par "//" une fois les espaces du debut otes.
bool EstCommentaire(const std::string& texte) {
    const std::size_t debut = texte.find_first_not_of(' ');
    return debut != std::string::npos && texte.compare(debut, 2, "//") == 0;
}

}  // namespace

int main() {
    std::string ligne;

    std::getline(std::cin, ligne);
    RetirerRetourChariot(ligne);
    const std::vector<std::string> noms_backend = LireNoms(ligne);

    std::getline(std::cin, ligne);
    RetirerRetourChariot(ligne);
    const std::vector<std::string> noms_definis = LireNoms(ligne);

    long long n = 0;
    if (std::getline(std::cin, ligne)) {
        std::istringstream flux(ligne);
        flux >> n;
    }

    long long decisions = 0;
    long long fuites = 0;
    long long definis = 0;

    for (long long numero = 1; numero <= n; ++numero) {
        if (!std::getline(std::cin, ligne)) {
            break;
        }
        RetirerRetourChariot(ligne);

        // "ZONE texte" : la zone jusqu'a la premiere espace, le texte apres.
        const std::size_t espace = ligne.find(' ');
        const std::string zone = ligne.substr(0, espace);
        const std::string texte =
            (espace == std::string::npos) ? std::string() : ligne.substr(espace + 1);

        if (EstCommentaire(texte)) {
            continue;
        }

        if (ContientUnDes(texte, noms_backend)) {
            if (zone == "INIT") {
                ++decisions;  // c'est la decision, une seule fois
            } else if (zone != "FABRIQUE") {
                ++fuites;
                std::cout << "FUITE " << numero << ' ' << zone << '\n';
            }
        }
        if (zone != "FABRIQUE" && ContientUnDes(texte, noms_definis)) {
            ++definis;
            std::cout << "DEFINI " << numero << ' ' << zone << '\n';
        }
    }

    std::cout << "DECISIONS " << decisions << '\n';
    std::cout << "FUITES " << fuites << '\n';
    std::cout << "DEFINIS " << definis << '\n';
    if (fuites == 0 && definis == 0) {
        std::cout << "ABSTRACTION TENUE" << '\n';
    } else {
        std::cout << "ABSTRACTION FUIT" << '\n';
    }
    return 0;
}
