#include "tlocFan.h"

#include <tlocMath/component_system/tlocComponentType.h>
#include <tlocMath/component_system/tlocTransform.h>
#include <tlocGraphics/component_system/tlocFan.h>

namespace tloc { namespace prefab { namespace graphics {

  using core_cs::Entity;
  using core_cs::EntityManager;
  using core_cs::ComponentPoolManager;

  using math_t::Circle_T;

  core_cs::Entity*
    CreateFan(core_cs::EntityManager& a_mgr,
              core_cs::ComponentPoolManager& a_poolMgr,
              math_t::Circlef32 a_circle,
              tl_size a_numSides)
  {
    using namespace core_cs;
    using namespace math_cs;
    using namespace math_cs::components;
    using namespace tloc::graphics::component_system;
    using namespace tloc::graphics::component_system::components;

    typedef ComponentPoolManager    pool_mgr;
    typedef pool_mgr::iterator      comp_pool_ptr;

    // Create the fan (and the fan pool if necessary)
    comp_pool_ptr cpool;
    if (a_poolMgr.Exists(fan) == false)
    { cpool = a_poolMgr.CreateNewPool<fan_sptr>(fan); }
    else
    { cpool = a_poolMgr.GetPool(fan); }

    typedef tloc::graphics::component_system::FanPool fan_pool;

    fan_pool* fanPool = (*cpool)->GetAs<fan_pool>();

    fan_pool::iterator itr = fanPool->GetNext();
    itr->SetElement(fan_sptr(new Fan(a_circle, Fan::sides(a_numSides)) ) );

    if (a_poolMgr.Exists(transform) == false)
    { cpool = a_poolMgr.CreateNewPool<transform_sptr>(transform); }
    else
    { cpool = a_poolMgr.GetPool(transform); }

    typedef tloc::math_cs::TransformPool  t_pool;

    t_pool* tPool = (*cpool)->GetAs<t_pool>();

    t_pool::iterator itrTransform = tPool->GetNext();
    itrTransform->SetElement(transform_sptr(new Transform()) );

    // Create an entity from the manager and return to user
    Entity* ent = a_mgr.CreateEntity();
    a_mgr.InsertComponent(ent, &*(itrTransform->GetElement()) );
    a_mgr.InsertComponent(ent, &*(itr->GetElement()) );

    return ent;
  }

};};};