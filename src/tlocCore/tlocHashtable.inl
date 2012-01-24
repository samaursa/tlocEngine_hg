#ifndef TLOC_HASH_TABLE_INL
#define TLOC_HASH_TABLE_INL

#ifndef TLOC_HASH_TABLE_H
#error "Must include header before including the inline file"
#endif

#include "tlocAlgorithms.inl"
#include "tlocIterator.inl"

namespace tloc { namespace core {

  //////////////////////////////////////////////////////////////////////////
  // Hashtable Iterator Base

#define HASHTABLE_ITR_BASE_TYPES typename T_Policies, bool T_Const
#define HASHTABLE_ITR_BASE_PARAMS T_Policies, T_Const

  template <HASHTABLE_ITR_BASE_TYPES>
  TL_FI HashtableItrBase<HASHTABLE_ITR_BASE_PARAMS>::
    HashtableItrBase( bucket_array_type* a_bucketContainer) 
    : m_bucketContainer(a_bucketContainer)
    , m_currBucket( a_bucketContainer.begin() )
    , m_currNode( (*(a_bucketContainer.begin() )).begin())
  {
  }

  template <HASHTABLE_ITR_BASE_TYPES>
  TL_FI HashtableItrBase<HASHTABLE_ITR_BASE_PARAMS>
    ::HashtableItrBase( bucket_array_type* a_bucketContainer, 
                        const local_iterator& a_currBucket, 
                        const bucket_iterator& a_currNode)
                        : m_bucketContainer(a_bucketContainer)
                        , m_currNode(a_currNode)
                        , m_currBucket(a_currBucket)
  {
  }


  template <HASHTABLE_ITR_BASE_TYPES>
  TL_FI void HashtableItrBase<HASHTABLE_ITR_BASE_PARAMS>::Increment()
  {
    bucket_array_type::const_iterator bucketEnd = m_bucketContainer->end();
    bucket_type::const_iterator itrEnd = (*m_currBucket).end();
    ++m_currNode;

    while(m_currNode == itrEnd)
    {
      ++m_currBucket;

      if (m_currBucket != bucketEnd)
      {
        m_currNode = *(m_currBucket).begin();
        itrEnd = *(m_currBucket).end();
      }
      else
      {
        m_currBucket = m_bucketContainer->begin();
        m_currNode = *(m_bucketContainer)->begin();
        break;
      }
    }
  }

  template <HASHTABLE_ITR_BASE_TYPES>
  TL_FI void HashtableItrBase<HASHTABLE_ITR_BASE_PARAMS>::IncrementBucket()
  {
    local_iterator bucketEnd = m_bucketContainer->end();

    TLOC_ASSERT_HASH_TABLE(m_currBucket != bucketEnd,
      "Already at the end of the bucket container!");

    ++m_currBucket;
    while( (*(m_currBucket)).size() == 0 )
    {
      ++m_currBucket;
      if (m_currBucket != bucketEnd)
      {
        m_currNode = (*(m_currBucket)).begin();
      }
      else
      {
        m_currBucket = m_bucketContainer->begin();
        m_currNode = (*(m_currBucket)).begin();
      }
    }
  }

  template <HASHTABLE_ITR_BASE_TYPES>
  TL_FI typename HashtableItrBase<HASHTABLE_ITR_BASE_PARAMS>::this_type&
    HashtableItrBase<HASHTABLE_ITR_BASE_PARAMS>
    ::operator = (const typename HashtableItrBase<HASHTABLE_ITR_BASE_PARAMS>::this_type &a_other)
  {
    m_currNode				= a_other.m_currNode;
    m_bucketContainer	= a_other.m_bucketContainer;
    m_currBucket			= a_other.m_currBucket;

    return *this;
  }

  template <HASHTABLE_ITR_BASE_TYPES>
  TL_FI bool HashtableItrBase<HASHTABLE_ITR_BASE_PARAMS>
    ::operator == (const typename HashtableItrBase<HASHTABLE_ITR_BASE_PARAMS>::this_type &a_other)
  {
    return ( m_currNode				 == a_other.m_currNode &&
             m_currBucket			 == a_other.m_currBucket );
  }

  template <HASHTABLE_ITR_BASE_TYPES>
  TL_FI bool HashtableItrBase<HASHTABLE_ITR_BASE_PARAMS>
    ::operator != (const typename HashtableItrBase<HASHTABLE_ITR_BASE_PARAMS>::this_type &a_other)
  {
    return !(operator==(a_other));
  }

