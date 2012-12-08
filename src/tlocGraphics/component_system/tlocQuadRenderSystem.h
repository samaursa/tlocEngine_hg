#pragma once
#ifndef _TLOC_GRAPHICS_COMPONENT_SYSTEM_QUAD_RENDER_SYSTEM_H_
#define _TLOC_GRAPHICS_COMPONENT_SYSTEM_QUAD_RENDER_SYSTEM_H_

#include <tlocCore/types/tlocStrongType.h>

#include <tlocCore/component_system/tlocEntityProcessingSystem.h>
#include <tlocCore/component_system/tlocEventManager.h>
#include <tlocCore/component_system/tlocEntityManager.h>
#include <tlocCore/component_system/tlocEntity.h>

#include <tlocGraphics/view_projection/tlocFrustum.h>

#include <tlocMath/vector/tlocVector3.h>
#include <tlocMath/matrix/tlocMatrix4.h>
#include <tlocMath/component_system/tlocComponentType.h>

namespace tloc { namespace graphics { namespace component_system {

  class QuadRenderSystem : public core::component_system::EntityProcessingSystem
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

    typedef core::tl_array<math::Vec3f32>::type               vec3_cont_type;

    typedef view_proj::Frustum::matrix_type                   matrix_type;

  public:
    QuadRenderSystem(event_manager* a_eventMgr, entity_manager* a_entityMgr);

    void AttachCamera(const entity_type* a_cameraEntity);

    virtual error_type Pre_Initialize();
    virtual error_type InitializeEntity(entity_manager* a_mgr,
                                        entity_type* a_ent);
    virtual error_type ShutdownEntity(entity_manager* a_mgr,
                                      entity_type* a_ent);

    virtual void ProcessEntity(entity_manager* a_mgr, entity_type* a_ent);

  private:
    vec3_cont_type      m_quadList;
    const entity_type*  m_sharedCam;
    matrix_type         m_vpMatrix;
  };

};};};

#endif