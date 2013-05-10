#include "tlocRigidBodyShapeComponent.h"

#include <tlocCore/smart_ptr/tlocSharedPtr.inl>
#include <tlocCore/component_system/tlocComponentPoolManager.inl>

namespace tloc { namespace physics { namespace component_system {

  RigidBodyShape::
    RigidBodyShape(const rigid_body_shape_def_type& a_rigidBodyShape)
    : base_type(k_component_type)
    , m_rigidBodyShape(a_rigidBodyShape)
  {
  }

  //////////////////////////////////////////////////////////////////////////
  // Explicit instantiations

  TLOC_EXPLICITLY_INSTANTIATE_SHARED_PTR(RigidBodyShape);
  TLOC_EXPLICITLY_INSTANTIATE_COMPONENT_POOL(rigid_body_shape_sptr);

};};};