  template <HASHTABLE_ITR_BASE_TYPES>
  const typename HashtableItrBase<HASHTABLE_ITR_BASE_PARAMS>::bucket_iterator&
    HashtableItrBase<HASHTABLE_ITR_BASE_PARAMS>::GetNode() const
  { 
    return m_currNode;
  }

  template <HASHTABLE_ITR_BASE_TYPES>
  const typename HashtableItrBase<HASHTABLE_ITR_BASE_PARAMS>::bucket_array_type*
    HashtableItrBase<HASHTABLE_ITR_BASE_PARAMS>::GetBucketArray() const
  { 
    return m_bucketContainer;
  }

  template <HASHTABLE_ITR_BASE_TYPES>
  const typename HashtableItrBase<HASHTABLE_ITR_BASE_PARAMS>::local_iterator&
    HashtableItrBase<HASHTABLE_ITR_BASE_PARAMS>::GetCurrBucket() const
  { 
    return m_currBucket;
  }

  template <HASHTABLE_ITR_BASE_TYPES>
  Pair<bool, typename HashtableItrBase<HASHTABLE_ITR_BASE_PARAMS>::size_type>
    HashtableItrBase<HASHTABLE_ITR_BASE_PARAMS>::GetCurrBucketNumber() const
  {
    bucket_array_type::const_iterator itr    = m_bucketContainer->begin();
    bucket_array_type::const_iterator itrEnd = m_bucketContainer->end();

    size_type count = 0;

    for (; itr != itrEnd; ++itr)
    {
      if (itr == m_currBucket) { return MakePair(true, count); }
      ++count;
    }

    // We have an invalid iterator
    return MakePair(false, count);
  }

  //------------------------------------------------------------------------
  // Hashtable Iterator

#define HASHTABLE_ITR_TYPES   typename T_Policies, bool T_Const
#define HASHTABLE_ITR_PARAMS  T_Policies, T_Const

  template <HASHTABLE_ITR_TYPES>
  HashtableItr<HASHTABLE_ITR_PARAMS>::HashtableItr(bucket_array_type* a_bucketContainer) 
    : base_type(a_bucketContainer, a_bucketContainer->begin(), 
                (*(a_bucketContainer->begin())).begin() ) 
  {
  }

  template <HASHTABLE_ITR_TYPES>
  HashtableItr<HASHTABLE_ITR_PARAMS>::HashtableItr(bucket_array_type* a_bucketContainer, 
    const local_iterator& a_currBucket, const bucket_iterator& a_currNode) 
    : base_type(a_bucketContainer, a_currBucket, a_currNode) 
  {
  }

  template <HASHTABLE_ITR_TYPES>
  HashtableItr<HASHTABLE_ITR_PARAMS>::HashtableItr(const this_type_non_const& a_other)
      : base_type(a_other.m_bucketContainer, a_other.m_currBucket, 
                  a_other.m_currNode)
  {
  }

  template <HASHTABLE_ITR_TYPES>
  typename HashtableItr<HASHTABLE_ITR_PARAMS>::reference 
    HashtableItr<HASHTABLE_ITR_PARAMS>::operator *() const
  { 
    return (*(base_type::m_currNode)).m_value();
  }

  template <HASHTABLE_ITR_TYPES>
  typename HashtableItr<HASHTABLE_ITR_PARAMS>::pointer
    HashtableItr<HASHTABLE_ITR_PARAMS>::operator ->() const
  { 
    return &(base_type::m_currNode->m_value() ); 
  }

  template <HASHTABLE_ITR_TYPES>
  typename HashtableItr<HASHTABLE_ITR_PARAMS>::this_type&
    HashtableItr<HASHTABLE_ITR_PARAMS>::operator ++() 
  { 
    base_type::Increment(); return *this; 
  }

  template <HASHTABLE_ITR_TYPES>
  typename HashtableItr<HASHTABLE_ITR_PARAMS>::this_type
    HashtableItr<HASHTABLE_ITR_PARAMS>::operator ++(int) 
  { 
    this_type temp(*this); base_type::Increment(); return temp; 
  }

  //////////////////////////////////////////////////////////////////////////
  // Hashtable

#define HASH_TABLE_TYPES typename T_Policies
#define HASH_TABLE_PARAMS T_Policies

