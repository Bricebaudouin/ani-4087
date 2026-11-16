#include <cmath>
#include <cstdlib>
#include <iostream>

namespace {

// Une pose de la tete dans le plan horizontal.
struct Pose {
    long long x = 0;       // millimetres
    long long z = 0;       // millimetres
    long long lacet = 0;   // milliemes de degre
};

// Lit une pose dans l'ordre : x, z, lacet.
std::istream &operator>>(std::istream &in, Pose &pose) {
    return in >> pose.x >> pose.z >> pose.lacet;
}

// Distance dans le plan (x, z) entre la pose du rendu et la pose relue,
// en millimetres, arrondie au plus proche.
long long EcartPosition(const Pose &rendu, const Pose &relue) {
    const double dx = static_cast<double>(relue.x - rendu.x);
    const double dz = static_cast<double>(relue.z - rendu.z);
    return std::llround(std::sqrt(dx * dx + dz * dz));
}

// Ecart de lacet sans son signe, en milliemes de degre.
long long EcartAngle(const Pose &rendu, const Pose &relue) {
    return std::llabs(relue.lacet - rendu.lacet);
}

} // namespace

int main() {
    int nbImages = 0;
    long long delai = 0;   // temps, en millisecondes, entre le rendu et la fin de trame
    std::cin >> nbImages >> delai;

    long long sommePosition = 0;
    long long sommeAngle = 0;

    for (int numero = 1; numero <= nbImages; ++numero) {
        Pose rendu;   // pose avec laquelle l'image a ete dessinee
        Pose relue;   // pose relue a la fin de trame
        std::cin >> rendu >> relue;

        const long long position = EcartPosition(rendu, relue);
        const long long angle = EcartAngle(rendu, relue);

        sommePosition += position;
        sommeAngle += angle;
        std::cout << numero << " POSITION " << position << " ANGLE " << angle << "\n";
    }

    // Sans image ou sans delai valide, aucune moyenne ni vitesse n'a de sens.
    if (nbImages <= 0 || delai <= 0) {
        std::cout << "AUCUNE\n";
        return 0;
    }

    // Les sommes sont positives ou nulles : la division entiere arrondit a l'entier inferieur.
    const long long nb = nbImages;
    std::cout << "MOYENNE POSITION " << sommePosition / nb << "\n";
    std::cout << "MOYENNE ANGLE " << sommeAngle / nb << "\n";
    // Des milliemes de degre par milliseconde sont des degres par seconde.
    std::cout << "VITESSE " << sommeAngle / (nb * delai) << "\n";
    return 0;
}
