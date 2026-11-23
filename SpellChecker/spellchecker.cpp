/*----------------------------------------------------------
 * Project: Spell Checker
 *
 * Date: 02-Dec-2026
 * Authors:
 *           A01798735 Valente Espinosa
 *           A01798554 Omar Chávez
 *----------------------------------------------------------*/

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Word {
  std::string text;
  int row;
  int col;
};

typedef std::unordered_map<std::string, std::vector<std::string>> soundex_map;

std::string lowercase(std::string word) {
  std::transform(word.begin(), word.end(), word.begin(),
                 [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
  return word;
}

std::string soundex(std::string word) {
  // Encode consonants
  std::string code;
  for (size_t i = 0; i < word.length(); i++) {
    const char c = word.at(i);
    if (i == 0) {
      code += c;
    } else if (c == 'b' or c == 'f' or c == 'p' or c == 'v') {
      code += '1';
    } else if (c == 'c' or c == 'g' or c == 'j' or c == 'k' or c == 'q' or c == 's' or c == 'x' or c == 'z') {
      code += '2';
    } else if (c == 'd' or c == 't') {
      code += '3';
    } else if (c == 'l') {
      code += '4';
    } else if (c == 'm' or c == 'n') {
      code += '5';
    } else if (c == 'r') {
      code += '6';
    }
  }

  // Make length equal to 7
  if (code.length() < 7) {
    code.append(7 - code.length(), '0');
  } else {
    code = code.substr(0, 7);
  }
  return code;
}

void map_from_dict(const std::string file_path, soundex_map &soundex_words) {
  std::ifstream file(file_path);
  if (file.fail()) {
    throw std::runtime_error("Unable to open dictionary: " + file_path);
  }

  std::string word;
  while (std::getline(file, word)) {
    if (word.empty()) {
      continue;
    }
    word = lowercase(word);
    soundex_words[soundex(word)].push_back(word);
  }
}

bool contains_word(const std::string &val, const std::vector<std::string> &words) {
  return std::binary_search(words.begin(), words.end(), val);
}

std::vector<Word> unknown_words(const std::string &file_path, soundex_map &soundex_words) {
  std::ifstream file(file_path);
  if (file.fail()) {
    throw std::runtime_error("Unable to open file: " + file_path);
  }

  std::vector<Word> words;
  std::unordered_set<std::string> reported;

  const std::regex reg_exp("[a-zA-Z]+");
  std::smatch match;
  std::string text;
  int row = 0;
  int col = 0;

  while (std::getline(file, text)) {
    row++;
    col = 1;
    while (std::regex_search(text, match, reg_exp)) {
      col += match.position();

      std::string word = match.str();
      std::string normalized = lowercase(word);
      std::string soundex_code = soundex(normalized);

      auto it = soundex_words.find(soundex_code);
      if ((it == soundex_words.end() || !contains_word(normalized, it->second)) && reported.insert(normalized).second) {
        words.push_back({.text = word, .row = row, .col = col});
      }

      col += match.length();
      text = match.suffix().str();
    }
  }

  return words;
}

int main(int argc, char *argv[]) {
  // Agregar comparación argc y lectura de argv para el archivo

  soundex_map soundex_words;
  map_from_dict("words.txt", soundex_words);

  // Tests
  std::vector<Word> ws = unknown_words("tooinkle.txt", soundex_words);
  for (const auto w : ws) {
    std::cout << w.row << ',' << w.col << '\t' << w.text << '\n';
  }

  return 0;
}
