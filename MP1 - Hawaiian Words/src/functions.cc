#include "functions.hpp"

#include <iostream>
#include <stdexcept>

/**
 * TODO: Implement this function
 * @param word word to change to lowercase
 * @return result of changing every character in word to lowercase
 * DONE
 */
std::string StringToLower(const std::string& word) {
  std::string endword = word;
  for (char& c : endword) {
    c = static_cast<char>(std::tolower(c));
  }
  return endword;
}

/**
 * TODO: Implement this function
 * @param v1 first character in potential vowel group
 * @param v2 second character in potential vowel group
 * @return true if v1 and v2 form a valid vowel group
 * DONE
 */
bool IsVowelGroup(const char& v1, const char& v2) {
  std::string str;
  str.append(1,v1);
  str.append(1,v2);
  return str == "ai" || str == "ae" || str == "ao" || str == "au" || str == "ei" || str == "eu" || str == "iu" || str == "oi" || str == "ou" || str == "ui";
}

/**
 * TODO: Complete this function
 * @param v potential vowel character
 * @return true if v is a vowel
 * DONE
 */
bool IsVowel(const char& v) {
  return v == 'a' || v == 'e' || v == 'i' || v == 'o' || v == 'u';
}

/**
 * TODO: Complete this function
 * @param c potential consonant character
 * @return true if c is a consonant
 * DONE
 */
bool IsConsonant(const char& c) {
  return c == 'p' || c == 'h' || c == 'k' || c == 'l' || c == 'm' || c == 'n' || c == 'w';
}

/**
 * TODO: Implement this function
 * @param c character to check validity of
 * @return true if input c is a valid character in the Hawaiian language, false
 * otherwise
 * DONE
 */
bool IsValidCharacter(const char& c) {
  return c == 'p' || c == 'k' || c == 'h' || c == 'l' || c == 'm' || c == 'n' ||
      c == 'w' || c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == ' ' || c == '\'';
}

/**
 * TODO: Implement this function
 * @param word word to check validity of
 * @return true if every character in word is a valid character in the Hawaiian
 * language, false otherwise
 * DONE
 */
bool IsValidWord(const std::string& word) {
  for (char x : word) {
    if (!(IsValidCharacter(x))) {
      return false;
    }
  }
  return true;
}

/**
 * TODO: Implement this function
 * @param c consonant to get pronunciation of
 * @param prev character before c, used for determining w translation
 *    set to null character as default if no value is passed in
 * @return pronunciation of the consonant c as a char
 * DONE
 */
char ConsonantPronunciation(const char& c, const char& prev) {
  if ((prev == 'e' || prev == 'i') && c == 'w') {
    return 'v';
  }
  return c;
}

/**
 * TODO: Implement this function
 * @param v1 first vowel in a vowel grouping
 * @param v2 second vowel in a vowel grouping
 * @return the pronunciation of the vowel grouping made up of v1 and v2
 * as a string
 * DONE
 */
std::string VowelGroupPronunciation(const char& v1, const char& v2) {
  if (v1 == 'a') {
    if (v2 == 'i' || v2 == 'e') {
      return "eye";
    }
    if (v2 == 'o' || v2 == 'u') {
      return "ow";
    }
  } else if (v1 == 'e' && v2 == 'i') {
    return "ay";
  } else if (v1 == 'e' && v2 == 'u') {
    return "eh-oo";
  } else if (v1 == 'i' && v2 == 'u') {
    return "ew";
  } else if (v1 == 'o') {
    if (v2 == 'i') {
      return "oy";
    }
    if (v2 == 'u') {
      return "ow";
    }
  } else if (v1 == 'u' && v2 == 'i') {
    return "ooey";
  }
  return "";
}

/**
 * TODO: Implement this function
 * @param v single vowel to get pronunciation of
 * @return the pronunciation of v as a string
 * 
 */
std::string SingleVowelPronunciation(const char& v) {
  if (v == 'a') {
    return "ah";
  }
  if (v == 'e') {
    return "eh";
  }
  if (v == 'i') {
    return "ee";
  }
  if (v == 'o') {
    return "oh";
  }
  if (v == 'u') {
    return "oo";
  }
  return "";
}

/**
 * TODO: Implement this function
 * @param prev first character in set of three passed to function
 * @param curr second character in set of three passed to function
 * @param next third character in set of three passed to function
 * @return pronunciation of curr using next and prev as needed to determine
 * result
 * DONE
 */
std::string ProcessCharacter(const char& prev,
                             const char& curr,
                             const char& next) {
                              
  char c = '\0';
  if (IsVowelGroup(curr, next)) {
    return VowelGroupPronunciation(curr,next);
  }
  if (IsVowel(curr)) {
    return SingleVowelPronunciation(curr);
  }
  if (IsConsonant(curr)) {
    c = ConsonantPronunciation(curr, prev);
  } else {
    c = curr;
  }

  std::string final_word;
  if (c != '\0') {
    final_word += c;
    return final_word;
  }
  return "";
}

/**
 * TODO: Implement this function
 * @param word string to get pronunciation of
 * @return pronunciation of word
 * DONE
 */
std::string Pronunciation(const std::string& word) {
  std::string finalresult;
  for (unsigned int i = 0; i < word.length(); i++) {
    char prev = '\0';
    char next = '\0';
    if (i != 0) {
      prev = word[i - 1];
    }
    if (i != word.length() - 1) {
      next = word[i + 1];
    }
    char curr = word[i];

    if (IsVowelGroup(curr, next)) {
      finalresult += VowelGroupPronunciation(curr, next);
      i++;
    } else {
      std::string tempcharprocess = ProcessCharacter(prev, curr, next);
      finalresult += tempcharprocess;
    }

    if (next != '\0' && next != '\'' && next != ' ' && IsVowel(curr) && i != word.length() - 1) {
      finalresult += '-';
    } else if (curr == ' ') {
      continue;
    }
  }
  
  return finalresult;
}

/**
 * TODO: Implement this function
 * @param hawaiian_word word to get pronunciation of
 * @return the pronunciation of hawaiian_word as a string if valid
 *  and throws an invalid_argument error if not valid
 * 
 */
std::string GetPronunciation(const std::string& hawaiian_word) {
  std::string lowerstring = StringToLower(hawaiian_word);
  if (!(IsValidWord(lowerstring))) {
    throw std::invalid_argument("not a valid word gang");
  } 
  return Pronunciation(lowerstring);
}