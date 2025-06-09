#ifndef SUPER_ARRAY_H
#define SUPER_ARRAY_H

#include <initializer_list> // For std::initializer_list
#include <iostream>         // For std::cout and std::endl
#include <random>           // For RNG
#include <time.h>           // For time()

// Stores a dynamic array
template <typename T>
class Array
{
  T* _arr = nullptr;    // Pointer to the dynamic array
  T _garbage;           // Garbage value to return when give invalid indexes
  int _currentSize = 0; // The number of valid elements in the array
  int _maxSize = 1;     // The amount of allocated memory

public:
  /**
   * Default Constructor
   */
  Array() :
    _garbage()
  {
    _arr = new T[_maxSize];
  }

  Array(int size) :
    _garbage(),
    _maxSize(size)
  {
    _arr = new T[_maxSize];
  }

  /**
   * Initializer List Constructor
   *
   * @param iList: The values for the Array
   */
  Array(const std::initializer_list<T>& iList)
  {
    // Allocate memory based on the size of the list
    _maxSize = iList.size() + 1;
    _arr = new T[_maxSize];

    // Add each element to the array
    for (int i = 0; i < iList.size(); i++)
      pushBack(*(iList.begin() + i));
  }

  /**
   * Copy Constructor
   */
  Array(const Array& copy) :
    _currentSize(copy._currentSize),
    _maxSize(copy._maxSize)
  {
    // Allocate new memory
    _arr = new T[_maxSize];

    // Copy each element
    for (int i = 0; i < _currentSize; ++i)
      _arr[i] = copy._arr[i];
  }

  /**
   * Destructor
   */
  ~Array()
  {
    if (_arr)
      delete[] _arr;
  }

  /**
   * Assignment operator
   *
   * @param copy: The Array to copy
   */
  Array& operator=(const Array& copy)
  {
    if (this == &copy)
      return *this;

    // Delete the old memory
    delete[] _arr;

    // Copy members
    _currentSize = copy._currentSize;
    _maxSize = copy._maxSize;

    // Allocate new memory and copy the elements
    _arr = new T[_maxSize];
    for (int i = 0; i < _currentSize; ++i)
      _arr[i] = copy._arr[i];

    return *this;
  }

  /**
   * Assignment operator with initiliazer list
   *
   * @param iList: The list of values to assign
   */
  Array& operator=(const std::initializer_list<T>& iList)
  {
    // Delete the old memory
    delete[] _arr;

    // Set the size to 0
    _currentSize = 0;

    // Set max size based on list length and allocate memory
    _maxSize = iList.size() + 1;
    _arr = new T[_maxSize];

    // Add each element to the array
    for (int i = 0; i < iList.size(); i++)
      pushBack(*(iList.begin() + i));

    return *this;
  }

  /**
   * Subscript operator
   *
   * @param n: The index to return
   */
  T& operator[](int n)
  {
    // Check if index is valid
    if (n < _currentSize and n >= 0)
      return _arr[n];

    // Return garbage for invalid index
    return _garbage;
  }

  /**
   * Subscript operator
   *
   * @param n: The index to return
   */
  T operator[](int n) const
  {
    // Check if index is valid
    if (n < _currentSize and n >= 0)
      return _arr[n];

    // Return garbage for invalid index
    return _garbage;
  }

  T* begin()
  {
    return _arr;
  }

  T* end()
  {
    return _arr + _currentSize;
  }

  const T* begin() const
  {
    return _arr;
  }

  const T* end() const
  {
    return _arr + _currentSize;
  }

  /**
   * Gets the number of the elements in the array
   *
   * @returns The size of the array
   */
  int getSize() const
  {
    return _currentSize;
  }

  /**
   * Gets a random number to index the array
   *
   * @returns An index between 0 and _currentSize - 1 (inclusive)
   */
  int randomIndex() const
  {
    if (_currentSize <= 0)
      return 0;
    static std::random_device device;
    std::uniform_int_distribution<int> index(0, _currentSize - 1);
    return index(device);
  }

  /**
   * Inserts a new element into the Array
   *
   * @param value: The element to be inserted
   * @param index: The index to insert into
   */
  void insert(const T& value, int n)
  {
    // Check for invalid index
    if (n < 0 or n > _currentSize)
      return;

    // Optimize for inserting into the end
    if (n == _currentSize)
    {
      if (_currentSize == _maxSize)
        grow();
      // Assign the value
      _arr[n] = value;

      // Increment size, and check if new memory needs to be allocated
      _currentSize++;
      return;
    }

    // Shift all elements up to the inserting index back one
    for (int i = _currentSize; i > n; --i)
      _arr[i] = _arr[i - 1];

    if (_currentSize == _maxSize)
      grow();
    // Assign the value
    _arr[n] = value;

    // Increment size, and check if new memory needs to be allocated
    _currentSize++;
  }

