/**
 * @file main.cpp
 * @brief Entry point for the passgen password generator.
 *
 * Compile: g++ -std=c++17 -Wall -o passgen main.cpp password.cpp
 * Run:     ./passgen [options]
 *
 * @author < Heitor Gomes de Sena Melo Mariz | marizheitor@gmail.com
 *           Guilherme Soares Fialho Ribeiro Dantas | guilhermefialho1208@gmail.com >
 * 
 * @date   21/04/2026
 */

#include <iostream>
#include <cstdlib>
#include <cmath>

#include "password.h"

/**
 * @brief Entry point. Orchestrates argument parsing, password generation,
 *        and optional quality report.
 */
int main(int argc, char* argv[]) {
  RunningOptions run_options;

  // (1) Parse and validate CLI arguments.
  validate_arguments(argc, argv, run_options);

  // (2) Generate password.
  auto password = generate_password(run_options);

  // (3) Print result — verbose shows quality info, plain shows only password.
  if (run_options.verbose) {
    std::cout << "Welcome to passgen v 1.0, © DIMAp 2026\n\n";

    double entropy    { 0.0 };
    size_t variability{ 0   };
    std::string quality = password_quality(password, entropy, variability);

    std::cout << ">>> The generated password is:\n" << password << "\n\n";

    std::cout << ">>> Variability (distinct characters in the password) is: "
              << variability << "\n"
              << "    Password quality: " << quality << ".\n"
              << "    Entropy: " << static_cast<int>(std::round(entropy))
              << " bits.\n\n";
  } else {
    std::cout << password << "\n";
  }

  return EXIT_SUCCESS;
}
