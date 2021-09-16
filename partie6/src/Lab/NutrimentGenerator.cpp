#include "NutrimentGenerator.hpp"
#include "Config.hpp"
#include "Application.hpp"
#include "Utility/Utility.hpp"
#include "Random/Random.hpp"
#include "NutrimentA.hpp"
#include "NutrimentB.hpp"

NutrimentGenerator::NutrimentGenerator()
    : compteur(sf::Time::Zero)
{}

void NutrimentGenerator::reset()
{
    compteur = sf::Time::Zero;
}

void NutrimentGenerator::update(sf::Time dt)
{
    compteur += dt;

    if (compteur > sf::seconds(getAppConfig()["generator"]["nutriment"]["delay"].toDouble())) {
        reset(); //remise a zero du compteur, evite une utilisation permanente

        Vec2d taille_env(getApp().getLabSize());
        double x(normal(taille_env.x/2,taille_env.x/4*taille_env.x/4));
        double y(normal(taille_env.y/2,taille_env.y/4*taille_env.y/4));
        Vec2d position(x,y);

        if (bernoulli(getApp().getEnv().getProb_nutriment())) {//deux types avec prob associé

            getAppEnv().addNutriment(new NutrimentA(uniform(getShortConfig().nutrimentA_min_qty,getShortConfig().nutrimentA_max_qty),position));
        } else {

            getAppEnv().addNutriment(new NutrimentB(uniform(getShortConfig().nutrimentB_min_qty,getShortConfig().nutrimentB_max_qty),position));

        }
    }
}

