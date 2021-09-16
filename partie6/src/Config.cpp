#include "Config.hpp"
#include <JSON/JSONSerialiser.hpp>
// window
Config::Config(std::string path) : mConfig(j::readFromFile(path))
    , simulation_debug(mConfig["debug"].toBool())
    , window_simulation_width(mConfig["window"]["simulation"]["width"].toDouble())
    , window_simulation_height(mConfig["window"]["simulation"]["height"].toDouble())
    , window_stats_width(mConfig["window"]["stats"]["width"].toDouble())
    , window_title(mConfig["window"]["title"].toString())
    , window_antialiasing_level(mConfig["window"]["antialiasing level"].toInt())

// stats
    , stats_refresh_rate(mConfig["stats"]["refresh rate"].toDouble())

// simulation

    , simulation_time_factor(mConfig["simulation"]["time"]["factor"].toDouble())
    , simulation_fixed_step(mConfig["simulation"]["time"]["fixed step"].toDouble())
    , simulation_time_max_dt(sf::seconds(mConfig["simulation"]["time"]["max dt"].toDouble()))
    , simulation_background(mConfig["simulation"]["background"].toString())
    , simulation_debug_background(mConfig["simulation"]["debug background"].toString())
    , simulation_size(mConfig["simulation"]["size"].toInt())
// generator
    , generator_nutriment_delay(mConfig["generator"]["nutriment"]["delay"].toDouble())
    , generator_nutriment_prob(mConfig["generator"]["nutriment"]["prob"].toDouble())

// nutriments
    ,nutrimentA_growth_speed(mConfig["nutriments"]["A"]["growth"]["speed"].toInt()),nutrimentA_min_temperature(mConfig["nutriments"]["A"]["growth"]["min temperature"].toInt())
    ,nutrimentA_max_temperature(mConfig["nutriments"]["A"]["growth"]["max temperature"].toInt())
    ,nutrimentA_min_qty(mConfig["nutriments"]["A"]["quantity"]["min"].toInt())
    ,nutrimentA_max_qty(mConfig["nutriments"]["A"]["quantity"]["max"].toInt())

    ,nutrimentB_growth_speed(mConfig["nutriments"]["B"]["growth"]["speed"].toInt()),nutrimentB_min_temperature(mConfig["nutriments"]["B"]["growth"]["min temperature"].toInt())
    ,nutrimentB_max_temperature(mConfig["nutriments"]["B"]["growth"]["max temperature"].toInt())
    ,nutrimentB_min_qty(mConfig["nutriments"]["B"]["quantity"]["min"].toInt())
    ,nutrimentB_max_qty(mConfig["nutriments"]["B"]["quantity"]["max"].toInt())
    ,nutrimentB_resistance_factor(mConfig["nutriments"]["B"]["resistance factor"].toDouble())
    ,nutrimentB_poison_factor(mConfig["nutriments"]["B"]["poison factor"].toDouble())
    ,nutrimentB_nutritive_factor(mConfig["nutriments"]["B"]["nutritive factor"].toDouble())
{}

// TODO : getter for debug
void Config::switchDebug()
{
    mConfig["debug"] = j::boolean(!mConfig["debug"].toBool());
    simulation_debug = mConfig["debug"].toBool();
}

bool Config::getDebug()
{
    return simulation_debug;
}
