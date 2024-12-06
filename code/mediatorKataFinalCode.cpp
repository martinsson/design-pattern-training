#include <iostream>
#include <catch2/catch_all.hpp>

class EquipmentsMediator;

enum class Event
{
    TurnOn,
    TurnOff
};

class Equipment
{
    int id;
    static int counter;

protected:
    bool isOn = false;
    EquipmentsMediator *mediator;

public:
    explicit Equipment();
    virtual ~Equipment() = default;
    void setMediator(EquipmentsMediator *equipmentsMediator);
    bool isSameAs(const Equipment *other);
    bool isTurnedOn();
};

class Light : public Equipment
{
public:
    void turnOn();
    void turnOff();
};

class Fan : public Equipment
{
public:
    void turnOn();
    void turnOff();
};

class Heater : public Equipment
{
public:
    void turnOn();
    void turnOff();
};

class EquipmentsMediator
{
public:
    virtual ~EquipmentsMediator() = default;
    virtual void notify(Equipment *equipment, Event event) = 0;
};

class EconomySaverMediator : public EquipmentsMediator
{
    Light *light;
    Fan *fan = nullptr;
    Heater *heater = nullptr;
public:
    EconomySaverMediator(Light *l, Fan *f, Heater *h);
    void notify(Equipment *equipment, const Event event) override;
private:
    void reactOnLight(const Event event);
    void reactOnFan(const Event event);
    void reactOnHeater(const Event event);
};

int Equipment::counter = 0;
Equipment::Equipment()
{
    id = counter++;
}
void Equipment::setMediator(EquipmentsMediator *equipmentsMediator)
{
    mediator = equipmentsMediator;
}
bool Equipment::isSameAs(const Equipment *other)
{
    return other != nullptr && id == other->id;
}
bool Equipment::isTurnedOn()
{
    return isOn;
}

void Light::turnOff()
{
    if (isOn)
    {
        isOn = false;
        std::cout << "Light is turned off." << std::endl;
    }
    else
    {
        std::cout << "Light is already off." << std::endl;
    }
}
void Light::turnOn()
{
    if (!isOn)
    {
        isOn = true;
        std::cout << "Light is turned on." << std::endl;
        mediator->notify(this, Event::TurnOn);
    }
}

void Fan::turnOff()
{
    if (isOn)
    {
        isOn = false;
        std::cout << "Fan is turned off." << std::endl;
    }
    else
    {
        std::cout << "Fan is already off." << std::endl;
    }
}
void Fan::turnOn()
{
    if (!isOn)
    {
        isOn = true;
        std::cout << "Fan is turned on." << std::endl;
        mediator->notify(this, Event::TurnOn);
    }
}

void Heater::turnOff()
{
    if (isOn)
    {
        isOn = false;
        std::cout << "Heater is turned off." << std::endl;
    }
    else
    {
        std::cout << "Heater is already off." << std::endl;
    }
}
void Heater::turnOn()
{
    if (!isOn)
    {
        isOn = true;
        std::cout << "Heater is turned on." << std::endl;
        mediator->notify(this, Event::TurnOn);
    }
}

EconomySaverMediator::EconomySaverMediator(Light *l, Fan *f, Heater *h)
{
    light = l;
    fan = f;
    heater = h;
    light->setMediator(this);
    fan->setMediator(this);
    heater->setMediator(this);
}
void EconomySaverMediator::notify(Equipment *equipment, const Event event)
{
    if (equipment->isSameAs(light))
    {
        reactOnLight(event);
    }
    else if (equipment->isSameAs(fan))
    {
        reactOnFan(event);
    }
    else if (equipment->isSameAs(heater))
    {
        reactOnHeater(event);
    }
}
void EconomySaverMediator::reactOnLight(const Event event)
{
    if (event == Event::TurnOn)
    {
        fan->turnOff();
        heater->turnOff();
    }
}
void EconomySaverMediator::reactOnFan(const Event event)
{
    if (event == Event::TurnOn)
    {
        light->turnOff();
        heater->turnOff();
    }
}
void EconomySaverMediator::reactOnHeater(const Event event)
{
    if (event == Event::TurnOn)
    {
        light->turnOff();
        fan->turnOff();
    }
}

class EquipmentTest
{
protected:
    std::unique_ptr<Light> light;
    std::unique_ptr<Fan> fan;
    std::unique_ptr<Heater> heater;
    std::unique_ptr<EconomySaverMediator> mediator;
    EquipmentTest()
        : light(std::make_unique<Light>()),
          fan(std::make_unique<Fan>()),
          heater(std::make_unique<Heater>()),
          mediator(std::make_unique<EconomySaverMediator>(light.get(), fan.get(), heater.get()))
    {
    }
};

TEST_CASE_METHOD(EquipmentTest, "Equipment Suite: LightTurnOn")
{
    light->turnOn();
    REQUIRE(light->isTurnedOn());
    REQUIRE_FALSE(fan->isTurnedOn());
    REQUIRE_FALSE(heater->isTurnedOn());
}

TEST_CASE_METHOD(EquipmentTest, "Equipment Suite: FanTurnOn")
{
    fan->turnOn();
    REQUIRE(fan->isTurnedOn());
    REQUIRE_FALSE(light->isTurnedOn());
    REQUIRE_FALSE(heater->isTurnedOn());
}

TEST_CASE_METHOD(EquipmentTest, "Equipment Suite: HeaterTurnOn")
{
    heater->turnOn();
    REQUIRE(heater->isTurnedOn());
    REQUIRE_FALSE(light->isTurnedOn());
    REQUIRE_FALSE(fan->isTurnedOn());
}

TEST_CASE_METHOD(EquipmentTest, "Equipment Suite: LightTurnOff")
{
    light->turnOn();
    light->turnOff();
    REQUIRE_FALSE(light->isTurnedOn());
}

TEST_CASE_METHOD(EquipmentTest, "Equipment Suite: FanTurnOff")
{
    fan->turnOn();
    fan->turnOff();
    REQUIRE_FALSE(fan->isTurnedOn());
}

TEST_CASE_METHOD(EquipmentTest, "Equipment Suite: HeaterTurnOff")
{
    heater->turnOn();
    heater->turnOff();
    REQUIRE_FALSE(heater->isTurnedOn());
}

TEST_CASE_METHOD(EquipmentTest, "Equipment Suite: SequentialTurnOn 1")
{
    light->turnOn();
    fan->turnOn();
    heater->turnOn();
    REQUIRE_FALSE(light->isTurnedOn());
    REQUIRE_FALSE(fan->isTurnedOn());
    REQUIRE(heater->isTurnedOn());
}

TEST_CASE_METHOD(EquipmentTest, "Equipment Suite: SequentialTurnOn 2")
{
    fan->turnOn();
    heater->turnOn();
    light->turnOn();
    REQUIRE(light->isTurnedOn());
    REQUIRE_FALSE(fan->isTurnedOn());
    REQUIRE_FALSE(heater->isTurnedOn());
}

TEST_CASE_METHOD(EquipmentTest, "Equipment Suite: SequentialTurnOn 3")
{
    heater->turnOn();
    light->turnOn();
    fan->turnOn();
    REQUIRE_FALSE(light->isTurnedOn());
    REQUIRE(fan->isTurnedOn());
    REQUIRE_FALSE(heater->isTurnedOn());
}

int main(int argc, char *argv[])
{
    Catch::ConfigData config;
    config.showSuccessfulTests = true; // Afficher les tests réussis
    Catch::Session session;
    session.useConfigData(config);
    session.applyCommandLine(argc, argv);
    return session.run();
}