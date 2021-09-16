#include "SimpleBacterium.hpp"
#include "Utility/Vec2d.hpp"
#include "Utility/Utility.hpp"
#include "Utility/Constants.hpp"
#include "Random/Random.hpp"
#include "Application.hpp"
#include "cmath"
#include "NutrimentA.hpp"
#include "NutrimentB.hpp"

using namespace std;

SimpleBacterium::SimpleBacterium(const Vec2d& position)
    :Bacterium(uniform(getConfig()["energy"]["min"].toDouble(),
                       getConfig()["energy"]["max"].toDouble()),
               position,
               Vec2d::fromRandomAngle(),
               uniform(getConfig()["radius"]["min"].toDouble(),
                       getConfig()["radius"]["max"].toDouble()),
               getConfig()["color"]),
     t(uniform(0.0,PI))
{
    addProperty("speed",MutableNumber::positive(getConfig()["speed"]["initial"].toDouble(), getConfig()["speed"]["rate"].toDouble(), getConfig()["speed"]["sigma"].toDouble()));
    addProperty("tumble better", MutableNumber::positive(getConfig()["tumble"]["better"]["initial"].toDouble(),getConfig()["tumble"]["better"]["rate"].toDouble(), getConfig()["tumble"]["better"]["sigma"].toDouble()));
    addProperty("tumble worse",MutableNumber::positive(getConfig()["tumble"]["worse"]["initial"].toDouble(), getConfig()["tumble"]["worse"]["rate"].toDouble(), getConfig()["tumble"]["worse"]["sigma"].toDouble()));
    getAppEnv().add_at_graph(s::SIMPLE_BACTERIA, true);
}

SimpleBacterium::SimpleBacterium(const SimpleBacterium& source)
    :Bacterium(source),
     t(source.t),
     t_bascule(source.t_bascule)
{
    getAppEnv().add_at_graph(s::SIMPLE_BACTERIA, true);
}

SimpleBacterium::~SimpleBacterium()
{
    getAppEnv().add_at_graph(s::SIMPLE_BACTERIA, false);
}


Bacterium* SimpleBacterium::clone()
{
    return new SimpleBacterium(*this);
}


j::Value& SimpleBacterium::getConfig() const
{
    return getAppConfig()["simple bacterium"];
}

void SimpleBacterium::move(sf::Time dt)
{

    Vec2d pos = position;
    Vec2d vitesse = getSpeedVector();
    vitesse = stepDiffEq(position, vitesse, dt, *this).speed; // vitesse constante

    if ((stepDiffEq(position, vitesse, dt, *this).position - position).lengthSquared()>0.001) {
        position = stepDiffEq(position, vitesse, dt, *this).position;  // déplacement
        consumeEnergy((position-pos).length()*get_energie_deplacement());  //consommation d'energie
    }

    t +=3 * dt.asSeconds(); //pour le dessin de la flagelle

    MutableNumber lambda;

    if(score>=ancien_score) {
        lambda = *(getProperty("tumble better"));
    }  else  {
        lambda = *(getProperty("tumble worse"));
    }

    t_bascule += dt.asSeconds();

    if(bernoulli(1-exp(-t_bascule/lambda.get()))) { //test pour savoir si la bactérie bascule
        if(getConfig()["tumble"]["algo"].toString()== "single random vector") {//bascule
            direction=Vec2d::fromRandomAngle();
            vitesse=getSpeedVector();

        }  else if(getConfig()["tumble"]["algo"].toString()=="best of N") {
            Vec2d best(0.0, 0.0);
            for(size_t i(0); i<20; ++i) {
                Vec2d dir(Vec2d::fromRandomAngle());
                if(getAppEnv().getPositionScore(dir + position)> getAppEnv().getPositionScore(best + position) ) {
                    best = dir;
                }
            }

            //CHOIX DE CONCEPTION les bacteries ne s'arrête pas sur les nutriments, sinon elle reste au centre, et si la consommation est lente
            //(ex : simple sur les nutriments bleus)
            //alors le programme parait statique -> plus de dynamisme
            if(getAppEnv().getNutrimentColliding(*this) == nullptr) {
                direction=best;
                vitesse=getSpeedVector();
            }
        }
        t_bascule = 0.0;
    }
}

Vec2d SimpleBacterium::getSpeedVector()
{
    return direction * getProperty("speed")->get();
}

Vec2d SimpleBacterium::f(Vec2d position, Vec2d speed) const
{
    Vec2d force(0.0,0.0);
    return force; //force nulle pour SimpleBacterie
}

void SimpleBacterium::drawOn(sf::RenderTarget& target) const
{
    Bacterium::drawOn(target);
    auto set_of_points = sf::VertexArray(sf::LinesStrip);
    set_of_points.append({{static_cast<float>(0),static_cast<float>(0)}, couleur.get()});
    for (int i(0); i <= 30; ++i) {
        set_of_points.append({{static_cast<float>(-i * rayon / 10.0), static_cast<float>(rayon * sin(t) * sin(2 * i / 10.0)) }, couleur.get()});
    }

    auto transform = sf::Transform(); // déclare une matrice de transformation
    // ici ensemble d'opérations comme des translations ou rotations faites  sur transform:
    transform.translate(position);
    transform.rotate(rotation/DEG_TO_RAD);     //Attention degré !
    target.draw(set_of_points, transform); // dessin de l'ensemble des points
}


void SimpleBacterium::update(sf::Time dt)
{
    Bacterium::update(dt);
    ancien_score = score;
    score = getAppEnv().getPositionScore(position);
}


void SimpleBacterium::mutate()
{
    couleur.mutate();
    //mutation des éléments parametrables
    getProperty("speed")->mutate();
    getProperty("tumble better")->mutate();
    getProperty("tumble worse")->mutate();
}

Quantity SimpleBacterium::eatableQuantity(NutrimentA& nutriment)
{
    return nutriment.eatenBy(*this);
}
Quantity SimpleBacterium::eatableQuantity(NutrimentB& nutriment)
{
    return nutriment.eatenBy(*this);
}

Quantity SimpleBacterium::eatableQuantity(Poison &nutriment)
{
    return nutriment.eatenBy(*this);
}

