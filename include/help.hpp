#ifndef HELP_HPP
#define HELP_HPP

#include <iostream>

// função pra mostrar o menu de ajuda no terminal
inline void print_help() {
    std::cout << "-- Welcome to passgen v 1.0, \u00A9 DIMAp --" << std::endl;
    std::cout << "This program generates passwords of specific length based on the selection of groups of characters.\n" << std::endl;

    std::cout << "Usage: passgen [<options>]\n" << std::endl;

    std::cout << "--len n            Size of the password (default is 7)." << std::endl;
    std::cout << "-l, --lower        Add random characters from [a-z]." << std::endl;
    std::cout << "-u, --upper        Add random characters from [A-Z]." << std::endl;
    std::cout << "-d, --digits       Add random characters from [0-9]." << std::endl;
    std::cout << "-o, --logograms    Add random characters from [#$%&@^~]." << std::endl;
    std::cout << "-p, --punctuation  Add random characters from [.,;:]." << std::endl;
    std::cout << "-q, --quotes       Add random characters from [\"']." << std::endl;
    std::cout << "-s, --slashes      Add random characters from [\\\\/|_-]." << std::endl;
    std::cout << "-m, --math         Add random characters from [<>*+!?=]." << std::endl;
    std::cout << "-b, --braces       Add random characters from [{}[]()]." << std::endl;
    std::cout << "-a, --all-groups   Add random characters from all the above groups." << std::endl;
    std::cout << "-v, --verbose      Prints extra information besides the password." << std::endl;
    std::cout << "-h, --help         Show this help screen." << std::endl;
}

#endif