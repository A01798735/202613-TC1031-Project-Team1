/*----------------------------------------------------------
 * Project: Spell Checker
 *
 * Date: 02-Dec-2026
 * Authors:
 *           A01798735 Valente Espinosa
 *           A01798554 Omar Chávez
 *----------------------------------------------------------*/

#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

struct word {
  std::string text;
  int line;
  int column;
};

bool read_words(const std::string file_path, std::vector<word> &words) {
  std::ifstream input_file(file_path);
  if (input_file.fail()) {
    return false;
  }
  std::regex reg_exp("[a-zA-Z]+");
  std::smatch match;
  std::string text;
  int line = 0;
  int column = 0;
  while (std::getline(input_file, text)) {
    line++;
    column = 1;
    while (std::regex_search(text, match, reg_exp)) {
      column += match.position();
      words.push_back({match.str(), line, column});
      column += match.length();
      text = match.suffix().str();
    }
  }
  return true;
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

int main(int argc, char *argv[]) {
  // std::string file_name = "tooinkle.txt";
  // std::vector<word> words;
  //
  // if (read_words(file_name, words)) {
  //   for (word w : words) {
  //     std::cout << w.text << "  (line " << w.line << ", column " << w.column << ")\n";
  //   }
  // } else {
  //   std::cout << "Unable to read file: " << file_name << "\n";
  // }
  std::cout << soundex("Monterrey") << '\n';
  return 0;
}
