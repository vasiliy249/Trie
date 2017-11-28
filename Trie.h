#pragma once

#define DECLARE_PTR(Interface) \
  class Interface;             \
  typedef std::shared_ptr<Interface> Interface ## Ptr;

const unsigned kCardinality = 26;

typedef std::vector<unsigned> Positions;

DECLARE_PTR(Trie)
class Trie {
public:
  Trie();
  void Insert(const std::string& key, unsigned post_number);
  Positions Find(const std::string& key);

  std::string GetMaxLenSubstr();
  std::pair<std::string, unsigned> GetMaxFreqSubstr();

private:
  DECLARE_PTR(Node)
  NodePtr root_;

  class Node {
  public:
    std::vector<NodePtr> children_;
    Positions positions_;
    Node() {
      children_.assign(kCardinality, NULL);
    }
  };

private:
  std::vector<std::string> FindMaxLenSubstr(NodePtr node, std::string sub);
  std::pair<std::string,unsigned> FindMaxFreqSubstr(NodePtr node, std::string sub);
};

TriePtr CreateTrieFromText(const std::string& path);