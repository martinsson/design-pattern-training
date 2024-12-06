#include <iostream>
#include <catch2/catch_all.hpp>

class ElectricEquipment;
class State
{
protected:
    ElectricEquipment *equipment;

public:
    State(ElectricEquipment *equipment);
    virtual ~State() = default;
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
    virtual void repair() = 0;
    virtual std::string getState() = 0;
};

class ElectricEquipment
{
    State *state;
public:
    ElectricEquipment();
    ~ElectricEquipment();
    void setState(State *newState);
    void turnOn();
    void turnOff();
    void repair();
    std::string getState();
};


class OnState final : public State
{
public:
    OnState(ElectricEquipment *equipment);
    void turnOn() override;
    void turnOff() override;
    void repair() override;
    std::string getState() override;
};

class OffState final : public State
{
public:
    OffState(ElectricEquipment *equipment);
    void turnOn() override;
    void turnOff() override;
    void repair() override;
    std::string getState() override;
};

class BrokenState final : public State
{
public:
    BrokenState(ElectricEquipment *equipment);
    void turnOn() override;
    void turnOff() override;
    void repair() override;
    std::string getState() override;
};


State::State(ElectricEquipment *equipment) : equipment(equipment) {}

ElectricEquipment::ElectricEquipment(){
    state = new OffState(this);
}
ElectricEquipment::~ElectricEquipment(){
    delete state;
}
void ElectricEquipment::setState(State *newState){
    delete state;
    state = newState;
}
void ElectricEquipment::turnOn(){
    state->turnOn();
}
void ElectricEquipment::turnOff(){
    state->turnOff();
}
void ElectricEquipment::repair(){
    state->repair();
}
std::string ElectricEquipment::getState(){
    return state->getState();
}

OnState::OnState(ElectricEquipment *equipment) : State(equipment) {}
void OnState::turnOn(){
    std::cout << "Equipment is already on." << std::endl;
}
void OnState::turnOff(){
    equipment->setState(new OffState(equipment));
    std::cout << "Equipment turned off." << std::endl;
}
void OnState::repair(){
    equipment->setState(new BrokenState(equipment));
    std::cout << "Repair when on breaks the equipemnt!!! Equipment is now broken." << std::endl;
}
std::string OnState::getState(){
    return "on";
}

OffState::OffState(ElectricEquipment *equipment) : State(equipment) {}
void OffState::turnOn()
{
    equipment->setState(new OnState(equipment));
    std::cout << "Equipment turned on." << std::endl;
}
void OffState::turnOff()
{
    std::cout << "Equipment is already off." << std::endl;
}
void OffState::repair()
{
    std::cout << "Equipment is not broken. Do not repair." << std::endl;
}
std::string OffState::getState()
{
    return "off";
}

BrokenState::BrokenState(ElectricEquipment *equipment) : State(equipment) {}
void BrokenState::turnOn()
{
    std::cout << "Equipment is broken, it cannot be turned on." << std::endl;
}
void BrokenState::turnOff()
{
    std::cout << "Equipment is broken, it cannot be turned off." << std::endl;
}
void BrokenState::repair()
{
    equipment->setState(new OffState(equipment));
    std::cout << "Equipment repaired." << std::endl;
}
std::string BrokenState::getState()
{
    return "broken";
}

TEST_CASE("Initial state is off")
{
    ElectricEquipment equipment;
    REQUIRE(equipment.getState() == "off");
}

TEST_CASE("Turn on from off state")
{
    ElectricEquipment equipment;
    equipment.turnOn();
    REQUIRE(equipment.getState() == "on");
}

TEST_CASE("Turn off from on state")
{
    ElectricEquipment equipment;
    equipment.turnOn();
    equipment.turnOff();
    REQUIRE(equipment.getState() == "off");
}

TEST_CASE("Repair from off state")
{
    ElectricEquipment equipment;
    equipment.repair();
    REQUIRE(equipment.getState() == "off");
}

TEST_CASE("Repair from on state")
{
    ElectricEquipment equipment;
    equipment.turnOn();
    equipment.repair();
    REQUIRE(equipment.getState() == "broken");
}

TEST_CASE("Turn on from broken state")
{
    ElectricEquipment equipment;
    equipment.turnOn();
    equipment.repair();
    equipment.turnOn();
    REQUIRE(equipment.getState() == "broken");
}

TEST_CASE("Turn off from broken state")
{
    ElectricEquipment equipment;
    equipment.turnOn();
    equipment.repair();
    equipment.turnOff();
    REQUIRE(equipment.getState() == "broken");
}

TEST_CASE("Repair from broken state")
{
    ElectricEquipment equipment;
    equipment.turnOn();
    equipment.repair();
    equipment.repair();
    REQUIRE(equipment.getState() == "off");
}

TEST_CASE("Electric Equipment State Kata")
{
    ElectricEquipment equipment;
    REQUIRE(equipment.getState() == "off");
    equipment.turnOn();
    REQUIRE(equipment.getState() == "on");
    equipment.repair();
    REQUIRE(equipment.getState() == "broken");
    equipment.repair();
    REQUIRE(equipment.getState() == "off");
    equipment.turnOn();
    REQUIRE(equipment.getState() == "on");
    equipment.turnOff();
    REQUIRE(equipment.getState() == "off");
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