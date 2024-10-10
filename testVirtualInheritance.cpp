//
// Created by Andrei on 20.11.23.
//

#include <AndreiUtils/json.hpp>
#include <AndreiUtils/utils.hpp>
#include <iostream>
#include <gtest/gtest.h>

using namespace AndreiUtils;
using namespace std;
using json = nlohmann::json;

class Concept {
public:
    explicit Concept(string name, json defaultData = {}) : name(std::move(name)), defaultData(std::move(defaultData)) {
        cout << "Called proper constructor of Concept: " << this->name << endl;
    }

    virtual ~Concept() = default;

    string name;
    json defaultData;

protected:
    Concept() : name(), defaultData() {
        cout << "Called default constructor of Concept" << endl;
    }
};

class PhysicalEntity : public virtual Concept {
public:
    explicit PhysicalEntity(string name, json defaultData = {}) : Concept(std::move(name), std::move(defaultData)) {
        cout << "Called proper constructor of PhysicalEntity: " << this->name << endl;
    }

    ~PhysicalEntity() override = default;

protected:
    PhysicalEntity() : Concept() {
        cout << "Called default constructor of PhysicalEntity" << endl;
    }
};

class Agent : public virtual PhysicalEntity {
public:
    explicit Agent(string name, json defaultData = {}) : Concept(std::move(name), std::move(defaultData)), PhysicalEntity("", {}) {
        cout << "Called proper constructor of Agent: " << this->name << endl;
    }

    ~Agent() override = default;

protected:
    Agent() : PhysicalEntity() {
        cout << "Called default constructor of Agent" << endl;
    }
};

class Human : public virtual Agent {
public:
    explicit Human(string name, json defaultData = {}) : Agent(std::move(name), std::move(defaultData)) {
        cout << "Called proper constructor of Human" << endl;
    }

    explicit Human(string name, string humanName, json defaultData = {}) :
            Concept(std::move(name), std::move(defaultData)), Agent(std::move(name), std::move(defaultData)),
            humanName(std::move(humanName)) {
        cout << "Called proper constructor of Human" << endl;
    }

    ~Human() override = default;

    std::string humanName;

protected:
    Human() : Agent() {
        cout << "Called default constructor of Agent" << endl;
    }
};

void testInheritance() {
    json j;
    j["data"]["properties"] = std::map<std::string, std::string>{{"LeftHand",  "Left"},
                                                                 {"RightHand", "Right"}};
    auto human = make_shared<Human>("Human", j);
    cout << human->defaultData.dump(4) << endl;
    auto human2 = make_shared<Human>("Human", "Andrei", j);
    cout << human2->defaultData.dump(4) << endl;
}

TEST(VirtualInheritanceTest, TestInheritance) {
    json j;
    j["data"]["properties"] = std::map<std::string, std::string>{{"LeftHand",  "Left"},
                                                                 {"RightHand", "Right"}};

    auto human = std::make_shared<Human>("Human", j);
    cout << human->defaultData.dump(4) << endl;
    ASSERT_TRUE(human->defaultData.is_null());

    auto human2 = std::make_shared<Human>("Human", "Andrei", j);
    cout << human->defaultData.dump(4) << endl;
    ASSERT_EQ(human2->defaultData["data"]["properties"]["LeftHand"], "Left");
    ASSERT_EQ(human2->defaultData["data"]["properties"]["RightHand"], "Right");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    cout << "Hello World!" << endl;

    // testInheritance();

    return RUN_ALL_TESTS();

}
