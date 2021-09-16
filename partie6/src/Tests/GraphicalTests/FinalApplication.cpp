/*
 * prjsv 2020
 * Marco Antognini & Jamila Sam
 */

#include <Config.hpp>
#include <Lab/SimpleBacterium.hpp>
#include <Lab/Swarm.hpp>
#include <Lab/SwarmBacterium.hpp>
#include <Lab/TwitchingBacterium.hpp>
#include <Lab/Obstacle.hpp>
#include <Lab/PoisonousBacterium.hpp>
#include "FinalApplication.hpp"
#include <Lab/Poison.hpp>

#include <cassert>

IMPLEMENT_MAIN(FinalApplication);

void FinalApplication::onRun()
{
    // Basic setup
    Application::onRun();
    // add graphs for statistics
    setStats(true);
    addGraph(s::GENERAL, { s::SIMPLE_BACTERIA, s::TWITCHING_BACTERIA, s::SWARM_BACTERIA, s::POISONOUS_BACTERIA, s::NUTRIMENT_SOURCES,s::DISH_TEMPERATURE}, 0, 150);
    addGraph(s::NUTRIMENT_QUANTITY, {s::NUTRIMENT_QUANTITY}, 0, 2000);
    addGraph(s::SIMPLE_BACTERIA, { s::BETTER, s::WORSE}, 0, 10);
    addGraph(s::TWITCHING_BACTERIA, { s::TENTACLE_LENGTH, s::TENTACLE_SPEED}, 0, 150);
    addGraph(s::BACTERIA, { s::SPEED}, 10, 50);
    addGraph(s::INFECTION, { s::NOMBRE_DE_VIRUS, s::NOMBRE_INFECTION, s::GENERATION}, 0, 200);
    setActiveGraph(0);
}

void FinalApplication::onSimulationStart()
{
    Application::onSimulationStart();
    getEnv().addSwarm(new Swarm("1"));
    getEnv().addSwarm(new Swarm("2"));
}

void FinalApplication::onEvent(sf::Event event, sf::RenderWindow&)
{
    if (event.type == sf::Event::KeyReleased) {
        switch (event.key.code) {
        default:
            break;

        case sf::Keyboard::S:
            getEnv().addBacterium(new SimpleBacterium(getCursorPositionInView()));
            break;

        case sf::Keyboard::T:
            getEnv().addBacterium(new TwitchingBacterium(getCursorPositionInView()));
            break;

        case sf::Keyboard::B:
            getEnv().remove_separateur();
            break;

        case sf::Keyboard::O:
            getEnv().addObstacle(new Obstacle(getCursorPositionInView()));
            break;

        case sf::Keyboard::P:
            getEnv().addBacterium(new PoisonousBacterium(getCursorPositionInView()));
            break;

        case sf::Keyboard::N:
            getEnv().addNutriment(new Poison(getAppConfig()["nutriments"]["poison"]["quantity"]["max"].toDouble(),getCursorPositionInView(),5));
            break;

        case sf::Keyboard::V:
            getEnv().addVirus(new Virus(getCursorPositionInView()));
            break;

        case sf::Keyboard::Num1: {
            auto swarm = getEnv().getSwarmWithId("1");
            assert(swarm != nullptr);
            getEnv().addBacterium(new SwarmBacterium(getCursorPositionInView(), swarm));
        }
        break;

        case sf::Keyboard::Num2: {
            auto swarm = getEnv().getSwarmWithId("2");
            assert(swarm != nullptr);
            getEnv().addBacterium(new SwarmBacterium(getCursorPositionInView(), swarm));
        }

        break;
        }
    }
}

