/*
 * prjsv 2020
 * Marco Antognini & Jamila Sam
 */

#include <Application.hpp>
#include <Lab/SimpleBacterium.hpp>

class SimpleBacteriaTest : public Application
{
public:
    SimpleBacteriaTest(int argc, char const** argv)
        : Application(argc, argv)
    {
    }

    virtual ~SimpleBacteriaTest() = default;

    virtual void onRun() override final;

    virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;

    std::string getHelpTextFile() const override final;
};

IMPLEMENT_MAIN(SimpleBacteriaTest);

void SimpleBacteriaTest::onRun()
{
    // Basic setup
    Application::onRun();
    setStats(false);
}

void SimpleBacteriaTest::onEvent(sf::Event event, sf::RenderWindow&)
{
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        default:
            break;

        case sf::Keyboard::S:
            getEnv().addBacterium(new SimpleBacterium(getCursorPositionInView()));
            break;
        }
    }
}

std::string SimpleBacteriaTest::getHelpTextFile() const
{
    return RES_LOCATION + "simple_bact_help.txt";
}
