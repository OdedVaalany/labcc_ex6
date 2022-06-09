//
// Created by oded vaalany on 08/06/2022.
//

#ifndef _HASHMAP_HPP_
#define _HASHMAP_HPP_
#include <vector>
#include <utility>
#include <exception>
#include <cmath>
#include <iterator>

#define START_CAPACITY 16
#define LOWER_LOADER_FACTOR 0.25
#define UPPER_LOADER_FACTOR 0.75

template<class KeyT, class ValueT>
class HashMap {
 private:
  std::vector<std::vector<std::pair<KeyT, ValueT>> *> _array;
  int _capacity;
  int _size;
  /**
   * The function implement the hasher function
   * @param key
   * @return cell number in the array that match the key;
   * */
  int hasher (KeyT key) const
  {
    return std::hash<KeyT>{} (key) & (_capacity - 1);
  }

  /**
   * The function fit the size of the array
   * */
  void rehasher ()
  {
    std::vector<KeyT> keys;
    std::vector<ValueT> values;
    double loader_factor = get_load_factor ();
    if (loader_factor > UPPER_LOADER_FACTOR)
      {
        for (auto &item: *this)
          {
            keys.push_back (item.first);
            values.push_back (item.second);
          }
        clear ();
        delete_array ();
        _capacity = std::pow (2, (int) std::log2 (_capacity) + 1);
        allocate_array ();
        load_data (keys, values);
      }
    if (loader_factor < LOWER_LOADER_FACTOR && _capacity > START_CAPACITY)
      {
        for (auto &item: *this)
          {
            keys.push_back (item.first);
            values.push_back (item.second);
          }
        clear ();
        delete_array ();
        _capacity = std::pow (2, (int) std::log2 (_capacity) - 1);
        allocate_array ();
        load_data (keys, values);
      }
  }

  /**
   * The function free all allocation memory of the array from the HashMap
   * */
  void delete_array ()
  {
    for (int i = 0; i < _capacity; ++i)
      {
        delete _array[i];
      }
  }

  /**
   * This function allocate the array in the heap memory
   * */
  void allocate_array ()
  {
    _array = std::vector<std::vector<std::pair<KeyT, ValueT>> *> (_capacity);
    for (int i = 0; i < _capacity; ++i)
      {
        _array[i] = new std::vector<std::pair<KeyT, ValueT>> ();
      }
  }

  /**
   * The function get keys and values vectors and load them in the array
   * @param keys vector
   * @param values vector
   * */
  void load_data (std::vector<KeyT> keys, std::vector<ValueT> values)
  {
    for (int i = 0; i < keys.size (); ++i)
      {
        if (!contains_key (keys[i]))
          {
            (_array[hasher (keys[i])])->push_back ({keys[i], values[i]});
            _size++;
          }
      }
    rehasher ();
  }
 public:
  /**
   * Default HashMap constructor
   * */
  HashMap ()
  {
    _size = 0;
    _capacity = START_CAPACITY;
    _array = std::vector<std::vector<std::pair<KeyT, ValueT>> *> (_capacity);
    for (int i = 0; i < _capacity; ++i)
      {
        _array[i] = new std::vector<std::pair<KeyT, ValueT>> ();
      }
  }

  /**
   * HashMap constructor that init the hash map with given
   * pairs of keys and value
   * @param keys vector of keys for the hash map
   * @param values vector of values for the hash map
   * */
  HashMap (std::vector<KeyT> keys, std::vector<ValueT> values)
  {
    if (values.size () != keys.size ())
      {
        throw std::runtime_error ("The given vector arn't the same size");
      }
    _size = 0;
    _capacity = START_CAPACITY;
    allocate_array ();
    load_data (keys, values);
  }

  /**
   * HashMap copy constructor
   * @param to_copy const reference to a hash map
   * */
  HashMap (const HashMap<KeyT, ValueT> &to_copy)
  {
    _size = to_copy._size;
    _capacity = to_copy._capacity;
    _array = std::vector<std::vector<std::pair<KeyT, ValueT>> *> (_capacity);
    for (int i = 0; i < _capacity; ++i)
      {
        _array[i] = new std::vector<std::pair<KeyT, ValueT>> (to_copy._array[i][0]);
      }
  }

  /**
   * HashMap destructor
   * */
  ~HashMap ()
  {
    delete_array ();
  }

