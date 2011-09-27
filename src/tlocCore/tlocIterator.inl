namespace tloc
{
  //////////////////////////////////////////////////////////////////////////
  // Pre-defined iterators

  //````````````````````````````````````````````````````````````````````````
  // output_iterator_base

  template <typename T_Container>
  TL_FI output_iterator_base<T_Container>::output_iterator_base( T_Container& aContainer )
    : m_container(aContainer)
  {
  }

  template <typename T_Container>
  TL_FI output_iterator_base<T_Container>&
    output_iterator_base<T_Container>::operator*()
  {
    return *this;
  }

  template <typename T_Container>
  TL_FI output_iterator_base<T_Container>&
    output_iterator_base<T_Container>::operator++()
  {
    return *this;
  }

  template <typename T_Container>
  TL_FI output_iterator_base<T_Container>&
    output_iterator_base<T_Container>::operator++( int )
  {
    return *this;
  }

  //````````````````````````````````````````````````````````````````````````
  // back_insert_iterator

  template <typename T_Container>
  TL_FI back_insert_iterator<T_Container>::back_insert_iterator( T_Container& aContainer )
    : output_iterator_base(aContainer)
  {
  }

  template <typename T_Container>
  TL_FI back_insert_iterator<T_Container>&
    back_insert_iterator<T_Container>::operator=( const_reference aValue )
  {
    m_container.push_back(aValue);
    return *this;
  }

  //````````````````````````````````````````````````````````````````````````
  // front_insert_iterator

  template <typename T_Container>
  TL_FI front_insert_iterator<T_Container>::front_insert_iterator( T_Container& aContainer )
    : output_iterator_base(aContainer)
  {
  }

  template <typename T_Container>
  TL_FI front_insert_iterator<T_Container>&
    front_insert_iterator<T_Container>::operator=( const_reference aValue )
  {
    m_container.push_front(aValue);
    return *this;
  }

  //````````````````````````````````````````````````````````````````````````
  // insert_iterator

  template <typename T_Container>
  TL_FI insert_iterator<T_Container>::insert_iterator( T_Container& aContainer,
    iterator_type aItr ) : output_iterator_base(aContainer), m_itr(aItr)
  {
  }

  template <typename T_Container>
  TL_FI insert_iterator<T_Container>&
    insert_iterator<T_Container>::operator=( const_reference aValue )
  {
    m_itr = m_container.insert(m_itr, aValue);
    ++m_itr;
    return *this;
  }

  template <typename T_Container>
  TL_FI insert_iterator<T_Container>&
    insert_iterator<T_Container>::operator=( const insert_iterator<T_Container>& aOther )
  {
    TLOC_ASSERT_ITERATOR(&m_container == aOther.m_container,
      "Iterators belong to different containers!");
    m_itr = aOther.m_itr;
    return *this;
  }

  //````````````````````````````````````````````````````````````````````````
  // reverse_iterator

  template <typename T_Itr>
  TL_FI reverse_iterator<T_Itr>::reverse_iterator() : m_itr()
  {
  }

  template <typename T_Itr>
  TL_FI reverse_iterator<T_Itr>::reverse_iterator( T_Itr aIterator )
    : m_itr(aIterator)
  {
  }

  template <typename T_Itr>
  template <typename U>
  TL_FI reverse_iterator<T_Itr>::reverse_iterator( const reverse_iterator<U>& aOther )
    : m_itr(aOther.base())
  {
  }

  template <typename T_Itr>
  TL_FI T_Itr reverse_iterator<T_Itr>::base() const
  {
    return m_itr;
  }

  template <typename T_Itr>
  template <typename U>
  TL_FI reverse_iterator<T_Itr>&
    reverse_iterator<T_Itr>::operator=( const reverse_iterator<U>& aOther )
  {
    m_itr = aOther.base();
    return *this;
  }

  template <typename T_Itr>
  TL_FI typename reverse_iterator<T_Itr>::reference
    reverse_iterator<T_Itr>::operator*() const
  {
    // reverse iterator is always pointer 1 past the element
    T_Itr itr(m_itr);
    --itr;
    return *itr;
  }

  template <typename T_Itr>
  TL_FI reverse_iterator<T_Itr>
    reverse_iterator<T_Itr>::operator+( difference_type aDistance ) const
  {
    reverse_iterator<T_Itr> itr(m_itr - aDistance);
    return itr;
  }

  template <typename T_Itr>
  TL_FI reverse_iterator<T_Itr>& reverse_iterator<T_Itr>::operator++()
  {
    --m_itr;
    return *this;
  }

  template <typename T_Itr>
  TL_FI reverse_iterator<T_Itr> reverse_iterator<T_Itr>::operator++( int )
  {
    T_Itr itr(*this);
    --m_itr;
    return *this;
  }

  template <typename T_Itr>
  TL_FI reverse_iterator<T_Itr>&
    reverse_iterator<T_Itr>::operator+=( difference_type aDistance )
  {
    m_itr -= aDistance;
    return *this;
  }

  template <typename T_Itr>
  TL_FI reverse_iterator<T_Itr>
    reverse_iterator<T_Itr>::operator-( difference_type aDistance )
  {
    reverse_iterator<T_Itr> itr(m_itr + aDistance);
    return itr;
  }

  template <typename T_Itr>
  TL_FI reverse_iterator<T_Itr>& reverse_iterator<T_Itr>::operator--()
  {
    ++m_itr;
    return *this;
  }

  template <typename T_Itr>
  TL_FI reverse_iterator<T_Itr> reverse_iterator<T_Itr>::operator--( int )
  {
    T_Itr itr(*this);
    ++m_itr;
    return *this;
  }

  template <typename T_Itr>
  TL_FI reverse_iterator<T_Itr>&
    reverse_iterator<T_Itr>::operator-=( difference_type aDistance )
  {
    m_itr += aDistance;
    return *this;
  }

  template <typename T_Itr>
  TL_FI typename reverse_iterator<T_Itr>::pointer
    reverse_iterator<T_Itr>::operator->() const
  {
    return &(operator*());
  }

  template <typename T_Itr>
  TL_FI typename reverse_iterator<T_Itr>::reference
    reverse_iterator<T_Itr>::operator[]( difference_type aDistance ) const
  {
    return m_itr[ - aDistance - 1];
  }

  //````````````````````````````````````````````````````````````````````````
  // Global operators for reverse_iterator

  template <typename T_Itr1, typename T_Itr2>
  TL_I bool operator==( const reverse_iterator<T_Itr1>& a,
                        const reverse_iterator<T_Itr2>& b )
  {
    return a.base() == b.base();
  }

  template <typename T_Itr1, typename T_Itr2>
  TL_I bool operator!=( const reverse_iterator<T_Itr1>& a,
                        const reverse_iterator<T_Itr2>& b )
  {
    return a.base() != b.base();
  }

  template <typename T_Itr1, typename T_Itr2>
  TL_I bool operator<( const reverse_iterator<T_Itr1>& a,
                       const reverse_iterator<T_Itr2>& b )
  {
    return a.base() > b.base();
  }

  template <typename T_Itr1, typename T_Itr2>
  TL_I bool operator<=( const reverse_iterator<T_Itr1>& a,
                        const reverse_iterator<T_Itr2>& b )
  {
    return a.base() >= b.base();
  }

  template <typename T_Itr1, typename T_Itr2>
  TL_I bool operator>( const reverse_iterator<T_Itr1>& a,
                       const reverse_iterator<T_Itr2>& b )
  {
    return a.base() < b.base();
  }

  template <typename T_Itr1, typename T_Itr2>
  TL_I bool operator>=( const reverse_iterator<T_Itr1>& a,
                        const reverse_iterator<T_Itr2>& b )
  {
    return a.base() <= b.base();
  }

  template <typename T_Itr1, typename T_Itr2>
  TL_I typename reverse_iterator<T_Itr1>::difference_type
    operator-( const reverse_iterator<T_Itr1>& a, const reverse_iterator<T_Itr2>& b )
  {
    return b.base() - a.base();
  }

  template <typename T_Itr>
  TL_I typename reverse_iterator<T_Itr>::difference_type
    operator+( typename reverse_iterator<T_Itr>::difference_type aDistance,
               const reverse_iterator<T_Itr>& a )
  {
    reverse_iterator<T_Itr> itr(a.base() - aDistance);
    return itr;
  }

  //////////////////////////////////////////////////////////////////////////
  // Gobal functions

  template <typename T_InputItr>
  TL_FI tl_size distance( T_InputItr aBegin, T_InputItr aEnd )
  {
    typedef Loki::TypeTraits<T_InputItr>   itrType;
    typedef Loki::Int2Type<itrType::isPointer>  itrTypeSelect;

    return detail::distance(aBegin, aEnd, itrTypeSelect());
  }


  template <typename T_InputItr, typename T_Distance>
  TL_FI void advance( T_InputItr aItr, T_Distance aN )
  {
    typedef typename iterator_traits<T_InputItr>::iterator_category itrCat;

    detail::advance(aItr, aN, itrCat());
  }

  //````````````````````````````````````````````````````````````````````````
  // Iterator construction

  template <typename T_Container>
  TL_FI back_insert_iterator<T_Container> back_inserter( T_Container& aContainer )
  {
    return back_insert_iterator<T_Container>(aContainer);
  }

  template <typename T_Container>
  TL_FI front_insert_iterator<T_Container> front_inserter( T_Container& aContainer )
  {
    return front_insert_iterator<T_Container>(aContainer);
  }

  template <typename T_Container>
  TL_FI insert_iterator<T_Container> inserter( T_Container& aContainer )
  {
    return insert_iterator<T_Container>(aContainer);
  }

  //////////////////////////////////////////////////////////////////////////
  // Detail

  namespace detail
  {
    template <typename T_InputItr>
    TL_FI tl_size distance(T_InputItr aBegin, T_InputItr aEnd, IsPtrItr)
    {
      return (tl_size)(aEnd - aBegin);
    }

    template <typename T_InputItr>
    TL_FI tl_size distance(T_InputItr aBegin, T_InputItr aEnd, IsComplexItr)
    {
      TLOC_ASSERT_WIP();
    }

    template <typename T_InputItr, typename T_Distance>
    TL_FI void advance( T_InputItr aItr, T_Distance aN, input_iterator_tag )
    {
      TLOC_ASSERT_ITERATOR(aN >= 0,
        "Iterator can only advance in the positive direction!");

      while (aN) { ++aItr; --aN; }
    }

    template <typename T_InputItr, typename T_Distance>
    TL_FI void advance( T_InputItr aItr, T_Distance aN, bidirectional_iterator_tag )
    {
      if (n > 0)
      {
        while (aN) { ++aItr; --aN; }
      }
      else
      {
        while (aN) { ++aItr; ++aN; }
      }
    }

    template <typename T_InputItr, typename T_Distance>
    TL_FI void advance( T_InputItr aItr, T_Distance aN, random_access_iterator_tag )
    {
      aItr = aItr + aN;
    }
  };
};