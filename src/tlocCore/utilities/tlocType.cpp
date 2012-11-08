#include "tlocType.h"
#include <tlocCore/types/tlocBasicTypes.h>
#include <tlocCore/types/tlocTypeTraits.h>

#include <limits>

namespace tloc { namespace core { namespace utils {

  namespace p_do_cast_to_32
  {
#define CHECK_AND_CAST_TO_32(_type_)\
  TLOC_ASSERT(a_value < std::numeric_limits<_type_>::max(), \
    "Casting will result in loss of information!");\
      return (_type_)a_value

    template <typename T_ReturnType, typename T_ToCast>
    T_ReturnType DoCastTo32(T_ToCast a_value)
    { return a_value; }

    template <>
    s32 DoCastTo32<s32, s64>(s64 a_value)
    { CHECK_AND_CAST_TO_32(s32); }

    template <>
    u32 DoCastTo32<u32, u64>(u64 a_value)
    { CHECK_AND_CAST_TO_32(u32); }

    template <>
    f32 DoCastTo32<f32, f64>(f64 a_value)
    { CHECK_AND_CAST_TO_32(f32); }

#undef CHECK_AND_CAST_TO_32
  };

  template <typename T_ReturnType, typename T_ToCast>
  T_ReturnType CastTo32(T_ToCast a_value)
  {
    return p_do_cast_to_32::DoCastTo32<T_ReturnType, T_ToCast>(a_value);
  }

  namespace p_cast_number
  {
#define CHECK_AND_CAST_TO_SIGNED(_type_, _toCast_)\
  TLOC_ASSERT(a_value < (_toCast_)std::numeric_limits<_type_>::max(), \
    "Casting will result in loss of information!");\
      return (_type_)a_value

#define CHECK_AND_CAST_TO_UNSIGNED(_type_)\
  TLOC_ASSERT(a_value >= 0, "Casting will result in loss of information!");\
  return (_type_)a_value

    template <typename T_ReturnType, typename T_ToCast>
    T_ReturnType DoCastNumber(T_ToCast a_value)
    { return a_value; }

    template <>
    s32 DoCastNumber<s32, u32>(u32 a_value)
    { CHECK_AND_CAST_TO_SIGNED(s32, u32); }

    template <>
    u32 DoCastNumber<u32, s32>(s32 a_value)
    { CHECK_AND_CAST_TO_UNSIGNED(u32); }

#undef CHECK_AND_CAST_TO_SIGNED

  };

  template <typename T_ReturnType, typename T_ToCast>
  T_ReturnType CastNumber(T_ToCast a_value)
  {
    return p_cast_number::DoCastNumber<T_ReturnType, T_ToCast>(a_value);
  }

  //------------------------------------------------------------------------
  // Explicit instantiations

  template s32 CastTo32(s32);
  template s32 CastTo32(s64);

  template u32 CastTo32(u32);
  template u32 CastTo32(u64);

  template f32 CastTo32(f32);
  template f32 CastTo32(f64);

  template s32 CastNumber(u32);
  template s32 CastNumber(s32);
  template u32 CastNumber(s32);
  template u32 CastNumber(u32);

};};};