// L'environnement a un seul endroit : appliquer des variables d'environnement
// par-dessus les reglages poses par le code, avec les regles exactes du module.
//
// Entree : trois lignes de reglages du code (backend/hz/interrupteurs/latence,
//          chemin du loader, pose), puis M lignes NOM=valeur.
// Sortie : dix lignes : les reglages finaux, la pose, APPLIQUEES et IGNOREES.

#include <cstddef>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

namespace {

struct Reglages {
    std::string backend;  // SIMULATEUR ou OPENXR
    long long hz = 0;
    bool demi = false;
    bool profondeur = false;
    bool liste = false;
    std::string loader;  // chemin, ou "-"
    long long latence = 0;
    bool pose_fixe = false;
    long long pose[5] = {0, 0, 0, 0, 0};  // lacet tangage x y z
};

void RetirerRetourChariot(std::string& texte) {
    if (!texte.empty() && texte.back() == '\r') {
        texte.pop_back();
    }
}

// Saute les espaces du debut, lit un signe + ou - facultatif, puis les
// chiffres, et s'arrete au premier autre signe. Sans chiffre, rend 0.
long long LireNombre(const std::string& texte, std::size_t& pos, bool& chiffres) {
    while (pos < texte.size() && texte[pos] == ' ') {
        ++pos;
    }
    bool negatif = false;
    if (pos < texte.size() && (texte[pos] == '+' || texte[pos] == '-')) {
        negatif = (texte[pos] == '-');
        ++pos;
    }

    const long long maximum = std::numeric_limits<long long>::max();
    long long valeur = 0;
    chiffres = false;
    while (pos < texte.size() && texte[pos] >= '0' && texte[pos] <= '9') {
        const long long chiffre = texte[pos] - '0';
        chiffres = true;
        if (valeur <= (maximum - chiffre) / 10) {
            valeur = valeur * 10 + chiffre;
        } else {
            valeur = maximum;  // on ne deborde pas
        }
        ++pos;
    }
    return negatif ? -valeur : valeur;
}

// Interrupteur : NON si le premier signe est 0 ou si la valeur est exactement
// "false". Tout le reste, y compris "False", "no" et "off", donne OUI.
// La valeur n'est pas vide (les valeurs vides sont ecartees avant).
bool LireInterrupteur(const std::string& valeur) {
    return !(valeur[0] == '0' || valeur == "false");
}

// Pose : au plus cinq nombres separes par des virgules et des espaces.
// Rend vrai si exactement cinq nombres ont ete lus (les cinq premiers comptent).
bool LirePose(const std::string& valeur, long long (&pose)[5]) {
    std::size_t pos = 0;
    std::size_t lus = 0;
    while (lus < 5 && pos < valeur.size()) {
        bool chiffres = false;
        const long long nombre = LireNombre(valeur, pos, chiffres);
        if (!chiffres) {
            break;
        }
        pose[lus] = nombre;
        ++lus;
        while (pos < valeur.size() && (valeur[pos] == ',' || valeur[pos] == ' ')) {
            ++pos;
        }
    }
    return lus == 5;
}

// Applique une variable. Rend vrai si elle a pose une valeur.
bool Appliquer(Reglages& r, const std::string& nom, const std::string& valeur) {
    if (valeur.empty()) {
        return false;  // une valeur vide est ignoree
    }
    if (nom == "NK_XR_BACKEND") {
        if (valeur == "openxr") {
            r.backend = "OPENXR";  // jamais SIMULATEUR
            return true;
        }
        return false;
    }
    if (nom == "NK_XR_HZ") {
        std::size_t pos = 0;
        bool chiffres = false;
        r.hz = LireNombre(valeur, pos, chiffres);
        return true;
    }
    if (nom == "NK_XR_SIM_LATENCY_MS") {
        std::size_t pos = 0;
        bool chiffres = false;
        r.latence = LireNombre(valeur, pos, chiffres);
        return true;
    }
    if (nom == "NK_XR_HALF_RATE") {
        r.demi = LireInterrupteur(valeur);
        return true;
    }
    if (nom == "NK_XR_DEPTH_LAYER") {
        r.profondeur = LireInterrupteur(valeur);
        return true;
    }
    if (nom == "NK_XR_LIST_EXT") {
        r.liste = LireInterrupteur(valeur);
        return true;
    }
    if (nom == "NK_XR_OPENXR_LOADER") {
        r.loader = valeur;
        return true;
    }
    if (nom == "NK_XR_SIM_POSE") {
        long long pose[5] = {0, 0, 0, 0, 0};
        if (!LirePose(valeur, pose)) {
            return false;
        }
        r.pose_fixe = true;
        for (int i = 0; i < 5; ++i) {
            r.pose[i] = pose[i];
        }
        return true;
    }
    return false;  // tout autre nom est ignore
}

const char* OuiNon(bool valeur) {
    return valeur ? "OUI" : "NON";
}

}  // namespace

