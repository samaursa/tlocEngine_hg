#include "tlocBoundingBoxRenderSystem.h"

#include <tlocCore/logging/tlocLogger.h>
#include <tlocGraphics/component_system/tlocBoundingBox.h>
#include <tlocGraphics/component_system/tlocSceneGraphSystem.h>
#include <tlocGraphics/component_system/tlocMaterialSystem.h>
#include <tlocPrefab/graphics/tlocQuad.h>
#include <tlocPrefab/graphics/tlocCuboid.h>
#include <tlocPrefab/graphics/tlocMaterial.h>

TLOC_DEFINE_THIS_FILE_NAME();

namespace tloc { namespace graphics { namespace component_system {

#define TLOC_BOUNDING_BOX_RENDER_SYSTEM_TEMPS   typename T_RendererSptr
#define TLOC_BOUNDING_BOX_RENDER_SYSTEM_PARAMS  T_RendererSptr

  namespace {

#ifdef TLOC_OS_WIN
    const char* vsSource =
      "#version 330 core                                        \n\
                                                                \n\
       in vec3 a_vertPos;                                       \n\
       uniform mat4 u_vp;                                       \n\
       uniform mat4 u_model;                                    \n\
                                                                \n\
       void main()                                              \n\
       {                                                        \n\
          mat4 vp = u_vp;                                       \n\
          if (u_vp[0][0] == 0.0 &&                              \n\
              u_vp[0][1] == 0.0 &&                              \n\
              u_vp[0][2] == 0.0)                                \n\
          { vp = mat4(1.0); }                                   \n\
                                                                \n\
          mat4 model = u_model;                                 \n\
          if (u_model[0][0] == 0.0 &&                           \n\
              u_model[0][1] == 0.0 &&                           \n\
              u_model[0][2] == 0.0)                             \n\
          { model = mat4(1.0); }                                \n\
                                                                \n\
          gl_Position = vp * model * vec4(a_vertPos, 1);        \n\
       }                                                        \n\
      ";

    const char* fsSource = 
      "#version 330 core                                        \n\
       out vec4 color;                                          \n\
                                                                \n\
       void main()                                              \n\
       {                                                        \n\
         color = vec4(1.0, 1.0, 1.0, 1.0);                      \n\
       }                                                        \n\
      ";
#else
    const char* vsSource =
      "#version 100                                             \n\
                                                                \n\
       attribute lowp vec3 a_vertPos;                           \n\
       uniform mat4 u_mvp;                                      \n\
                                                                \n\
       void main()                                              \n\
       {                                                        \n\
          mat4 mvp = u_mvp;                                     \n\
          if (u_mvp[0][0] == 0.0 &&                             \n\
              u_mvp[0][1] == 0.0 &&                             \n\
              u_mvp[0][2] == 0.0)                               \n\
          { mvp = mat4(1.0); }                                  \n\
                                                                \n\
          gl_Position = u_mvp * vec4(a_vertPos, 1);             \n\
       }                                                        \n\
      ";
    
    const char* fsSource =
      "#version 100                                             \n\
                                                                \n\
       void main()                                              \n\
       {                                                        \n\
         gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);               \n\
       }                                                        \n\
      ";
#endif

  };

  // ///////////////////////////////////////////////////////////////////////
  // BoundingBoxRenderSystem

