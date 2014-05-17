#ifndef _TLOC_PREFAB_GRAPHICS_CUBOID_H_
#define _TLOC_PREFAB_GRAPHICS_CUBOID_H_

#include <tlocPrefab/tlocPrefabBase.h>

#include <tlocCore/component_system/tlocEntity.h>
#include <tlocCore/component_system/tlocEntityManager.h>
#include <tlocCore/component_system/tlocComponentPoolManager.h>

#include <tlocMath/types/tlocCuboid.h>

namespace tloc { namespace prefab { namespace graphics {

  class Cuboid
    : public Prefab_I
  {
  public:
    typedef Cuboid              this_type;
    typedef math_t::Cuboidf32   cuboid_type;

  public:
    Cuboid(entity_mgr_ptr a_entMgr, comp_pool_mgr_ptr a_poolMgr)
           : Prefab_I(a_entMgr, a_poolMgr)
           , m_texCoords(true)
           , m_normals(true)
           , m_cuboid(cuboid_type (cuboid_type::width(1.0f),
                                   cuboid_type::height(1.0f),
                                   cuboid_type::depth(1.0f)) )
    { }

    entity_ptr    Create();
    void          Add(entity_ptr a_ent);

    TLOC_DECL_PARAM_VAR(bool, TexCoords, m_texCoords);
    TLOC_DECL_PARAM_VAR(bool, Normals, m_normals);
    TLOC_DECL_PARAM_VAR(cuboid_type, Dimensions, m_cuboid);

  };

};};};

#endif