#include <iostream>

bool isInBounds(int totalLength, int totalWidth, int x, int y, int length, int width) {

    return 0 <= x && 0 <= x + length && x + length <= totalLength
           && 0 <= y && 0 <= y + width && y + width <= totalWidth;

}

int getInteger() {

    int result;
    do {
        std::cin >> result;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cerr << "Invalid input: must be a number" << std::endl;
        } else {
            return result;
        }
    } while (true);


}

enum FieldType : char {
    EMPTY = 'E',
    WATER_ENERGY_PLANT = 'W',
    SOLAR_ENERGY_PLANT = 'S',
    WIND_ENERGY_PLANT = 'I',
};

FieldType **createBlueprint(int length, int width) {

    FieldType **blueprint;
    blueprint = new FieldType *[width];
    for (int row = 0; row < width; row++) {
        blueprint[row] = new FieldType[length];
        for (int col = 0; col < length; col++) {
            blueprint[row][col] = FieldType::EMPTY;
        }
    }
    return blueprint;

}


FieldType selectFieldType() {

    do {
        std::cout << "W) Water energy plant" << std::endl;
        std::cout << "S) Solar energy plant" << std::endl;
        std::cout << "I) Wind energy plant" << std::endl;
        std::cout << "B) Back" << std::endl;

        std::cout << "Please select a building to place: ";

        char buildingSelected;
        std::cin >> buildingSelected;

        switch (toupper(buildingSelected)) {
            case 'W':
                return WATER_ENERGY_PLANT;
            case 'S':
                return SOLAR_ENERGY_PLANT;
            case 'I':
                return WIND_ENERGY_PLANT;
            case 'B':
                return EMPTY;
            default:
                std::cout << "Not a valid option" << std::endl;
        }
    } while (true);

}

void placeBuilding(FieldType **blueprint, int length, int width) {

    FieldType selectedFieldType = selectFieldType();

    std::cout << "Enter a start position X (0 - " << length - 1 << "): ";
    int x = getInteger();

    std::cout << "Enter a start position Y (0 - " << width - 1 << "): ";
    int y = getInteger();

    std::cout << "Enter the length of the Building: ";
    int buildingLength = getInteger();

    std::cout << "Enter the width of the Building: ";
    int buildingWidth = getInteger();

    if (!isInBounds(length, width, x, y, buildingLength, buildingWidth)) {
        std::cout << "Input is outside the construction area." << std::endl;
        return;
    }

    for (int row = y; row < y + buildingWidth; row++) {
        for (int col = x; col < std::min(length, x + buildingLength); col++) {
            if (blueprint[row][col] != EMPTY) {
                std::cout << "Space is already occupied!" << std::endl;
                return;
            }
        }
    }

    for (int row = y; row < y + buildingWidth; row++) {
        for (int col = x; col < std::min(length, x + buildingLength); col++) {
            blueprint[row][col] = selectedFieldType;
        }
    }
}

void deleteArea(FieldType **blueprint, int length, int width) {

    std::cout << "Enter a start position X (0 - " << length - 1 << "): ";
    int x = getInteger();

    std::cout << "Enter a start position Y (0 - " << width - 1 << "): ";
    int y = getInteger();

    std::cout << "Enter the length of the area to delete: ";
    int buildingLength = getInteger();

    std::cout << "Enter the width of the area to delete: ";
    int buildingWidth = getInteger();

    if (!isInBounds(length, width, x, y, buildingLength, buildingWidth)) {
        std::cout << "Input is outside the construction area." << std::endl;
        return;
    }

    for (int row = y; row < y + buildingWidth; row++) {
        for (int col = x; col < x + buildingLength; col++) {
            blueprint[row][col] = EMPTY;
        }
    }
}

void showConstructionPlan(FieldType **blueprint, int length, int width) {

    for (int row = 0; row < width; row++) {
        for (int col = 0; col < length; col++) {
            std::cout << blueprint[row][col];
        }
        std::cout << std::endl;
    }
    system("pause");

}

void capycity(int length, int width) {

    std::cout << "Welcome to Capycity, please select an option: " << std::endl;

    FieldType **blueprint = createBlueprint(length, width);

    while (true) {
        std::cout << "1) Place Building" << std::endl;
        std::cout << "2) Delete Area" << std::endl;
        std::cout << "3) Show current construction plan" << std::endl;
        std::cout << "4) Quit" << std::endl;
        std::cout << std::endl;

        std::cout << "Please select a number: ";
        char option;
        std::cin >> option;

        switch (option) {
            case '1':
                placeBuilding(blueprint, length, width);
                break;
            case '2':
                deleteArea(blueprint, length, width);
                break;
            case '3':
                showConstructionPlan(blueprint, length, width);
                break;
            case '4':
                return;
            default:
                std::cout << "Not a valid option" << std::endl;
        }
    }

}

int main(int argc, char **argv) {

    if (argc != 3) {
        std::cout << "Usage: " << std::endl;
        std::cout << "\tcapycity <length> <width>" << std::endl;
        return 1;
    }

    try {
        int length = std::stoi(argv[1]);
        int width = std::stoi(argv[2]);
        capycity(length, width);
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