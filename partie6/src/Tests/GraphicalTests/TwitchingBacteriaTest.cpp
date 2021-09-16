/*
 * prjsv 2020
 * Marco Antognini
 */

#include <Application.hpp>
#include <Lab/TwitchingBacterium.hpp>

class TwitchingBacteriaTest : public Application
{
public:
    TwitchingBacteriaTest(int argc, char const** argv)
        : Application(argc, argv)
    {
    }

    virtual ~TwitchingBacteriaTest() = default;

    virtual void onRun() override final;

    virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;
    std::string getHelpTextFile() const override final;
};

IMPLEMENT_MAIN(TwitchingBacteriaTest);

void TwitchingBacteriaTest::onRun()
{
    // Basic setup
    Application::onRun();
    setStats(false);
}

void TwitchingBacteriaTest::onEvent(sf::Event event, sf::RenderWindow&)
{
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        default:
            break;

        case sf::Keyboard::T:
            getEnv().addBacterium(new TwitchingBacterium(getCursorPositionInView()));
            break;

        }

    }
}



std::string TwitchingBacteriaTest::getHelpTextFile() const
{
    return RES_LOCATION + "twitching_help_step4.txt";
}