  /**
   * This function return the number of keys-values in the hash map
   * @return The size as int number
   * */
  int size () const
  {
    return _size;
  }

  /**
   * This function return the capacity of the hash map
   * @return the capacity as int number
   * */
  int capacity () const
  {
    return _capacity;
  }

  /**
   * This function check if the hash map is empty
   * @return true if it's empty\n
   * false either
   * */
  bool empty () const
  {
    return _size == 0;
  }

  /**
   * The function insert the key and value into the map
   * @param key, key for the given value
   * @param value, value for the given key
   * @return true if the pair is not exist in the hash map and successfully
   * inserted\n false if it exist or if the pair hasn't inserted
   * */
  bool insert (KeyT key, ValueT value)
  {
    if (contains_key (key))
      {
        return false;
        return false;
      }
    (_array[hasher (key)])->push_back ({key, value});
    _size++;
    rehasher ();
    return true;
  }

  /**
   * The function check if the given key is already into the hash map
   * @param key to check
   * @return true if the key is in the hash map \n false either
   * */
  bool contains_key (const KeyT &key) const
  {
    auto a = _array.begin ();
    for (auto &item: *(_array.at (hasher (key))))
      {
        if (item.first == key)
          {
            return true;
          }
      }
    return false;
  }

  /**
   * The function return match value to the given key in the hash map, \n
   * throw exception if the key isn't in the hash map
   * @param key to check
   * @return reference to the value
   * */
  ValueT &at (KeyT key)
  {
    int place = hasher (key);
    for (std::pair<KeyT, ValueT> &item: *_array[place])
      {
        if (item.first == key)
          {
            return item.second;
          }
      }
    throw std::out_of_range ("Key not found");
  }
  const ValueT &at (KeyT key) const
  {
    int place = hasher (key);
    for (std::pair<KeyT, ValueT> &item: *_array[place])
      {
        if (item.first == key)
          {
            return item.second;
          }
      }
    throw std::out_of_range ("Key not found");
  }

  /**
   * The function remove key-value pair from the hash map, using given key
   * @param key to remove
   * @return true if the pair successfully removed \n false either
   * */
  bool erase (const KeyT key)
  {
    for (auto i = _array[hasher (key)][0].begin ();
         i != _array[hasher (key)][0].end (); ++i)
      {
        if (i->first == key)
          {
            _array[hasher (key)]->erase (i);
            _size--;
            rehasher ();
            return true;
          }
      }
    return false;
  }

  /**
   * The function return the loader factor of the hash map
   * @return size/capacity as double type
   * */
  double get_load_factor () const
  {
    return ((double) _size) / ((double) capacity ());
  }

  /**
   * The function search for the bucket (=cell) that contain the given key,
   * and return the amount of element in the match bucket.\n
   * throw exception if the key isn't exist
   * @param key to search
   * @return number of the element in the same bucket (=cell)
   * */
  int size_bucket (KeyT key) const
  {
    if (contains_key (key))
      {
        int place = hasher (key);
        return (*_array[place]).size ();
      }
    throw std::out_of_range ("Key not found");
  }

  /**
   * The function search for the bucket (=cell) that contain the given key,
   * and return the index of the match bucket.\n
   * throw exception if the key isn't exist
   * @param key to search
   * @return index of the match bucket (=cell)
   * */
  int index_bucket (KeyT key) const
  {
    if (contains_key (key))
      {
        return hasher (key);
      }
    throw std::out_of_range ("Key not found");
  }

  /**
   * The function clear all the element from the hash map,
   * without changing the capacity
   * */
  void clear ()
  {
    for (auto &bucket: _array)
      {
        bucket->clear ();
        bucket->shrink_to_fit ();
      }
    _size = 0;
  }

  /**
   * The operator allow to copy the data from one element into this element
   * @param to_copy is a hash map to copy
   * @return reference to this hash map
   * */
  HashMap &operator= (const HashMap &to_copy)
  {
    _size = to_copy._size;
    _capacity = to_copy._capacity;
    for (int i = 0; i < _capacity; ++i)
      {
        delete _array[i];
      }
    _array = std::vector<std::vector<std::pair<KeyT, ValueT>> *> (_capacity);
    for (int i = 0; i < _capacity; ++i)
      {
        _array[i] = new std::vector<std::pair<KeyT, ValueT>>
            (to_copy._array[i][0]);
      }
  }

