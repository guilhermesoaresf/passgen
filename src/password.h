/**
 * @file password.h
 * @brief Declarations for the passgen password generator.
 *
 * Contains the RunningOptions struct and the signatures of all functions
 * used to parse arguments, generate passwords, and evaluate their quality.
 *
 * @author < Heitor Gomes de Sena Melo Mariz | marizheitor@gmail.com
 *           Guilherme Soares Fialho Ribeiro Dantas | guilhermefialho1208@gmail.com >
 * @date   21/04/2026
 */

#ifndef PASSWORD_H
#define PASSWORD_H

#include <string>
#include <string_view>
#include <random>

//== Default password length when --len is not specified.
constexpr size_t default_pass_length{ 7 };

/**
 * @brief Stores all options parsed from the command-line interface.
 */
struct RunningOptions {
  size_t pass_length{ default_pass_length }; ///< Desired password length.

  bool lower       { false }; ///< Include lowercase letters [a-z].
  bool upper       { false }; ///< Include uppercase letters [A-Z].
  bool digits      { false }; ///< Include digits [0-9].
  bool logograms   { false }; ///< Include logograms [#$%&@^`~].
  bool punctuation { false }; ///< Include punctuation [.,;:].
  bool quotes      { false }; ///< Include quotes ["'].
  bool slashes     { false }; ///< Include slashes [\/|_-].
  bool math        { false }; ///< Include math symbols [<>*+!?=].
  bool braces      { false }; ///< Include braces [{}[]()].
  bool verbose     { false }; ///< Print extra information (quality/entropy).
  bool force       { false }; ///< Force the requested password length.

  /// @brief Returns true if at least one character group is selected.
  bool any_true_group() const {
    return lower or upper or digits or logograms or punctuation or quotes
        or slashes or math or braces;
  }

  /// @brief Returns the number of active character groups.
  size_t active_groups() const {
    return (size_t)lower + (size_t)upper + (size_t)digits + (size_t)logograms
         + (size_t)punctuation + (size_t)quotes + (size_t)slashes
         + (size_t)math + (size_t)braces;
  }
};

/**
 * @brief Prints an optional error message followed by the help screen,
 *        then exits with EXIT_FAILURE.
 * @param msg  Error description (empty = plain help request).
 */
void usage(std::string_view msg = "");

/**
 * @brief Parses and validates all command-line arguments, filling
 *        @p run_options accordingly.
 * @param argc        Argument count (from main).
 * @param argv        Argument vector (from main).
 * @param run_options Output struct to be populated.
 */
void validate_arguments(int argc, char* argv[], RunningOptions& run_options);

/**
 * @brief Picks one random character from @p group, appends it to @p pass,
 *        and adds all remaining characters of the group to @p pool,
 *        ensuring the chosen character won't repeat until the pool resets.
 * @param group  Source character set (modified in place).
 * @param pass   Password string being built.
 * @param gen    Mersenne-Twister PRNG.
 * @param pool   Shared pool of available characters for subsequent picks.
 */
void add_char(std::string& group, std::string& pass, std::mt19937& gen, std::string& pool);

/**
 * @brief Generates a password according to the options in @p run_options.
 * @param run_options  Parsed CLI options.
 * @return The generated password as a std::string.
 */
std::string generate_password(const RunningOptions& run_options);

/**
 * @brief Evaluates password quality using Shannon entropy: H = L * log2(R),
 *        where R is the number of distinct characters and L is the length.
 * @param password     The password to evaluate.
 * @param entropy      Output: computed entropy in bits.
 * @param variability  Output: number of distinct characters in the password.
 * @return Qualitative label as a std::string.
 */
std::string password_quality(std::string_view password, double& entropy, size_t& variability);

#endif // PASSWORD_H
