#ifndef DNA_STRAND_HPP
#define DNA_STRAND_HPP

#include "node.hpp"

class DNAstrand {
public:
  DNAstrand() = default;
  DNAstrand(const DNAstrand& rhs) = delete;
  DNAstrand& operator=(const DNAstrand& rhs) = delete;
  ~DNAstrand();
  void SpliceNodes(Node* b4_last_match, Node* after_occr, DNAstrand& to_splice_in);
  Node* DeleteMatchedNodes(Node* start, const char* pattern);
  Node* FindLastMatch(const char* pattern, Node*& b4_last_match);
  void SpliceIn(const char* pattern, DNAstrand& to_splice_in);
private:
  Node* head_ = nullptr;
  Node* tail_ = nullptr;
};

#endif