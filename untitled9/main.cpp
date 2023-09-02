#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

int money[1000];
int sum;

//функция сортировки банкомата
void sort(){
    for (int i = 0 ; i <1000; ++i) {
        int maxind = 0;
        for (int j = 0; j <= i; ++j) {
            if (money[j] < money[maxind])
                maxind = j;
        }
        int temp = money[i];
        money[i] = money[maxind];
        money[maxind] = temp;
    }
}

//функция "заполнения банкомата"

void filling () {
    std::srand(std::time(nullptr));
    int check;
    for (int i=0; i<1000; ++i) {
        check = 100 + std::rand() % 5001;
        if (check - 100 == 0 || check - 200 == 0 || check - 500 == 0 || check - 1000 == 0 || check - 2000 == 0 ||
            check - 5000 == 0) {
            money[i] = check;
            sort();
        } else
            --i;
    }
}



int main() {
    char symbol;
    std::cout << "Enter + to top up the ATM, - to withdraw cash." << std::endl;
    std::cin >> symbol;
    if (symbol == '+') {
        std::ofstream bankIn(("..\\bank.bin"), std::ios::binary | std::ios::out);
        if (bankIn.is_open() == 0) {
            std::cout << "Path no valid!";
            return 0;
        }
        filling();

        for (int i = 0; i < 1000; ++i) {
            bankIn.write((char *) &money[i], sizeof(money[i]));
        }
        bankIn.close();
    } else {
        if (symbol == '-') {
            std::ifstream bankOut(("..\\bank.bin"), std::ios::binary | std::ios::in);

            std::cout << "Enter the amount of money, a multiple of 100." << std::endl;
            std::cin >> sum;
            if (sum % 100 != 0) {
                std::cout << "Err";
                return 0;
            } else {
                std::ofstream receipt("..\\bill.txt", std::ios::out);

                if (receipt.is_open() == 0) {
                    std::cout << "Path no valid!";
                    return 0;
                }
                for (int i = 0; i < 1000; ++i) {
                    bankOut.read((char *) &money[i], sizeof(money[i]));
                    if (sum - money[i] >= 0) {
                        if (money[i] != 0) {
                            receipt << money[i] << " ";
                        }
                        sum -= money[i];
                        money[i] = 0;
                    }
                }
                bankOut.close();
            }
            if (sum == 0) {
                std::cout << "The operation was completed successfully. Your bills are in the bill.txt file."
                          << std::endl;
            } else
                std::cout << "Error, there are no suitable bills.";
        }
        std::ofstream bankIn(("..\\bank.bin"), std::ios::binary | std::ios::out);
        if (bankIn.is_open() == 0) {
            std::cout << "Path no valid!";
            return 0;
        }
        for (int i = 0; i < 1000; ++i) {
            bankIn.write((char *) &money[i], sizeof(money[i]));
        }
        bankIn.close();
    }
}
