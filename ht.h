#ifndef HT_H
#define HT_H
#include <vector>
#include <iostream>
#include <cmath>




typedef size_t HASH_INDEX_T;








// Complete - Base Prober class
template <typename KeyType>
struct Prober {
    // Data members
    HASH_INDEX_T start_;    // initial hash location, h(k)
    HASH_INDEX_T m_;        // table size
    size_t numProbes_;      // probe attempts for statistic tracking
    static const HASH_INDEX_T npos = (HASH_INDEX_T)-1; // used to indicate probing failed
    void init(HASH_INDEX_T start, HASH_INDEX_T m, const KeyType& key)
    {
        (void) key;  // avoid unused argument warnings since base class doesn't use key
        start_ = start;
        m_ = m;
        numProbes_ = 0;
    }
    HASH_INDEX_T next() {
        throw std::logic_error("Not implemented...should use derived class");
    }
};




// Almost Complete - Fill in the if statement below.
template <typename KeyType>
struct LinearProber : public Prober<KeyType> {




    HASH_INDEX_T next()
    {
        // Complete the condition below that indicates failure
        // to find the key or an empty slot
        if(this->numProbes_ == this->m_) {  //If numProbes is equal to m_, that means that we have iterated over across where the map has started to where it has ended
            return this->npos;
        }
        HASH_INDEX_T loc = (this->start_ + this->numProbes_) % this->m_;
        this->numProbes_++;
        return loc;
    }
};




// To be completed
template <typename KeyType, typename Hash2>
struct DoubleHashProber : public Prober<KeyType>
{
    Hash2 h2_;              /// h2(k)
    HASH_INDEX_T dhstep_;   /// Stepsize to use for double hash probing




    /// Moduli to use for double hashing as table increases (resizes)
    static const HASH_INDEX_T DOUBLE_HASH_MOD_VALUES[];
    /// The number of elements in the array above
    static const int DOUBLE_HASH_MOD_SIZE;




    //==================================
    // Add data members, as desired
    //==================================
    HASH_INDEX_T Index; //Will be used to iterate from start in next()
    HASH_INDEX_T Index_counter; //Will be used to count how many times Index was used
private:
    // Complete
    HASH_INDEX_T findModulusToUseFromTableSize(HASH_INDEX_T currTableSize)
    {
        HASH_INDEX_T modulus = DOUBLE_HASH_MOD_VALUES[0];
        // find the modulus that is just smaller than the table size
       
       
        for(int i=0; i < DOUBLE_HASH_MOD_SIZE && DOUBLE_HASH_MOD_VALUES[i] < currTableSize; i++)
        {
            modulus = DOUBLE_HASH_MOD_VALUES[i];
           
        }
       
        return modulus;
    }
public:
    /**
     * @brief Construct a new Double Hash Prober
     *     Accepts information that must be provided outside the hash table
     *      (i.e. apart from internal hash table info/parameters)
     *        
     *
     * @param h2  h2(k) - Object with an operator()(const KeyType&) defined for it
     */
    DoubleHashProber(const Hash2& h2 = Hash2()) : h2_(h2) {}
   
    /**
     * @brief Supplies info the hash table must provide
     *
     * @param start Starting location for probing (i.e. h1(k))
     * @param m     Table size
     * @param key   Key (in case further hashing is necessary)
     */
    // Complete
    void init(HASH_INDEX_T start, HASH_INDEX_T m, const KeyType& key) //Do this thing first
    {
        Prober<KeyType>::init(start, m, key);
        HASH_INDEX_T modulus = findModulusToUseFromTableSize(m);
        // Compute probe stepsize given modulus and h2(k)
        dhstep_ = modulus - h2_(key) % modulus;
        Index = 0;
        Index_counter = 0;
    }




