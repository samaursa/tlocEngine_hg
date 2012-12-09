#ifndef _TLOC_PREFAB_GRAPHICS_CAMERA_H_
#define _TLOC_PREFAB_GRAPHICS_CAMERA_H_

#include <tlocMath/component_system/tlocTransform.h>
#include <tlocGraphics/component_system/tlocProjectionComponent.h>

namespace tloc { namespace prefab { namespace graphics {

  core::component_system::Entity*
    CreateCamera(core::component_system::EntityManager& a_mgr,
                 core::component_system::ComponentPoolManager& a_poolMgr)
  {
    using namespace core::component_system;
    using namespace math::component_system;
    using namespace math::component_system::components;
    using namespace tloc::graphics::component_system;
    using namespace tloc::graphics::component_system::components;

    typedef ComponentPoolManager    pool_mgr;
    typedef pool_mgr::iterator      comp_pool_ptr;

    comp_pool_ptr cpool;

    // Get or create the transform pool
    if (a_poolMgr.Exists(transform) == false)
    { cpool = a_poolMgr.CreateNewPool<TransformPtr>(transform); }
    else
    { cpool = a_poolMgr.GetPool(transform); }

    typedef tloc::math::component_system::TransformPool  t_pool;

    t_pool* tPool = (*cpool)->GetAs<t_pool>();

    t_pool::iterator itrTransform = tPool->GetNext();
    itrTransform->GetElement() = new Transform();

    // Get or create the projection pool
    if (a_poolMgr.Exists(projection) == false)
    { cpool = a_poolMgr.CreateNewPool<ProjectionPtr>(projection); }
    else
    { cpool = a_poolMgr.GetPool(projection); }

    typedef tloc::graphics::component_system::ProjectionPool p_pool;

    p_pool* pPool = (*cpool)->GetAs<p_pool>();

    p_pool::iterator itrProjection = pPool->GetNext();
    itrProjection->GetElement() = new Projection();

    // Create an entity from the manager and return to user
    Entity* ent = a_mgr.CreateEntity();
    a_mgr.InsertComponent(ent, &*(itrTransform->GetElement()) );
    a_mgr.InsertComponent(ent, &*(itrProjection->GetElement()) );

    return ent;
  }

};};};

#endif