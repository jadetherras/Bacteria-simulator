
/*
 * prjsv 2016
 * Marco Antognini
 */

#ifndef INFOSV_CONFIG_HPP
#define INFOSV_CONFIG_HPP

#include <string>
#include "JSON/JSON.hpp"
#include <SFML/Graphics.hpp>

// Define resources location
std::string const RES_LOCATION = "../res/";

// Define the location of the config file
std::string const DEFAULT_CFG = "app.json";

// Define the location of the font file (could be in the config instead of here)
std::string const FONT_LOCATION = RES_LOCATION + "sansation.ttf";


class Config
{
private:
    j::Value mConfig;
    bool simulation_debug;

public:
    Config(std::string path);

    // enables / disables debug mode
    void switchDebug();
    bool getDebug();

    // returns read
    j::Value& getJsonRead()
    {
        return mConfig;
    }

    j::Value const& getJsonRead() const
    {
        return mConfig;
    }


    //window
    const double window_simulation_width;
    const double window_simulation_height;
    const double window_stats_width;
    const std::string  window_title;
    const int window_antialiasing_level;


    // stats
    const double stats_refresh_rate;
    const std::string stats_log_folder = "logs/";
    const std::string stats_log_prefix = "log_";
    const std::string stats_log_header = "# Plot with GNUPLOT : gnuplot -e \"plot for [i=1:6] 'log_0.txt' u i w l title columnheader(i)\"";

    // debug
    const sf::Color debug_text_color = sf::Color::White;
    const size_t default_debug_text_size = 20;


    //simulation

    const double  simulation_time_factor;
    const double  simulation_fixed_step;
    const sf::Time  simulation_time_max_dt;
    const std::string simulation_background;
    const std::string simulation_debug_background;
    const int simulation_size;

    // generator
    const double generator_nutriment_delay;
    const double generator_nutriment_prob;

    //nutriments

    const int nutrimentA_growth_speed;
    const int nutrimentA_min_temperature;
    const int nutrimentA_max_temperature;
    const int nutrimentA_min_qty;
    const int nutrimentA_max_qty;
    const std::string nutrimentA_texture;

    const int nutrimentB_growth_speed;
    const int nutrimentB_min_temperature;
    const int nutrimentB_max_temperature;
    const int nutrimentB_min_qty;
    const int nutrimentB_max_qty;
    const std::string nutrimentB_texture;

    const double nutrimentB_resistance_factor;
    const double nutrimentB_poison_factor;
    const double nutrimentB_nutritive_factor;
};

#endif // INFOSV_CONFIG_HPP

