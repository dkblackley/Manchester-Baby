/*
This class is used simply to call either the baby or the assembler
*/
#include "connecter.h"
#include <iostream>

  using namespace std;

int main(int argc, char const *argv[]) {

  bool running = true;

  while (running) {

    cout << "Enter 1 to run the Macnhester baby" << '\n';
    cout << "Enter 2 to run the assembler" << '\n';
    cout << "Enter 0 to quit \n" << '\n';

    int input;

    cin >> input;

    if(cin.fail()) {
      cin.clear();
      cin.ignore();
      cout << "Please input an Integer.";
    } else {

      switch (input) {
        case 0:
          running = false;
          cout << "Quitting Program....." << '\n';
          break;
        case 1:
          babyMenu();
          break;
        case 2:
          assemblerMenu();
        default:
          cout << "Number not within range" << '\n';
          break;
        }
      }
    }
}
