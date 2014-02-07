#ifndef TLOC_ENTITY_MANAGER_H
#define TLOC_ENTITY_MANAGER_H

#include <tlocCore/tlocCoreBase.h>

#include <tlocCore/containers/tlocContainers.h>
#include <tlocCore/component_system/tlocEntity.h>
#include <tlocCore/component_system/tlocEntityEvent.h>
#include <tlocCore/component_system/tlocEventManager.h>
#include <tlocCore/component_system/tlocComponent.h>
#include <tlocCore/component_system/tlocComponentType.h>
#include <tlocCore/base_classes/tlocNonCopyable.h>

namespace tloc { namespace core { namespace component_system {

  class EntityManager
    : public core_bclass::NonCopyable_I
  {
  public:
    typedef Entity                                entity_type;
    typedef entity_vptr                           entity_ptr_type;
    typedef component_vptr                        component_ptr_type;

    typedef containers::
      tl_array<entity_ptr_type>::type             entity_cont;
    typedef containers::
      tl_array<Entity::entity_id>::type           entity_id_cont;
    typedef containers::
      tl_array<entity_cont>::type                 component_entity_cont;

    typedef Pair<entity_ptr_type,
                 component_ptr_type>              ent_comp_pair_type;
    typedef containers::
      tl_array<ent_comp_pair_type>::type          ent_comp_pair_cont;

    typedef Entity::component_list                component_cont;
    typedef Entity::entity_id                     entity_id_type;

  public:
    EntityManager(event_manager_sptr a_eventManager);
    virtual ~EntityManager();

    entity_ptr_type   CreateEntity();
    void              DestroyEntity(entity_ptr_type a_entity);
    entity_ptr_type   GetEntity(tl_int a_index);

    void        InsertComponent(entity_ptr_type a_entity, component_ptr_type a_component);
    bool        RemoveComponent(entity_ptr_type a_entity, component_ptr_type a_component);

    void        Update();

    component_cont* GetComponents(entity_ptr_type a_entity, components::value_type a_type);

    TLOC_DECL_AND_DEF_GETTER(entity_cont::size_type, GetUnusedEntities,
                             m_removedEntities.size());

  private:

    void DoUpdateAndCleanComponents();
    void DoUpdateAndCleanEntities();

    bool DoRemoveComponent(entity_ptr_type a_entity, component_ptr_type a_component);

  private:

    entity_cont             m_entities;
    entity_id_cont          m_removedEntities;
    component_entity_cont   m_componentsAndEntities;
    event_manager_sptr      m_eventMgr;
    entity_id_type          m_nextId;

    ent_comp_pair_cont      m_compToRemove;
    entity_cont             m_entitiesToRemove;
  };

  //------------------------------------------------------------------------
  // typedefs

  TLOC_TYPEDEF_SHARED_PTR(EntityManager, entity_manager);

};};};

#endif