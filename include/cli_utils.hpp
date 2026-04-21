#ifndef CLI_UTILS_HPP
#define CLI_UTILS_HPP

#include <string>
#include "options.hpp"

// ve se é uma opção longa tipo --help
inline bool is_long_option(const std::string& arg) {
    return arg.size() > 2 && arg[0] == '-' && arg[1] == '-';
}

// ve se é opção curta tipo -h
inline bool is_short_option(const std::string& arg) {
    return arg.size() > 1 && arg[0] == '-' && arg[1] != '-';
}

// liga todos os filtros de uma vez
inline void enable_all_groups(RunningOptions& options) {
    options.use_lower = true;
    options.use_upper = true;
    options.use_digits = true;
    options.use_logograms = true;
    options.use_punctuation = true;
    options.use_quotes = true;
    options.use_slashes = true;
    options.use_math = true;
    options.use_braces = true;
    options.use_all_groups = true;
}

inline bool set_group_from_long_option(const std::string& arg, RunningOptions& options) {
    if (arg == "--lower") options.use_lower = true;
    else if (arg == "--upper") options.use_upper = true;
    else if (arg == "--digits") options.use_digits = true;
    else if (arg == "--logograms") options.use_logograms = true;
    else if (arg == "--punctuation") options.use_punctuation = true;
    else if (arg == "--quotes") options.use_quotes = true;
    else if (arg == "--slashes") options.use_slashes = true;
    else if (arg == "--math") options.use_math = true;
    else if (arg == "--braces") options.use_braces = true;
    else if (arg == "--all-groups") enable_all_groups(options);
    else return false;

    return true;
}

// valida as letrinhas das flags
inline bool is_valid_short_flag(char option) {
    return option == 'l' || option == 'u' || option == 'd' ||
           option == 'o' || option == 'p' || option == 'q' ||
           option == 's' || option == 'm' || option == 'b' ||
           option == 'a' || option == 'v' || option == 'h';
}

inline bool apply_short_flag(char option, RunningOptions& options) {
    switch (option) {
        case 'l': options.use_lower = true; break;
        case 'u': options.use_upper = true; break;
        case 'd': options.use_digits = true; break;
        case 'o': options.use_logograms = true; break;
        case 'p': options.use_punctuation = true; break;
        case 'q': options.use_quotes = true; break;
        case 's': options.use_slashes = true; break;
        case 'm': options.use_math = true; break;
        case 'b': options.use_braces = true; break;
        case 'a': enable_all_groups(options); break;
        case 'v': options.verbose = true; break;
        case 'h': options.show_help = true; break;
        default: return false;
    }
    return true;
}

// checa se o usuario nao esqueceu de escolher pelo menos 1 grupo
inline bool no_group_selected(const RunningOptions& options) {
    return !options.use_lower && !options.use_upper && !options.use_digits &&
           !options.use_logograms && !options.use_punctuation && !options.use_quotes &&
           !options.use_slashes && !options.use_math && !options.use_braces;
}

#endif