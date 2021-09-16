#include <iostream>
#include<SFML/Graphics.hpp>
#include"PetriDish.hpp"
#include "Lab/Nutriment.hpp"
#include "Lab/Bacterium.hpp"
#include "Lab/CircularBody.hpp"
#include "Utility/Vec2d.hpp"
#include "Utility/Utility.hpp"
#include <vector>
#include "Config.hpp"
#include "Application.hpp"
using namespace std;

PetriDish::PetriDish (const Vec2d& position, double rayon)
    : CircularBody(position,rayon),
      temperature(getAppConfig()["petri dish"]["temperature"]["default"].toDouble()),
      puissance((getAppConfig()["petri dish"]["gradient"]["exponent"]["max"].toDouble()+getAppConfig()["petri dish"]["gradient"]["exponent"]["min"].toDouble())/2),
      prob_nutriment(getAppConfig()["generator"]["nutriment"]["prob"].toDouble()),
      separateur(false)

{
    nb[s::SIMPLE_BACTERIA] = 0;
    nb[s::TWITCHING_BACTERIA] = 0;
    nb[s::SWARM_BACTERIA] = 0;
    nb[s::POISONOUS_BACTERIA] = 0;
    nb[s::NUTRIMENT_SOURCES] = nutriments.size();
    nb[s::DISH_TEMPERATURE] = temperature;
}


PetriDish::~PetriDish ()
{
    reset();
}

bool PetriDish::addBacterium(Bacterium* bacteries) //Permet de savoir si l'ajout est un succès
{
    if(contains(*bacteries)) {
        this->bacteries.push_back(bacteries);
        return true;
    }
    delete bacteries;
    bacteries = nullptr;
    return false;
}

bool PetriDish::addBacterium_clone(Bacterium* bacteries) //Permet de savoir si l'ajout est un succès
{
    if(contains(*bacteries)) {
        this->bacteries_sup.push_back(bacteries);
        return true;
    }
    delete bacteries;
    bacteries = nullptr;
    return false;
}

bool PetriDish::addNutriment(Nutriment* nutriments)
{
    if(contains(*nutriments)) {
        this->nutriments.push_back(nutriments);
        return true;
    }
    delete nutriments;
    nutriments = nullptr;
    return false;
}

bool PetriDish::addSwarm(Swarm* groupe_b)
{

    this->groupe_bacteries.push_back(groupe_b);
    return true;
}

Swarm* PetriDish::getSwarmWithId(string id) const
{
    for (auto groupe : groupe_bacteries) {
        if (groupe->get_ident() == id) {
            return groupe;
        }
    }
    return nullptr;
}

void PetriDish::update (sf::Time dt)//Faire évoluer les bactéries en fonction du temps
{
    for(unsigned int i(0); i < nutriments.size(); ++i) {
        nutriments[i]->update(dt);
        if (nutriments[i]->morte()) {
            delete nutriments[i];
            nutriments[i] = nullptr;
        }
    }
    nutriments.erase(std::remove(nutriments.begin(), nutriments.end(), nullptr), nutriments.end());

    for(unsigned int i(0); i < bacteries.size(); ++i ) {
        if(bacteries[i] != nullptr) {
            bacteries[i]->update(dt);
            if (bacteries[i]->morte()) {
                delete bacteries[i];
                bacteries[i] = nullptr;
            }
        }
    }
    bacteries.erase(std::remove(bacteries.begin(), bacteries.end(), nullptr), bacteries.end());
    append(bacteries_sup,bacteries);
    bacteries_sup.clear();

    for(unsigned int i(0); i < groupe_bacteries.size(); ++i ) {
        groupe_bacteries[i]->update(dt);
    }

    for(unsigned int i(0); i < virus.size(); ++i ) {
        if(virus[i] != nullptr) {
            virus[i]->update(dt);
            if (virus[i]->morte()) {
                delete virus[i];
                virus[i] = nullptr;
            }
        }
    }
    virus.erase(std::remove(virus.begin(), virus.end(), nullptr), virus.end());
    append(virus_sup,virus);
    virus_sup.clear();

}

void PetriDish::drawOn (sf::RenderTarget& targetWindow) const//Dessine la boite avec les nutriments et les bactéries
{
    auto border = buildAnnulus(position,rayon, sf::Color::Black, 5);
    targetWindow.draw(border);

    if (separateur) {
        Vec2d R(0,rayon);
        auto barriere = buildLine(position-R, position+R, sf::Color::Black, 3);
        targetWindow.draw(barriere);
    }

    for (auto nut : nutriments) {
        nut->drawOn(targetWindow);
    }
    for (auto bact : bacteries) {
        bact->drawOn(targetWindow);
    }

    for (auto o : obstacles) {
        o->drawOn(targetWindow);
    }

    for (auto v : virus) {
        v->drawOn(targetWindow);
    }
}

