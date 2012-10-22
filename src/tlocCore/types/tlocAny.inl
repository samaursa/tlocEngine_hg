#ifndef _TLOC_CORE_TYPES_ANY_INL_
#define _TLOC_CORE_TYPES_ANY_INL_

#ifndef _TLOC_CORE_TYPES_ANY_H_
#error "Must include header before including the inline file"
#endif

#include "tlocAny.h"

namespace tloc { namespace core { namespace types {

  namespace p_any
  {
    namespace detail
    {

      //////////////////////////////////////////////////////////////////////////
      // Policy_IT Class

#define POLICY_TEMP    typename T
#define POLICY_PARAMS  T
#define POLICY_TYPE    typename Policy_IT<POLICY_PARAMS>

      template <POLICY_TEMP>
      POLICY_TYPE::size_type
        Policy_IT<POLICY_PARAMS>::GetSize() const
      { return sizeof(value_type); }

      template <POLICY_TEMP>
      POLICY_TYPE::value_type const *
        Policy_IT<POLICY_PARAMS>::CastSource(void const* a_source)
      { return (reinterpret_cast<value_type const*>(a_source)); }

      template <POLICY_TEMP>
      POLICY_TYPE::value_type const *
        Policy_IT<POLICY_PARAMS>::CastSource(void* const* a_source)
      { return *(reinterpret_cast<value_type* const*>(a_source)); }

#undef POLICY_TEMP
#undef POLICY_PARAMS
#undef POLICY_TYPE

      //////////////////////////////////////////////////////////////////////////
      // Simple Type 

#define SIMPLE_TYPE_TEMP   typename T
#define SIMPLE_TYPE_PARAMS T
#define SIMPLE_TYPE_TYPE   typename SimpleType_T

      template <SIMPLE_TYPE_TEMP>
      void SimpleType_T<SIMPLE_TYPE_PARAMS>
        ::Delete(void** )
      { }

      template <SIMPLE_TYPE_TEMP>
      void SimpleType_T<SIMPLE_TYPE_PARAMS>
        ::Copy(void const* a_source, void** a_dest)
      { new(a_dest) value_type(*reinterpret_cast<value_type const*>(a_source)); }

      template <SIMPLE_TYPE_TEMP>
      void SimpleType_T<SIMPLE_TYPE_PARAMS>
        ::Clone(void* const* a_source, void** a_dest)
      { *a_dest = *a_source; }


      template <SIMPLE_TYPE_TEMP>
      void SimpleType_T<SIMPLE_TYPE_PARAMS>
        ::Move(void* const* a_source, void** a_dest)
      { *a_dest = *a_source; }

      template <SIMPLE_TYPE_TEMP>
      void* SimpleType_T<SIMPLE_TYPE_PARAMS>
        ::GetValue(void** a_source)
      { return reinterpret_cast<void*>(a_source); }

#undef SIMPLE_TYPE_TEMP   
#undef SIMPLE_TYPE_PARAMS
#undef SIMPLE_TYPE_TYPE 

      //////////////////////////////////////////////////////////////////////////
      // Complex Type

#define COMPLEX_TYPE_TEMP   typename T
#define COMPLEX_TYPE_PARAMS T
#define COMPLEX_TYPE_TYPE   typename ComplexType_T<COMPLEX_TYPE_PARAMS>

      template <COMPLEX_TYPE_TEMP>
      void ComplexType_T<COMPLEX_TYPE_PARAMS>
        ::Delete(void** a_unknownType)
      {
        value_type** x = reinterpret_cast<value_type**>(a_unknownType);
        delete *x;
        *x = NULL;
      }

      template <COMPLEX_TYPE_TEMP>
      void ComplexType_T<COMPLEX_TYPE_PARAMS>
        ::Copy(void const* a_source, void** a_dest)
      { *a_dest = new value_type(*CastSource(a_source)); }

      template <COMPLEX_TYPE_TEMP>
      void ComplexType_T<COMPLEX_TYPE_PARAMS>
        ::Clone(void* const* a_source, void** a_dest)
      { *a_dest = new value_type(*CastSource(a_source)); }


      template <COMPLEX_TYPE_TEMP>
      void ComplexType_T<COMPLEX_TYPE_PARAMS>
        ::Move(void* const* a_source, void** a_dest)
      {
        (*(reinterpret_cast<value_type**>(a_dest)) )->~value_type();
        **(reinterpret_cast<value_type**>(a_dest)) = *CastSource(a_source);
      }

      template <COMPLEX_TYPE_TEMP>
      void* ComplexType_T<COMPLEX_TYPE_PARAMS>
        ::GetValue(void** a_source)
      { return *a_source; }

#undef COMPLEX_TYPE_TEMP   
#undef COMPLEX_TYPE_PARAMS
#undef COMPLEX_TYPE_TYPE 
    };

    //////////////////////////////////////////////////////////////////////////
    // Helper functions

    template <typename T>
    detail::Policy_I* GetPolicy()
    {
      TLOC_STATIC_ASSERT( (Loki::IsSameType<T, Any>::value == false),
        Cannot_use_Any_class_for_policy_selection);
      static typename SelectPolicy<T>::type policyForT;
      return &policyForT;
    };

  };

  //////////////////////////////////////////////////////////////////////////
  // Any Class

  template <typename T>
  Any::Any(const T& a_other)
    : m_policy(p_any::GetPolicy<p_any::detail::Empty>()), m_object(NULL)
  {
    Assign(a_other);
  }

  template <typename T>
  void Any::Assign(const T& a_other)
  {
    Reset();
    m_policy = p_any::GetPolicy<T>();
    m_policy->Copy(&a_other, &m_object);
  }

  template <typename T>
  T& Any::Cast()
  {
    assert(m_policy == p_any::GetPolicy<T>());
    T* ret = reinterpret_cast<T*>(m_policy->GetValue(&m_object));
    return *ret;
  }

};};};

#endif