//
// Created by עודד ועלני on 09/06/2022.
//
#include "HashMap.hpp"

class InvalidKey : public std::invalid_argument {
  using std::invalid_argument::invalid_argument;
};

class Dictionary : public HashMap<std::string, std::string> {
 public:
  Dictionary (const std::vector<std::string> &keys,
              const std::vector<std::string> &values)
      : HashMap<std::string, std::string> (keys, values)
  {}
  Dictionary (const Dictionary &dict)
      : HashMap<std::string, std::string> (dict)
  {}

  ~Dictionary()
  {
    HashMap<std::string, std::string>::~HashMap();
  }
  /**
   * The function use the erase function of the father, and throw invalid_key
   * exception if the key isn't in the dictionary
   * @param key as string
   * @return string value match to the key
   * */
  bool erase (const std::string &key)
  {
    if (!HashMap<std::string, std::string>::erase (key))
      {
        throw InvalidKey ("Key not found");
      }
    return true;
  }

  /**
   * The function get begin iter and end iter at least from type forward
   * iterator, and load the data from the range into the dictionary
   * @param begin start point
   * @param end end point (not include)
   * */
  template<class Iterator>
  void update (Iterator begin, Iterator end)
  {
    for (; begin != end; ++begin)
      {
        insert (begin->first, begin->second);
      }
  }
};

