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
    std::map<Material *, int> materials;
    int capacity;

protected:
    Building(int basePrice, char label, char *name, std::map<Material *, int> materials, int capacity) : basePrice(basePrice),
                                                                                         label(label), name(name),
                                                                                         materials(materials), capacity(capacity) {}

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

    std::map<Material *, int> getMaterials() const {
        return materials;
    }

    int getTotalCosts() const {
        int materialPrice = 0;
        for (auto iter: materials) {
            Material *material = iter.first;
            int count = iter.second;
            materialPrice += material->getPrice() * count;
        }
        return basePrice + materialPrice;
    }

    int getCapacity() const {
        return capacity;
    }
};

class WaterEnergyPlant : public Building {
private:
    WaterEnergyPlant() : Building(1, 'W', "WaterEnergyPlant",
                                  {{&Wood::instance(),2}, {&Plastic::instance(), 2}, { &Metal::instance(), 1}}, 403) {}

public:
    static WaterEnergyPlant &instance() {
        static WaterEnergyPlant INSTANCE;
        return INSTANCE;
    }
};

class SolarEnergyPlant : public Building {
private:
    SolarEnergyPlant() : Building(1, 'S', "SolarEnergyPlant",
                                  {{&Plastic::instance(), 3}, {&Metal::instance(),2}}, 201) {}

public:
    static SolarEnergyPlant &instance() {
        static SolarEnergyPlant INSTANCE;
        return INSTANCE;
    }
};

class WindEnergyPlant : public Building {
private:
    WindEnergyPlant() : Building(1, 'I', "WindEnergyPlant",
                                 {{&Wood::instance(),2}, {&Plastic::instance(),1}, { &Metal::instance(),2}}, 333) {}

public:
    static WindEnergyPlant &instance() {
        static WindEnergyPlant INSTANCE;
        return INSTANCE;
    }
};

class Blueprint {
protected:
    int length;
    int width;
    Building ***buildings;
public:
    Blueprint(int length, int width) : length(length), width(width) {
        buildings = new Building **[width];
        for (int row = 0; row < width; row++) {
            buildings[row] = new Building *[length];
            for (int col = 0; col < length; col++) {
                buildings[row][col] = nullptr;
            }
        }
    }

    ~Blueprint() {
        for (int row = 0; row < width; row++) {
            delete buildings[row];
        }
        delete buildings;
    }

    // Is this your mentioned functor?
    bool operator()(Blueprint *blueprint) const {
        if (length != blueprint->length || width != blueprint->width){
            return false;
        }

        for(int row = 0; row < width; row++) {
            for(int col = 0; col < length; col++) {
                if (buildings[row][col]  != blueprint->buildings[row][col]) {
                    return false;
                }
            }
        }

        return true;
    }

    float getMetric() const {
        int capacity = 0;
        int totalCost = 0;
        int area = width * length;

        for(int row = 0; row < width; row++) {
            for (int col = 0; col < length; col++) {
                Building *building = buildings[row][col];
                if (building == nullptr) {
                    continue;
                }
                capacity += building->getCapacity();
                totalCost += building->getTotalCosts();
            }
        }

        if (totalCost * area == 0) {
            return 0;
        }

        return float(capacity) / (float)(totalCost * area);
    }

    bool placeBuilding(Building *building, int x, int y, int length, int width) {
        for (int row = y; row < y + width; row++) {
            for (int col = x; col < x + length; col++) {
                if (buildings[row][col] != nullptr) {
                    return false;
                }
            }
        }

        for (int row = y; row < y + width; row++) {
            for (int col = x; col < x + length; col++) {
                buildings[row][col] = building;
            }
        }
        return true;
    }

    bool isInBounds(int x, int y, int length, int width) const {

        return 0 <= x && 0 <= x + length && x + length <= this->length
               && 0 <= y && 0 <= y + width && y + width <= this->width;

    }

    void deleteArea(int x, int y, int length, int width) {
        for (int row = y; row < y + width; row++) {
            for (int col = x; col < x + length; col++) {
                buildings[row][col] = nullptr;
            }
        }
    }

