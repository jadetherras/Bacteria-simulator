/*
 * prjsv 2020
 * Marco Antognini & Jamila Sam
 */

#include <Application.hpp>
#include <Lab/Swarm.hpp>
#include <Lab/SwarmBacterium.hpp>

#include <array>
#include <cassert>

class SwarmBacteriaTest : public Application
{
public:
    SwarmBacteriaTest(int argc, char const** argv)
        : Application(argc, argv)
    {
    }

    virtual ~SwarmBacteriaTest() = default;

    virtual void onRun() override final;

    virtual void onSimulationStart() override final;

    std::string getHelpTextFile() const override final;

    virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;
};

IMPLEMENT_MAIN(SwarmBacteriaTest);

void SwarmBacteriaTest::onRun()
{
    // Basic setup
    Application::onRun();
    setStats(false);
}

void SwarmBacteriaTest::onSimulationStart()
{
    // Note: other swarm Ids can be added to app,json if wanted
    Application::onSimulationStart();
    for (auto i = 1; i < 3; ++i) {
        auto id = std::to_string(i);
        getEnv().addSwarm(new Swarm(id));
    }
}

void SwarmBacteriaTest::onEvent(sf::Event event, sf::RenderWindow&)
{
    // new num keys can be added to handle new swarm Ids
    if (event.type == sf::Event::KeyPressed) {
        if (sf::Keyboard::Num1 <= event.key.code && event.key.code <= sf::Keyboard::Num2) {
            auto id = std::to_string(event.key.code - sf::Keyboard::Num0);
            auto swarm = getEnv().getSwarmWithId(id);
            assert(swarm != nullptr);
            getEnv().addBacterium(new SwarmBacterium(getCursorPositionInView(), swarm));
        }
        // Add comtrole for swarm bacteria if needed
    }
}
std::string SwarmBacteriaTest::getHelpTextFile() const
{
    return RES_LOCATION + "swarm_help_step4.txt";
}
