#pragma once
#include"Random/Random.hpp"

class MutableNumber
{
public :
    MutableNumber (double valeur, double probmut, double sigma,bool inf=0, double borneinf=0, bool sup=0, double bornesup=0);  //Constructeur avec des paramètres
    MutableNumber (j::Value const& config); //constructeur qui initiatise avec les données de app.json
    MutableNumber()=default;
    double get() const; //pour obtenir la valeur du nombre susceptible de muter
    void set(double value); //permet de modifier la valeur du nombre susceptible de muter
    void mutate(); // fonction qui fait muter de manière contingente (dépend de la probabilité de muter) la valeur
    static MutableNumber probability(double initialValue, double mutationProbability, double sigma);//renvoie un nombre mutable de valeur entre 0 et 1 à partir de paramètres
    static MutableNumber probability(j::Value const& config);//renvoie un nombre mutable entre 0 et 1 à partir de paramètres à partir des données de app.json
    static MutableNumber positive(double initialValue, double mutationProbability, double sigma, bool hasMax=0, double max=0); //renvoie un nombre mutable positif à partir de paramètres
    static MutableNumber positive(j::Value const& config, bool hasMax=false, double max=0);//renvoie un nombre mutable positif à partir des données de app.json

private :
    double valeur;
    double probmut;
    double sigma;
    bool inf;
    double borneinf;
    bool sup;
    double bornesup;
    void plafonner(); //ramène la valeur dans les bornes décidées si celle-ci les dépasse
};
