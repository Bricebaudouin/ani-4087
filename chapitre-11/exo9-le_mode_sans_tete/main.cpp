// Le mode sans tete : comparer deux executions du meme test, sans fenetre.
//
// Entree : hz, la pose (LIBRE ou FIXE lacet tangage x y z), T, puis si T > 0
//          l'horloge lue a chaque image (ns) pour l'execution A, puis B.
// Sortie : PERIODE, POSE, une ligne par image (BRUT, RELATIF, SCRIPTE),
//          puis le verdict sur BRUT et sur RELATIF.

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

namespace {

// Cadence minimale : a 1 hz ou moins, le simulateur prend 72.
constexpr long long kCadenceParDefaut = 72;
constexpr long long kNanosecondesParSeconde = 1000000000LL;

// Premiere image (1, 2, ...) ou la valeur n'est pas nulle, ou 0 s'il n'y en a pas.
std::size_t PremiereDifference(const std::vector<long long>& valeurs) {
    for (std::size_t i = 0; i < valeurs.size(); ++i) {
        if (valeurs[i] != 0) {
            return i + 1;
        }
    }
    return 0;
}

void AfficherVerdict(const std::string& nom, const std::vector<long long>& valeurs) {
    const std::size_t premiere = PremiereDifference(valeurs);
    if (premiere == 0) {
        std::cout << nom << " IDENTIQUE" << '\n';
    } else {
        std::cout << nom << " DIFFERENT " << premiere << '\n';
    }
}

}  // namespace

int main() {
    long long hz = 0;
    std::cin >> hz;
    if (hz <= 1) {
        hz = kCadenceParDefaut;
    }
    const long long periode = kNanosecondesParSeconde / hz;  // arrondie a l'entier inferieur

    // Sans fenetre, la pose ne bouge pas : FIXE, ou la pose de depart.
    long long pose[5] = {0, 0, 0, 1700, 0};  // lacet tangage x y z
    std::string mot;
    std::cin >> mot;
    if (mot == "FIXE") {
        for (int i = 0; i < 5; ++i) {
            std::cin >> pose[i];
        }
    }

    long long t = 0;
    std::cin >> t;
    if (t < 0) {
        t = 0;
    }
    const std::size_t images = static_cast<std::size_t>(t);

    std::vector<long long> horloge_a(images, 0);
    std::vector<long long> horloge_b(images, 0);
    for (std::size_t i = 0; i < images; ++i) {
        std::cin >> horloge_a[i];
    }
    for (std::size_t i = 0; i < images; ++i) {
        std::cin >> horloge_b[i];
    }

    std::cout << "PERIODE " << periode << '\n';
    std::cout << "POSE " << pose[0] << ' ' << pose[1] << ' ' << pose[2] << ' ' << pose[3] << ' '
              << pose[4] << '\n';

    std::vector<long long> bruts(images, 0);
    std::vector<long long> relatifs(images, 0);
    for (std::size_t i = 0; i < images; ++i) {
        // L'instant predit vaut l'horloge lue plus la periode.
        const long long predit_a = horloge_a[i] + periode;
        const long long predit_b = horloge_b[i] + periode;
        const long long predit_a_premiere = horloge_a[0] + periode;
        const long long predit_b_premiere = horloge_b[0] + periode;

        bruts[i] = predit_b - predit_a;
        relatifs[i] = (predit_b - predit_b_premiere) - (predit_a - predit_a_premiere);
        const long long scripte = static_cast<long long>(i + 1) * periode;

        std::cout << (i + 1) << " BRUT " << bruts[i] << " RELATIF " << relatifs[i] << " SCRIPTE "
                  << scripte << '\n';
    }

    AfficherVerdict("BRUT", bruts);
    AfficherVerdict("RELATIF", relatifs);
    return 0;
}
