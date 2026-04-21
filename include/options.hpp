#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <string>

// grupos de caracteres pro gerador
enum class CharacterGroup {
    Lower,
    Upper,
    Digits,
    Logograms,
    Punctuation,
    Quotes,
    Slashes,
    Math,
    Braces
};

struct RunningOptions {
    int length = 7;

    // flags de tipos de caracteres
    bool use_lower = false;
    bool use_upper = false;
    bool use_digits = false;
    bool use_logograms = false;
    bool use_punctuation = false;
    bool use_quotes = false;
    bool use_slashes = false;
    bool use_math = false;
    bool use_braces = false;

    bool use_all_groups = false;
    bool verbose = false;
    bool show_help = false;

    // sistema de erro 
    bool has_error = false;
    std::string error_message = ""; 
};

#endif