  template <TLOC_BOUNDING_BOX_RENDER_SYSTEM_TEMPS>
  BoundingBoxRenderSystem_T<TLOC_BOUNDING_BOX_RENDER_SYSTEM_PARAMS>::
    BoundingBoxRenderSystem_T(event_manager_ptr a_eventMgr, 
                              entity_manager_ptr a_entityMgr)
    : base_type(a_eventMgr, a_entityMgr,
                register_type().Add<BoundingBox2D>().Add<BoundingBox3D>(), 
                "BoundingBoxRenderSystem")
  { 
    m_scene.AddSystem<gfx_cs::MaterialSystem>();
    m_meshSys = m_scene.AddSystem<mesh_sys_ptr::value_type>();
  }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  template <TLOC_BOUNDING_BOX_RENDER_SYSTEM_TEMPS>
  auto
    BoundingBoxRenderSystem_T<TLOC_BOUNDING_BOX_RENDER_SYSTEM_PARAMS>::
    Pre_Initialize() -> error_type
  {
    return ErrorSuccess;
  }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  template <TLOC_BOUNDING_BOX_RENDER_SYSTEM_TEMPS>
  auto
    BoundingBoxRenderSystem_T<TLOC_BOUNDING_BOX_RENDER_SYSTEM_PARAMS>::
    InitializeEntity(entity_ptr a_ent) -> error_type
  {
    if (a_ent->HasComponent<math_cs::Transformf32>() == false)
    {
      TLOC_LOG_GFX_WARN_FILENAME_ONLY() << "Entity " << a_ent 
        << " does not have a Transform component.";

      return ErrorFailure;
    }

    auto t      = a_ent->GetComponent<math_cs::Transformf32>();
    auto entMgr = m_scene.GetEntityManager();
    auto newEnt = entMgr->CreateEntity();

    auto vss = core_io::FileContents
      (core_io::Path("hard_coded_default_shader/defaultVS.glsl"), vsSource);
    auto fss = core_io::FileContents
      (core_io::Path("hard_coded_default_shader/defaultFS.glsl"), fsSource);
      
    if (a_ent->HasComponent<BoundingBox2D>())
    {
      auto bb = a_ent->GetComponent<BoundingBox2D>();
      entMgr->InsertComponent(core_cs::EntityManager::Params(newEnt, t).Orphan(true));
      m_scene.CreatePrefab<pref_gfx::QuadNoTexCoords>().Dimensions(bb->Get()).Add(newEnt);
      m_scene.CreatePrefab<pref_gfx::Material>().Add(newEnt, vss, fss);
    }
    else
    {
      auto bb = a_ent->GetComponent<BoundingBox3D>();
      entMgr->InsertComponent(core_cs::EntityManager::Params(newEnt, t).Orphan(true));
      m_scene.CreatePrefab<pref_gfx::cuboid_no_normals_no_tex>()
        .Dimensions(bb->Get()).Add(newEnt);
      m_scene.CreatePrefab<pref_gfx::Material>().Add(newEnt, vss, fss);
    }

    return ErrorSuccess;
  }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  template <TLOC_BOUNDING_BOX_RENDER_SYSTEM_TEMPS>
  auto
    BoundingBoxRenderSystem_T<TLOC_BOUNDING_BOX_RENDER_SYSTEM_PARAMS>::
    Post_Initialize() -> error_type
  {
    m_scene.Initialize();
    return ErrorSuccess;
  }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  template <TLOC_BOUNDING_BOX_RENDER_SYSTEM_TEMPS>
  void
    BoundingBoxRenderSystem_T<TLOC_BOUNDING_BOX_RENDER_SYSTEM_PARAMS>::
    SetCamera(const_entity_ptr a_camera)
  {
    base_type::SetCamera(a_camera);
    m_meshSys->SetCamera(this->GetCamera());
  }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  template <TLOC_BOUNDING_BOX_RENDER_SYSTEM_TEMPS>
  void
    BoundingBoxRenderSystem_T<TLOC_BOUNDING_BOX_RENDER_SYSTEM_PARAMS>::
    SetRenderer(renderer_type a_renderer)
  {
    base_type::SetRenderer(a_renderer);
    m_meshSys->SetRenderer(this->GetRenderer());
  }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  template <TLOC_BOUNDING_BOX_RENDER_SYSTEM_TEMPS>
  void
    BoundingBoxRenderSystem_T<TLOC_BOUNDING_BOX_RENDER_SYSTEM_PARAMS>::
    Pre_ProcessActiveEntities(f64)
  { m_scene.Process(); }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  template <TLOC_BOUNDING_BOX_RENDER_SYSTEM_TEMPS>
  void
    BoundingBoxRenderSystem_T<TLOC_BOUNDING_BOX_RENDER_SYSTEM_PARAMS>::
    ProcessEntity(entity_ptr, f64)
  { }

  // -----------------------------------------------------------------------
  // explicit instantiation

  template class BoundingBoxRenderSystem_T<>;
  template class BoundingBoxRenderSystem_T<gfx_rend::renderer_depth32_sptr>;
  template class BoundingBoxRenderSystem_T<gfx_rend::renderer_depth64_sptr>;

};};};

// -----------------------------------------------------------------------
// explicit instantiation

#include <tlocCore/smart_ptr/tloc_smart_ptr.inl.h>

using namespace tloc::gfx_cs;
TLOC_EXPLICITLY_INSTANTIATE_ALL_SMART_PTRS(BoundingBoxRenderSystem);
TLOC_EXTERN_TEMPLATE_VIRTUAL_STACK_OBJECT_NO_COPY_CTOR_NO_DEF_CTOR(BoundingBoxRenderSystem);