  /**
   * Adds an element to the front of the array
   *
   * @param value: The element to be inserted
   */
  void pushFront(const T& value)
  {
    insert(value, 0);
  }

  /**
   * Adds an element to the back of the array
   *
   * @param value: The element to be inserted
   */
  void pushBack(const T& value)
  {
    insert(value, _currentSize);
  }

  /**
   * Removes an element from the array
   *
   * @returns: The value that was removed
   */
  T remove(int n)
  {
    // Check for invalid index
    if (n < 0 or n >= _currentSize or _currentSize <= 0)
      return _garbage; // Function must return an object T, so return garbage member

    // Decrement current size and save the value to be removed
    _currentSize--;
    T temp = _arr[n];

    // Shift all elements after index forward one
    for (int i = n; i < _currentSize; ++i)
      _arr[i] = _arr[i + 1];

    // Check if memory can be deallocated
    if (_maxSize > 1 and _currentSize <= _maxSize / 2)
      shrink();

    // Return the removed value
    return temp;
  }

  /**
   * Removes an element from the front of the array
   *
   * @returns: The value that was removed
   */
  T popFront()
  {
    return remove(0);
  }

  /**
   * Removes an element from the back of the array
   *
   * @returns: The value that was removed
   */
  T popBack()
  {
    return remove(_currentSize - 1);
  }

  /**
   * Randomly shuffles the array
   */
  void shuffle()
  {
    // Seed RNG so shuffling is truly random
    srand(time(NULL));

    for (int i = 0; i < _currentSize; ++i)
    {
      // Generate a random index
      int index = rand() % _currentSize;

      // Swap arr[i] with random index
      T temp = _arr[i];
      _arr[i] = _arr[index];
      _arr[index] = temp;
    }
  }

  /**
   * Sorts the array using merge sort
   *
   * @param ascending = true:   Whether to sort the array in ascending or descending order
   */
  void sort(bool ascending = true)
  {
    mergeSort(_arr, _currentSize, ascending);
  }

  /**
   * Prints the to the screen using std::cout
   */
  void print()
  {
    // Loop through each element
    for (int i = 0; i < _currentSize; ++i)
    {
      // Print a space before all elements except the first
      if (i != 0)
        std::cout << " ";
      std::cout << _arr[i];
    }
  }

  /**
   * Empties the array
   */
  void clear()
  {
    // Remove elements until size is zero
    while (_currentSize > 0)
      popBack();
  }

private:
  /**
   * Doubles the allocated memory for the array
   */
  void grow()
  {
    // Double the max size and allocate new memory
    _maxSize *= 2;
    T* temp = new T[_maxSize];

    // Copy old array to new array
    for (int i = 0; i < _currentSize; ++i)
      temp[i] = _arr[i];

    // Deallocate old memory
    delete[] _arr;

    // Assign new pointer to array
    _arr = temp;
  }

  /**
   * Halves the allocated memory for the array
   */
  void shrink()
  {
    // Halve the max size and allocate new memory
    _maxSize = std::max(1, _maxSize / 2);
    T* temp = new T[_maxSize];

    // Copy elements from old array
    for (int i = 0; i < _currentSize; ++i)
      temp[i] = _arr[i];

    // Delete old array and copy the new one
    delete[] _arr;
    _arr = temp;
  }

  /**
   * Merge Sort
   *
   * @param arr				The array to sort
   * @param n					The size of the array
   * @param ascending Sort the array in ascending order?
   */
  void mergeSort(T arr[], int n, bool ascending = true)
  {
    // Base case if there is only one or fewer elements
    if (n < 2)
      return;

    // Find half of the array
    int half = n >> 1;

    // Sort the first half and the second half
    mergeSort(arr, half, ascending);
    mergeSort(arr + half, n - half, ascending);

    // Merge the two halves
    // If ascending, merge the two halves in ascending order
    if (ascending)
    {
      int i = 0;
      int j = half;

      // Loop through the two halves
      for (; i < j and j < n;)
      {
        // If the element in the second half doesnt need to be inserted, continue
        if (arr[i] <= arr[j])
        {
          ++i;
          continue;
        }

        // Shift the elements in the first half to the right and insert the element from the second half
        T temp = arr[j];
        memmove(arr + i + 1, arr + i, sizeof(T) * (j - i));
        arr[i] = temp;

        // Increment the indexes
        ++i;
        ++j;
      }
    }
    else // If descending
    {
      int i = 0;
      int j = half;
      // Loop through the two halves
      for (; i < j and j < n;)
      {
        // If the element in the second half doesnt need to be inserted, continue
        if (arr[i] >= arr[j])
        {
          ++i;
          continue;
        }

        // Shift the elements in the first half to the right and insert the element from the second half
        T temp = arr[j];
        memmove(arr + i + 1, arr + i, sizeof(T) * (j - i));
        arr[i] = temp;

        // Increment the indexes
        ++i;
        ++j;
      }
    }
  }
};

#endif //! SUPER_ARRAY_H