  template <HASH_TABLE_TYPES>
  typename Hashtable<HASH_TABLE_PARAMS>::bucket_iterator
    Hashtable<HASH_TABLE_PARAMS>::m_dummyNode =
    typename Hashtable<HASH_TABLE_PARAMS>::bucket_type::iterator();

  template <HASH_TABLE_TYPES>
  Hashtable<HASH_TABLE_PARAMS>::Hashtable()
    : m_elementCount(0), m_rehashPolicy()
  {
  }

  template <HASH_TABLE_TYPES>
  TL_FI Hashtable<HASH_TABLE_PARAMS>::Hashtable(size_type a_bucketCount)
    : m_elementCount(0)
  {
    TLOC_ASSERT_HASH_TABLE(a_bucketCount < 10000000, "Bucket count is too large!");

    size_type newBucketCount =
      (size_type)m_rehashPolicy.get_next_bucket_count((u32)a_bucketCount);
    DoAllocateBuckets(newBucketCount);
  }

  template <HASH_TABLE_TYPES>
  TL_FI Hashtable<HASH_TABLE_PARAMS>::Hashtable(const this_type& a_other)
    : m_bucketArray(a_other.m_bucketArray)
    , m_elementCount(a_other.m_elementCount)
    , m_rehashPolicy(a_other.m_rehashPolicy)
  {
  }

  template <HASH_TABLE_TYPES>
  TL_FI Hashtable<HASH_TABLE_PARAMS>::~Hashtable()
  {
  }

  template <HASH_TABLE_TYPES>
  TL_FI void Hashtable<HASH_TABLE_PARAMS>
    ::DoAllocateBuckets(size_type a_numBuckets)
  {
    m_bucketArray.resize(a_numBuckets);
  }

