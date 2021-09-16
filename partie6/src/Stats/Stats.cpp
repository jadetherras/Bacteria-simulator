#include "Stats.hpp"
#include <iostream>
#include "Application.hpp"
#include "Config.hpp"

using namespace std;

Stats::Stats()
    :identifiant_actif(-1), //pas d'identifiant actif au début
     compteur(sf::Time::Zero)
{}

void Stats::drawOn(sf::RenderTarget& target) const
{
    unsigned int val(identifiant_actif);
    if (val >=0 and val < ensemble.size()) {
        ensemble[identifiant_actif]->graphe->drawOn(target);
    }
}

void Stats::update(sf::Time dt)
{
    compteur+=dt;
    if (compteur > sf::seconds(getAppConfig()["stats"]["refresh rate"].toDouble())) {
        for (unsigned int i(0); i<ensemble.size(); ++i) {
            std::unordered_map<string,double> new_data(getAppEnv().fetchData(ensemble[i]->nom)); //creation des nouvelles données

            ensemble[i]->graphe->updateData(compteur,new_data); //ajout des nouvelles données
        }
        compteur = sf::Time::Zero;
    }
}

std::string Stats::getCurrentTitle() const
{
    return ensemble[identifiant_actif]->nom;
}

void Stats::next()
{
    if (ensemble.size() != 0) {
        identifiant_actif = (identifiant_actif+1)%ensemble.size();
    }

}

void Stats::previous()
{
    if (ensemble.size() != 0) {
        identifiant_actif = identifiant_actif-1;
        if(identifiant_actif < 0) {
            identifiant_actif = ensemble.size()-1;
        } else {
            identifiant_actif =  identifiant_actif%ensemble.size();
        }
    }
}

void Stats::reset()
{
    for(unsigned int i(0); i < ensemble.size(); ++i) {
        ensemble[i]->graphe->reset();
    }

}

void Stats::addGraph(int identifiant, const std::string& titre, const std::vector<std::string>& titres, double min, double max, const Vec2d& size)
{
    unsigned int val(identifiant);

    if(val == ensemble.size()) {
        ensemble.push_back(new Graphique({unique_ptr<Graph>(new Graph(titres, size, min, max)),titre}));
        identifiant_actif = identifiant;
    } else if(val>=0 and val<ensemble.size()) {
        ensemble[identifiant]->graphe.reset(new Graph(titres, size, min, max)); //utilisation du reset des pointeurs uniques
        ensemble[identifiant]->nom = titre;
    }
}

void Stats::setActive(int identifiant)
{
    unsigned int val (identifiant);
    if (val < ensemble.size()) {
        identifiant_actif = identifiant;
    }
}
