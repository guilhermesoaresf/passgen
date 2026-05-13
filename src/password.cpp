/**
 * @file password.cpp
 * @brief Implementations for the passgen password generator.
 *
 * @author < Heitor Gomes de Sena Melo Mariz | marizheitor@gmail.com
 *           Guilherme Soares Fialho Ribeiro Dantas | guilhermefialho1208@gmail.com >
 * @date   21/04/2026
 */

#include "password.h"

#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <cmath>

/**
 * @brief Prints an optional error message followed by the help screen,
 *        then exits with EXIT_FAILURE.
 * @param msg  Error description (empty = plain help request).
 */
void usage(std::string_view msg) {
  // If an error message was provided, print it before the help screen.
  if (!msg.empty()) {
    std::cout << "Error: " << msg << "\n\n";
  }

  std::cout <<
    "This program generates passwords of specific length\n"
    "based on the selection of groups of characters.\n\n"
    "Usage: passgen [<options>]\n"
    "  --len n            Size of the password (default is 7).\n"
    "  -l, --lower        Add random characters from [a-z].\n"
    "  -u, --upper        Add random characters from [A-Z].\n"
    "  -d, --digits       Add random characters from [0-9].\n"
    "  -o, --logograms    Add random characters from [#$%&@^`~].\n"
    "  -p, --punctuation  Add random characters from [.,;:].\n"
    "  -q, --quotes       Add random characters from [\"'].\n"
    "  -s, --slashes      Add random characters from [\\/|_-].\n"
    "  -m, --math         Add random characters from [<>*+!?=].\n"
    "  -b, --braces       Add random characters from [{}[]()].\n"
    "  -a, --all-groups   Add random characters from all the above groups.\n"
    "  -v, --verbose      Prints extra information besides the password.\n"
    "  -h, --help         Show this help screen.\n"
    "  --force            Force the password size.\n";

  exit(EXIT_FAILURE);
}

/**
 * @brief Parses and validates all command-line arguments, filling
 *        @p run_options accordingly.
 * @param argc        Argument count (from main).
 * @param argv        Argument vector (from main).
 * @param run_options Output struct to be populated.
 */
void validate_arguments(int argc, char* argv[], RunningOptions& run_options) {
  // Start at 1 to skip argv[0], which is the executable name.
  for (int i{ 1 }; i < argc; i++) {
    std::string arg = argv[i];

    if (arg == "--help" or arg == "-h") {
      usage(); // Print help and exit immediately.
    }
    else if (arg == "--len") {
      // --len requires the next token to be a positive integer.
      if (i + 1 < argc) {
        try {
          int val = std::stoi(argv[++i]); // Consume the next argument as the value.
          if (val <= 0)
            usage("Password size must be greater than zero.");
          run_options.pass_length = val;
        } catch (...) {
          // std::stoi throws if the string is not a valid integer.
          usage("Invalid numeric value for --len.");
        }
      } else {
        usage("--len requires a numeric value.");
      }
    }
    // Long-form single flags — each maps directly to a bool in RunningOptions.
    else if (arg == "--force") run_options.force = true;
    else if (arg == "--lower") run_options.lower = true;
    else if (arg == "--upper") run_options.upper  = true;
    else if (arg == "--digits") run_options.digits = true;
    else if (arg == "--logograms") run_options.logograms = true;
    else if (arg == "--punctuation") run_options.punctuation = true;
    else if (arg == "--quotes") run_options.quotes = true;
    else if (arg == "--slashes") run_options.slashes = true;
    else if (arg == "--math") run_options.math = true;
    else if (arg == "--braces") run_options.braces = true;
    else if (arg == "--verbose") run_options.verbose = true;
    else if (arg == "--all-groups" or arg == "-a") {
      run_options.math = run_options.lower = run_options.upper = run_options.braces =
      run_options.slashes = run_options.quotes = run_options.punctuation =
      run_options.logograms = run_options.digits = true;
    }
    // Short-option cluster, e.g. -ludsm — iterate over each letter.
    else if (arg[0] == '-' and arg.length() > 1) {
      for (size_t j{ 1 }; j < arg.length(); j++) {
        switch (arg[j]) {
          case 'l': run_options.lower = true; break;
          case 'u': run_options.upper = true; break;
          case 'd': run_options.digits = true; break;
          case 'o': run_options.logograms = true; break;
          case 'p': run_options.punctuation= true; break;
          case 'q': run_options.quotes = true; break;
          case 's': run_options.slashes = true; break;
          case 'm': run_options.math = true; break;
          case 'b': run_options.braces = true; break;
          case 'v': run_options.verbose = true; break;
          case 'a':
            run_options.lower = run_options.upper = run_options.digits =
            run_options.logograms = run_options.punctuation = run_options.quotes =
            run_options.slashes = run_options.math = run_options.braces = true;
            break;
          default: usage("Unknown short argument.");
        }
      }
    } else {
      // Any unrecognised token that doesn't start with '-' is invalid.
      usage("Unknown argument: " + arg);
    }
  }

  // If the user didn't select any group, default to lowercase only.
  if (!run_options.any_true_group()) {
    run_options.lower = true;
  }

  // Without --force, the password must be long enough to fit one char from
  // every active group, so we expand pass_length if necessary.
  if (!run_options.force and run_options.active_groups() > run_options.pass_length) {
    run_options.pass_length = run_options.active_groups();
  }
}

