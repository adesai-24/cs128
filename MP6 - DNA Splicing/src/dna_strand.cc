#include "dna_strand.hpp"
#include <stdexcept>


void DNAstrand::SpliceIn(const char* pattern, DNAstrand& to_splice_in) {
    if (pattern == nullptr || to_splice_in.head_ == nullptr || &to_splice_in == this) {
        return;
    }
    
    Node* b4_last_match = nullptr;
    Node* last_match = FindLastMatch(pattern, b4_last_match);
    
    if (last_match == nullptr) {
        throw std::runtime_error("DAWG THIS AIN'T WORKING");
    }
    
    Node* after_occr = DeleteMatchedNodes(last_match, pattern);
    SpliceNodes(b4_last_match, after_occr, to_splice_in);
}

Node* DNAstrand::FindLastMatch(const char* pattern, Node*& b4_last_match) {
    Node* current = head_;
    Node* last_match = nullptr;
    Node* prev = nullptr;

    while (current != nullptr) {
        Node* temp = current;
        const char* pattern_ptr = pattern;
        
        while (temp != nullptr && *pattern_ptr != '\0' && temp->data == *pattern_ptr) {
            temp = temp->next;
            pattern_ptr++;
        }
        
        if (*pattern_ptr == '\0') {
            last_match = current;
            b4_last_match = prev;
        }
        
        prev = current;
        current = current->next;
    }
    
    return last_match;
}

Node* DNAstrand::DeleteMatchedNodes(Node* start, const char* pattern) {
    Node* current = start;
    const char* pattern_ptr = pattern;
    
    while (current != nullptr && *pattern_ptr != '\0') {
        Node* temp = current;
        current = current->next;
        delete temp;
        pattern_ptr++;
    }
    
    return current;
}

void DNAstrand::SpliceNodes(Node* b4_last_match, Node* after_occr, DNAstrand& to_splice_in) {
    if (b4_last_match != nullptr) {
        b4_last_match->next = to_splice_in.head_;
    } else {
        head_ = to_splice_in.head_;
    }
    
    if (to_splice_in.tail_ != nullptr) {
        to_splice_in.tail_->next = after_occr;
    } else {
        if (b4_last_match != nullptr) {
            b4_last_match->next = after_occr;
        } else {
            head_ = after_occr;
        }
    }
    
    to_splice_in.head_ = nullptr;
    to_splice_in.tail_ = nullptr;
}

DNAstrand::~DNAstrand() {
    Node* current = head_;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
}
