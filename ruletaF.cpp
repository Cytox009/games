#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <random>
#include <chrono>
#include <thread>
#include <locale>
#include <algorithm>
#include <vector>
#include <numeric>
#include <filesystem>
#include <shlobj.h>
#include <windows.h>
#include <utility>
#include <sstream>
#include <tuple>

namespace fs = std::filesystem;

struct comma_facet : public std::numpunct<char> {
protected:
    virtual char do_thousands_sep() const override { return ','; }
    virtual std::string do_grouping() const override { return "\3"; }
};

std::string formatWithCommas(int value) {
    std::stringstream ss;
    ss.imbue(std::locale(ss.getloc(), new comma_facet));
    ss << std::fixed << value;
    return ss.str();
}

std::string getDocumentsPath() {
    char path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, 0, path))) {
        return std::string(path);
    } else {
        std::cerr << "Error getting documents path." << std::endl;
        return "";
    }
}

// Check if a file exists
bool fileExists(const std::string& fileName) {
    std::ifstream file(fileName);
    return file.good();
}

// Save money to a file
void saveMoney(int money, const std::string& fileName) {
    std::string documentsPath = getDocumentsPath();
    if (documentsPath.empty()) {
        std::cerr << "Failed to get the Documents path." << std::endl;
        return;
    }

    std::string fullPath = documentsPath + "\\" + fileName;

    if (remove(fullPath.c_str()) != 0) {
        std::cerr << "Error deleting file." << std::endl;
    }

    std::ofstream file(fullPath);
    if (file.is_open()) {
        file << money;
        file.close();
        std::cout << "File opened and written successfully." << std::endl;

        std::string command = "attrib +h \"" + fullPath + "\"";
        if (system(command.c_str()) == 0) {
            std::cout << "File set as hidden." << std::endl;
        } else {
            std::cerr << "Failed to set file as hidden." << std::endl;
        }

        system("cls");
        system("color 2");
        std::cout << "Progress saved successfully!";
        Sleep(160);
        system("cls");
    } else {
        std::cerr << "Error opening file for writing." << std::endl;
    }
}

// Load money from a file
int loadMoney(const std::string& fileName) {
    std::string documentsPath = getDocumentsPath();
    if (documentsPath.empty()) {
        std::cerr << "Failed to get the Documents path." << std::endl;
        return 0;
    }

    std::string fullPath = documentsPath + "\\" + fileName;

    std::ifstream file(fullPath);
    int money = 0;
    if (file.is_open()) {
        file >> money;
        file.close();
    } else {
        std::cerr << "Error opening file for reading." << std::endl;
    }
    return money;
}

// Recover money at the start of the game
int recoverMoneyAtStart() {
    int money;
    std::string file = getDocumentsPath() + "\\money.txt";
    if (fileExists(file)) {
        money = loadMoney("money.txt");
        system("color 2");
        std::cout << "Progress recovered successfully!";
        Sleep(500);
        system("cls");
    } else {
        money = 200000;
    }
    return money;
}

void displayError() {
    system("cls");
    system("color 4");
    std::cout << "Neplatna hodnota!\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    system("cls");
    system("color e");
}

void setConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