    // To be completed
    HASH_INDEX_T next()
    {
        if(Index_counter == 0){ //If the index is 0, then we just return Index = this->start_
            Index = this->start_;
            ++Index_counter;
            return Index;
        }else if(Index_counter == this->m_){  //If the counter is equal to the table size m, then the hashtable should end
            return this->npos;
        }
        HASH_INDEX_T loc = (Index + dhstep_) % this->m_;
        Index = loc;
        ++Index_counter;
        return Index;
    }
};




// Initialization of static array (do not alter)
template <typename KeyType, typename Hash2>
const HASH_INDEX_T DoubleHashProber<KeyType, Hash2>::DOUBLE_HASH_MOD_VALUES[] =
{
    7, 19, 43, 89, 193, 389, 787, 1583, 3191, 6397, 12841, 25703, 51431, 102871,
    205721, 411503, 823051, 1646221, 3292463, 6584957, 13169963, 26339921, 52679927,
    105359939, 210719881, 421439749, 842879563, 1685759113
};




// Initialization of static array size (do not alter)
template <typename KeyType, typename Hash2>
const int DoubleHashProber<KeyType, Hash2>::DOUBLE_HASH_MOD_SIZE =
    sizeof(DoubleHashProber<KeyType, Hash2>::DOUBLE_HASH_MOD_VALUES)/sizeof(HASH_INDEX_T);




// Hash Table Interface
template<
    typename K,
    typename V,
    typename Prober = LinearProber<K>,
    typename Hash = std::hash<K>,
    typename KEqual = std::equal_to<K> >
class HashTable
{
public:
    typedef K KeyType;
    typedef V ValueType;
    typedef std::pair<KeyType, ValueType> ItemType;
    typedef Hash Hasher;
    struct HashItem {
        ItemType item;
        bool deleted;
        HashItem(const ItemType& newItem){
            item = newItem;
            deleted = false;
        }
    };




    /**
     * @brief Construct a new Hash Table object
     *
     * @param resizeAlpha Loading factor threshold at which the table should resize
     * @param prober Probing object of type Prober
     * @param hash Hash functor that supports hash(key) and returns a HASH_INDEX_T
     * @param kequal Functor that checks equality of two KeyType objects
     */
    HashTable(
        double resizeAlpha = 0.4,
        const Prober& prober = Prober(),
        const Hasher& hash = Hasher(),
        const KEqual& kequal = KEqual());




    /**
     * @brief Destroy the Hash Table object and delete all remaining
     *         key,value pairs
     *
     */
    ~HashTable();




    /**
     * @brief Returns true if the table has no non-deleted key,value pairs,
     *        and false otherwise
     *
     */
    bool empty() const;




    /**
     * @brief Returns number of (non-deleted) key,value pairs in the table
     *
     * @return size_t
     */
    size_t size() const;




    /**
     * @brief Inserts a new item into the map, or, if an item with the
     *        given key already exists, it updates the Value of that item
     *        with the second value of the pair, p
     *
     * @param p Pair to insert  
     * @throw std::logic_error If no free location can be found
     */
    void insert(const ItemType& p);




    /**
     * @brief Removes (marks as deleted) the item with the given key.  
     * Does nothing if an item with the given key does not exist.
     *
     * @param key
     */
    void remove(const KeyType& key);




    /**
     * @brief Finds an item with the given key and returns a pointer
     * to the key,value pair
     *
     * @param key
     * @return ItemType const* nullptr is returned if the key does not exist
     */
    ItemType const * find(const KeyType& key) const;
    ItemType * find(const KeyType& key);




    /**
     * @brief Returns the value corresponding to the given key
     *
     * @param key
     * throw std::out_of_range if the key does not exist
     * @return ValueType Value associated with key
     */
    const ValueType& at(const KeyType& key) const;
    ValueType& at(const KeyType& key);
    const ValueType& operator[](const KeyType& key) const;
    ValueType& operator[](const KeyType& key);




