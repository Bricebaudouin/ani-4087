#include <iostream>
#include <string>

namespace {

// Fausse session : elle reproduit le drapeau "trame ouverte" du moteur
// et compte les appels acceptes et refuses.
class Session {
public:
    // Attendre est refuse tant qu'une trame est ouverte.
    bool Wait() {
        if (trameOuverte_) {
            return Refuser();
        }
        return true;
    }

    // Ouvrir est refuse si une trame est deja ouverte.
    bool Begin() {
        if (trameOuverte_) {
            return Refuser();
        }
        trameOuverte_ = true;
        ++nbDebuts_;
        return true;
    }

    // Fermer est refuse si aucune trame n'est ouverte.
    bool End() {
        if (!trameOuverte_) {
            return Refuser();
        }
        trameOuverte_ = false;
        ++nbFins_;
        return true;
    }

    int Debuts() const { return nbDebuts_; }
    int Fins() const { return nbFins_; }
    int Refus() const { return nbRefus_; }
    bool Ouverte() const { return trameOuverte_; }

private:
    bool Refuser() {
        ++nbRefus_;
        return false;
    }

    bool trameOuverte_ = false;
    int nbDebuts_ = 0;
    int nbFins_ = 0;
    int nbRefus_ = 0;
};

// Ferme la trame automatiquement, par son destructeur, quelle que soit
// la sortie du bloc (fin de boucle, continue ou return).
// Non copiable : une copie fermerait la trame deux fois.
class GardeTrame {
public:
    explicit GardeTrame(Session &session) : session_(session) {}
    ~GardeTrame() { session_.End(); }

    GardeTrame(const GardeTrame &) = delete;
    GardeTrame &operator=(const GardeTrame &) = delete;

private:
    Session &session_;
};

// Debut commun aux deux boucles : on attend, puis on ouvre la trame.
// Si Wait est refuse, la boucle n'a pas d'etat : N et C deviennent V.
char DebuterImage(Session &session, char signe) {
    if (!session.Wait() && (signe == 'N' || signe == 'C')) {
        signe = 'V';
    }
    session.Begin();
    return signe;
}

// Sans garde : End est ecrit en bas du corps de boucle,
// donc C (continue) et R (return) le sautent.
int BoucleSansGarde(Session &session, const std::string &signes) {
    int images = 0;
    for (const char signeLu : signes) {
        const char signe = DebuterImage(session, signeLu);
        if (signe == 'C') {
            continue;            // les vues echouent : End est saute
        }
        if (signe == 'R') {
            return images;       // on s'arrete : End est saute
        }
        if (signe == 'N') {
            ++images;
        }
        session.End();
    }
    return images;
}

// Avec garde : le destructeur appelle End sur tous les chemins.
int BoucleAvecGarde(Session &session, const std::string &signes) {
    int images = 0;
    for (const char signeLu : signes) {
        const char signe = DebuterImage(session, signeLu);
        GardeTrame garde(session);   // creee juste apres Begin
        if (signe == 'C') {
            continue;
        }
        if (signe == 'R') {
            return images;
        }
        if (signe == 'N') {
            ++images;
        }
    }
    return images;
}

void Afficher(const char *nom, const Session &session, int images) {
    std::cout << nom
              << " DEBUTS " << session.Debuts()
              << " FINS " << session.Fins()
              << " REFUS " << session.Refus()
              << " IMAGES " << images
              << " OUVERTE " << (session.Ouverte() ? "OUI" : "NON") << "\n";
}

} // namespace

int main() {
    int nbImages = 0;
    std::cin >> nbImages;

    std::string signes;
    if (nbImages > 0) {
        std::cin >> signes;
    }

    Session sansGarde;
    Afficher("SANS", sansGarde, BoucleSansGarde(sansGarde, signes));

    Session avecGarde;
    Afficher("AVEC", avecGarde, BoucleAvecGarde(avecGarde, signes));

    return 0;
}
