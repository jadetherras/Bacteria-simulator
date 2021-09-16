#include<iostream>
#include"Random/Random.hpp"
#include"MutableNumber.hpp"
using namespace std;

MutableNumber :: MutableNumber (double valeur, double probmut, double sigma,bool inf, double borneinf, bool sup, double bornesup)
    : valeur(valeur),
      probmut(probmut),
      sigma(sigma),
      inf(inf),
      borneinf(borneinf),
      sup(sup),
      bornesup(bornesup)
{}

MutableNumber :: MutableNumber (j::Value const& config)
    :
    valeur(config["initial"].toDouble()),
    probmut(config["rate"].toDouble()),
    sigma(config["sigma"].toDouble()),
    inf(config["clamp min"].toBool()),
    borneinf(config["min"].toDouble()),
    sup(config["clamp max"].toBool()),
    bornesup(config["max"].toDouble())

{}

double MutableNumber::get() const
{
    return valeur;
}

void MutableNumber::set(double value)
{
    valeur=value;
    plafonner();
}

void MutableNumber::mutate()
{
    if (bernoulli(probmut)) {
        valeur+=normal(0, sigma*sigma);
    }
    plafonner();
}

void MutableNumber:: plafonner()
{
    if (inf) {
        if (valeur<borneinf)
            valeur=borneinf;
    }

    if (sup) {
        if (valeur>bornesup)
            valeur=bornesup;
    }

}

MutableNumber MutableNumber:: probability(double initialValue, double mutationProbability, double sigma)
{
    return MutableNumber(initialValue, mutationProbability, sigma, true, 0, true, 1);
}

MutableNumber MutableNumber:: probability(j::Value const& config)
{
    return MutableNumber(config["initial"].toDouble(),config["rate"].toDouble(),config["initial"].toDouble(),true,0,true,1);
}

MutableNumber MutableNumber:: positive(double initialValue, double mutationProbability, double sigma, bool hasMax, double max)
{
    return MutableNumber (initialValue, mutationProbability, sigma, 1,0, hasMax, max);
}

MutableNumber MutableNumber:: positive(j::Value const& config, bool hasMax, double max)
{
    return MutableNumber(config["initial"].toDouble(),config["rate"].toDouble(),config["initial"].toDouble(),true,0,hasMax,max);
}
