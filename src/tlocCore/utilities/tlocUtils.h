#ifndef TLOC_UTILS_H
#define TLOC_UTILS_H

#include <tlocCore/tlocCoreBase.h>
#include <tlocCore/types/tlocTypeTraits.h>
#include <tlocCore/utilities/tlocTemplateUtils.h>

namespace tloc { namespace core { namespace utils {

  //------------------------------------------------------------------------
  // Nested functions

#define TL_NESTED_FUNC_BEGIN(funcName) struct funcName ## _nestedFunctionClass { static
#define TL_NESTED_FUNC_END() }
#define TL_NESTED_CALL(funcName) funcName ##  _nestedFunctionClass:: funcName

  ///-------------------------------------------------------------------------
  /// Used to avoid code duplication for operators. Requires definition of
  /// operators == and <
  ///
  /// @param object The type itself
  ///-------------------------------------------------------------------------
#define TLOC_DECLARE_OPERATOR_NOT_EQUAL(object)\
  bool operator !=(object const& a_other) const { return !(operator==(a_other)); }
#define TLOC_DECLARE_OPERATORS(object) \
  TLOC_DECLARE_OPERATOR_NOT_EQUAL(object) \
  bool operator > (object const& a_other) const { return a_other.operator<(*this); }\
  bool operator <=(object const& a_other) const { return !(operator>(a_other)); }\
  bool operator >=(object const& a_other) const { return !(operator<(a_other)); }


  ///-------------------------------------------------------------------------
  /// These macros allow you to declare getters and setters quickly. Note that in
  /// this engine member variables should not be accessed directly, even by the
  /// host class itself.
  /// Reason1: to maintain consistency
  /// Reason2: to allow devs to debug get/set operations
  ///
  /// The following macros make it easier/quicker to declare and define getters
  /// and setters. The problem ofcourse is that we cannot break into macros. This
  /// is why only declarations are also provided which can be defined immediately
  /// or later by the developer to allow breaking into the getter/setter.
  ///------------------------------------------------------------------------

  //------------------------------------------------------------------------
  // Getters

#define TLOC_DECL_GETTER(_type_, _name_)\
  const _type_ _name_() const
#define TLOC_DECL_GETTER_DIRECT(_type_, _name_)\
  _type_&       _name_()
#define TLOC_DECL_GETTER_CONST_DIRECT(_type_, _name_)\
  _type_ const & _name_() const

#define TLOC_DECL_AND_DEF_GETTER(_type_, _name_, _var_)\
  TLOC_DECL_GETTER(_type_, _name_) { return _var_; }

#define TLOC_DECL_AND_DEF_GETTER_DIRECT(_type_, _name_, _var_)\
  TLOC_DECL_GETTER_DIRECT(_type_, _name_) { return _var_; }
#define TLOC_DECL_AND_DEF_GETTER_CONST_DIRECT(_type_, _name_, _var_)\
  TLOC_DECL_GETTER_CONST_DIRECT(_type_, _name_) { return (_var_); }

  //------------------------------------------------------------------------
  // Setters

#define TLOC_DECL_SETTER(_type_, _name_)\
  void            _name_(_type_ const & a_in)

#define TLOC_DECL_SETTER_CHAIN(_type_, _name_)\
  this_type&      _name_(_type_ const & a_in)

#define TLOC_DECL_SETTER_BY_VALUE(_type_, _name_)\
  void            _name_(_type_ a_in)

#define TLOC_DECL_SETTER_BY_VALUE_CHAIN(_type_, _name_)\
  this_type&      _name_(_type_ a_in)

#define TLOC_DECL_AND_DEF_SETTER(_type_, _name_, _var_)\
  TLOC_DECL_SETTER(_type_, _name_) { _var_ = a_in; }

#define TLOC_DECL_AND_DEF_SETTER_BY_VALUE(_type_, _name_, _var_)\
  TLOC_DECL_SETTER_BY_VALUE(_type_, _name_) { _var_ = a_in; }

#define TLOC_DECL_AND_DEF_SETTER_CHAIN(_type_, _name_, _var_)\
  TLOC_DECL_SETTER_CHAIN(_type_, _name_) { _var_ = a_in; return *this; }

#define TLOC_DECL_AND_DEF_SETTER_BY_VALUE_CHAIN(_type_, _name_, _var_)\
  TLOC_DECL_SETTER_BY_VALUE_CHAIN(_type_, _name_) { _var_ = a_in; return *this; }

