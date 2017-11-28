#include "stdafx.h"
#include "Trie.h"

namespace fs = boost::filesystem;

Trie::Trie() {
  root_ = NodePtr(new Node());
}

void Trie::Insert(const std::string & key, unsigned post_number) {
  std::string lower_key = boost::to_lower_copy(key);
  NodePtr search_node = root_;
  for each (const char& sym in lower_key) {
    if (!search_node->children_[sym - 'a']) {
      search_node->children_[sym - 'a'] = NodePtr(new Node());
    }
    search_node = search_node->children_[sym - 'a'];
  }
  search_node->positions_.push_back(post_number);
}

Positions Trie::Find(const std::string& key) {
  std::string lower_key = boost::to_lower_copy(key);
  NodePtr search_node = root_;
  for each (const char& sym in lower_key) {
    if (!search_node->children_[sym - 'a']) {
      return Positions();
    }

    search_node = search_node->children_[sym - 'a'];
  }

  return search_node ? search_node->positions_ : Positions();
}

std::string Trie::GetMaxLenSubstr() {
  std::vector<std::string> res;
  for (char i = 0; i < kCardinality; ++i) {
    std::vector<std::string> tmp = 
      FindMaxLenSubstr(root_->children_[i], std::string(1, i + 'a'));
    res.insert(res.end(), tmp.begin(), tmp.end());
  }

  unsigned max_size = std::numeric_limits<unsigned>::min();
  std::string ret;
  for each(auto const& word in res) {
    if (word.size() > max_size) {
      ret = word;
      max_size = word.size();
    }
  }
  return ret;
}

std::pair<std::string, unsigned> Trie::GetMaxFreqSubstr() {
  std::vector< std::pair<std::string, unsigned> > res;
  for (char i = 0; i < kCardinality; ++i) {
    res.push_back(FindMaxFreqSubstr(root_->children_[i], std::string(1, i + 'a')));
  }

  std::pair<std::string, unsigned> ret(std::string(), std::numeric_limits<unsigned>::min());
  for each(auto const& val in res) {
    if (val.second > ret.second) {
      ret = val;
    }
  }
  return ret;
}

std::vector<std::string> Trie::FindMaxLenSubstr(NodePtr node, std::string sub) {
  if (!node) {
    return std::vector<std::string>();
  }
  std::vector<std::string> ret;
  if (!node->positions_.empty()) {
    ret.push_back(sub);
  }
  for (char i = 0; i < kCardinality; ++i) {
      std::vector<std::string> tmp = 
        FindMaxLenSubstr(node->children_[i], sub + std::string(1, i + 'a'));
      ret.insert(ret.end(), tmp.begin(), tmp.end());
  }

  return ret;
}

std::pair<std::string, unsigned> Trie::FindMaxFreqSubstr(NodePtr node, std::string sub) {
  std::pair<std::string, unsigned> ret(std::string(), std::numeric_limits<unsigned>::min());

  if (!node) {
    return ret;
  }

  if (node->positions_.size() > ret.second) {
    ret = std::make_pair(sub, node->positions_.size());
  }

  for (char i = 0; i < kCardinality; ++i) {
    auto tmp = FindMaxFreqSubstr(node->children_[i], sub + std::string(1, i + 'a'));
    if (tmp.second > ret.second) {
      ret = tmp;
    }
  }

  return ret;
}

TriePtr CreateTrieFromText(const std::string& path) {
  fs::path path_to_text(path);
  if (fs::exists(path_to_text)) {
    fs::ifstream stream(path_to_text);
    unsigned post_num = 0;
    TriePtr trie(new Trie());
    for (std::string line; std::getline(stream, line);) {
      if (!line.empty()) {
        std::vector<std::string> words;
        boost::split(words,
                     line,
                     boost::is_any_of("\t \n.,:;!@#$%^&*()_-=\[]"),
                     boost::token_compress_on);
        
        for each(auto const& word in words) {
          if (!word.empty()) {
            trie->Insert(word, post_num);
          }
        }
        ++post_num;
      }
    }
    return trie;
  }
  return TriePtr();
}
