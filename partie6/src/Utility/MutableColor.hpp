#pragma once
#include"MutableNumber.hpp"
#include<array>
#include<SFML/Graphics.hpp>

class MutableColor
{
public :
    MutableColor(j::Value const& config);//initialise les couleurs via app.json
    void mutate();//fait muter l'intensité de chaque couleur pour faire muter la couleur globale
    sf::Color get() const;//retourne l'intenité de chaque couleur primaire

private :
    std::array<MutableNumber,4> RGBA;//intensité de chaque couleur primaire (synthèse additive)
};
