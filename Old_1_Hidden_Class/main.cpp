using namespace std;

#include<iostream>
#include<stdlib.h>
#include"class1.hpp"
#include<stdexcept>

int main(int argc, char *argv[]) {
  int size = 256;
  if(argc>1) size = atoi(argv[1]);
  if(size < 1) size = 256;
  Class1 c(size);
  int noProblem = 1;
  while(noProblem) {
    enum { Int = 1, Float = 2, Double = 3 } choice;
    cout << "Choose a type: int 1, float 2, double 3 : ";
    cin >> (int&)choice;
    cout << endl;
    int index;
    switch(choice) {
      case Int : int intVar;
                 cout << "Give an int : ";
                 cin >> intVar;
                 cout << endl << "Give its index : ";
                 cin >> index;
                 try {
                  c.setInt(intVar, index);
                 }
                 catch (const exception& e) {
                  cout << "Exception: " << e.what() << endl;
                  noProblem = 0;
                 }
                 break;
      case Float : float floatVar;
                   cout << "Give a float : ";
                   cin >> floatVar;
                   cout << endl << "Give its index : ";
                   cin >> index;
                   try {
                    c.setFloat(floatVar, index);
                   }
                   catch (const exception& e) {
                    cout << "Exception: " << e.what() << endl;
                    noProblem = 0;
                   }
                   break;
      case Double : double doubleVar;
                   cout << "Give a double : ";
                   cin >> doubleVar;
                   cout << endl << "Give its index : ";
                   cin >> index;
                   try {
                    c.setDouble(doubleVar, index);
                   }
                   catch (const exception& e) {
                    cout << "Exception: " << e.what() << endl;
                    noProblem = 0;
                   }
                   break;
    }
  }
  noProblem = 1;
  int index;
  for(index = 0; noProblem; index++) {
    try {
      cout << "At " << index << " int : " << c.getInt(index) << endl;
    }
    catch(exception& e) {
      cout << "Exception : " << e.what() << endl;
      noProblem = 0;
    }
  }
  noProblem = 1;
  for(index = 0; noProblem; index++) {
    try {
      cout << "At " << index << " float : " << c.getFloat(index) << endl;
    }
    catch(exception& e) {
      cout << "Exception : " << e.what() << endl;
      noProblem = 0;
    }
  }
  noProblem = 1;
  for(index = 0; noProblem; index++) {
    try {
      cout << "At " << index << " double : " << c.getDouble(index) << endl;
    }
    catch(exception& e) {
      cout << "Exception : " << e.what() << endl;
      noProblem = 0;
    }
  }
  return 0;
}