  // -----------------------------------------------------------------------
  // For parameters

  // This will declare and define getter/setter with the passed name. It
  // will also return *this (reference of the instance) to allow chaining
  // these methods.
  // NOTE: You need to typedef the class as this_type

  // Variable name is _name_
  // Getter name is Get_name_
  // Setter name is Set_name_

#define TLOC_DECL_PARAM_VAR(_type_, _methodName_, _var_)\
  this_type& _methodName_(const _type_& a_value)\
  {\
    _var_ = a_value;\
    return *this;\
  }\
  _type_ _var_

  //------------------------------------------------------------------------
  // Expansions

#define TLOC_DECL_AND_DEF_GETTERS_DIRECT(_type_, _name_, _var_)\
  TLOC_DECL_AND_DEF_GETTER_DIRECT(_type_, _name_, _var_)\
  TLOC_DECL_AND_DEF_GETTER_CONST_DIRECT(_type_, _name_, _var_)

  // -----------------------------------------------------------------------
  // This macro returns the file instead of the full path from the macro
  // __FILE__

#define TLOC_THIS_FILE_NAME() \
#include <tlocCore/string/tlocString.h>\
  static const char8* const TLOC_FILE_NAME = \
  core_str::StrRChr(__FILE__, '/') ? StrRChr(__FILE__, '/') + 1 : __FILE__;

  // -----------------------------------------------------------------------
  // Enum counter.
  //
  // This class is useful to count enums that are bit shifted, where a 'count'
  // doesn't give us the correct answer (for full usage, see tests)
  // -----------------------------------------------------------------------
  template <int T_CountInBits, bool T_CountZero = false>
  struct EnumCounter
  {
    enum { result = 1 + EnumCounter<T_CountInBits / 2, T_CountZero>::result };
  };

  template <bool T_CountZero>
  struct EnumCounter<0, T_CountZero>
  {
    enum { result = 0 };
  };

  template <>
  struct EnumCounter<1, false>
  {
    enum { result = 1 };
  };

  template <>
  struct EnumCounter<1, true>
  {
    enum { result = 2 };
  };

  // -----------------------------------------------------------------------
  // Enum to Index converter

  template <int T_Enum, bool T_IncludeZero = false>
  struct EnumToIndex
  {
    enum { result = 0 + EnumCounter<T_Enum / 2, T_IncludeZero>::result };
  };

  template <bool T_IncludeZero>
  struct EnumToIndex<0, T_IncludeZero>
  {
    TLOC_STATIC_ASSERT(false, Enum_cannot_be_converted_to_index_when_not_including_zero);
  };

  template <>
  struct EnumToIndex<0, true>
  {
    enum { result = 0 };
  };

  template <>
  struct EnumToIndex<1, false>
  {
    enum { result = 0 };
  };

  template <>
  struct EnumToIndex<1, true>
  {
    enum { result = 1 };
  };

  // -----------------------------------------------------------------------
  // Used to prevent a struct or class from being initialized by declaring a
  // constructor with a static assert. Must be inherited from.
  //
  // \tparam  T A type that cannot be evaluated until compilation.
  // -----------------------------------------------------------------------

  template <typename T>
  struct TypedefHelperBase
  {
    TypedefHelperBase()
    {
      TLOC_STATIC_ASSERT
       ((Loki::IsSameType<T,DummyStruct>::value),
        Cannot_instantiate_typedef_helper_USE_typedef_type_declared_in_helper);
    }
  };

};};};


#endif