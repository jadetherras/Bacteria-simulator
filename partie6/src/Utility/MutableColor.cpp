#include<iostream>
#include"MutableColor.hpp"
#include"Random/Random.hpp"
#include"MutableNumber.hpp"
using namespace std;

MutableColor::MutableColor(j::Value const& config)
    :RGBA ({config["r"], config["g"], config["b"], config["a"]})
{}

void MutableColor::mutate()
{
    for (unsigned int i(0); i<=4; ++i) {
        RGBA[i].mutate();
    }
}

sf::Color MutableColor::get() const
{
    return { static_cast<sf::Uint8>(RGBA[0].get() * 255),
             static_cast<sf::Uint8>(RGBA[1].get() * 255),
             static_cast<sf::Uint8>(RGBA[2].get() * 255),
             static_cast<sf::Uint8>(RGBA[3].get() * 255) };
}

