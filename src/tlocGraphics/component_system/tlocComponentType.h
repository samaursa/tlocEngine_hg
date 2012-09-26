#ifndef TLOC_GFX_COMPONENT_TYPE_H
#define TLOC_GFX_COMPONENT_TYPE_H

#include <tlocCore/component_system/tlocComponentType.h>

namespace tloc { namespace graphics { namespace component_system {

  namespace components
  {
    enum type
    {
      mesh = core::component_system::components_group::graphics,
      material,

      count
    }; typedef tl_int value_type;
  };

};};};

#endif