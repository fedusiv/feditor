#ifndef __FSTRING_HPP__
#define __FSTRING_HPP__

#include <vector>
#include <string>


class FString{
  private:
    std::vector<int> data; // This is real data how it's stored. As ints
  
  public:
    // Static fields
    static FString StringToUtf8(std::string string);
    static std::string Utf8ToString(FString fstring);

    // Non-static fields
    FString() : data() {}
    FString(std::initializer_list<int> values) : data(values) {}
    FString(const FString& other) : data(other.data) {}
    FString(FString&& other) noexcept : data(std::move(other.data)) {}
    FString(std::string str){ 
      auto d = FString::StringToUtf8(str);
      data = d.data;
    }

    // Default push_back function. to keep as buffer line
    void push_back(int value){
      data.push_back(value);
    }
    size_t size() const {
          return data.size();
    }
    // Access element at a specific index
    int operator[](size_t index) const {
        return data[index];
    }
    // Overload the += operator to append another BufferLine
    FString& operator+=(const FString& other) {
        data.insert(data.end(), other.data.begin(), other.data.end());
        return *this;
    }
    // Copy assignment operator
    FString& operator=(const FString& other) {
        if (this != &other) {
            data = other.data;
        }
        return *this;
    }
    FString& operator=(const std::string& str) {
        auto d = FString::StringToUtf8(str);
        data = d.data;
        return *this;
    }
    // Move assignment operator
    FString& operator=(FString&& other) noexcept {
        if (this != &other) {
            data = std::move(other.data);
        }
        return *this;
    }
    bool empty() const {
        return data.empty();
    }
    auto begin() const {
        return data.begin();
    }
    auto end() const {
        return data.end();
    }
    void clear(){
        data.clear();
    }
    bool operator==(const FString& other) const {
        return data == other.data;
    }
    // Inequality operator (!=)
    bool operator!=(const FString& other) const {
        return !(*this == other);
    }
    // Returns -1 when no element if found.
    int find_last_of(int symbol){
        int pos = data.size() - 1;
        while(pos >= 0){
          if(data[pos]==symbol){
            break;
          }
          pos--;
        }
        return pos;
    }
    FString substr(int pos){
      FString sub;
      int endIndex = data.size();
      if(pos < 0){
          pos = 0;      
      }
      while(pos < endIndex){
        sub.push_back(data[pos]);
        pos++;
      }
      return sub;
    }
    std::string to_string(){
      return FString::Utf8ToString(*this);
    }
    std::vector<std::string> getPrint() const{
        return {"FString", FString::Utf8ToString(*this)};
    }
  
  class iterator {
    private:
        std::vector<int>::iterator it;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = int;
        using difference_type = std::ptrdiff_t;
        using pointer = int*;
        using reference = int&;

        iterator(std::vector<int>::iterator iterator) : it(iterator) {}
        iterator() {}

        // Overload dereference operator
        int& operator*() {
            return *it;
        }
        // Overload prefix increment operator (++it)
        iterator& operator++() {
            ++it;
            return *this;
        }
        // Overload postfix increment operator (it++)
        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }
        // Overload prefix decrement operator (--it)
        iterator& operator--() {
            --it;
            return *this;
        }
        // Overload postfix decrement operator (it--)
        iterator operator--(int) {
            iterator temp = *this;
            --(*this);
            return temp;
        }
        // Overload addition assignment operator (it += n)
        iterator& operator+=(difference_type n) {
            it += n;
            return *this;
        }
        // Overload addition operator (it + n)
        iterator operator+(difference_type n) const {
            iterator temp = *this;
            return temp += n;
        }
        // Overload subtraction assignment operator (it -= n)
        iterator& operator-=(difference_type n) {
            it -= n;
            return *this;
        }
        // Overload subtraction operator (it - n)
        iterator operator-(difference_type n) const {
            iterator temp = *this;
            return temp -= n;
        }
        // Overload equality operator (it == other)
        bool operator==(const iterator& other) const {
            return it == other.it;
        }
        // Overload inequality operator (it != other)
        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
        // Overload less than operator (it < other)
        bool operator<(const iterator& other) const {
            return it < other.it;
        }
        // Overload greater than operator (it > other)
        bool operator>(const iterator& other) const {
            return other < *this;
        }
        // Overload less than or equal to operator (it <= other)
        bool operator<=(const iterator& other) const {
            return !(other < *this);
        }
        // Overload greater than or equal to operator (it >= other)
        bool operator>=(const iterator& other) const {
            return !(*this < other);
        }
        // Overload subtraction operator to get the difference between iterators (it - other)
        difference_type operator-(const iterator& other) const {
            return it - other.it;
        }
    };

    // Function to get the beginning iterator
    iterator begin() {
        return iterator(data.begin());
    }

    // Function to get the ending iterator
    iterator end() {
        return iterator(data.end());
    }
    FString::iterator insert(FString::iterator pos, int value){
      std::ptrdiff_t index = pos - begin();
      data.insert(data.begin() + index, value);
      return begin() + index;
    }
    template <typename InputIt>
    FString::iterator insert(FString::iterator pos, InputIt inFirst, InputIt  inLast ){
      std::ptrdiff_t index = pos - begin();
      data.insert(data.begin()+index,inFirst, inLast );
      return begin() + index;
    }
    FString::iterator erase(FString::iterator pos){
      std::ptrdiff_t index = pos-begin();
      data.erase(data.begin()+index);
      return begin() + index;
    }
    FString::iterator erase(FString::iterator start, FString::iterator end){
      std::ptrdiff_t indexStart = start-begin();
      std::ptrdiff_t indexEnd = end-begin();
      data.erase(data.begin() + indexStart, data.begin() + indexEnd);
      return begin() + indexStart;
    }

};

typedef typename std::vector<FString> BufferData;
typedef typename std::vector<FString> FStringVector;

void BufferLineSplit(FString *bline, BufferData& data);
void BufferDataDeleteLine(BufferData& data, int lineId);

#ifdef DEBUG_INFO
void PrintDataLine(FString line);
#endif // DEBUG_INFO

#endif // __FSTRING_HPP__
