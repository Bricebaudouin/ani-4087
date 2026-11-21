// Une question, un itineraire : du registre a la bibliotheque du runtime.
//
// Entree : K machines. Pour chacune : la valeur du registre (chemin du
//          manifeste, ou "-"), un entier L (-1 si illisible), L lignes.
// Sortie : le chemin suivi, pas a pas, ou la raison de l'echec.

#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>

namespace {

long long LireEntier(const std::string& ligne) {
    long long valeur = 0;
    std::istringstream flux(ligne);
    flux >> valeur;
    return valeur;
}

void RetirerRetourChariot(std::string& texte) {
    if (!texte.empty() && texte.back() == '\r') {
        texte.pop_back();
    }
}

// Scan de texte, pas un vrai lecteur JSON. Rend une chaine vide si la valeur
// a ete extraite, sinon la raison de l'echec.
std::string ExtraireValeur(const std::string& texte, std::string& valeur) {
    const std::string cle = "\"library_path\"";

    const std::size_t posCle = texte.find(cle);
    if (posCle == std::string::npos) {
        return "CLE";
    }
    const std::size_t posDeuxPoints = texte.find(':', posCle + cle.size());
    if (posDeuxPoints == std::string::npos) {
        return "DEUX_POINTS";
    }
    const std::size_t ouvrant = texte.find('"', posDeuxPoints);
    if (ouvrant == std::string::npos) {
        return "GUILLEMET";
    }
    const std::size_t fermant = texte.find('"', ouvrant + 1);
    if (fermant == std::string::npos) {
        return "GUILLEMET";
    }

    // Le premier guillemet suivant ferme la valeur, meme precede d'un antislash.
    const std::string brut = texte.substr(ouvrant + 1, fermant - ouvrant - 1);

    // On saute un antislash quand le signe suivant est aussi un antislash.
    valeur.clear();
    for (std::size_t i = 0; i < brut.size(); ++i) {
        if (brut[i] == '\\' && i + 1 < brut.size() && brut[i + 1] == '\\') {
            continue;
        }
        valeur += brut[i];
    }
    if (valeur.empty()) {
        return "VIDE";
    }
    return "";
}

bool EstAbsolu(const std::string& valeur) {
    if (valeur.size() >= 2 && valeur[1] == ':') {
        return true;
    }
    return valeur[0] == '\\' || valeur[0] == '/';
}

}  // namespace

int main() {
    std::string ligne;
    long long k = 0;
    if (std::getline(std::cin, ligne)) {
        k = LireEntier(ligne);
    }

    for (long long i = 1; i <= k; ++i) {
        std::string registre;
        if (!std::getline(std::cin, registre)) {
            break;
        }
        RetirerRetourChariot(registre);

        long long l = 0;
        if (std::getline(std::cin, ligne)) {
            l = LireEntier(ligne);
        }

        // Le texte du manifeste : chaque ligne suivie d'un saut de ligne.
        std::string texte;
        for (long long j = 0; j < l; ++j) {
            if (!std::getline(std::cin, ligne)) {
                break;
            }
            texte += ligne;
            texte += '\n';
        }

        if (registre == "-") {
            std::cout << i << " REGISTRE AUCUN" << '\n';
            continue;
        }

        std::cout << i << " MANIFESTE " << registre << '\n';
        if (l == -1) {
            std::cout << i << " ECHEC ILLISIBLE" << '\n';
            continue;
        }

        std::string valeur;
        const std::string raison = ExtraireValeur(texte, valeur);
        if (!raison.empty()) {
            std::cout << i << " ECHEC " << raison << '\n';
            continue;
        }
        std::cout << i << " VALEUR " << valeur << '\n';

        std::string chemin;
        if (EstAbsolu(valeur)) {
            std::cout << i << " ABSOLU" << '\n';
            chemin = valeur;
        } else {
            std::cout << i << " RELATIF" << '\n';
            // Dossier du manifeste : jusqu'au dernier separateur, compris.
            const std::size_t dernier = registre.find_last_of("\\/");
            chemin = registre.substr(0, dernier + 1) + valeur;
        }
        std::cout << i << " CHARGE " << chemin << '\n';
    }
    return 0;
}