int main() {
    Reglages r;
    std::string ligne;

    // Ligne 1 : BACKEND HZ DEMI PROFONDEUR LISTE LATENCE.
    std::getline(std::cin, ligne);
    {
        std::istringstream flux(ligne);
        std::string demi;
        std::string profondeur;
        std::string liste;
        flux >> r.backend >> r.hz >> demi >> profondeur >> liste >> r.latence;
        r.demi = (demi == "OUI");
        r.profondeur = (profondeur == "OUI");
        r.liste = (liste == "OUI");
    }

    // Ligne 2 : le chemin du loader, qui peut contenir des espaces.
    std::getline(std::cin, r.loader);
    RetirerRetourChariot(r.loader);

    // Ligne 3 : LIBRE, ou FIXE lacet tangage x y z.
    std::getline(std::cin, ligne);
    {
        std::istringstream flux(ligne);
        std::string mot;
        flux >> mot;
        if (mot == "FIXE") {
            r.pose_fixe = true;
            for (int i = 0; i < 5; ++i) {
                flux >> r.pose[i];
            }
        }
    }

    // Les variables : M lignes NOM=valeur.
    long long m = 0;
    if (std::getline(std::cin, ligne)) {
        std::istringstream flux(ligne);
        flux >> m;
    }

    long long appliquees = 0;
    long long ignorees = 0;
    for (long long i = 0; i < m; ++i) {
        if (!std::getline(std::cin, ligne)) {
            break;
        }
        RetirerRetourChariot(ligne);

        // Le nom va jusqu'au premier '=', la valeur est tout ce qui suit.
        const std::size_t egal = ligne.find('=');
        const std::string nom = ligne.substr(0, egal);
        const std::string valeur =
            (egal == std::string::npos) ? std::string() : ligne.substr(egal + 1);

        if (Appliquer(r, nom, valeur)) {
            ++appliquees;
        } else {
            ++ignorees;
        }
    }

    std::cout << "BACKEND " << r.backend << '\n';
    std::cout << "HZ " << r.hz << '\n';
    std::cout << "DEMI " << OuiNon(r.demi) << '\n';
    std::cout << "PROFONDEUR " << OuiNon(r.profondeur) << '\n';
    std::cout << "LISTE " << OuiNon(r.liste) << '\n';
    std::cout << "LOADER " << r.loader << '\n';
    std::cout << "LATENCE " << r.latence << '\n';
    if (r.pose_fixe) {
        std::cout << "POSE FIXE";
        for (int i = 0; i < 5; ++i) {
            std::cout << ' ' << r.pose[i];
        }
        std::cout << '\n';
    } else {
        std::cout << "POSE LIBRE" << '\n';
    }
    std::cout << "APPLIQUEES " << appliquees << '\n';
    std::cout << "IGNOREES " << ignorees << '\n';
    return 0;
}
