// Les quatre chemins : rejouer la recherche du casque, puis le repli.
//
// Entree : K machines, une ligne "D V L R B G" chacune.
// Sortie : une ligne par etape franchie, puis le niveau atteint,
//          puis le nombre de bibliotheques chargees et liberees.

#include <iostream>
#include <string>

namespace {

struct Machine {
    std::string demande;   // D : OPENXR ou SIMULATEUR
    std::string variable;  // V : -, BON, SANS ou ABSENT
    std::string loader;    // L : BON, SANS ou ABSENT
    std::string registre;  // R : AUCUN, ILLISIBLE, SANSCLE, KO, NEGO_KO ou BON
    std::string liaison;   // B : OUI ou NON
    std::string carte;     // G : OUI ou NON
};

void Rejouer(int numero, const Machine& m) {
    int chargees = 0;
    int liberees = 0;
    bool casque = false;

    if (m.demande == "SIMULATEUR") {
        // Aucun chemin n'est essaye.
        std::cout << numero << " BACKEND SIMULATEUR DEMANDE" << '\n';
    } else {
        bool chargee = false;  // une bibliotheque est chargee en ce moment
        bool entree = false;   // et elle a son point d'entree
        std::string source;

        // 1. La variable d'environnement.
        if (m.variable != "-") {
            if (m.variable == "ABSENT") {
                std::cout << numero << " VARIABLE IGNOREE" << '\n';
            } else {
                std::cout << numero << " VARIABLE CHARGEE" << '\n';
                ++chargees;
                chargee = true;
                entree = (m.variable == "BON");
                source = "VARIABLE";
            }
        }

        // 2. Le loader standard, seulement si rien n'est charge.
        if (!chargee) {
            if (m.loader == "ABSENT") {
                std::cout << numero << " LOADER ABSENT" << '\n';
            } else {
                std::cout << numero << " LOADER CHARGE" << '\n';
                ++chargees;
                chargee = true;
                entree = (m.loader == "BON");
                source = "LOADER";
            }
        }

        // Une bibliotheque chargee sans point d'entree est liberee.
        if (chargee && !entree) {
            std::cout << numero << " SANS_ENTREE" << '\n';
            ++liberees;
            chargee = false;
        }

        // 3. La decouverte directe, s'il n'y a toujours pas de point d'entree.
        if (!entree) {
            std::cout << numero << " DIRECT " << m.registre << '\n';
            if (m.registre == "NEGO_KO" || m.registre == "BON") {
                ++chargees;
                chargee = true;
                entree = (m.registre == "BON");
                source = "DIRECT";
            }
        }

        // 4. Le bilan du backend.
        if (!entree) {
            std::cout << numero << " BACKEND INDISPONIBLE" << '\n';
            if (chargee) {
                ++liberees;
                chargee = false;
            }
        } else {
            std::cout << numero << " BACKEND OPENXR " << source << '\n';
            if (m.liaison == "NON") {
                // La session est detruite, sa bibliotheque liberee.
                std::cout << numero << " LIAISON ECHOUEE" << '\n';
                ++liberees;
                chargee = false;
            } else {
                casque = true;
            }
        }
    }

    // Le niveau : on finit toujours sur l'un des trois.
    if (casque) {
        std::cout << numero << " NIVEAU 1 CASQUE" << '\n';
    } else if (m.carte == "OUI") {
        std::cout << numero << " NIVEAU 2 SIMULATEUR CARTE" << '\n';
    } else {
        std::cout << numero << " NIVEAU 3 SIMULATEUR LOGICIEL" << '\n';
    }
    std::cout << numero << " CHARGEES " << chargees << " LIBEREES " << liberees << '\n';
}

}  // namespace

int main() {
    long long k = 0;
    std::cin >> k;

    if (k <= 0) {
        std::cout << "AUCUNE MACHINE" << '\n';
        return 0;
    }

    for (long long i = 1; i <= k; ++i) {
        Machine m;
        if (!(std::cin >> m.demande >> m.variable >> m.loader >> m.registre >> m.liaison >>
              m.carte)) {
            break;
        }
        Rejouer(static_cast<int>(i), m);
    }
    return 0;
}
