#include <iostream>


void func2() {
    int *p = nullptr;
    *p = 3;
}


void func1() {
    func2();
}

int main(int argc, char** argv) {
    func1();
    return 0;
}