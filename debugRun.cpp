#include <iostream>
#include <unistd.h>

void func2() {
    int sum = 0;
    int i = 0;
    while(1) {
        ++i;
        sum += i;
        std::cout << "i is: " << i << ", sum is: " << sum << std::endl;
        sleep(1);
    }
}

void func1() {
    func2();
}

int main(int argc, char **argv) {
    func1();
}