std::tuple<int, int, int, bool> displayBetOptions() {
    int betN1 = -3, betN2 = -3;
    int choice = 0;
    int type = 0;
    bool clearConsole = false;
    bool repeat = true;
    while (repeat) {
        repeat = false;
        std::cout << "Zadejte moznost: \n";
        std::cout << "[1] Pro barvu\n";
        std::cout << "[2] Pro cislo\n";
        std::cout << "[3] Pro oboji\n";
        std::cout << "\n[4] Pro nastaveni\n";
        std::cout << "[5] Pro ukonceni programu\n";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                type = 1;
                bool repeatColor = true;
                while (repeatColor) {
                    repeatColor = false;
                    system("cls");
                    std::cout << "Zadejte barvu (cervena(1)/cerna(2)): ";
                    std::cin >> betN1;
                    if (betN1 != 1 && betN1 != 2) {                  
                        repeatColor = true;
                        displayError();
                    } else {
                        return std::make_tuple(betN1, betN2, type, clearConsole);
                    }
                }
                break;
            }
            case 2: {
                type = 2;
                bool repeatNumber = true;
                while (repeatNumber) {
                    repeatNumber = false;
                    system("cls");
                    std::cout << "Zadejte cislo (0-36): ";
                    std::cin >> betN2;
                    if (betN2 > 36 || betN2 < 0) {
                        repeatNumber = true;
                        displayError();
                    } else {
                        return std::make_tuple(betN1, betN2, type, clearConsole);
                    }
                }
                break;
            }
            case 3: {
                type = 3;
                bool repeat31 = true;
                while (repeat31) {
                    repeat31 = false;
                    system("cls");
                    std::cout << "Zadejte barvu (cervena(1)/cerna(2)): ";
                    std::cin >> betN1;
                    if (betN1 != 1 && betN1 != 2) {                  
                        repeat31 = true;
                        displayError();
                    }                    
                }
                bool repeat32 = true;
                while (repeat32) {
                    std::cout << "Zadejte cislo (0-36): ";
                    std::cin >> betN2;
                    if (betN2 > 36 || betN2 < 0) {
                        system("cls");
                        repeat32 = true;
                        displayError();
                    } else {
                        return std::make_tuple(betN1, betN2, type, clearConsole);
                    }
                }                    
            break;
            }
            case 5: {
                type = 4;
                return std::make_tuple(betN1, betN2, type, clearConsole);
            }
            case 4: {
                type = 5;
                int choice5;
                system("cls");
                std::cout << "[1] Pro nastaveni cisteni konzole\n";
                std::cin >> choice5;
                if (choice5 == 1) {
                    std::string AN1;
                    system("cls");
                    std::cout << "Chcete mazat konzoli pri vypisovani cisel ? [A/N]\n";
                    std::cin >> AN1;
                    if (AN1 == "a" || AN1 == "A") {
                        system("cls");
                        system("color 2");
                        std::cout << "Hodnota byla nastavena na ""True""";
                        std::this_thread::sleep_for(std::chrono::milliseconds(450));
                        clearConsole = true;
                        system("cls");
                        system("color e");                    
                    } else if (AN1 == "n" || AN1 == "N") {
                        system("cls");
                        system("color 2");
                        std::cout << "Hodnota byla nastavena na ""False""";
                        std::this_thread::sleep_for(std::chrono::milliseconds(450));
                        clearConsole = false;
                        system("cls");
                        system("color e");
                    } else {
                        repeat = true;
                        displayError();
                    }
                }
                repeat = true;
                break;
            }
            default: {
                repeat = true;
                displayError();
                break;
            }
        }
    }
    return std::make_tuple(betN1, betN2, type, clearConsole);
}

std::pair<int, int> displayBetAmount(int money, int type, int displaybetN1, int displaybetN2) {
    bool repeat = true;
    
    bool repeat32 = true;
    int betAmount;
    int sazka1 = 0; int sazka2 = 0;
    while (repeat) {
        repeat = false;
        std::string formattedMoney = formatWithCommas(money);
       if (type == 1) {
            bool repeat11 = true;
            while (repeat11) {
                repeat11 = false;
                std::cout << "Vas aktualni zustatek je: " << formattedMoney << std::endl;
                std::cout << "Zadejte castku, kterou chcete vsadit na barvu: ";
                std::cin >> sazka1;
                if (sazka1 > money || sazka1 < 0) {
                    repeat11 = true;
                    displayError();
                } else {
                    return std::make_pair(sazka1, sazka2);
                }
            }
        }
       if (type == 2) {
            bool repeat21 = true;
            while (repeat21) {
                repeat21 = false;
                std::cout << "Vas aktualni zustatek je: " << formattedMoney << std::endl;
                std::cout << "Zadejte castku, kterou chcete vsadit na " << displaybetN2 << ": ";
                std::cin >> sazka2;
                if (sazka2 > money || sazka2 < 0) {
                    repeat21 = true;
                    displayError();
                } else {
                    return std::make_pair(sazka1, sazka2);
                }
            }
        }
       if (type == 3) {
            bool repeat31 = true;
            while (repeat31) {
                repeat31 = false;
                formattedMoney = formatWithCommas(money);
                std::cout << "Vas aktualni zustatek je: " << formattedMoney << std::endl;
                std::cout << "Zadejte castku, kterou chcete vsadit na barvu: ";
                std::cin >> sazka1;
                if (sazka1 > money || sazka1 < 0) {
                    repeat31 = true;
                    displayError();
                }
            }
            bool repeat32 = true;
            while (repeat32) {
                repeat32 = false;
                money -= sazka1;
                system("cls");
                formattedMoney = formatWithCommas(money);
                std::cout << "Vas aktualni zustatek je: " << formattedMoney << std::endl;
                std::cout << "Zadejte castku, kterou chcete vsadit na " << displaybetN2 << ": ";
                std::cin >> sazka2;
                if (sazka2 > money || sazka2 < 0) {
                    repeat32 = true;
                    displayError();
                } else {
                    return std::make_pair(sazka1, sazka2);
                }
            }
        }
    }
    return std::make_pair(sazka1, sazka2);
}

