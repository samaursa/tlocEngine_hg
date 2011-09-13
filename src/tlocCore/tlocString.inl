namespace tloc
{
  //////////////////////////////////////////////////////////////////////////
  // Macros (mostly for assertions)

#define TLOC_ASSERT_STRING_EMPTY_STRING() \
  TLOC_ASSERT_STRING( *(GetEmptyString(char32())) == 0, \
  "EmptyString is corrupted! (no longer empty)");

#define TLOC_ASSERT_STRING_INDEX(_index) \
  TLOC_ASSERT_STRING( _index < length(), "Invalid index!")

# define TLOC_ASSERT_STRING_RANGE(_RangeBegin, _RangeEnd) \
  TLOC_ASSERT(_RangeBegin <= _RangeEnd, \
  # _RangeBegin _CRT_WIDE(" must be smaller than ") _CRT_WIDE(# _RangeEnd) L"!")

  //////////////////////////////////////////////////////////////////////////
  // StringBase<T>

  //------------------------------------------------------------------------
  // Constants

  template <typename T>
  TL_I const typename StringBase<T>::EmptyString StringBase<T>::sm_emptyString = { 0 };

  template <typename T>
  const tl_size StringBase<T>::m_MaxSize = (tl_size) - 2;

  template <typename T>
  const tl_size StringBase<T>::sm_defaultCapacity = 2;

  //------------------------------------------------------------------------
  // Ctors

  template <typename T>
  TL_I StringBase<T>::StringBase()
    : m_begin(NULL)
    , m_end(NULL)
    , m_capacity(NULL)
  {
    DoAllocateSelf();
  }

  template <typename T>
  TL_I StringBase<T>::StringBase(const StringBase<T>& aOther, tl_size aPosition,
                                 tl_size aN /* = npos */)
    : m_begin(NULL)
    , m_end(NULL)
    , m_capacity(NULL)
  {
    TLOC_ASSERT_STRING(aPosition <= aOther.length(), "aPosition is out out range!");

    if (aN == npos)
    {
      RangeInitialize(aOther.m_begin + aPosition, aOther.m_end);
    }
    else
    {
      RangeInitialize(aOther.m_begin + aPosition, aOther.m_begin + aPosition + aN);
    }
  }

  template <typename T>
  TL_I StringBase<T>::StringBase(const T* aPtr, tl_size aNumChars)
  {
    RangeInitialize(aPtr, aPtr + aNumChars);
  }

  template <typename T>
  TL_I StringBase<T>::StringBase(const T* aPtr)
  {
    RangeInitialize(aPtr);
  }

  template <typename T>
  TL_I  StringBase<T>::~StringBase()
  {
    DoDeallocateSelf();
  }

  //------------------------------------------------------------------------
  // Iterators

  template <typename T>
  TL_I typename StringBase<T>::iterator StringBase<T>::begin()
  {
    return m_begin;
  }

  template <typename T>
  TL_I typename StringBase<T>::const_iterator StringBase<T>::begin() const
  {
    return m_begin;
  }

  template <typename T>
  TL_I typename StringBase<T>::iterator StringBase<T>::end()
  {
    return m_end;
  }

  template <typename T>
  TL_I typename StringBase<T>::const_iterator StringBase<T>::end() const
  {

  }

  //------------------------------------------------------------------------
  // Empty Strings

  template <typename T>
  TL_I const char8* StringBase<T>::GetEmptyString( char8 )
  {
    return sm_emptyString.m_Empty8;
  }

  template <typename T>
  TL_I const uchar8* StringBase<T>::GetEmptyString( uchar8 )
  {
    return sm_emptyString.m_EmptyU8;
  }

  template <typename T>
  TL_I const char32* StringBase<T>::GetEmptyString( char32 )
  {
    return sm_emptyString.m_Empty32;
  }

  //------------------------------------------------------------------------
  // Element access

  template <typename T>
  TL_I const T& StringBase<T>::operator[]( tl_size aPos ) const
  {
    TLOC_ASSERT_STRING_INDEX(aPos);
    return *(m_begin + aPos);
  }

  template <typename T>
  TL_I T& StringBase<T>::operator[]( tl_size aPos )
  {
    TLOC_ASSERT_STRING_INDEX(aPos);
    return *(m_begin + aPos);
  }

  template <typename T>
  TL_I const T& StringBase<T>::at( tl_size aPos ) const
  {
    TLOC_ASSERT_STRING_INDEX(aPos);
    return *(m_begin + aPos);
  }

  template <typename T>
  TL_I T& StringBase<T>::at( tl_size aPos )
  {
    TLOC_ASSERT_STRING_INDEX(aPos);
    return *(m_begin + aPos);
  }

  //------------------------------------------------------------------------
  // Capacity

  template <typename T>
  TL_I tl_size StringBase<T>::size() const
  {
    return (tl_size)(m_end - m_begin);
  }

  template <typename T>
  TL_I tl_size StringBase<T>::length() const
  {
    return (tl_size)(m_end - m_begin);
  }

  template <typename T>
  TL_I tl_size StringBase<T>::max_size() const
  {
    return m_MaxSize;
  }

  template <typename T>
  TL_I void StringBase<T>::resize(const tl_size& newSize)
  {
  }

  template <typename T>
  TL_I tl_size StringBase<T>::capacity() const
  {
    // Do not report the memory allocated for the null terminator
    return (tl_size)(m_capacity - m_begin - 1);
  }

  /*!
   * \brief Sets the capacity of the string to the specified capacity.
   *
   * The capacity may increase or shrink. This may result in some of the string
   * being stripped off. Setting the new capacity to 'npos' will ensure that
   * the capacity == size.
   *
   * \param const tl_size & newCapacity
   * \return tl_size
   * \sa resize(), reserve()
   */
  template <typename T>
  TL_I void StringBase<T>::set_capacity(const tl_size& newCapacity)
  {
    if (newCapacity != capacity())
    {
      const tl_size oldSize = m_end - m_begin;
      const tl_size cap = (newCapacity == npos) ? oldSize : newCapacity;

      if (cap > 0)
      {
        const tl_size oldSize = m_end - m_begin;

        m_begin = DoReAllocate(cap + 1); // +1 for the trailing 0

        if (cap < oldSize)
        {
          m_end = m_begin + cap; // cap is an index and capacity starts from 0
                                 // so no +1 here
        }
        else
        {
          m_end = m_begin + oldSize;
        }

        *m_end = 0;
        m_capacity = m_begin + cap + 1;
      }
      else
      {
        DoDeallocateSelf();
        DoAllocateSelf();
      }
    }
  }

  template <typename T>
  TL_I void StringBase<T>::reserve(const tl_size& newSize)
  {
    TLOC_ASSERT_STRING(newSize < m_MaxSize, "Reserve request is too large!");
    TLOC_ASSERT_STRING_WARN(newSize < capacity(), "Reserve request is "
      "smaller than the current capacity. Call resize() for shrinking.");

    if (newSize > capacity())
    {

    }
  }

  template <typename T>
  TL_I void StringBase<T>::clear()
  {
    *m_begin = T(0);
    m_end   = m_begin;
  }

  template <typename T>
  TL_I bool StringBase<T>::empty()
  {
    return (m_begin == m_end);
  }

  //------------------------------------------------------------------------
  // Modifiers

  template <typename T>
  TL_I StringBase<T>& StringBase<T>::operator+=( const StringBase<T>& aStr )
  {
    return append(aStr);
  }

  template <typename T>
  TL_I StringBase<T>& StringBase<T>::operator+=( const T* charStr )
  {
    return append(charStr);
  }

  template <typename T>
  TL_I StringBase<T>& StringBase<T>::operator+=( const T& character )
  {
    return *this;
  }

  template <typename T>
  TL_I StringBase<T>& StringBase<T>::append( const StringBaseT& aStr )
  {
    return append(aStr.m_begin, aStr.m_end);
  }

  template <typename T>
  TL_I StringBase<T>& StringBase<T>::append( const StringBaseT& aStr,
                                             tl_size aPos, tl_size aNumChars )
  {
    TLOC_ASSERT_STRING(aPos < aStr.size(),
      "Position passed is outside of string's range!");
    TLOC_ASSERT_STRING_WARN( (aPos + aNumChars) < aStr.size(),
      "Number of characters to copy passes the string's range!");

    u32 maxCharsLeft = (aStr.m_end - aStr.m_begin) - aPos;
    return append(aStr.begin + aPos, aPos < maxCharsLeft ? aPos : maxCharsLeft);
  }

  template <typename T>
  TL_I StringBase<T>& StringBase<T>::append( const T* charArray, tl_size aNumChars )
  {
    TLOC_ASSERT_STRING(StrLen(charArray) >= aNumChars,
      "Number of characters to copy exceeds the range of the character array!");

    return append(charArray, charArray + aNumChars);
  }

  template <typename T>
  TL_I StringBase<T>& StringBase<T>::append( const T* charStr )
  {
    return append(charStr, charStr + StrLen(charStr));
  }

  template <typename T>
  TL_I StringBase<T>& StringBase<T>::append( tl_size aNumChars, T aChar )
  {
    if (aNumChars > 0)
    {
      const tl_size currSize = m_end - m_begin;
      const tl_size currCap  = m_capacity - m_begin;

      if ( (currSize + aNumChars) > (currCap  - 1) )
      {
        // call reserve
      }

      tlFill(m_end, m_end + aNumChars, aChar);
      m_end += aNumChars;
      *m_end = 0;
    }

    return *this;
  }

  template <typename T>
  template <typename T_InputIterator>
  TL_I StringBase<T>& StringBase<T>::append( T_InputIterator aBegin,
                                             T_InputIterator aEnd )
  {
    if (aBegin != aEnd)
    {
      const tl_size oldSize = m_end - m_begin;
      const tl_size oldCapacity = m_capacity - m_begin;
      const tl_size rangeSize = (tl_size)(aEnd - aBegin);

      if ( (oldSize + rangeSize) > (oldCapacity - 1) ) // -1 because of /0
      {
        DoReAllocateAndAdjust(oldSize + rangeSize + 1);
      }

      m_end = tlCopy(aBegin, aEnd, m_end);
      *m_end = 0;
    }

    return *this;
  }

  //------------------------------------------------------------------------
  // Operations

  template <typename T>
  TL_I const T* StringBase<T>::c_str()
  {
    return m_begin;
  }

  //------------------------------------------------------------------------
  // Internal functions

  template <typename T>
  TL_I T* StringBase<T>::DoAllocate( const tl_size& aSize )
  {
    TLOC_ASSERT_STRING(aSize > 1,
      "Allocation size must be greater than 1 for the null terminator");

    return (T*)TL_MALLOC(aSize * sizeof(T));
  }

  template <typename T>
  TL_I T* StringBase<T>::DoReAllocate( const tl_size& aSize )
  {
    TLOC_ASSERT_STRING(aSize > 1,
      "Allocation size must be greater than 1 for the null terminator");

    // Make sure that the size is not 0, otherwise we would trying to reallocate
    // the empty string
    if ( (m_end - m_begin) != 0) { return (T*)TL_REALLOC(m_begin, sizeof(T) * aSize); }
    else { return DoAllocate(aSize); }
  }

  /*!
   * \brief Reallocates to aSize
   *
   * Increases the storage capacity by reallocation. The function also readjusts
   * the iterators.
   *
   * \return void
   * \note Function does not do anything if ReAllocate() fails
   * \warning
   */
  template <typename T>
  TL_I void StringBase<T>::DoReAllocateAndAdjust( const tl_size& aSize )
  {
    TLOC_ASSERT_STRING(aSize > capacity(),
      "This function can only increase the storage capacity! (passed size is "
      L"smaller than the current capacity)");

    const tl_size prevSize = m_end - m_begin;
    const tl_size newCap   = aSize;

    T* ptr = DoReAllocate(aSize);

    TLOC_ASSERT_STRING(ptr != NULL, "Could not allocate/re-allocate!");

    if (ptr)
    {
      m_begin     = ptr;
      m_end       = m_begin + prevSize;
      m_capacity  = m_begin + newCap;
    }
  }

  /*!
   * \brief Reallocates automatically
   *
   * Increases the storage capacity by reallocation. This function automatically
   * resizes the array according to the formula:
   * \f[
   * newCapacity = m_capacity ? capacity() * 2 : sm_defaultCapacity;
   * \f]
   * and adjusts the iterators. If successful, old iterators are considered
   * invalid.
   *
   * \return void
   * \note Function does not do anything if ReAllocate() fails
   * \warning
   */
  template <typename T>
  void StringBase<T>::DoReAllocateAndAdjust()
  {
    const tl_size prevCap  = capacity();
    const tl_size newCap   = prevCap ? (2 * prevCap) : sm_defaultCapacity;

    DoReAllocateAndAdjust(newCap);
  }

  template <typename T>
  TL_I void StringBase<T>::DoFree( T* aPtr )
  {
    TL_FREE(aPtr);
  }

  template <typename T>
  TL_I void StringBase<T>::DoAllocateSelf()
  {
    TLOC_ASSERT_STRING_EMPTY_STRING();

    m_begin     = const_cast<T*>( GetEmptyString(T()) );
    m_end       = m_begin;
    m_capacity  = m_begin + 1;
  }

  template <typename T>
  TL_I void StringBase<T>::DoAllocateSelf( const tl_size& aSize )
  {
    TLOC_ASSERT_STRING(aSize <= (tl_size)std::numeric_limits<tl_size>::max,
      "Allocating too large a value!");

    if (aSize > 1)
    {
      m_begin     = DoAllocate(aSize);
      m_end       = m_begin;
      m_capacity  = m_begin + aSize;
    }
    else
    {
      DoAllocateSelf();
    }
  }

  template <typename T>
  TL_I void StringBase<T>::DoDeallocateSelf()
  {
    if (m_begin != GetEmptyString(T()))
    {
      DoFree(m_begin);
    }
  }

  template <typename T>
  void StringBase<T>::RangeInitialize( const T* aPtrBegin )
  {
    TLOC_ASSERT_STRING(aPtrBegin, "aPtrBegin cannot be NULL!");

    RangeInitialize(aPtrBegin, aPtrBegin + StrLen(aPtrBegin));
  }

  template <typename T>
  void StringBase<T>::RangeInitialize( const T* aPtrBegin, const T* aPtrEnd )
  {
    if (aPtrBegin != aPtrEnd)
    {
      TLOC_ASSERT_STRING_RANGE(aPtrBegin, aPtrEnd);

      DoAllocateSelf( (tl_size)(aPtrEnd - aPtrBegin + 1));

      m_end = tlCopy(aPtrBegin, aPtrEnd, m_begin);
      *m_end = 0; // Null terminator
    }
    else
    {
      DoAllocateSelf();
    }
  }

  //////////////////////////////////////////////////////////////////////////
  // Free functions

  template <typename T>
  TL_I tl_size StrLen( const T* aCharStr )
  {
    const T* lTemp = aCharStr;

    while (*lTemp) { ++lTemp; }

    return (tl_size)(lTemp - aCharStr);
  }

  template <>
  TL_I tl_size StrLen( const char8* aCharStr)
  {
    return (tl_size)strlen(aCharStr); // According to EASTL, this should call intrinsics
  }

  template <typename T>
  TL_I s32 StrCmp( const T* aPtr1, const T* aPtr2 )
  {
    while (*aPtr1 != 0 && *aPtr2 != 0)
    {
      if (*aPtr1 != *aPtr2)
      {
        return *aPtr1 > *aPtr2 ? 1 : -1;
      }

      ++aPtr1; ++aPtr2;
    }

    if (*aPtr1 != 0 || *aPtr2 != 0)
    {
      return *aPtr1 > *aPtr2 ? 1 : -1;
    }

    return 0;
  }

  template <typename T>
  TL_I s32 StrCmp( const T* aPtr1, const T* aPtr2, const tl_size& aNumChars )
  {
    for (tl_size i = aNumChars; i > 0; --i)
    {
      if (*aPtr1 != aPtr2)
      {
        return *aPtr1 > *aPtr2 ? 1 : -1;
      }
    }

    return 0;
  }

  template <>
  TL_I s32 StrCmp( const char8* aPtr1, const char8* aPtr2, const tl_size& aNumChars )
  {
    return memcmp(aPtr1, aPtr2, aNumChars);
  }
};