  //------------------------------------------------------------------------
  // Iterator access

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::iterator
    Hashtable<HASH_TABLE_PARAMS>::begin()
  {
    iterator itr(&m_bucketArray);
    if ( (*(itr.m_currBucket)).empty())
    {
      itr.IncrementBucket();
    }
    return itr;
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::const_iterator
    Hashtable<HASH_TABLE_PARAMS>::begin() const
  {
    const_iterator itr(&m_bucketArray);
    if ( (*(itr.m_currBucket)).empty() )
    {
      itr.IncrementBucket();
    }
    return itr;
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::iterator
    Hashtable<HASH_TABLE_PARAMS>::end()
  {
    return iterator(&m_bucketArray, m_bucketArray.end(), m_dummyNode);
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::const_iterator
    Hashtable<HASH_TABLE_PARAMS>::end() const
  {
    return const_iterator(&m_bucketArray, m_bucketArray.end(), m_dummyNode);
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::bucket_iterator
    Hashtable<HASH_TABLE_PARAMS>
    ::begin(typename Hashtable<HASH_TABLE_PARAMS>::size_type a_bucketNumber)
  {
    buckets_array_type::iterator itr(m_bucketArray.begin());
    advance(itr, a_bucketNumber);
    return (*(itr)).begin();
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::const_bucket_iterator
    Hashtable<HASH_TABLE_PARAMS>
    ::begin(typename Hashtable<HASH_TABLE_PARAMS>::size_type a_bucketNumber) const
  {
    buckets_array_type::const_iterator itr(m_bucketArray.begin());
    advance(itr, a_bucketNumber);
    return (*(itr)).begin();
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::bucket_iterator
    Hashtable<HASH_TABLE_PARAMS>
    ::end(typename Hashtable<HASH_TABLE_PARAMS>::size_type a_bucketNumber)
  {
    buckets_array_type::iterator itr(m_bucketArray.end());
    advance(itr, a_bucketNumber);
    return (*(itr)).end();
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::const_bucket_iterator
    Hashtable<HASH_TABLE_PARAMS>
    ::end(typename Hashtable<HASH_TABLE_PARAMS>::size_type a_bucketNumber) const
  {
    buckets_array_type::const_iterator itr(m_bucketArray.end());
    advance(itr, a_bucketNumber);
    return (*(itr)).end();
  }

  //------------------------------------------------------------------------
  // Capacity

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::size_type
    Hashtable<HASH_TABLE_PARAMS>::size() const
  {
    return m_elementCount;
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::size_type
    Hashtable<HASH_TABLE_PARAMS>::bucket_count() const
  {
    return (size_type)m_bucketArray.size();
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::size_type
    Hashtable<HASH_TABLE_PARAMS>::bucket_size
    (typename Hashtable<HASH_TABLE_PARAMS>::size_type a_bucketNumber) const
  {
    buckets_array_type::const_iterator itr(m_bucketArray.begin());
    advance(itr, a_bucketNumber);
    return (*itr).size();
  }

  //------------------------------------------------------------------------
  // Hashing queries

  template <HASH_TABLE_TYPES>
  TL_FI f32 Hashtable<HASH_TABLE_PARAMS>::load_factor() const
  {
    return (f32)m_elementCount / (f32)bucket_count();
  }

  template <HASH_TABLE_TYPES>
  TL_FI const typename Hashtable<HASH_TABLE_PARAMS>::rehash_policy_type&
    Hashtable<HASH_TABLE_PARAMS>::get_rehash_policy() const
  {
    return m_rehashPolicy;
  }

  template <HASH_TABLE_TYPES>
  TL_FI void Hashtable<HASH_TABLE_PARAMS>::set_rehash_policy(const
    typename Hashtable<HASH_TABLE_PARAMS>::rehash_policy_type& a_rehashPolicy)
  {
    m_rehashPolicy = a_rehashPolicy;
  }

  template <HASH_TABLE_TYPES>
  TL_FI f32 Hashtable<HASH_TABLE_PARAMS>::get_max_load_factor() const
  {
    return DoGetMaxLoadFactor(m_rehashPolicy);
  }

  template <HASH_TABLE_TYPES>
  TL_FI void Hashtable<HASH_TABLE_PARAMS>::
    set_max_load_factor(f32 a_maxLoadFactor)
  {
    DoSetMaxLoadFactor(a_maxLoadFactor, m_rehashPolicy);
  }

  //------------------------------------------------------------------------
  // Modifiers

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::insert_return_type
    Hashtable<HASH_TABLE_PARAMS>::insert(const value_type& a_value)
  {
    return DoInsertValue(a_value, unique_keys());
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::iterator
    Hashtable<HASH_TABLE_PARAMS>::insert(const_iterator, 
                                         const value_type& a_value)
  {
    // First argument ignored as it is useless for a hashtable. Insert() has
    // the same signature as the other functions for generic algorithms to work
    // properly. 

    return insert_return_selector(DoInsertValue(a_value, unique_keys()) );
  }

  template <HASH_TABLE_TYPES>
  template <typename T_InputItr>
  void Hashtable<HASH_TABLE_PARAMS>::insert(T_InputItr a_first, 
                                            T_InputItr a_last)
  {
    for (; a_first != a_last; ++a_first)
    {
      DoInsertValue(*a_first, unique_keys());
    }
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::iterator
    Hashtable<HASH_TABLE_PARAMS>::erase(iterator a_position)
  {
    // This operation will be a little slower for a bucket that is singly_linked
    // or for a vector<> that maintains element order when erasing

    return DoErase(a_position, bucket_iterator_type()); 
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::iterator
    Hashtable<HASH_TABLE_PARAMS>::erase(iterator a_first, iterator a_last)
  {
    for (; a_first != a_last; ++a_first)
    {
      DoErase(a_first, bucket_iterator_type());
    }

    return a_first;
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::reverse_iterator
    Hashtable<HASH_TABLE_PARAMS>::erase(reverse_iterator a_position)
  {
    return reverse_iterator(DoErase((++a_position).base()) ); 
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::reverse_iterator
    Hashtable<HASH_TABLE_PARAMS>::erase(reverse_iterator a_first,  
                                        reverse_iterator a_last)
  {
    return reverse_iterator(erase((++a_last).base(), (++a_first).base()) );
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::size_type
    Hashtable<HASH_TABLE_PARAMS>::erase(const key_type& a_key)
  {
    DoErase(a_key, unique_keys());
  }

  template <HASH_TABLE_TYPES>
  TL_FI void Hashtable<HASH_TABLE_PARAMS>::clear()
  {
    m_bucketArray.clear();
  }

  //------------------------------------------------------------------------
  // Operations

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::iterator
    Hashtable<HASH_TABLE_PARAMS>::find(const key_type& a_key)
  {
    const hash_code_type hc = get_hash_code(a_key);
    return find_by_hash(hc);
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::const_iterator
    Hashtable<HASH_TABLE_PARAMS>::find(const key_type& a_key) const
  {
    const hash_code_type hc = get_hash_code(a_key);
    return find_by_hash(hc);
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::iterator
    Hashtable<HASH_TABLE_PARAMS>::find_by_hash(u32 a_hashCode)
  {
    const size_type n = (size_type)bucket_index(a_hashCode, (u32)bucket_count());

    buckets_array_type::iterator itr = m_bucketArray.begin();
    advance(itr, n);

    bucket_type::iterator itrN    = (*itr).begin();
    bucket_type::iterator itrNEnd = (*itr).end();

    while (itrN != itrNEnd)
    {
      if (compare(a_hashCode, &(*itrN)))
      {
        return iterator(&m_bucketArray, itr, itrN);
      }

      ++itrN;
    }

    return end();
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::const_iterator
    Hashtable<HASH_TABLE_PARAMS>::find_by_hash(u32 a_hashCode) const
  {
    const_iterator itr = (remove_const(this))->find_by_hash(a_hashCode);
    add_const(this);
    return itr;
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::size_type
    Hashtable<HASH_TABLE_PARAMS>::count(const key_type& a_key) const
  {
    return DoCount(a_key, unique_keys());
  }

  template <HASH_TABLE_TYPES>
  TL_FI Pair<typename Hashtable<HASH_TABLE_PARAMS>::iterator, 
    typename Hashtable<HASH_TABLE_PARAMS>::iterator>
    Hashtable<HASH_TABLE_PARAMS>::equal_range(const key_type& a_key)
  {
    // This function assumes unique_keys is false as it does not make sense
    // to call this function when unique_keys is true

    const hash_code_type hc = get_hash_code(a_key);
    iterator itr = find_by_hash(a_key);

    if (itr == end()) return 0;

    bucket_iterator       itrB, itrBStart = itr.m_currNode;
    const_bucket_iterator itrBEnd = (*(itr)).end();

    ++itrB; // we have at least one match
    for (; itrB != itrBEnd; ++itrB)
    {
      if (hc != get_hash_code(*itrB))
      {
        break;
      }
    }

    return MakePair(iterator(m_bucketArray, itr, itrBStart), 
                    iterator(m_bucketArray, itr, itrB));
  }

  template <HASH_TABLE_TYPES>
  TL_FI Pair<typename Hashtable<HASH_TABLE_PARAMS>::const_iterator, 
    typename Hashtable<HASH_TABLE_PARAMS>::const_iterator>
    Hashtable<HASH_TABLE_PARAMS>::equal_range(const key_type& a_key) const
  {
    Pair<const_iterator, const_iterator> p = 
      (remove_const(this))->equal_range(a_key);
    add_const(this);
    return p;
  }

  //------------------------------------------------------------------------
  //------------------------------------------------------------------------
  // Internal functions

  //------------------------------------------------------------------------
  // Insert Helpers

  template <HASH_TABLE_TYPES>
  TL_FI Pair<typename Hashtable<HASH_TABLE_PARAMS>::iterator, bool>
    Hashtable<HASH_TABLE_PARAMS>::DoInsertValue
    (const typename Hashtable<HASH_TABLE_PARAMS>::value_type& a_value,
           typename Hashtable<HASH_TABLE_PARAMS>::keys_are_unique)
  {
    const key_type& k = extract_key(a_value);
    const hash_code_type c = get_hash_code(k);

    iterator itr = find_by_hash(c);

    // TODO: Remove extra copies created by end() here and when returning <false>
    if (itr == end())
    {
      const Pair<bool, u32> rehash = m_rehashPolicy.get_rehash_required
        ( (u32)bucket_count(), (u32)m_elementCount, (u32)1);

      if (rehash.first) { DoRehash(rehash.second); }

      element_type newElement (a_value, c);

      const size_type n = (size_type)bucket_index(c, (u32)bucket_count());
      buckets_array_type::iterator itrB = m_bucketArray.begin(); 
      advance(itrB, n);

      // Select the appropriate push() function (front or back)
      return MakePair(iterator(&m_bucketArray, itrB, 
        DoPushSelect(itrB, newElement, bucket_iterator_type()) ), true);
    }

    return MakePair(end(), false);
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::iterator
    Hashtable<HASH_TABLE_PARAMS>::DoInsertValue
    (const typename Hashtable<HASH_TABLE_PARAMS>::value_type& a_value,
           typename Hashtable<HASH_TABLE_PARAMS>::keys_are_not_unique)
  {
    const Pair<bool, u32> rehash = m_rehashPolicy.get_rehash_required
      ( (u32)bucket_count(), (u32)m_elementCount, (u32)1);

    if (rehash.first) { DoRehash(rehash.second); }

    const key_type& k = extract_key(a_value);
    const hash_code_type c = get_hash_code(k);

    iterator itr = find_by_hash(c);
    element_type newElement (a_value, c);

    if (itr != end())
    {
      buckets_array_type::iterator itrB = itr.m_currBucket;
      // Select the appropriate insert() function (insert() or insert_after())
      return iterator(m_bucketArray, itrB, 
                      DoInsertSelect(itr.m_currBucket, itr.m_currNode, 
                                     newElement, bucket_iterator_type()) );
    }
    else
    {
      const size_type n = (size_type)bucket_index(c, (u32)bucket_count());

      buckets_array_type::iterator itrB = m_bucketArray.begin(); 
      advance(itrB, n);

      // Select the appropriate push() function (front or back)
      return iterator(m_bucketArray, itrB, 
                      DoPushSelect(itrB, newElement, bucket_iterator_type()) );
    }
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::bucket_type::iterator
    Hashtable<HASH_TABLE_PARAMS>::DoPushSelect(local_iterator& a_itr,
      const element_type& a_elem, bidirectional_iterator_tag)
  {
    (*(a_itr)).push_back(a_elem);
    ++m_elementCount;

    bucket_iterator itrN = (*(a_itr)).end();
    return --itrN;
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::bucket_type::iterator
    Hashtable<HASH_TABLE_PARAMS>::DoPushSelect(local_iterator& a_itr,
      const element_type& a_elem, forward_iterator_tag)
  {
    (*(a_itr)).push_front(a_elem);
    ++m_elementCount;

    return (*(a_itr)).begin();
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::bucket_type::iterator
    Hashtable<HASH_TABLE_PARAMS>::DoInsertSelect(local_iterator& a_itr, 
      bucket_iterator a_currNode, const element_type& a_elem, 
      forward_iterator_tag)
  {
    ++m_elementCount;
    return (*(a_itr)).insert_after(a_currNode, a_elem);
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::bucket_type::iterator
    Hashtable<HASH_TABLE_PARAMS>::DoInsertSelect(local_iterator& a_itr, 
      bucket_iterator a_currNode, const element_type& a_elem, 
      bidirectional_iterator_tag)
  {
    ++m_elementCount;
    return (*(a_itr)).insert(a_currNode, a_elem);
  }

  //------------------------------------------------------------------------
  // Erase helpers

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::iterator
    Hashtable<HASH_TABLE_PARAMS>::DoErase(iterator a_position, 
                                          forward_iterator_tag)
  {
    --m_elementCount;
    return (*(a_position.m_currBucket)).erase(a_position.m_currNode);
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::iterator
    Hashtable<HASH_TABLE_PARAMS>::DoErase(iterator a_position, 
                                          bidirectional_iterator_tag)
  {
    --m_elementCount;
    return (*(a_position.m_currBucket)).erase(a_position.m_currNode);
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::size_type
    Hashtable<HASH_TABLE_PARAMS>::DoErase(const key_type& a_key, keys_are_unique)
  {
    const hash_code_type hc = get_hash_code(a_key);
    iterator itr = find_by_hash(a_key);

    if (itr == end()) return 0;

    (*(itr.m_currBucket)).erase(itr.m_currNode);
    --m_elementCount;

    return 1;

    //bucket_iterator itrB    = (*(itr)).begin();
    //bucket_iterator itrBEnd = (*(itr)).end();

    //for (; itrB != itrBEnd; ++itrB)
    //{
    //  if ( c == get_hash_code(*itrB))
    //  {
    //    (*(itr)).erase(itrB);
    //    --m_elementCount;
    //  }
    //}
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::size_type
    Hashtable<HASH_TABLE_PARAMS>::DoErase(const key_type& a_key, keys_are_not_unique)
  {
    // Relies on values with the sake key to be placed in sequence

    const hash_code_type hc = get_hash_code(a_key);
    iterator itr = find_by_hash(a_key);

    if (itr == end()) return 0;

    bucket_iterator itrB          = itr.m_currNode();
    const bucket_iterator itrBEnd = (*(itr.m_currBucket)).end();

    bucket_iterator itrEraseBegin, itrEraseEnd = itrB;
    ++itrEraseEnd; // we need to erase at least the one we found

    const size_type currElemCount = m_elementCount;

    --m_elementCount; // We have found at least one

    ++itrB; // We already counted this one
    // Find the end
    for (; itrB != itrBEnd; ++itrB)
    {
      if ( hc != get_hash_code(*itrB))
      {
        itrEraseEnd = itrB;
        break;
      }
      --m_elementCount;
    }

    (*(itr)).erase(itrEraseBegin, itrEraseEnd);
    return currElemCount - m_elementCount;
  }

  //------------------------------------------------------------------------
  // count helpers

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::size_type 
    Hashtable<HASH_TABLE_PARAMS>::DoCount(const key_type& a_key, 
                                          keys_are_unique)
  {
    const_iterator itr = find(a_key);

    if (itr != end()) { return 1; }
    else { return 0; }

    //const hash_code_type c = get_hash_code(a_key);
    //const size_type n = (size_type)bucket_index(a_key, c, bucket_count());

    //local_iterator itr = m_bucketArray.begin();
    //advance(itr, n);

    //bucket_iterator itrB    = (*(itr)).begin();
    //bucket_iterator itrBEnd = (*(itr)).end();

    //for (; itrB != itrBEnd; ++itrB)
    //{
    //  if (c == get_hash_code(*itrB))
    //  {
    //    return 1;
    //  }
    //}

    //return 0;
  }

  template <HASH_TABLE_TYPES>
  TL_FI typename Hashtable<HASH_TABLE_PARAMS>::size_type 
    Hashtable<HASH_TABLE_PARAMS>::DoCount(const key_type& a_key, 
                                          keys_are_not_unique)
  {
    const hash_code_type hc = get_hash_code(a_key);
    iterator itr = find_by_hash(a_key);

    if (itr == end()) return 0;

    bucket_iterator itrB          = itr.m_currNode;
    const_bucket_iterator itrBEnd = (*(itr.m_currBucket)).end();

    size_type elementCount = 1; // we already know there is at least one

    ++itrB; // already counted the first one 
    for (; itrB != itrBEnd; ++itrB)
    {
      if (hc == get_hash_code(*itrB))
      {
        ++elementCount;
      }
    }

    return elementCount;
  }
    
  //------------------------------------------------------------------------
  // Rehashing

  template <HASH_TABLE_TYPES>
  TL_FI void Hashtable<HASH_TABLE_PARAMS>::DoRehash(size_type a_bucketCount)
  {
    buckets_array_type newArray;
    newArray.resize(a_bucketCount);

    const_local_iterator itrB = m_bucketArray.begin();
    const_local_iterator itrBEnd = m_bucketArray.end();

    for (; itrB != itrBEnd; ++itrB)
    {
      bucket_type::const_iterator itrN = (*itrB).begin();
      bucket_type::const_iterator itrNEnd = (*itrB).end();

      while(itrN != itrNEnd)
      {
        const element_type& currElem = *itrN;
        const size_type newBucketIndex =
          (size_type)bucket_index(currElem, a_bucketCount);

        TLOC_ASSERT_HASH_TABLE(newBucketIndex < a_bucketCount,
          "Invalid bucket index!");

        local_iterator itrNew(newArray.begin());
        advance(itrNew, newBucketIndex);

        (*itrNew).push_back(currElem);

        ++itrN;
      }
    }

    m_bucketArray.swap(newArray);

  }

  //------------------------------------------------------------------------
  // Load factor overloads and sanity checks

  template <HASH_TABLE_TYPES>
  TL_FI f32 Hashtable<HASH_TABLE_PARAMS>::
    DoGetMaxLoadFactor(prime_rehash_policy) const
  {
    return m_rehashPolicy.get_max_load_factor();
  }

  template <HASH_TABLE_TYPES>
  TL_FI void Hashtable<HASH_TABLE_PARAMS>::DoSetMaxLoadFactor
    (f32 a_maxLoadFactor, prime_rehash_policy)
  {
    m_rehashPolicy.set_max_load_factor(a_maxLoadFactor);
  }

};};

#endif