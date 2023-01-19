#include <iostream>
#include <vector>
#include <set>
#include <map>

int getInteger() {

    int result;
    do {
        std::cin >> result;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cerr << "Invalid input: must be a number! Try again: ";
        } else {
            return result;
        }
    } while (true);

}

class Material {
protected:
    int price;
    char *name;

protected:
    Material(int price, char *name) : price(price), name(name) {}

public:
    char *getName() const {
        return name;
    }

    int getPrice() const {
        return price;
    }
};

class Wood : public Material {
private:
    Wood() : Material(1, "Wood") {}

public:
    static Wood &instance() {
        static Wood INSTANCE;
        return INSTANCE;
    }
};

class Plastic : public Material {
private:
    Plastic() : Material(2, "Plastic") {}

public:
    static Plastic &instance() {
        static Plastic INSTANCE;
        return INSTANCE;
    }
};

class Metal : public Material {
private:
    Metal() : Material(3, "Metal") {}

public:
    static Metal &instance() {
        static Metal INSTANCE;
        return INSTANCE;
    }
};

class Building {
protected:
    int basePrice;
    char label;
    char *name;
    std::vector<Material *> materials;

protected:
    Building(int basePrice, char label, char *name, std::vector<Material *> materials) : basePrice(basePrice),
                                                                                         label(label), name(name),
                                                                                         materials(materials) {}

public:
    char *getName() const {
        return name;
    }

    char getLabel() const {
        return label;
    }

    int getBasePrice() const {
        return basePrice;
    }

    std::vector<Material *> getMaterials() const {
        return materials;
    }

    int getTotalCosts() const {
        int materialPrice = 0;
        for (auto material: materials) {
            materialPrice += material->getPrice();
        }
        return basePrice + materialPrice;
    }
};

class WaterEnergyPlant : public Building {
private:
    WaterEnergyPlant() : Building(1, 'W', "WaterEngeryPlant",
                                  {&Wood::instance(), &Wood::instance(), &Plastic::instance(), &Plastic::instance(),
                                   &Metal::instance()}) {}

public:
    static WaterEnergyPlant &instance() {
        static WaterEnergyPlant INSTANCE;
        return INSTANCE;
    }
};

class SolarEnergyPlant : public Building {
private:
    SolarEnergyPlant() : Building(1, 'S', "SolarEnegeryPlant",
                                  {&Plastic::instance(), &Plastic::instance(), &Plastic::instance(), &Metal::instance(),
                                   &Metal::instance()}) {}

public:
    static SolarEnergyPlant &instance() {
        static SolarEnergyPlant INSTANCE;
        return INSTANCE;
    }
};

class WindEnergyPlant : public Building {
private:
    WindEnergyPlant() : Building(1, 'I', "WindEnergyPlant",
                                 {&Wood::instance(), &Wood::instance(), &Plastic::instance(), &Metal::instance(),
                                  &Metal::instance()}) {}

public:
    static WindEnergyPlant &instance() {
        static WindEnergyPlant INSTANCE;
        return INSTANCE;
    }
};

class CapyCitySim {
protected:
    int length;
    int width;
    Building ***blueprint;
public:
    CapyCitySim(int length, int width) : length(length), width(width) {
        createBlueprint();
    }

    ~CapyCitySim() {
        for (int row = 0; row < width; row++) {
            for (int col = 0; col < length; col++) {
                delete blueprint[row][col];
            }
            delete blueprint[row];
        }
        delete blueprint;
    }


    bool isInBounds(int x, int y, int length, int width) const {

        return 0 <= x && 0 <= x + length && x + length <= this->length
               && 0 <= y && 0 <= y + width && y + width <= this->width;

    }

    void loop() {
        std::cout << "Welcome to Capycity, please select an option: " << std::endl;

        while (true) {
            std::cout << "1) Place Building" << std::endl;
            std::cout << "2) Delete Area" << std::endl;
            std::cout << "3) Show current construction plan" << std::endl;
            std::cout << "4) Quit" << std::endl;
            std::cout << std::endl;

            std::cout << "Please select a number: ";
            char option;
            std::cin >> option;
            std::cout << std::endl;

            switch (option) {
                case '1':
                    placeBuilding();
                    break;
                case '2':
                    deleteArea();
                    break;
                case '3':
                    showConstructionPlan();
                    break;
                case '4':
                    return;
                default:
                    std::cout << "Not a valid option" << std::endl;
            }
        }
    }

    void createBlueprint() {
        blueprint = new Building **[width];
        for (int row = 0; row < width; row++) {
            blueprint[row] = new Building *[length];
            for (int col = 0; col < length; col++) {
                blueprint[row][col] = nullptr;
            }
        }
    }

