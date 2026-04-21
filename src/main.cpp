#include <iostream>
#include "options.hpp"
#include "help.hpp"
#include "cli_utils.hpp"

int main() {
    RunningOptions options;

    
    std::cout << "Estrutura inicial do passgen criada com sucesso." << std::endl << std::endl;

    std::cout << "Valores iniciais das flags:" << std::endl;
    std::cout << "length = " << options.length << std::endl;
    
    
    std::cout << "verbose = " << (options.verbose ? "true" : "false") << std::endl;
    std::cout << "show_help = " << (options.show_help ? "true" : "false") << std::endl;

    std::cout << "\nTestando o print da tela de ajuda:" << std::endl;
    print_help();

    return 0;
}