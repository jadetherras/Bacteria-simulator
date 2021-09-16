/*
 * prjsv 2020
 * Marco Antognini & Jamila Sam
 */

#include <Application.hpp>
#include <Lab/TwitchingBacterium.hpp>

class MockTwitching : public TwitchingBacterium
{
public:
    using TwitchingBacterium::TwitchingBacterium;
    using TwitchingBacterium::moveGrip;
    using TwitchingBacterium::getRadius;
};


class GripTest : public Application
{
public:
    GripTest(int argc, char const** argv)
        : Application(argc, argv)
    {
    }

    virtual ~GripTest() = default;

    virtual void onRun() override final;
    std::string getHelpTextFile() const override final;
    virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;
};

IMPLEMENT_MAIN(GripTest);

void GripTest::onRun()
{
    // Basic setup
    Application::onRun();
    setStats(false);
}

void GripTest::onEvent(sf::Event event, sf::RenderWindow&)
{
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        default:
            break;

        case sf::Keyboard::T:
            MockTwitching* twitching(new MockTwitching(getCursorPositionInView()));
            twitching->moveGrip(Vec2d(5,5)*twitching->getRadius());
            getEnv().addBacterium(twitching);
            break;
        }
    }
    // Twitching bacteria
    //(controls to be added here if needed)
}

std::string GripTest::getHelpTextFile() const
{
    return RES_LOCATION + "grip_help_step4.txt";
}