    void showConstructionPlan() {
        std::map<Building *, int> buildingCount;
        int buildingsCounter = 0;
        for (int row = 0; row < width; row++) {
            for (int col = 0; col < length; col++) {
                Building *building = blueprint[row][col];
                if (building == nullptr) {
                    std::cout << "~";
                    continue;
                }

                if (buildingCount.contains(building)) {
                    buildingCount[building] += 1;
                } else {
                    buildingCount.insert({building, 1});
                }
                std::cout << building->getLabel();
                buildingsCounter++;
            }
            std::cout << std::endl;
        }

        int totalCosts = 0;
        for (auto count: buildingCount) {
            std::cout << std::endl;
            Building *building = count.first;
            int counter = count.second;
            totalCosts += building->getTotalCosts() * counter;
            std::cout << building->getName() << " costs $" << building->getTotalCosts() << " including:" << std::endl;
            std::cout << "\tbase price of $" << building->getBasePrice() << std::endl;
            for (auto material: building->getMaterials()) {
                std::cout << "\t" << material->getName() << " $" << material->getPrice() << std::endl;
            }
        }
        std::cout << std::endl;
        for(auto count : buildingCount) {
            Building *building = count.first;
            int counter = count.second;
            std::cout << counter << "x " << building->getName() << " for $" <<  (counter * building->getTotalCosts()) << std::endl;
        }

        std::cout << "Total plan costs $" << totalCosts << std::endl;
        system("pause");

    }

    Building *selectBuilding() {

        do {
            std::cout << std::endl;
            std::cout << "W) Water energy plant" << std::endl;
            std::cout << "S) Solar energy plant" << std::endl;
            std::cout << "I) Wind energy plant" << std::endl;
            std::cout << "B) Back" << std::endl;

            std::cout << "Please select a building to place: ";

            char buildingSelected;
            std::cin >> buildingSelected;

            switch (toupper(buildingSelected)) {
                case 'W':
                    return &WaterEnergyPlant::instance();
                case 'S':
                    return &SolarEnergyPlant::instance();
                case 'I':
                    return &WindEnergyPlant::instance();
                case 'B':
                    return nullptr;
                default:
                    std::cout << "Not a valid option" << std::endl;
            }
        } while (true);

    }

    void placeBuilding() {

        Building *selectedBuilding = selectBuilding();

        if (selectedBuilding == nullptr) {
            return;
        }

        std::cout << "Enter a start position X (0 - " << this->length - 1 << "): ";
        int x = getInteger();

        std::cout << "Enter a start position Y (0 - " << this->width - 1 << "): ";
        int y = getInteger();

        std::cout << "Enter the length of the Building: ";
        int length = getInteger();

        std::cout << "Enter the width of the Building: ";
        int width = getInteger();

        if (!isInBounds(x, y, length, width)) {
            std::cout << "Input is outside the construction area." << std::endl;
            system("pause");
            return;
        }

        for (int row = y; row < y + width; row++) {
            for (int col = x; col < x + length; col++) {
                if (blueprint[row][col] != nullptr) {
                    std::cout << "Space is already occupied!" << std::endl;
                    system("pause");
                    return;
                }
            }
        }

        for (int row = y; row < y + width; row++) {
            for (int col = x; col < x + length; col++) {
                blueprint[row][col] = selectedBuilding;
            }
        }
    }


    void deleteArea() {

        std::cout << "Enter a start position X (0 - " << length - 1 << "): ";
        int x = getInteger();

        std::cout << "Enter a start position Y (0 - " << width - 1 << "): ";
        int y = getInteger();

        std::cout << "Enter the length of the area to delete: ";
        int length = getInteger();

        std::cout << "Enter the width of the area to delete: ";
        int width = getInteger();

        if (!isInBounds(x, y, length, width)) {
            std::cout << "Input is outside the construction area." << std::endl;
            system("pause");
            return;
        }

        for (int row = y; row < y + width; row++) {
            for (int col = x; col < x + length; col++) {
                blueprint[row][col] = nullptr;
            }
        }
    }

};

int main(int argc, char **argv) {

    if (argc != 3) {
        std::cout << "Usage: " << std::endl;
        std::cout << "\tcapycity <length> <width>" << std::endl;
        return 1;
    }

    try {
        int length = std::stoi(argv[1]);
        int width = std::stoi(argv[2]);
        CapyCitySim capyCitySim(length, width);
        capyCitySim.loop();
    }
    catch (std::invalid_argument const &ex) {
        std::cout << "Input must be of 2 integer numbers." << std::endl;
        return 2;
    }
    catch (std::out_of_range const &ex) {
        std::cout << "The number is too big." << std::endl;
        return 3;
    }

}