    // Debug / Performance functions
    void reportAll(std::ostream& out) const;
    void clearTotalProbes() { totalProbes_ = 0; }
    size_t totalProbes() const { return totalProbes_; }
private:
    /**
     * @brief Helper routine to find a given key
     *
     * @param key
     * @return HashItem* returns nullptr if key does not exist
     */
    HashItem * internalFind(const KeyType& key) const;
    /**
     * @brief Performs the probing sequence and returns the index
     * of the table location with the given key or the location where
     * key can be inserted (i.e. the index now contains nullptr) but is
     * available.
     *
     * @param key
     * @return returns npos is the key does not exist and
     * no free location is available
     */
    HASH_INDEX_T probe(const KeyType& key) const;




    // Constant to signify an invalid hash location is being returned
    static const HASH_INDEX_T npos = Prober::npos;




    /**
     * @brief Resizes the hash table replacing the old with a new
     * table of the next prime size given in CAPACITIES.  Must rehash
     * all non-deleted items while freeing all deleted items.
     *
     * Must run in O(m) where m is the new table size
     *
     * @throws std::logic_error if no more CAPACITIES exist
     */
    void resize();




    // Data members
    std::vector<HashItem*> table_; // actual hash table
    Hasher hash_;  
    KEqual kequal_;
    mutable Prober prober_;  // mutable allows const member functions to modify this member
    // debug/performance counters
    mutable size_t totalProbes_; // mutable allows const member functions to modify this member
    // prime capacities to be used when resizing/rehashing is needed
    static const HASH_INDEX_T CAPACITIES[];
    HASH_INDEX_T mIndex_;  // index to CAPACITIES




    // ADD MORE DATA MEMBERS HERE, AS NECESSARY
    size_t size_counter;  //This will count how many indexes of the hashtable vector is filled up with false deleted bools
    size_t not_nullptr_counter;   //This will count how many non nullptr HashItems are there in the vector
    double resizeAlpha_;
};




// ----------------------------------------------------------------------------
//                           Hash Table Implementation
// ----------------------------------------------------------------------------




// Static array of prime table sizes
template<typename K, typename V, typename Prober, typename Hash, typename KEqual>
const HASH_INDEX_T HashTable<K,V,Prober,Hash,KEqual>::CAPACITIES[] =
    {
        11, 23, 47, 97, 197, 397, 797, 1597, 3203, 6421, 12853, 25717, 51437, 102877,
        205759, 411527, 823117, 1646237, 3292489, 6584983, 13169977, 26339969, 52679969,
        105359969, 210719881, 421439783, 842879579, 1685759167
    };




// To be completed
template<typename K, typename V, typename Prober, typename Hash, typename KEqual>
HashTable<K,V,Prober,Hash,KEqual>::HashTable(
    double resizeAlpha, const Prober& prober, const Hasher& hash, const KEqual& kequal)
       :  hash_(hash), kequal_(kequal), prober_(prober), mIndex_(0), size_counter(0), not_nullptr_counter(0), resizeAlpha_(resizeAlpha)
{
    // Initialize any other data members as necessary
    table_.resize(CAPACITIES[mIndex_]);
}




// To be completed
template<typename K, typename V, typename Prober, typename Hash, typename KEqual>
HashTable<K,V,Prober,Hash,KEqual>::~HashTable()
{
    //std::cout << "DESTRUCTOR" << std::endl;
    for(size_t i = 0; i < table_.size(); ++i){
        if(table_[i] != nullptr){
            delete table_[i];
        }
    }
}




// To be completed
template<typename K, typename V, typename Prober, typename Hash, typename KEqual>
bool HashTable<K,V,Prober,Hash,KEqual>::empty() const
{
    return size_counter == 0;
}




// To be completed
template<typename K, typename V, typename Prober, typename Hash, typename KEqual>
size_t HashTable<K,V,Prober,Hash,KEqual>::size() const
{
    return size_counter;
}




