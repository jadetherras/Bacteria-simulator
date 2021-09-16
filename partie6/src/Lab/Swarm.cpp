#include "Swarm.hpp"
#include "Utility/Vec2d.hpp"
#include "Application.hpp"
#include "SwarmBacterium.hpp"

using namespace std;

Swarm::Swarm(const string &id)
    :identificateur(id),
     groupe(),
     leader(nullptr)
{}

Swarm::~Swarm()
{
    //on libère la memoire et on "tue" les bacteries pour les supprimer dans petridish
    for (auto* bac : groupe) {
        bac->setEnergie(0.0);
        bac = nullptr;
        delete bac;
    }
    leader = nullptr;
    delete leader;
}

void Swarm::add_SwarmBacterium(SwarmBacterium* bacterie)
{
    if(bacterie != nullptr) {
        if(groupe.empty()) {
            leader = bacterie;
        }
        groupe.push_back(bacterie);
    }
}

void Swarm::sup_SwarmBacterium(SwarmBacterium* bacterie)
{
    if (is_leader(*bacterie)) {
        leader = nullptr;
        update(sf::Time::Zero); //dans le cas ou le leader est supprimé, on a momentanement plus de leader.
        //On a fait le choix d'actualiser au moyen de la methode update (qui est override mais n'utilise pas le temps)
    }
    for(unsigned int i(0); i<groupe.size(); ++i) {
        if(groupe[i]->getPosition() == bacterie->getPosition()) {
            groupe[i] = nullptr;
        }
    }
    groupe.erase(std::remove(groupe.begin(), groupe.end(), nullptr), groupe.end());
}

Vec2d Swarm::position_leader() const
{
    return leader->getPosition();
}

bool Swarm::is_leader(const SwarmBacterium& bac) const
{
    return position_leader()== bac.getPosition();
}

MutableColor Swarm::couleur_depart() const
{
    return getAppConfig()["swarms"][identificateur]["color"];
}

void Swarm::update(sf::Time dt)
{
    if (!groupe.empty()) {
        int best(0);

        //choix du leader
        if(groupe.size()>=2) {
            for (unsigned int i(1); i<groupe.size(); ++i) {
                if (groupe[i] != nullptr) {
                    if(getAppEnv().getPositionScore(groupe[best]->getPosition())<getAppEnv().getPositionScore(groupe[i]->getPosition())) {
                        best = i;
                    }
                }
            }
        }

        leader = groupe[best];
    }
}


std::string Swarm::get_ident() const
{
    return identificateur;
}
