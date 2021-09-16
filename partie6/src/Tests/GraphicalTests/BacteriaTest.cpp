/*
 * prjsv 2020
 * Marco Antognini & Jamila Sam
 * WARNING : Works only in step3
 */

#include <Application.hpp>
#include <Lab/Bacterium.hpp>
#include <Utility/Types.hpp>
#include <Lab/NutrimentA.hpp>

constexpr Quantity DEFAULT_ENERGY(60);
constexpr Quantity DEFAULT_RADIUS(10);

class MockBacterium : public Bacterium
{
public:
    MockBacterium(Vec2d const& position)
        : Bacterium(DEFAULT_ENERGY, position, Vec2d::fromRandomAngle(),
                    DEFAULT_RADIUS,
                    getAppConfig()["simple bacterium"]["color"])
    {
    }

    j::Value& getConfig() const
    {
        return getAppConfig()["simple bacterium"];
    }


protected:
    virtual Bacterium* clone() const
    {
        /* ne fait rien pour le moment
        (cette méthode devra être modifiée lorsque vous aurez codé
        la division)
        */
        return nullptr;
    }

    virtual void move(sf::Time dt)
    {
        consumeEnergy(0.1);
    }
};



class BacteriaTest : public Application
{
public:
    BacteriaTest(int argc, char const** argv)
        : Application(argc, argv)
    {
    }

    virtual ~BacteriaTest() = default;

    virtual void onRun() override final;

    virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;

    std::string getHelpTextFile() const override final;
};

IMPLEMENT_MAIN(BacteriaTest);

void BacteriaTest::onRun()
{
    // Basic setup
    Application::onRun();
    setStats(false);
}

void BacteriaTest::onEvent(sf::Event event, sf::RenderWindow&)
{
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        default:
            break;

        case sf::Keyboard::S:
            getEnv().addBacterium(new MockBacterium(getCursorPositionInView()));
            break;
        }
    }
}

std::string BacteriaTest::getHelpTextFile() const
{
    return RES_LOCATION + "bact_help_step3.txt";
}
