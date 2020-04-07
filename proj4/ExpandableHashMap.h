#ifndef expandable_hashmap
#define expandable_hashmap
#include <iostream>
#include <vector>
#include <list>
#include "provided.h"

template <typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
    ExpandableHashMap(double maximumLoadFactor = 0.5);
    ~ExpandableHashMap();
    void reset();
    int size() const;
    void associate(const KeyType& key, const ValueType& value);

      // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;

      // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
    }
    unsigned int getBucketNumber(const KeyType& key) const;
      // C++11 syntax for preventing copying and assignment
    ExpandableHashMap(const ExpandableHashMap&) = delete;
    ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;
    
private:
    //size of this returns the number of buckets in a hashmap

    std::vector<std::list<std::pair<KeyType, ValueType> > > m_values;
    //std::vector<KeyType> m_keys;
    //number of keys in the hashmap
    int m_size;
    double m_maxLoadFactor;
};

template <typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor){
    m_size = 0;
    if(maximumLoadFactor > 0)
        m_maxLoadFactor = maximumLoadFactor;
    else
        m_maxLoadFactor = 0.5;
    for(int i = 0; i < 8; i++){
        std::list<std::pair<KeyType,ValueType> > temp;
        m_values.push_back(temp);
    }
    
}

template <typename KeyType, typename ValueType>
unsigned int ExpandableHashMap<KeyType, ValueType>::getBucketNumber(const KeyType& key) const
{
    unsigned int hasher(const KeyType& k); // prototype
    unsigned int h = hasher(key);
    return h % m_values.size();
}


template <typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap()
{

}

template <typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset()
{

    std::vector<std::list<std::pair<KeyType, ValueType> > > temp;
    for(int i = 0; i < 8; i++){
        std::list<std::pair<KeyType,ValueType> > tempList;
        temp.push_back(tempList);
    }
    m_values = temp;
    std::cerr <<temp.size() << "SIZe" << m_values.size() << std::endl;
    m_size = 0;
}


template <typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const
{
    return m_size;  // Delete this line and implement this function correctly
}


// The associate method associates one item (key) with another (value).
// If no association currently exists with that key, this method inserts
// a new association into the hashmap with that key/value pair. If there is
// already an association with that key in the hashmap, then the item
// associated with that key is replaced by the second parameter (value).
// Thus, the hashmap must contain no duplicate keys.
template <typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    //Checks if there is already a node in the hash map with that key
    if(this->find(key) != nullptr){
        *(this->find(key)) = value;
        return;
    }
    m_size++;
        
    //Checks if the hash map is hitting the load factor and needs to be rehashed

    
    if(m_size/(double)(m_values.size()) > m_maxLoadFactor){
        ExpandableHashMap<KeyType, ValueType> temp(m_maxLoadFactor);
        int initSize = m_values.size();
        //adds twice the buckets to the temp hashmap for rehashing
        for(int i = 8; i < initSize*2; i++){
            std::list<std::pair<KeyType,ValueType> > tempList;
            temp.m_values.push_back(tempList);
        }
        //rehashing
        for(int i = 0; i < m_values.size(); i++){
            for(auto j = m_values[i].begin(); j != m_values[i].end(); j++){
                temp.m_values[temp.getBucketNumber(j->first)].push_back(std::make_pair(j->first,j->second));
            }
        }
        //setting original to temp
        m_values = temp.m_values;
        //This is to make sure the nodes at the pointers aren't deleted when temps destructor is called since we need them in the other hash map were using
    }
    //Since the key is not present in the hash map, this adds the key value pair to the hash map
    m_values[getBucketNumber(key)].push_back(std::make_pair(key,value));
    
}


// If no association exists with the given key, return nullptr; otherwise,
// return a pointer to the value associated with that key. This pointer can be
// used to examine that value, and if the hashmap is allowed to be modified, to
// modify that value directly within the map (the second overload enables
// this). Using a little C++ magic, we have implemented it in terms of the
// first overload, which you must implement.
template <typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
{
    if(m_values.size() == 0)
        return nullptr;
    
    int h = getBucketNumber(key);
    for(auto iter = m_values[h].begin(); iter != m_values[h].end(); iter++){
        if(iter->first == key)
            return &(iter->second);
    }
    return nullptr;
}

#endif //expandable_hashmap