void PetriDish::reset() //supprime les bactéries et nutriments
{
    for (auto* nutri : nutriments) {
        delete nutri;
        nutri = nullptr;
    }
    nutriments.clear();
    for (auto* bac : bacteries) {
        delete bac;
        bac = nullptr;
    }
    bacteries.clear();
    for (auto* G : groupe_bacteries) {
        delete G;
        G = nullptr;
    }
    groupe_bacteries.clear();

    for (auto* o : obstacles) {
        delete o;
        o = nullptr;
    }
    obstacles.clear();

    for (auto* v : virus) {
        delete v;
        v = nullptr;
    }
    virus.clear();

    separateur = false;
    resetTemp();
    resetGrad();
    resetProb_nutriment();
}

void PetriDish::resetTemp()
{
    temperature = getAppConfig()["petri dish"]["temperature"]["default"].toDouble();
}

void PetriDish::resetGrad()
{
    puissance = (getAppConfig()["petri dish"]["gradient"]["exponent"]["max"].toDouble()+getAppConfig()["petri dish"]["gradient"]["exponent"]["min"].toDouble())/2;
}

double PetriDish::getTemperature() const
{
    return temperature;
}

void PetriDish::decreaseTemperature()
{
    if (temperature- getAppConfig()["petri dish"]["temperature"]["min"].toDouble() > getAppConfig()["petri dish"]["temperature"]["min"].toDouble()) {
        temperature -= getAppConfig()["petri dish"]["temperature"]["delta"].toDouble();
    }
}

void PetriDish::increaseTemperature()
{
    if (temperature+getAppConfig()["petri dish"]["temperature"]["min"].toDouble() < getAppConfig()["petri dish"]["temperature"]["max"].toDouble()) {
        temperature += getAppConfig()["petri dish"]["temperature"]["delta"].toDouble();
    }
}

Nutriment* PetriDish::getNutrimentColliding(CircularBody const& body) const
{
    for (auto* N : nutriments) {
        if(*N&body) {
            return N;
        }
    }
    return nullptr;
}

double PetriDish::getPositionScore(const Vec2d& p, bool poisonous) const
{
    double score(0);
    if (nutriments.size()==0) {
        return score;
    } else {
        for (unsigned int i(0); i<nutriments.size(); ++i) {
            if(!(poisonous and nutriments[i]->est_poison())) {
                score += nutriments[i]->getRadius()/pow(distance(p,nutriments[i]->getPosition()),puissance);
            }
        }
        return score;
    }
}

double PetriDish:: getGradientExponent() const
{
    return puissance;
}

void PetriDish:: decreaseGradientExponent()
{
    if (puissance-getAppConfig()["petri dish"]["gradient"]["exponent"]["delta"].toDouble() > getAppConfig()["petri dish"]["gradient"]["exponent"]["min"].toDouble()) {
        puissance -= getAppConfig()["petri dish"]["gradient"]["exponent"]["delta"].toDouble();
    }
}

void PetriDish::increaseGradientExponent()
{
    if (puissance+getAppConfig()["petri dish"]["gradient"]["exponent"]["delta"].toDouble() < getAppConfig()["petri dish"]["gradient"]["exponent"]["max"].toDouble()) {
        puissance += getAppConfig()["petri dish"]["gradient"]["exponent"]["delta"].toDouble();
    }
}

std::unordered_map<string, double> PetriDish:: fetchData(const string & titre)
{
    std::unordered_map<string, double> new_data;

    if(titre==s::NUTRIMENT_QUANTITY) {

        Quantity quant_tot(0);
        for (auto N : nutriments)  {
            quant_tot += N->getRadius();
        }

        new_data[titre]=quant_tot;

    } else if (titre==s::GENERAL) {
        /*new_data[s::SIMPLE_BACTERIA] = 0;
        new_data[s::TWITCHING_BACTERIA] = 0;
        new_data[s::SWARM_BACTERIA] = 0;
        new_data[s::POISONOUS_BACTERIA] = 0;
        for (auto B : bacteries) {
            if(B!=nullptr) {
                B->getType(new_data); //polymorphisme. Quest : mieux vaut-il utiliser la construction/destruction et avoir un attribut
                //ou cette méthode ?
            }
        }*/
        nb[s::NUTRIMENT_SOURCES] = nutriments.size();
        nb[s::DISH_TEMPERATURE] = temperature;
        return nb;

    } else if (titre==s::SIMPLE_BACTERIA) {

        new_data[s::BETTER] = 0;
        new_data[s::WORSE] = 0;
        double nb(0);
        for (auto B : bacteries) {
            if(B!=nullptr and B->getProperty("tumble better")!=nullptr) {
                new_data[s::BETTER] += B->getProperty("tumble better")->get();
                new_data[s::WORSE] += B->getProperty("tumble worse")->get();
                nb +=1;
            }
        }
        if (nb!=0) {
            new_data[s::BETTER] = new_data[s::BETTER]/nb;
            new_data[s::WORSE] = new_data[s::WORSE]/nb;
        }


    } else if (titre==s::TWITCHING_BACTERIA) {

        new_data[s::TENTACLE_LENGTH] = 0;
        new_data[s::TENTACLE_SPEED] = 0;
        double nb(0);
        for (auto B : bacteries) {
            if(B!=nullptr and B->getProperty("tentacle speed")!=nullptr) {
                new_data[s::TENTACLE_LENGTH] += B->getProperty("tentacle length")->get();
                new_data[s::TENTACLE_SPEED] += B->getProperty("tentacle speed")->get();
                nb +=1;
            }
        }
        if (nb!=0) {
            new_data[s::TENTACLE_LENGTH] = new_data[s::TENTACLE_LENGTH]/nb;
            new_data[s::TENTACLE_SPEED] = new_data[s::TENTACLE_SPEED]/nb;
        }

    } else if(titre==s::INFECTION) {

        new_data[s::NOMBRE_DE_VIRUS] = virus.size();
        new_data[s::NOMBRE_INFECTION] = 0;
        new_data[s::GENERATION] = 0;
        for(auto V : virus) {
            new_data[s::GENERATION] += V->get_generation();
        }
        if (virus.size()!=0) {
            new_data[s::GENERATION]=new_data[s::GENERATION]/virus.size();
        }
        for(auto B : bacteries) {
            if (B->est_infecte()) {
                ++new_data[s::NOMBRE_INFECTION];
            }
        }
        if (bacteries.size()!=0) {
            new_data[s::NOMBRE_INFECTION]=new_data[s::NOMBRE_INFECTION]*100/bacteries.size();
        }


    } else if (titre==s::BACTERIA) {

        new_data[s::SPEED] = 0;
        double nb(0);
        for (auto B : bacteries) {
            if(B!=nullptr and B->getProperty("speed")!=nullptr) {
                new_data[s::SPEED] += B->getProperty("speed")->get();
                nb +=1;
            }
        }
        if (nb!=0) {
            new_data[s::SPEED] = new_data[s::SPEED]/nb;
        }

    } else {
        throw std::invalid_argument("titre invalide");
    }

    return new_data;
}