/**
 * @brief Parses and validates all command-line arguments, filling
 *        @p run_options accordingly.
 * @param argc        Argument count (from main).
 * @param argv        Argument vector (from main).
 * @param run_options Output struct to be populated.
 */
void add_char(std::string& group, std::string& pass, std::mt19937& gen, std::string& pool) {
  // Pick a random index within the group.
  std::uniform_int_distribution<size_t> dist(0, group.length() - 1);
  size_t idx = dist(gen);

  // Append the chosen character to the password.
  pass += group[idx];

  // Remove the chosen character from the group using swap-with-last,
  // which avoids shifting elements and runs in O(1).
  group[idx] = group.back();

  // Add all remaining characters (everything except the last, which is now
  // the one we just used) to the shared pool. This ensures the chosen char
  // won't appear in the pool until it is reset, reducing repetitions.
  pool += group.substr(0, group.length() - 1);
}

/**
 * @brief Generates a password according to the options in @p run_options.
 *
 * Algorithm:
 *  1. For each active group, pick one guaranteed distinct char via add_char.
 *     After this step password.length() == active_groups().
 *  2a. If --force: shuffle and trim down to pass_length via pop_back.
 *  2b. Otherwise: fill remaining slots from the shared pool, resetting it
 *      from reserve_char when exhausted to minimise repetitions.
 *  3. Final shuffle to hide the group origin of each character.
 *
 * @param run_options  Parsed CLI options.
 * @return The generated password as a std::string.
 */
