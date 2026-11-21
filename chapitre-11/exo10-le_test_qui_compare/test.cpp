#include <iostream>

namespace {

// Cadence minimale : a 1 hz ou moins, on prend 72.
constexpr long long kCadenceParDefaut = 72;
constexpr long long kNanosecondesParSeconde = 1000000000LL;

// La droite de la tete, vue de dessus en (x, z), pour un lacet de 0 a 3
// (quarts de tour) : (1, 0), (0, -1), (-1, 0) puis (0, 1).
constexpr long long kDroiteX[4] = {1, 0, -1, 0};
constexpr long long kDroiteZ[4] = {0, -1, 0, 1};

// Reste de la division par 4, toujours entre 0 et 3, meme pour un lacet negatif.
int LacetModulo4(long long lacet) {
    long long reste = lacet % 4;
    if (reste < 0) {
        reste += 4;
    }
    return static_cast<int>(reste);
}

}  // namespace

int main() {
    long long hz = 0;
    long long ipd = 0;
    long long images = 0;
    std::cin >> hz >> ipd >> images;

    long long x = 0;
    long long y = 0;
    long long z = 0;
    long long k0 = 0;
    long long dk = 0;
    long long vx = 0;
    std::cin >> x >> y >> z >> k0 >> dk >> vx;

    if (hz <= 1) {
        hz = kCadenceParDefaut;
    }
    const long long periode = kNanosecondesParSeconde / hz;  // arrondie a l'entier inferieur
    const long long demi_ecart = ipd / 2;                    // ipd est pair

    std::cout << "PERIODE " << periode << '\n';

    for (long long t = 1; t <= images; ++t) {
        // La tete a l'image t.
        const int lacet = LacetModulo4(k0 + (t - 1) * dk);
        const long long tete_x = x + (t - 1) * vx;
        const long long tete_y = y;
        const long long tete_z = z;

        // Les yeux sont de part et d'autre de la tete, le long de sa droite.
        const long long droite_x = kDroiteX[lacet];
        const long long droite_z = kDroiteZ[lacet];

        const long long gauche_x = tete_x - demi_ecart * droite_x;
        const long long gauche_z = tete_z - demi_ecart * droite_z;
        const long long droit_x = tete_x + demi_ecart * droite_x;
        const long long droit_z = tete_z + demi_ecart * droite_z;

        // L'instant est scripte : t x p, jamais l'heure.
        const long long instant = t * periode;

        std::cout << t << " G " << gauche_x << ' ' << tete_y << ' ' << gauche_z << " D "
                  << droit_x << ' ' << tete_y << ' ' << droit_z << " I " << instant << '\n';
    }
    return 0;
}