void displayLoseMessage(int money, int lastNumber) {
    system("cls");
    system("color e");
    std::string color;
    if (lastNumber == 0) {
        color = "Zelena";
    } else if (lastNumber % 2 == 0) {
        color = "Cerna";
    } else {
        color = "Cervena";
    }
    std::cout << "Prohra. Padlo cislo: " << lastNumber << " (" << color << ")\n";
    if (money < 6000) {
        money = 6000;
    }
    std::string formattedMoney = formatWithCommas(money);
    std::cout << "Vas aktualni zustatek je: " << formattedMoney << "\n";
    system("pause");
}

int displayWinMessage(int money, int bet1, int bet2, int lastNumber) {
    system("cls");
    system("color e");
    std::string color;
    if (lastNumber == 0) {
        color = "Zelena";
    } else if (lastNumber % 2 == 0) {
        color = "Cerna";
    } else {
        color = "Cervena";
    }
    int winnings1 = bet1 * 2;
    int winnings2 = bet2 * 14;
    int totalWinnings = winnings1 + winnings2;
    money += totalWinnings;
    std::string formattedMoney = formatWithCommas(money);
    std::string formattedWin = formatWithCommas(totalWinnings);
    std::cout << "===========================================\n";
    std::cout << "                   VYHRA\n";
    std::cout << "===========================================\n";
    std::cout << "Padlo cislo: " << lastNumber << " (" << color << ")\n";
    std::cout << "Gratuluji, vyhrali jste: " << formattedWin << "!\n";
    std::cout << "Vas aktualni zustatek je: " << formattedMoney << "\n";
    system("pause");
    return money;
}

int generateRandomValue(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(min, max);
    return distr(gen);
}

int main() {
    system("color e");
    bool reset = true;
    int money = recoverMoneyAtStart();
    int startingNumber = generateRandomValue(0, 36);

    while (reset) {
        reset = false;
        system("cls");
        system("color e");

        int colorChoice = 0, betNumber = 0, sazka1 = 0, sazka2 = 0, money2 = money, lastColor = 0, type = 0, betN1 = 0, betN2 = 0;
        bool clearConsole = false;
        
        std::tuple<int, int, int, bool> betNs = displayBetOptions();
        betN1 = std::get<0>(betNs);
        betN2 = std::get<1>(betNs);
        type = std::get<2>(betNs);
        clearConsole = std::get<3>(betNs);
        int displayBetN1 = betN1;
        int displayBetN2 = betN2;
        if (type == 4) return 0;
        system("cls");
        std::pair<int, int> sazky = displayBetAmount(money, type, displayBetN1, displayBetN2);
        sazka1 = sazky.first;
        sazka2 = sazky.second;

        money -= (sazka1 + sazka2);
        system("cls");
        // Game start
        int lastNumber = 0, spins = 0, spinDuration = generateRandomValue(46, 56);
        for (int i = startingNumber; spinDuration != 0; i++, spinDuration--, spins++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(120));
            if (clearConsole == true && spins > 25) {
                spins = 0;
                system("cls");
            }
            if (i > 36) i = 0;
            if (i == 0) {
                setConsoleColor(2);
                std::cout << "0 ";
                lastNumber = 0;
            } else if (i % 2 == 0) {
                setConsoleColor(8);
                std::cout << i << " ";
                lastNumber = i;
                lastColor = 2;
            } else {
                setConsoleColor(4);
                std::cout << i << " ";
                lastNumber = i;
                lastColor = 1;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        if (betN1 == lastColor && betN2 == lastNumber) {
            money = displayWinMessage(money, sazka1, sazka2, lastNumber);
        } else if (betN1 == lastColor && betN2 != lastNumber) {
            sazka2 = 0;
            money = displayWinMessage(money, sazka1, sazka2, lastNumber);
        } else if (betN2 == lastNumber && betN1 != lastColor) {
            sazka1 = 0;
            money = displayWinMessage(money, sazka1, sazka2, lastNumber);
        } else {
            displayLoseMessage(money, lastNumber);
        }

        startingNumber = lastNumber;
        saveMoney(money, "money.txt");
        reset = true;
    }

    return 0;
}
