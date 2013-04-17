#ifndef _TLOC_MATH_COMPONENT_SYSTEM_PROJECTION_COMPONENT_H_
#define _TLOC_MATH_COMPONENT_SYSTEM_PROJECTION_COMPONENT_H_

#include <tlocMath/tlocMathBase.h>

#include <tlocCore/smart_ptr/tlocSharedPtr.h>
#include <tlocCore/component_system/tlocComponent.h>
#include <tlocCore/component_system/tlocComponentPoolManager.h>

#include <tlocMath/projection/tlocFrustum.h>

namespace tloc { namespace math { namespace component_system {

  class Projection : public core::component_system::Component_T<Projection>
  {
  public:
    typedef core::component_system::Component_T<Projection>  base_type;

    typedef proj::frustum_persp           frustum_type;
    typedef frustum_type::Params          frustum_param_type;

  public:
    Projection();
    explicit Projection(const frustum_type& a_frustum);

    void SetFrustum(const frustum_param_type& a_params);

    TLOC_DECL_AND_DEF_GETTER_CONST_DIRECT(frustum_type, GetFrustumRef, m_frustum);
    TLOC_DECL_AND_DEF_SETTER(frustum_type, SetFrustum, m_frustum);

  private:
    frustum_type  m_frustum;
  };

  //------------------------------------------------------------------------
  // Typedefs

  typedef core::smart_ptr::SharedPtr<Projection>    ProjectionPtr;
  typedef core::component_system::
          ComponentPool_TI<ProjectionPtr>           ProjectionPool;

};};};

#endif