std::string generate_password(const RunningOptions& run_options) {
  // Character sets for each group.
  std::string lower_chars       = "abcdefghijklmnopqrstuvwxyz";
  std::string upper_chars       = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  std::string digits_chars      = "0123456789";
  std::string logograms_chars   = "#$%&@^`~";
  std::string punctuation_chars = ".,;:";
  std::string quotes_chars      = "\"'";
  std::string slashes_chars     = "\\/|_-";
  std::string math_chars        = "<>*+!?=";
  std::string braces_chars      = "{}[]()";

  std::string password;     // Final password being assembled.
  std::string pool;         // Available chars for filling remaining slots.
  std::string reserve_char; // Full backup of all active chars for pool reset.

  // Seed the Mersenne Twister with a true random value.
  std::random_device rd;
  std::mt19937 gen(rd());

  // Step 1: for each active group, guarantee one distinct character.
  // add_char picks one char, appends it to password, and feeds the rest
  // of that group into the shared pool for later use.
  if (run_options.lower) { 
    reserve_char += lower_chars;       
    add_char(lower_chars,password,gen,pool); }
  if (run_options.upper) { 
    reserve_char += upper_chars;       
    add_char(upper_chars,password,gen,pool); }
  if (run_options.digits) { 
    reserve_char += digits_chars;      
    add_char(digits_chars,password,gen,pool); }
  if (run_options.logograms) { 
    reserve_char += logograms_chars;   
    add_char(logograms_chars,password,gen,pool); }
  if (run_options.punctuation) { 
    reserve_char += punctuation_chars;
    add_char(punctuation_chars,password,gen,pool); }
  if (run_options.quotes) { 
    reserve_char += quotes_chars;
    add_char(quotes_chars,password,gen,pool); }
  if (run_options.slashes) { 
    reserve_char += slashes_chars;    
     add_char(slashes_chars,password,gen,pool); }
  if (run_options.math) { 
    reserve_char += math_chars;
    add_char(math_chars,password,gen,pool); }
  if (run_options.braces) { 
    reserve_char += braces_chars;
    add_char(braces_chars,password,gen,pool); 
  }

  // Step 2a: --force mode.
  // The password already has active_groups() chars (one per group).
  // Shuffle so the trimming below removes random groups, not just the last
  // ones added, then pop chars until we reach the requested length.
  if (run_options.force) {
    std::shuffle(password.begin(), password.end(), gen);
    while (password.length() > run_options.pass_length)
      password.pop_back();
    return password;
  }

  // Step 2b: normal mode — fill remaining slots from the shared pool.
  // Shuffle the pool first to randomise the order of available characters.
  std::shuffle(pool.begin(), pool.end(), gen);

  while (password.length() < run_options.pass_length) {
    // When the pool runs out, refill it from the full reserve and reshuffle,
    // so we cycle through all characters before repeating any.
    if (pool.empty()) {
      pool += reserve_char;
      std::shuffle(pool.begin(), pool.end(), gen);
    }

    // Pick a random character from the pool using the same swap-with-last
    // technique as add_char to avoid repetitions within a cycle.
    std::uniform_int_distribution<size_t> dist(0, pool.length() - 1);
    size_t idx = dist(gen);
    password += pool[idx];
    pool[idx] = pool.back();
    pool = pool.substr(0, pool.length() - 1);
  }

  // Step 3: shuffle the final password so the position of each character
  // gives no hint about which group it came from.
  std::shuffle(password.begin(), password.end(), gen);

  return password;
}

/**
 * @brief Evaluates password quality using Shannon entropy: H = L * log2(R),
 *        where R is the number of distinct characters and L is the length.
 *
 * Categories (bits):
 *   H <= 25         -> Very Weak
 *   26 <= H <= 45   -> Weak
 *   46 <= H <= 80   -> Good
 *   81 <= H <= 100  -> Strong
 *   101 <= H <= 120 -> Very Strong
 *   H > 120         -> Excellent
 *
 * @param password     The password to evaluate.
 * @param entropy      Output: computed entropy in bits.
 * @param variability  Output: number of distinct characters in the password.
 * @return Qualitative label as a std::string.
 */
std::string password_quality(std::string_view password, double& entropy, size_t& variability) {
  // Copy the password into a temp string so we can sort and deduplicate it
  // without modifying the original.
  std::string temp{ password };
  std::sort(temp.begin(), temp.end());

  // std::unique moves duplicate elements to the end and returns an iterator
  // to the new logical end; erase removes the duplicates.
  auto cut = std::unique(temp.begin(), temp.end());
  temp.erase(cut, temp.end());

  // Variability = number of distinct characters in the password.
  variability = temp.length();

  size_t R = temp.length();  // Alphabet size (distinct chars).
  size_t L = password.length(); // Password length.

  // Shannon entropy: H = L * log2(R).
  // Guard against log2(0) (R==0) and log2(1)==0 (R==1, all chars identical).
  entropy = (R > 1) ? static_cast<double>(L) * std::log2(static_cast<double>(R)) : 0.0;

  // Classify into 6 qualitative categories based on entropy in bits.
  if      (entropy < 26)  return "Very Weak";
  else if (entropy < 46)  return "Weak";
  else if (entropy < 81)  return "Good";
  else if (entropy < 101) return "Strong";
  else if (entropy < 121) return "Very Strong";
  else                    return "Excellent";
}
