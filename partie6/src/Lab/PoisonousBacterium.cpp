#include "PoisonousBacterium.hpp"
#include "Utility/Vec2d.hpp"
#include "Utility/Utility.hpp"
#include "Utility/Constants.hpp"
#include "Random/Random.hpp"
#include "Application.hpp"
#include "NutrimentA.hpp"
#include "NutrimentB.hpp"
#include "Poison.hpp"
#include "cmath"

using namespace std;

PoisonousBacterium::PoisonousBacterium(const Vec2d &position)
    :Bacterium(uniform(getConfig()["energy"]["min"].toDouble(),
                       getConfig()["energy"]["max"].toDouble()),
               position,
               Vec2d::fromRandomAngle(),
               uniform(getConfig()["radius"]["min"].toDouble(),
                       getConfig()["radius"]["max"].toDouble()),
               getConfig()["color"]),
     poison(0),
     rebond(sf::Time::Zero),
     t(uniform(0.0,PI))
{
    addProperty("speed",MutableNumber::positive(getConfig()["speed"]["initial"].toDouble(), getConfig()["speed"]["rate"].toDouble(), getConfig()["speed"]["sigma"].toDouble()));
    addProperty("puissance poison",MutableNumber::positive(getConfig()["puissance poison"]["initial"].toDouble(), getConfig()["puissance poison"]["rate"].toDouble(), getConfig()["puissance poison"]["sigma"].toDouble()));
    getAppEnv().add_at_graph(s::POISONOUS_BACTERIA, true); //ajout au graphe
}

PoisonousBacterium::PoisonousBacterium(const PoisonousBacterium& source)
    :Bacterium(source),
     poison(source.poison),
     rebond(source.rebond),
     t(source.t)
{
    getAppEnv().add_at_graph(s::POISONOUS_BACTERIA, true); //ajout au graphe du clone
}

PoisonousBacterium::~PoisonousBacterium()
{

    getAppEnv().add_at_graph(s::POISONOUS_BACTERIA, false); //retiré du graphe
}

j::Value& PoisonousBacterium::getConfig() const
{
    return getAppConfig()["poisonous bacterium"];
}

void PoisonousBacterium::ajout_poison(Quantity Q)
{
    poison += Q;
}

Vec2d PoisonousBacterium::getSpeedVector()
{
    return direction * getProperty("speed")->get();
}

Vec2d PoisonousBacterium::f(Vec2d position, Vec2d speed) const
{
    Vec2d force(0.0,0.0);
    return force;
}

void PoisonousBacterium::mutate()
{
    couleur.mutate();
    getProperty("speed")->mutate();
    //la puissance du poison relaché mute
    getProperty("puissance poison")->mutate();
}

Bacterium* PoisonousBacterium::clone()
{
    return new PoisonousBacterium(*this);
}

void PoisonousBacterium::drawOn(sf::RenderTarget& targetWindow) const
{
    Bacterium::drawOn(targetWindow);

    for(unsigned int i(0); i<8; ++i) {
        auto set_of_points = sf::VertexArray(sf::LinesStrip);

        set_of_points.append({{static_cast<float>(0),static_cast<float>(0)}, couleur.get()});
        for (int i(0); i <= 20; ++i) {
            set_of_points.append({{static_cast<float>(-i * rayon / 10.0), static_cast<float>(rayon * sin(t) * sin(2 * i / 10.0)) }, couleur.get()});
        }

        auto transform = sf::Transform(); // déclare une matrice de transformation
        // ici ensemble d'opérations comme des translations ou rotations faites  sur transform:
        transform.translate(position);
        transform.rotate((rotation+i*15)/DEG_TO_RAD);     //Attention degré !
        targetWindow.draw(set_of_points, transform); // dessin de l'ensemble des points
    }

    if(isDebugOn()==true)  {
        //ajout de la puissance du poison qu'elle lache
        Vec2d decalage{0,15};
        auto const text = buildText(std::to_string(poison), position+decalage, getAppFont(), 15, sf::Color::Red, 0.f);
        targetWindow.draw(text);
        //anneau si en rebond
        if(rebond >sf::Time::Zero) {
            auto anneaux = buildAnnulus(position,rayon+10, sf::Color::Red, 2);
            targetWindow.draw(anneaux);
        }
    }
}

void PoisonousBacterium::update(sf::Time dt)
{
    Bacterium::update(dt);
    generation += dt;

    //generation du poison
    if (generation >= sf::seconds(getConfig()["temps_generation_P"].toDouble())) {
        generation = sf::Time::Zero;
        Quantity P(poison);
        if (P!=0) {
            if (poison>=getAppConfig()["nutriments"]["poison"]["quantity"]["max"].toDouble()) {
                P =getAppConfig()["nutriments"]["poison"]["quantity"]["max"].toDouble();
            }
            getAppEnv().addNutriment(new Poison(P,position, getProperty("puissance poison")->get()));
            poison -= P;
            rebond = sf::seconds(getConfig()["speed"]["rebond"].toDouble());
        }
    }

}

void PoisonousBacterium::move(sf::Time dt)
{

    t +=3 * dt.asSeconds();

    Vec2d pos = position;
    Vec2d vitesse = getSpeedVector();
    bool en_rebond(false);

    if(rebond>sf::Time::Zero) {
        vitesse=2*getSpeedVector();
        rebond-=dt;
        en_rebond = true;
    }

    vitesse = stepDiffEq(position, vitesse, dt, *this).speed;

    if ((stepDiffEq(position, vitesse, dt, *this).position - position).lengthSquared()>0.001) {

        position = stepDiffEq(position, vitesse, dt, *this).position;// déplacement

        if(!en_rebond) {
            consumeEnergy((position-pos).length()*get_energie_deplacement());
        } else {
            consumeEnergy((position-pos).length()*get_energie_deplacement()*0.5);
        }
        //lors d'un rebond, la bacterie voit sa vitesse doublée mais ça perte d'énergie est la meme
    }

    Vec2d best(0.0, 0.0);
    for(size_t i(0); i<20; ++i) {
        Vec2d dir(Vec2d::fromRandomAngle());
        if(getAppEnv().getPositionScore(dir + position, true)> getAppEnv().getPositionScore(best + position, true) ) {
            best = dir;
        }
    }
    if(getAppEnv().getNutrimentColliding(*this) == nullptr) {
        direction=best;
        vitesse=getSpeedVector();
    }

}

Quantity PoisonousBacterium::eatableQuantity(NutrimentA& nutriment)
{
    return nutriment.eatenBy(*this);
}
Quantity PoisonousBacterium::eatableQuantity(NutrimentB& nutriment)
{
    return nutriment.eatenBy(*this);
}

Quantity PoisonousBacterium::eatableQuantity(Poison& nutriment)
{
    return nutriment.eatenBy(*this);
}