/**
     * @brief Inserts a new item into the map, or, if an item with the
     *        given key already exists, it updates the Value of that item
     *        with the second value of the pair, p
     *
     * @param p Pair to insert  
     * @throw std::logic_error If no free location can be found
     */
// To be completed
template<typename K, typename V, typename Prober, typename Hash, typename KEqual>
void HashTable<K,V,Prober,Hash,KEqual>::insert(const ItemType& p)
{
   double current_alpha = (double)(not_nullptr_counter)/(double)(CAPACITIES[mIndex_]);
   if(current_alpha >= resizeAlpha_){
      resize();
   }
    HASH_INDEX_T index = this->probe(p.first);
    if(index >= CAPACITIES[mIndex_]){  //If this is greater, then throw
        throw std::logic_error("The size has reached its limit");
    }


    //We also need to increase size of if the HashItem is deleted true and then increase the size when convertting the HashItem to deleted false
    if(table_[index] == nullptr){  //If it is nullptr, then insert
        HashItem* new_Item = new HashItem(p); //New makes it that the deleted should be false
        table_[index] = new_Item;
        ++size_counter;
        ++not_nullptr_counter;
    }else if(table_[index] != nullptr/* && table_[index]->item.first == p.first*/){ //If the keys are the same
        if(table_[index]->item.first == p.first && table_[index]->deleted == false){ //The left part of the == sign is the problem
            table_[index]->item.second = p.second;
        }else if(table_[index]->item.first == p.first && table_[index]->deleted == true){ //If deleted is true, then we must convert it back to false and then the size increases by 1
             table_[index]->item.second = p.second;
             table_[index]->deleted = false;
             ++size_counter;
             ++not_nullptr_counter;
        }
    }
}




// To be completed




 /**
     * @brief Removes (marks as deleted) the item with the given key.  
     * Does nothing if an item with the given key does not exist.
     *
     * @param key
  */
template<typename K, typename V, typename Prober, typename Hash, typename KEqual>
void HashTable<K,V,Prober,Hash,KEqual>::remove(const KeyType& key)
{
    HASH_INDEX_T index = this->probe(key);
    if(table_[index] == nullptr){ //If it is nullptr, do nothing

    }else if(table_[index] != nullptr){ //If it is not nullptr, then mark as deleted
             if(table_[index]->deleted == true){  //If it is true, then don't need to decrease size counter

             }else{//If it is false, then do need to decrease size counter
                  table_[index]->deleted = true;
                  --size_counter;
             }
    }
}








// Complete
template<typename K, typename V, typename Prober, typename Hash, typename KEqual>
typename HashTable<K,V,Prober,Hash,KEqual>::ItemType const * HashTable<K,V,Prober,Hash,KEqual>::find(const KeyType& key) const
{
    //std::cout << "FIND 1" << std::endl;
    HASH_INDEX_T h = this->probe(key);
    if((npos == h) || nullptr == table_[h] ){
        return nullptr;
    }
    return &table_[h]->item;
}




// Complete
template<typename K, typename V, typename Prober, typename Hash, typename KEqual>
typename HashTable<K,V,Prober,Hash,KEqual>::ItemType * HashTable<K,V,Prober,Hash,KEqual>::find(const KeyType& key)
{
    HASH_INDEX_T h = this->probe(key);
    if((npos == h) || nullptr == table_[h] || table_[h]->deleted == true){
        return nullptr;
    }
    return &table_[h]->item;
}




// Complete
template<typename K, typename V, typename Prober, typename Hash, typename KEqual>
const typename HashTable<K,V,Prober,Hash,KEqual>::ValueType& HashTable<K,V,Prober,Hash,KEqual>::at(const KeyType& key) const
{
    HashItem const * item = this->internalFind(key);
    if(item == nullptr) { throw std::out_of_range("Bad key"); }
    return item->item.second;
}




