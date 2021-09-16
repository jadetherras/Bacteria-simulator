#include "TwitchingBacterium.hpp"
#include "Utility/Vec2d.hpp"
#include "Utility/Utility.hpp"
#include "Utility/Constants.hpp"
#include "Random/Random.hpp"
#include "Application.hpp"
#include "cmath"
#include "NutrimentA.hpp"
#include "NutrimentB.hpp"

using namespace std;

TwitchingBacterium:: TwitchingBacterium(const Vec2d &position)
    : Bacterium(uniform(getConfig()["energy"]["min"].toDouble(), getConfig()["energy"]["max"].toDouble()),
                position,
                Vec2d::fromRandomAngle(),
                uniform(getConfig()["radius"]["min"].toDouble(), getConfig()["radius"]["max"].toDouble()),
                getConfig()["color"]),
      grapin(position, getRadius()/4.0),
      phase(IDLE)
{
    addProperty("tentacle speed",MutableNumber::positive(getConfig()["tentacle"]["speed"]["initial"].toDouble(), getConfig()["tentacle"]["speed"]["rate"].toDouble(), getConfig()["tentacle"]["speed"]["sigma"].toDouble()));
    addProperty("tentacle length", MutableNumber::positive(getConfig()["tentacle"]["length"]["initial"].toDouble(), getConfig()["tentacle"]["length"]["rate"].toDouble(), getConfig()["tentacle"]["length"]["sigma"].toDouble()));
    getAppEnv().add_at_graph(s::TWITCHING_BACTERIA, true);
}

TwitchingBacterium::TwitchingBacterium(const TwitchingBacterium& source)
    :Bacterium(source),
     grapin(source.position, getRadius()/4.0),
     phase(IDLE)
{
    getAppEnv().add_at_graph(s::TWITCHING_BACTERIA, true);
}

TwitchingBacterium::~TwitchingBacterium()
{
    getAppEnv().add_at_graph(s::TWITCHING_BACTERIA, false);
}

j::Value& TwitchingBacterium::getConfig() const
{
    return getAppConfig()["twitching bacterium"];
}

Quantity TwitchingBacterium:: get_cons_move() const
{
    return getConfig()["energy"]["consumption factor"]["move"].toDouble();
}

Quantity TwitchingBacterium:: get_cons_tent() const
{
    return getConfig()["energy"]["consumption factor"]["tentacle"].toDouble();
}

void TwitchingBacterium::move(sf::Time dt)
{
    Vec2d best(0.0, 0.0);
    Vec2d ancienne_position(position);

    //algorithme pour le mouvement
    switch (phase) {
    case (IDLE) :
        phase = WAIT_TO_DEPLOY;
        break;

    case (WAIT_TO_DEPLOY) :
        for(size_t i(0); i<20; ++i) {
            Vec2d dir(Vec2d::fromRandomAngle());
            if(getAppEnv().getPositionScore(dir + position)> getAppEnv().getPositionScore(best + position) ) {
                best = dir;
            }
        }
        if((best[0]!=0.0) and (best[1]!=0.0)) {    //Prévoit le cas ou best est le vecteur nul
            direction=best;
        }
        phase = DEPLOY;
        break;

    case DEPLOY :
        if(getAppEnv().global_colliding(grapin)) {
            phase = RETRACT;
            break;
        }
        moveGrip(direction * getProperty("tentacle speed")->get() * dt.asSeconds());
        consumeEnergy(get_cons_tent() * getProperty("tentacle speed")->get() * dt.asSeconds());
        if (getAppEnv().getNutrimentColliding(grapin) != nullptr) {
            phase = ATTRACT;
            break;
        } else if (getProperty("tentacle length")->get() <= (grapin.getPosition()-position).length()) {
            phase = RETRACT;
            break;
        }
        break;

    case ATTRACT :
        if (getAppEnv().getNutrimentColliding(*(this)) != nullptr) {
            phase = EAT;
            break;
        }
        if (getAppEnv().getNutrimentColliding(grapin) != nullptr and !getAppEnv().global_colliding(*this)) {
            CircularBody::move((grapin.getPosition()-position).normalised() * getProperty("tentacle speed")->get() * getConfig()["speed factor"].toDouble() * dt.asSeconds());
            consumeEnergy((position - ancienne_position).length()*get_cons_move());
        } else {
            phase = RETRACT;
            break;
        }
        break;

    case RETRACT :
        if ((grapin.getPosition()-position).length()<=rayon) {
            phase = IDLE;
            break;
        }
        moveGrip((position - grapin.getPosition()).normalised() * getProperty("tentacle speed")->get() * dt.asSeconds());
        consumeEnergy(get_cons_tent() * getProperty("tentacle speed")->get() * dt.asSeconds());
        break;

    case EAT :
        if (getAppEnv().getNutrimentColliding(*(this)) == nullptr) {
            phase = IDLE;
        }
        break;
    }
}

void TwitchingBacterium::drawOn(sf::RenderTarget& target) const
{
    Bacterium::drawOn(target);
    //dessin du grapin et de la tentacule
    draw_tent_grapin(target);
}

void TwitchingBacterium::moveGrip(const Vec2d& delta)
{
    grapin.move(delta);
}

void TwitchingBacterium::draw_tent_grapin(sf::RenderTarget& target) const
{
    //tentacule
    auto line = buildLine(position, grapin.getPosition(), couleur.get(),1.0);
    target.draw(line);
    //grapin (choix d'un cercle plein plus visible)
    auto const circle = buildCircle(grapin.getPosition(), 5, couleur.get());
    target.draw(circle);

}

Bacterium* TwitchingBacterium::clone()
{
    Vec2d decalage(25*Vec2d::fromRandomAngle());
    TwitchingBacterium cl(*this);
    cl.CircularBody::move(decalage);
    return new TwitchingBacterium(cl);
}

void TwitchingBacterium:: mutate()
{
    couleur.mutate();
    //mutation des éléments parametrables
    getProperty("tentacle speed")->mutate();
    getProperty("tentacle length")->mutate();
}

Vec2d TwitchingBacterium:: f(Vec2d position, Vec2d speed) const
{
    Vec2d retour(0.0,0.0);
    return retour;
}

void TwitchingBacterium::update(sf::Time dt)
{
    Bacterium::update(dt);
}

Quantity TwitchingBacterium::eatableQuantity(NutrimentA& nutriment)
{
    return nutriment.eatenBy(*this);
}
Quantity TwitchingBacterium::eatableQuantity(NutrimentB& nutriment)
{
    return nutriment.eatenBy(*this);
}

Quantity TwitchingBacterium::eatableQuantity(Poison& nutriment)
{
    return nutriment.eatenBy(*this);
}
