// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "Signal.h"

class Animal {
public:
    virtual void makeSound() = 0;
};

class Dog : Animal {
public:
    void makeSound() override {
        std::cout << "Woof!\n";
    }
};

class Cat : Animal {
public:
    void makeSound() override {
        std::cout << "Meow!\n";
    }
};

void goodbye() {
    std::cout << "Goodbye World!\n";
}

int main()
{
    /*
    // the next few lines are examples on how to use the Signal class
    */
    
    Dog pet1;
    Cat pet2;

    // declare signal
    Signal<void> mySignal;

    // connect functions to the signal
    mySignal.connect([&pet1] { pet1.makeSound(); }); // capture-lambda
    mySignal.connect(&pet2, &Cat::makeSound); // member overload
    mySignal.connect(&goodbye); // free function

    // emit the signal, calling all connected functions
    mySignal.emit();

    return 0;
}