  /**
   * The operator check if the keys,data are the same in both hash maps
   * @param lhs is the object from the left
   * @param rhs is the object from the right
   * @return true if the data sets are the same\n false either
   * */
  friend bool operator== (const HashMap &lhs, const HashMap &rhs)
  {
    return true;
  }

  /**
   * The operator check if the keys,data aren't the same in both hash maps
   * @param lhs is the object from the left
   * @param rhs is the object from the right
   * @return true if the data sets aren't the same\n false either
   * */
  friend bool operator!= (const HashMap &lhs, const HashMap &rhs)
  {
    return !(lhs == rhs);
  }

  /**
   * This function are allow to access to data by the key in the hash map
   * @param key to search in the hash map
   * @return valueT that match to the key, else nullptr
   * */
  ValueT &operator[] (const KeyT &key)
  {
    for (auto &item: *_array[hasher (key)])
      {
        if (item.first == key)
          {
            return item.second;
          }
      }
    return this->begin ()->second;
  }
  const ValueT &operator[] (const KeyT &key) const
  {
    for (auto &item: *_array[hasher (key)])
      {
        if (item.first == key)
          {
            return item.second;
          }
      }
    return this->begin ()->second;
  }

  class Iterator {
   private:
    const HashMap<KeyT, ValueT> &_father;
    const std::vector<std::vector<std::pair<KeyT, ValueT>> *> &_place;
    int _cell;
    int _obj;
   public:
    typedef std::ptrdiff_t difference_type;
    typedef std::pair<KeyT, ValueT> value_type;
    typedef std::pair<KeyT, ValueT> *pointer;
    typedef const std::pair<KeyT, ValueT> &reference;
    typedef std::forward_iterator_tag Iterator_category;

    /**
     * constructor for the const Iterator
     * */
    Iterator (const Iterator &iter)
        : _father (iter._father), _place (_father._array)
    {
      _obj = iter._obj;
      _cell = iter._cell;
    }

    Iterator (HashMap<KeyT, ValueT> *father)
        : _father (*father), _place (_father._array)
    {
      _cell = 0;
      while (_place.capacity () > _cell && _place[_cell]->empty ())
        {
          _cell++;
        }
      _obj = 0;
    }

    /**
     * allow access to the pointer of the Iterator
     * @return const pointer of the value;
     * */
    pointer operator-> ()
    {
      return &(_place[_cell]->at (_obj));
    }

    /**
     * allow access to the value of the Iterator
     * @return const value
     * */
    const reference operator* () const
    {
      return (_place[_cell]->at (_obj));
    }

    Iterator &operator= (Iterator &iter)
    {
      _place = iter._place;
      _cell = iter._cell;
      _obj = iter._obj;
      return *this;
    }

    /**
     * Prefix increment
     * @return const reference to the value of the Iterator
     * */
    Iterator &operator++ ()
    {
      _obj++;
      if (_obj >= _place[_cell]->size ())
        {
          _cell++;
          while (_place.capacity () > _cell && _place[_cell]->empty ())
            {
              _cell++;
            }
          _obj = 0;
        }
      return *this;
    }

    /**
     * Postfix increment
     * @return const reference to the value of the Iterator
     * */
    Iterator operator++ (int)
    {
      auto temp = *this;
      _obj++;
      if (_obj >= _place[_cell]->size ())
        {
          _cell++;
          while (_place.capacity () > _cell && _place[_cell]->empty ())
            {
              _cell++;
            }
          _obj = 0;
        }
      return temp;
    }

    /**
     * allow to check if the Iterator are the same
     * @param lhs the left object
     * @param rhs the right object
     * @return true if it's the same \n false either
     * */
    friend bool operator== (const Iterator &lhs,
                            const Iterator &rhs)
    {
      return lhs._cell == rhs._cell && lhs._obj == rhs._obj;
    }

    /**
     * allow to check if the Iterator aren't the same
     * @param lhs the left object
     * @param rhs the right object
     * @return true if it isn't the same \n false either
     * */
    friend bool operator!= (const Iterator &lhs,
                            const Iterator &rhs)
    {
      return lhs._cell != rhs._cell || lhs._obj != rhs._obj;
    }
  };

  Iterator begin ()
  {
    return Iterator (this);
  }

  Iterator end ()
  {
    int i = 0;
    auto itr = Iterator (this);
    while (i < _size)
      {
        itr++;
        i++;
      }
    return itr;
  }

};

#endif //_HASHMAP_HPP_
