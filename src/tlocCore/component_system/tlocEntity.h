#ifndef TLOC_ENTITY_H
#define TLOC_ENTITY_H

#include <tlocCore/tlocCoreBase.h>

#include <tlocCore/smart_ptr/tloc_smart_ptr.h>

#include <tlocCore/tlocAssert.h>
#include <tlocCore/memory/tlocBufferArg.h>
#include <tlocCore/containers/tlocContainers.h>
#include <tlocCore/base_classes/tlocDebugName.h>
#include <tlocCore/component_system/tlocComponent.h>
#include <tlocCore/component_system/tlocComponentType.h>
#include <tlocCore/component_system/tlocComponentMapper.h>
#include <tlocCore/configs/tlocBuildConfig.h>
#include <tlocCore/base_classes/tlocNonCopyable.h>

namespace tloc { namespace core { namespace component_system {

  class EntityWorld;
  class EntityManager;

  class Entity
    : public base_classes::DebugName
    , public core_bclass::NonCopyable_I
  {
  public:
    friend class EntityManager;

    typedef Entity                                      this_type;
    typedef base_classes::DebugName_TI
      <core_cfg::BuildConfig::build_config_type>        base_type;

    typedef components::value_type                      component_type;
    typedef core::component_system::
                  component_sptr_array                  component_list;
    typedef containers::tl_array<component_list>::type  component_list_list;
    typedef tl_size                                     entity_id;
    typedef tl_size                                     size_type;

    Entity(entity_id  a_id);
    Entity(entity_id  a_id, BufferArg a_debugName);

    bool operator==(const this_type& a_other) const;
    TLOC_DECLARE_OPERATOR_NOT_EQUAL(this_type);

    bool                        HasComponent(component_type a_type) const;
    const component_list&       GetComponents(component_type a_type) const;

    template <typename T_ComponentType>
    core_sptr::SharedPtr<T_ComponentType>
                                GetComponent(size_type a_index = 0) const;

    template <typename T_ComponentType>
    bool                        HasComponent() const;

    entity_id                   GetID() const;
    size_type                   GetIndex() const;
    const component_list_list&  GetComponentsList() const;

    void                        Activate() const;
    void                        Deactivate() const;
    TLOC_DECL_AND_DEF_GETTER(bool,  IsActive, m_active);

    using base_type::GetDebugName;
    using base_type::SetDebugName;

  protected:

    void                        SetID(entity_id a_id);
    void                        SetIndex(size_type a_index);

    component_list&             DoGetComponents(component_type a_type);
    void                        InsertComponent(component_sptr a_type);

    component_list_list&        GetComponentsList();

  private:

    entity_id           m_id;
    size_type           m_index;
    mutable bool        m_active;
    component_list_list m_allComponents;
  };

  //------------------------------------------------------------------------
  // template definitions

  template <typename T_ComponentType>
  core_sptr::SharedPtr<T_ComponentType>
    Entity::GetComponent(size_type a_index) const
  {
    TLOC_STATIC_ASSERT( 
      (Loki::Conversion<T_ComponentType*, core_cs::Component*>::exists),
      T_ComponentType_is_not_a_valid_component);

    TLOC_ASSERT(HasComponent(T_ComponentType::k_component_type),
      "Component doesn't exist in this entity");
    typedef ComponentMapper<T_ComponentType> cmapper;
    cmapper temp = GetComponents(T_ComponentType::k_component_type);
    return temp[a_index];
  }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  template <typename T_ComponentType>
  bool
    Entity::HasComponent() const
  { 
    TLOC_STATIC_ASSERT( 
      (Loki::Conversion<T_ComponentType*, core_cs::Component*>::exists),
      T_ComponentType_is_not_a_valid_component);

    return HasComponent(T_ComponentType::k_component_type);
  }

  //------------------------------------------------------------------------
  // typedef

  TLOC_TYPEDEF_ALL_SMART_PTRS(Entity, entity);
  TLOC_TYPEDEF_VIRTUAL_STACK_OBJECT_NO_COPY_NO_DEF_CTOR(Entity, entity);

  typedef containers::tl_array<entity_vptr>::type            entity_ptr_array;
  typedef containers::tl_array<const_entity_vptr>::type      const_entity_ptr_array;
};};};

///-------------------------------------------------------------------------
/// @note one of the few one of the few inline files we include in the
/// header because of linker issues (because it is not a template) ;)
/// http://www.parashift.com/c++-faq/inline-member-fns.html
///-------------------------------------------------------------------------
#include <tlocCore/component_system/tlocEntity.inl.h>

#endif