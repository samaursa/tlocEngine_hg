#ifndef TLOC_MATH_INL
#define TLOC_MATH_INL

#ifndef TLOC_MATH_H
#error "Must include header before including the inline file"
#endif

#include <tlocCore/types/tlocTypes.inl>

namespace tloc {

  //////////////////////////////////////////////////////////////////////////
  // Math<T>

  template <typename T>
  TL_FI T Math<T>::Ceil(const T& aValue)
  {
    return std::ceil(aValue);
  }

  template <typename T>
  TL_FI T Math<T>::Floor(const T& aValue)
  {
    return std::floor(aValue);
  }

  template <typename T>
  TL_FI T Math<T>::Abs(const T& aValue)
  {
    return std::abs(aValue);
  }

  template <typename T>
  TL_FI T Math<T>::FAbs(const T& aValue)
  {
    return std::fabs(aValue);
  }

  template <typename T>
  TL_FI T Math<T>::ATan(const T& aValue)
  {
    return std::atan(aValue);
  }

  template <typename T>
  TL_FI T Math<T>::ATan2(const T& aValue1, const T& aValue2)
  {
    return std::atan2(aValue1, aValue2);
  }

  template <typename T>
  TL_FI T Math<T>::Log( const T& aLog )
  {
    return std::log(aLog);
  }

  template <typename T>
  TL_FI T Math<T>::Sin( const T& aValInRad )
  {
    return std::sin(aValInRad);
  }

  template <typename T>
  TL_FI T Math<T>::Cos( const T& aValInRad )
  {
    return std::cos(aValInRad);
  }

  template <typename T>
  TL_FI T Math<T>::Degree(const T& aValueInRadian)
  {
    return aValueInRadian  * Math<T>::RAD_TO_DEG;
  }

  template <typename T>
  TL_FI T Math<T>::Radian(const T& aValueInDegrees)
  {
    return aValueInDegrees * Math<T>::DEG_TO_RAD;
  }

  template <typename T>
  TL_FI T Math<T>::Sqrt(const T& aValue)
  {
    return std::sqrt(aValue);
  }

  template <typename T>
  TL_FI T Math<T>::InvSqrt(const T& aValue)
  {
    return ((T)1) / Sqrt(aValue);
  }

  //------------------------------------------------------------------------
  // Misc Functions

  template <typename T>
  TL_FI bool Math<T>::IsNaN(const T& aValue)
  {
    return aValue != aValue; //-V501
  }

  template <typename T>
  TL_FI bool Math<T>::Approx(const T& aValue1, const T& aValue2, T eps /* = T(1e-6) */)
  {
    T toCompare = Abs(aValue1 - aValue2);
    return (toCompare < eps);
  }

  template <typename T>
  TL_FI bool Math<T>::IsPowerOfTwo( const u32& aValue )
  {
    return (aValue != 0) && ((aValue & (aValue - 1)) == 0);
  }

  //------------------------------------------------------------------------
  // Fast specialized functions

  template <typename T>
  TL_FI T Math<T>::FastInvSqrt(const T& aValue)
  {
    f32 lLength = (f32)aValue;

    // Calculate length inverse
    f32 xhalf = 0.5f*lLength;
    s32 i = *(s32*)&lLength; // get bits for floating value
    i = 0x5f375a86- (i>>1); // gives initial guess y0
    lLength = *(f32*)&i; // convert bits back to float
    lLength = lLength*(1.5f-xhalf*lLength*lLength); // Newton step, repeating increases accuracy

    return (T)lLength;
  }

  template <typename T>
  TL_FI u32 Math<T>::FastPowOfTwo( const u32& aPower )
  {
    return 0x00000001 << aPower;
  }

  template <typename T>
  TL_FI s32 Math<T>::FastSignInt( const T& aRealIn )
  {
    if (((s32&)aRealIn & 0x7FFFFFF)==0) return 0;
    return (signed ((s32&)aRealIn & 0x80000000) >> 31) | 1;
  }

  //------------------------------------------------------------------------
  // Simple Interpolations

  template <typename T>
  TL_FI T Math<T>::Lerp(const T& aValue1, const T& aValue2, const T& aBias)
  {
    return (aBias * aValue1) + ((1 - aBias) * aValue2);
  }

};

#endif