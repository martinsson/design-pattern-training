#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <catch2/catch_session.hpp>

#include <iostream>

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
    explicit Equipment()
    {
        id = counter++;
    }

    virtual ~Equipment() = default;
    void setMediator(EquipmentsMediator *equipmentsMediator)
    {
        mediator = equipmentsMediator;
    }

    bool isSameAs(const Equipment *other) const
    {
        return other != nullptr && id == other->id;
    }
    bool isTurnedOn() const
    {
        return isOn;
    }
};
int Equipment::counter = 0;

class Light : public Equipment
{

public:
    void turnOn();

    void turnOff()
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
};

class Fan : public Equipment
{

public:
    void turnOn();

    void turnOff()
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
};

class Heater : public Equipment
{

public:

    void turnOn();

    void turnOff()
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
};

class EquipmentsMediator
{
public:
    virtual ~EquipmentsMediator() = default;

    virtual void notify(Equipment *equipment, Event event) = 0;
};

class EconomySaverMediator : public EquipmentsMediator
{
    Light *light = nullptr;
    Fan *fan = nullptr;
    Heater *heater = nullptr;

public:
    EconomySaverMediator(Light *l, Fan *f, Heater *h){
        light = l;
        fan = f;
        heater = h;
        light->setMediator(this);
        fan->setMediator(this);
        heater->setMediator(this);
    }

    void notify(Equipment *equipment, const Event event) override
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

private:
    void reactOnLight(const Event event) const
    {
        if (event == Event::TurnOn)
        {
            fan->turnOff();
            heater->turnOff();
        }
    }

    void reactOnFan(const Event event) const
    {
        if (event == Event::TurnOn)
        {
            light->turnOff();
            heater->turnOff();
        }
    }

    void reactOnHeater(const Event event) const
    {
        if (event == Event::TurnOn)
        {
            light->turnOff();
            fan->turnOff();
        }
    }
};

void Light::turnOn()
{
    if (!isOn)
    {
        isOn = true;
        std::cout << "Light is turned on." << std::endl;
        mediator->notify(this, Event::TurnOn);
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

void Heater::turnOn()
{
    if (!isOn)
    {
        isOn = true;
        std::cout << "Heater is turned on." << std::endl;
        mediator->notify(this, Event::TurnOn);
    }
}


class EquipmentTest
{
protected:
    Light *light = new Light();
    Fan* fan = new Fan();
    Heater* heater = new Heater();
    EconomySaverMediator* mediator = new EconomySaverMediator(light, fan, heater);
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
    Catch::Session session;
    session.applyCommandLine(argc, argv);
    return session.run();
}