// Complete
template<typename K, typename V, typename Prober, typename Hash, typename KEqual>
typename HashTable<K,V,Prober,Hash,KEqual>::ValueType& HashTable<K,V,Prober,Hash,KEqual>::at(const KeyType& key)
{
    HashItem * item = this->internalFind(key);
    if(item == nullptr) { throw std::out_of_range("Bad key"); }
    return item->item.second;
}




// Complete
template<typename K, typename V, typename Prober, typename Hash, typename KEqual>
const typename HashTable<K,V,Prober,Hash,KEqual>::ValueType& HashTable<K,V,Prober,Hash,KEqual>::operator[](const KeyType& key) const
{
    return this->at(key);
}




// Complete
template<typename K, typename V, typename Prober, typename Hash, typename KEqual>
typename HashTable<K,V,Prober,Hash,KEqual>::ValueType& HashTable<K,V,Prober,Hash,KEqual>::operator[](const KeyType& key)
{
    return this->at(key);
}




// Complete
template<typename K, typename V, typename Prober, typename Hash, typename KEqual>
typename HashTable<K,V,Prober,Hash,KEqual>::HashItem* HashTable<K,V,Prober,Hash,KEqual>::internalFind(const KeyType& key) const
{
    HASH_INDEX_T h = this->probe(key);
    if((npos == h) || nullptr == table_[h] ){
        return nullptr;
    }
    return table_[h];
}








// To be completed
template<typename K, typename V, typename Prober, typename Hash, typename KEqual>
void HashTable<K,V,Prober,Hash,KEqual>::resize()
{
    ++mIndex_;  //Increase to the next CAPACITIES array value which is the next prime number
    std::vector<HashItem*> temp;
    not_nullptr_counter = 0;
    for(size_t i = 0; i < CAPACITIES[mIndex_]; ++i){
        temp.push_back(nullptr);
    }
    for(size_t i = 0; i < table_.size(); ++i){  //Loop through table_ to find the non nullptr values to be insertted in the new temp vector
        if(table_[i] != nullptr){ //If it is not nullptr, then we need to place them in the new resized temp vector
            if(table_[i]->deleted == false){ //If it is not nullptr, then we need to place them in the new resized temp vector
                HASH_INDEX_T h = hash_(table_[i]->item.first) % CAPACITIES[mIndex_];
                ++not_nullptr_counter;
                temp[h] = table_[i];
            }else{
                delete table_[i];
                table_[i] = nullptr;
            }
        }
    }
    table_ = temp;
}




// Almost complete
template<typename K, typename V, typename Prober, typename Hash, typename KEqual>
HASH_INDEX_T HashTable<K,V,Prober,Hash,KEqual>::probe(const KeyType& key) const
{
    HASH_INDEX_T h = hash_(key) % CAPACITIES[mIndex_];
    prober_.init(h, CAPACITIES[mIndex_], key);
    HASH_INDEX_T loc = prober_.next();
    totalProbes_++;
    while(Prober::npos != loc)
    {
        if(nullptr == table_[loc]/* || table_[loc]->deleted == true*/) { //Need to detect the deleted being equal to true which is kind of like nullptr
            return loc;
        }
       
        // fill in the condition for this else if statement which should
        // return 'loc' if the given key exists at this location
        //Might have to compare the second of the pair in order for this if condition to work
        else if(table_[loc]->item.first == key && table_[loc]->deleted == false) { //Only detects keys that have existing HashItem
            return loc;
        }
        loc = prober_.next();
        totalProbes_++;
    }
    return npos;
}




// Complete
template<typename K, typename V, typename Prober, typename Hash, typename KEqual>
void HashTable<K, V, Prober, Hash, KEqual>::reportAll(std::ostream& out) const
{
  for(HASH_INDEX_T i = 0; i < CAPACITIES[mIndex_]; ++i)
  {
    if(table_[i] != nullptr)
    {
      out << std::boolalpha << "Bucket " << i << ": " << table_[i]->item.first << " " << table_[i]->item.second << " " << table_[i]->deleted << std::endl;
    }
  }
}




#endif