bool PetriDish::addObstacle(Obstacle* obs)
{
    if(contains(*obs)) {
        this->obstacles.push_back(obs);
        return true;
    }
    delete obs;
    obs = nullptr;
    return false;
}

bool PetriDish::doesCollideWithObstacle(CircularBody const& body)  const
{
    for (auto O : obstacles) {
        if(O->en_collision(body)) {
            return true;
        }
    }
    return false;
}

Bacterium* PetriDish::getBacteriesColliding(CircularBody const& body) const
{
    for (auto* B : bacteries) {
        if(*B&body) {
            return B;
        }
    }
    return nullptr;
}

bool PetriDish::addVirus(Virus* V)
{
    if(contains(*V)) {
        this->virus.push_back(V);
        return true;
    }
    delete V;
    V = nullptr;
    return false;
}

bool PetriDish::addVirus_clone(Virus* V) //Permet de savoir si l'ajout est un succès
{
    if(contains(*V)) {
        this->virus_sup.push_back(V);
        return true;
    }
    delete V;
    V = nullptr;
    return false;
}

double PetriDish::getPositionBact(const Vec2d& p) const
{
    double score(0);
    if (bacteries.size()==0) {
        return score;
    } else {
        for (unsigned int i(0); i<bacteries.size(); ++i) {
            score += bacteries[i]->getRadius()/pow(distance(p,bacteries[i]->getPosition()),puissance);
        }
        return score;
    }
}

void PetriDish::remove_separateur()
{
    separateur = !separateur;
    if (separateur == true) {
        for (auto N : nutriments) {
            if (doesCollideWithSeparator(*N)) {
                N->setQuantity(0);
            }
        }
        for (auto V : virus) {
            if (doesCollideWithSeparator(*V)) {
                V->setEnergie(0);
            }
        }
        for (auto B : bacteries) {
            if (doesCollideWithSeparator(*B)) {
                B->setEnergie(0);
            }
        }
    }
}

bool PetriDish::doesCollideWithSeparator(const CircularBody& body) const
{
    bool collision(false);
    if (body.getPosition()[0]<=position[0]) {
        collision = body.getPosition()[0]+body.getRadius()>=position[0];
    } else {
        collision = body.getPosition()[0]-body.getRadius()<=position[0];
    }
    return separateur and collision;
}

void PetriDish::resetProb_nutriment()
{
    prob_nutriment = getAppConfig()["generator"]["nutriment"]["prob"].toDouble();
}

double PetriDish::getProb_nutriment() const
{
    return prob_nutriment;
}

void PetriDish::decreaseProb_nutriment()
{
    if (prob_nutriment-getAppConfig()["generator"]["nutriment"]["delta"].toDouble() >= 0.0) {
        prob_nutriment -= getAppConfig()["generator"]["nutriment"]["delta"].toDouble();
    }

    if (prob_nutriment < 0.05) {
        prob_nutriment = 0.0;
    }
}

void PetriDish::increaseProb_nutriment()
{
    if (prob_nutriment+getAppConfig()["generator"]["nutriment"]["delta"].toDouble() <= 1.0) {
        prob_nutriment += getAppConfig()["generator"]["nutriment"]["delta"].toDouble();
    }
}

void PetriDish::add_at_graph(std::string nom, bool ajout)
{
    if (ajout) {
        ++nb[nom];
    } else {
        --nb[nom];
    }
}
