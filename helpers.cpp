#include <iostream>
#include <string>






void die(std::string functionName) {
    std::cerr<< "killing " + functionName;
    exit(EXIT_FAILURE);
    
}