    Building ***getBuildings() const {
        return buildings;
    }

    std::map<Building*, int> countBuildings() const {
        std::map<Building *, int> buildingCount;
        for (int row = 0; row < width; row++) {
            for (int col = 0; col < length; col++) {
                Building *building = buildings[row][col];
                if (building == nullptr) {
                    continue;
                }

                if (buildingCount.contains(building)) {
                    buildingCount[building] += 1;
                } else {
                    buildingCount.insert({building, 1});
                }
            }
        }
        return buildingCount;
    }
};

class CapyCitySim {
protected:
    int length;
    int width;
    std::vector<Blueprint*> blueprints;
    Blueprint* blueprint;
public:
    CapyCitySim(int length, int width) : length(length), width(width) {
        blueprint = new Blueprint(length, width);
    }

    ~CapyCitySim() {
        delete blueprint;
        for(auto blueprint : blueprints) {
            delete blueprint;
        }
    }

    void loop() {
        std::cout << "Welcome to Capycity, please select an option: " << std::endl;

        while (true) {
            std::cout << "1) Place Building" << std::endl;
            std::cout << "2) Delete Area" << std::endl;
            std::cout << "3) Show current construction plan" << std::endl;
            std::cout << "4) Add new construction plan" << std::endl;
            std::cout << "5) Quit" << std::endl;
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
                    std::sort(blueprints.begin(), blueprints.end(), [](Blueprint* a, Blueprint* b) { return a->getMetric() > a->getMetric(); });
                    std::cout << "Saved plans:" << std::endl;
                    for(auto blueprint : blueprints) {
                        showConstructionPlan(blueprint);
                    }

                    std::cout << "Current plan:" << std::endl;
                    showConstructionPlan(blueprint);
                    system("pause");
                    break;
                case '4':
                    createBlueprint();
                    break;
                case '5':
                    return;
                default:
                    std::cout << "Not a valid option" << std::endl;
            }
        }
    }

    void createBlueprint() {
        for(auto blueprint : blueprints) {
            if (this->blueprint->operator()(blueprint)) {
                std::cout << "This Plan already exists." << std::endl;
                std::cout << "Drop current plan and create new? (y/N): ";
                char input;
                std::cin >> input;
                if (tolower(input) == 'y') {
                    blueprint = new Blueprint(length, width);
                }
                return;
            }
        }
        blueprints.push_back(blueprint);
        blueprint = new Blueprint(length, width);
    }

    void showConstructionPlan(Blueprint* blueprint) {
        Building *** buildings = blueprint->getBuildings();
        std::map<Building *, int> buildingCount = blueprint->countBuildings();
        int buildingsCounter = 0;
        for (int row = 0; row < width; row++) {
            for (int col = 0; col < length; col++) {
                Building *building = buildings[row][col];
                if (building == nullptr) {
                    std::cout << "~";
                    continue;
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
            for (auto materialIter: building->getMaterials()) {
                std::cout << "\t" << materialIter.second << "x " << materialIter.first->getName() << " $" << materialIter.first->getPrice() << std::endl;
            }
        }
        std::cout << std::endl;
        for(auto count : buildingCount) {
            Building *building = count.first;
            int counter = count.second;
            std::cout << counter << "x " << building->getName() << " for $" <<  (counter * building->getTotalCosts()) << std::endl;
        }

        std::cout << "Total plan costs $" << totalCosts << std::endl;
        std::cout << "Planmetric " << blueprint->getMetric() << std::endl;

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

        if (!blueprint->isInBounds(x, y, length, width)) {
            std::cout << "Input is outside the construction area." << std::endl;
            system("pause");
            return;
        }

        bool placed = blueprint->placeBuilding(selectedBuilding, x, y, length, width);
        if (!placed) {
            std::cout << "Space is already occupied!" << std::endl;
            system("pause");
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

        if (!blueprint->isInBounds(x, y, length, width)) {
            std::cout << "Input is outside the construction area." << std::endl;
            system("pause");
            return;
        }

        blueprint->deleteArea(x,y,length, width);
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