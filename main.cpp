#include "stdafx.h"
#include "Trie.h"

int main(int argc, char* argv[]) {
  namespace po = boost::program_options;
  std::string path_to_data;
  po::options_description desc("General options");
  desc.add_options()
    ("help,h", "Show help")
    ("file,f", po::value<std::string>(&path_to_data)->required(), "Input/output file with data");
  po::variables_map vm;

  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    if (vm.count("help")) {
      std::cout << desc << std::endl;
    }
    po::notify(vm);

    TriePtr trie = CreateTrieFromText(path_to_data);
    if (!trie) {
      std::cout << "invalid file\n";
      return 0;
    }

    std::cout << "Max substring by len: " << trie->GetMaxLenSubstr() << std::endl;

    auto val = trie->GetMaxFreqSubstr();
    std::cout << "Max substring by freq: "
              << val.first
              << " ("
              << val.second
              << " times)"
              << std::endl;

    std::string key;
    do {
      std::cout << "Enter the key:\n";
      std::cin >> key;
      if (key == "exit") {
        return 0;
      }
      if (key == "insert") {
        std::cout << "Enter inserting key:\n";
        std::string ins_key;
        std::cin >> ins_key;
        trie->Insert(ins_key, 239);
        std::cout << "Max substring by len: " << trie->GetMaxLenSubstr() << std::endl;
        continue;
      }
      Positions positions = trie->Find(key);
      std::cout << "Occurances count: " << positions.size() << (positions.size() ? " in posts:" : "");
      for each(auto const& val in positions) {
        std::cout << " " << val;
      }
      std::cout << std::endl;

    } while (!key.empty());
  } catch (...) {
    std::cerr << "error, usage: -h or --help" << std::endl;
    return 0;
  }

  return 0;
}