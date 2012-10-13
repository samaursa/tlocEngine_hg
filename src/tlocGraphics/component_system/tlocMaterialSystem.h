#pragma once
#ifndef _TLOC_GRAPHICS_COMPONENT_SYSTEM_MATERIAL_SYSTEM_H_
#define _TLOC_GRAPHICS_COMPONENT_SYSTEM_MATERIAL_SYSTEM_H_

#include <tlocCore/containers/tlocContainers.h>
#include <tlocCore/component_system/tlocEntityProcessingSystem.h>
#include <tlocCore/component_system/tlocEventManager.h>
#include <tlocCore/component_system/tlocEntityManager.h>
#include <tlocCore/component_system/tlocEntity.h>

#include <tlocGraphics/opengl/tlocShaderProgram.h>

namespace tloc { namespace graphics { namespace component_system {

  class MaterialSystem : public core::component_system::EntityProcessingSystem
  {
  public:
    typedef core::component_system::EntityProcessingSystem  base_type;
    using base_type::component_type;
    using base_type::error_type;

    using base_type::event_manager;
    using base_type::entity_manager;
    using base_type::entity_type;
    using base_type::event_type;
    using base_type::event_value_type;

    typedef gl::ShaderProgram                       shader_prog_type;
    typedef gl::ShaderProgram::object_handle        shader_handle_type;
    typedef core::Pair<shader_handle_type,
                       const entity_type*>          shader_entity_pair_type;
    typedef core::Array<shader_entity_pair_type>    cont_type;

  public:
    MaterialSystem(event_manager* a_eventMgr, entity_manager* a_entityMgr);

    virtual error_type Initialize();
    virtual error_type Shutdown();
    virtual bool CheckProcessing();
    virtual void ProcessEntity(entity_manager* a_mgr, entity_type* a_ent);

    virtual void Pre_OnEvent(const event_type& a_event);
    virtual void Post_OnEvent(const event_type& a_event);

  private:
    bool        m_dirty;
    cont_type   m_shaderEntPair;

  };

};};};

#endif