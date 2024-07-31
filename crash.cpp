#include <iostream>
#include <fstream>
#include <cstdlib>
#include <direct.h>
#include <ctime>
#include <filesystem>
#include <Windows.h>
#include <string>
#include <iomanip>
#include <random>
#include <chrono>
#include <thread>
#include <conio.h>
#include <sstream>
#include <vector>
#include <numeric>
#include <algorithm>
using namespace std;
namespace fs = std::filesystem;

struct comma_facet : public numpunct<char> {
protected:
    virtual char do_thousands_sep() const {
        return ',';
    }

    virtual string do_grouping() const {
        return "\3";
    }
};

string formatWithCommas(int value) {
    stringstream ss;
    ss.imbue(locale(ss.getloc(), new comma_facet));
    ss << fixed << value;
    return ss.str();
}

string getDesktopPath() {
    const char* homeDrive = getenv("HOMEDRIVE");
    const char* homePath = getenv("HOMEPATH");
    if (homeDrive && homePath) {
        string desktopPath = string(homeDrive) + string(homePath) + "\\Desktop\\money.txt";
        return desktopPath;
    }
    return "";
}

void saveMoney(int money, const string& fileName) {

    if (remove(fileName.c_str()) != 0) {
        cerr << "Error deleting file." << endl;
    }

    ofstream file(fileName);
    if (file.is_open()) {
        file << money;
        file.close();

        string command = "attrib +h \"" + fileName + "\"";
        system(command.c_str());
        system("cls");
        system("color 2");
        cout << "Postup byl upesne ulozen !";
        Sleep(160);
        system("cls");
    } else {
        cerr << "Error opening file for writing." << endl;
    }
}

int loadMoney(const string& fileName) {
    ifstream file(fileName);
    int money = 0;
    if (file.is_open()) {
        file >> money;
        file.close();
    } else {
        cerr << "Error opening file for reading." << endl;
    }
    return money;
}

bool fileExists(const string& fileName) {
    return fs::exists(fileName);
}

void error1() {
	system("cls");
    system("color 4");
    cout << "Neplatna hodnota !\n";
    Sleep(800);
    system("color e");
    system("cls");
}

double generateCrashTime(std::mt19937& gen, const std::vector<double>& values, const std::vector<double>& weights) {
    std::discrete_distribution<> distr(weights.begin(), weights.end());
    return values[distr(gen)];
}

void drawGraph(const vector<double>& multipliers) {
    int height = 10; // Výška grafu
    double maxMultiplier = *max_element(multipliers.begin(), multipliers.end());

    for (int i = height; i > 0; --i) {
        for (double multiplier : multipliers) {
            if (multiplier >= (maxMultiplier / height) * i) {
                cout << "|";
            } else {
                cout << " ";
            }
        }
        cout << endl;
    }

    // Osy X
    for (size_t i = 0; i < multipliers.size(); ++i) {
        cout << "-";
    }
    cout << endl;
}

void setConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

int main() {
    system("color e");
    bool reset = true;
    int money; int sazka;
    string fileName = getDesktopPath();
    if (fileExists(fileName)) {
        money = loadMoney(fileName);
		system("color 2");
		std::cout << "Postup byl uspesne obnoven !";
		Sleep(500);
    } else {
        money = 6000;
    }
    string formattedMoney = formatWithCommas(money);
	string formattedWin = formatWithCommas(money);
    while (reset) {
        reset = false;
        system("cls");
        srand(time(nullptr));
        bool sazeni = true;
        
        while (sazeni) {
            sazeni = false;
            system("color e");
            system("cls");
            cout << "Vas aktualni zustatek: " << formattedMoney;
            cout << "\nZadejte vasi sazku: ";
            cin >> sazka;
            if (sazka > money || sazka < 1) {
			    error1();        
                sazeni = true;
            }
        }
        money = money - sazka;

        //GENERATION BEGIN--
        std::random_device rd;
        std::mt19937 gen(rd());

        std::vector<double> values = {
            1.00, 2.00, 3.00, 4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 10.00,
            12.00, 14.00, 16.00, 18.00, 20.00, 25.00, 30.00,
            30.50, 31.00, 32.00, 33.00, 34.00, 35.00
        };

        std::vector<double> weights = {
            0.20, 0.15, 0.12, 0.10, 0.08, 0.07, 0.06, 0.05, 0.04, 0.03,
            0.025, 0.02, 0.015, 0.015, 0.015, 0.01, 0.007,
            0.006, 0.005, 0.004, 0.003, 0.002, 0.001
        };

        double crashTime = generateCrashTime(gen, values, weights);

        //END--

        double multiplier = 1.0;
        bool hasCrashed = false;
        bool cashedOut = false;
        system("cls");

        vector<double> multipliers;
        while (!hasCrashed && !cashedOut) {

            if (multiplier < 2) {
                this_thread::sleep_for(chrono::milliseconds(350));
            } else if (multiplier < 4.5) {
                this_thread::sleep_for(chrono::milliseconds(230));
            } else if (multiplier < 12) {
                this_thread::sleep_for(chrono::milliseconds(150));
            } else {
                this_thread::sleep_for(chrono::milliseconds(120));
            }
            //
            
            multiplier += 0.1;
            multipliers.push_back(multiplier);
            system("cls");
            drawGraph(multipliers);
            //
            cout << "\rNasobic: " << multiplier << "x   \n" << flush;
            cout << "Stisknete enter pro vybrani penez." << endl;

            if (_kbhit()) {
                char ch = _getch();
                if (ch == 13) {
                    system("cls");
                    cashedOut = true;
                    double winnings = sazka * multiplier;
                    formattedWin = formatWithCommas(winnings);
                    setConsoleColor(2);
                    drawGraph(multipliers);
                    setConsoleColor(7);
                    cout << "Maximalni mozna vyhra byla: " << crashTime << "x" << endl;
                    cout << "Penize jste vyzvedli na nasobici " << multiplier << "x! Vyhravate: " << formattedWin << endl;
                    money += winnings;
                    formattedMoney = formatWithCommas(money);
                    cout << "Vas aktualni zustatek je: " << formattedMoney << endl;
                    cout <<"\n";
                    system("pause");
                    system("cls");
                    saveMoney(money, fileName);
                    multiplier = crashTime - 1;
                }
            }

            if (multiplier >= crashTime) {
                hasCrashed = true;
                system("cls");
                setConsoleColor(4);
                drawGraph(multipliers);
                setConsoleColor(7);
                cout << "Crash na nasobici " << multiplier << "x. Prohra." << endl;
                if (money < 6000) { money = 6000; }
                formattedMoney = formatWithCommas(money);
                cout << "Vas aktualni zustatek je: " << formattedMoney << endl;
                cout <<"\n";
                system("pause");
                system("cls");
                saveMoney(money, fileName);
            }
        }
        reset = true;
    }
    return 0;
}
