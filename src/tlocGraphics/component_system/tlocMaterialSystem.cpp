#include "tlocMaterialSystem.h"

#include <tlocCore/component_system/tlocComponentMapper.h>
#include <tlocCore/containers/tlocContainers.inl>

#include <tlocGraphics/component_system/tlocComponentType.h>
#include <tlocGraphics/component_system/tlocMaterial.h>
#include <tlocGraphics/opengl/tlocOpenGL.h>

namespace tloc { namespace graphics { namespace component_system {

  MaterialSystem::MaterialSystem
    (event_manager* a_eventMgr, entity_manager* a_entityMgr)
    : base_type(a_eventMgr, a_entityMgr
    , core::Variadic<component_type, 1>(components::material))
  { }

  MaterialSystem::error_type MaterialSystem::Initialize()
  {
    using namespace core::component_system;
    typedef graphics::component_system::Material mat_type;

    for (cont_type::iterator itr = m_shaderEntPair.begin(),
         itrEnd = m_shaderEntPair.end(); itr != itrEnd; ++itr)
    {
      const entity_type* ent = itr->second;
      ComponentMapper<mat_type> mat = ent->GetComponents(components::material);


    }

    return ErrorSuccess();
  }

  MaterialSystem::error_type MaterialSystem::Shutdown()
  { return ErrorSuccess(); }

  bool MaterialSystem::CheckProcessing()
  {
    return m_dirty;
  }

  void MaterialSystem::ProcessEntity(entity_manager* a_mgr, entity_type* a_ent)
  {
    TLOC_UNUSED_2(a_mgr, a_ent);
  }

  void MaterialSystem::Pre_OnEvent(const event_type& a_event)
  {
    using namespace tloc::core::component_system;

    event_value_type type = a_event.GetType();

    switch(type)
    {
    case entity_events::insert_component:
      {
        const EntityComponentEvent& entEvent = a_event.GetAs<EntityComponentEvent>();
        const entity_type* ent = entEvent.GetEntity();

        m_shaderEntPair.push_back(core::MakePair(shader_handle_type(0), ent));
        m_dirty = true;
        break;
      }
    case entity_events::remove_component:
      {
        //using namespace core::component_system;
        //typedef graphics::component_system::Material mat_type;

        //const EntityComponentEvent& entEvent = a_event.GetAs<EntityComponentEvent>();
        //const entity_type* ent = entEvent.GetEntity();

        //ComponentMapper<mat_type> mat = ent->GetComponents(components::material);
        //for (tl_size i = 0; i < mat.size(); ++i)
        //{
        //  for (cont_type::iterator itr = m_allShaders.begin(),
        //       cont_type::iterator itrEnd = m_allShaders.end();
        //       itr != itrEnd; ++itr)
        //  {
        //    if (
        //  }
        //}
      }
    }
  }

  void MaterialSystem::Post_OnEvent(const event_type& a_event)
  { TLOC_UNUSED(a_event); }

};};};
