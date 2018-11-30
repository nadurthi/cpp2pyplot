// Example program
#include <iostream>
#include <string>
#include <sstream>
int main()
{
    int i=3;
    int *pt = &i;
std::stringstream input(std::stringstream::out|std::stringstream::binary);
input.write(reinterpret_cast<const char*>(pt),sizeof(int));
input.flush();
//input << i;
std::cout << input.str() << "\n";
std::cout << "ok " << *pt << "\n";
std::cout.write(reinterpret_cast<const char*>(&i),sizeof(i));
}