#include "tlocEntitySystemBase.h"

#include <tlocCore/tlocAssert.h>
#include <tlocCore/data_structures/tlocVariadic.inl.h>

namespace tloc { namespace core { namespace component_system {

  // ///////////////////////////////////////////////////////////////////////
  // EntitySystemBase::Register

  EntitySystemBase::Register::this_type&
    EntitySystemBase::Register::
    Add(component_info_type a_info)
  { 
    m_registeredComps.push_back(a_info);
    return *this;
  }

  // -----------------------------------------------------------------------

  enum
  {
    k_systemInitialized = 0,
    k_count
  };

  //////////////////////////////////////////////////////////////////////////
  // typedefs]

  typedef EntitySystemBase::error_type      error_type;

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  EntitySystemBase::
    EntitySystemBase(event_manager_ptr a_eventMgr, 
                     entity_manager_ptr a_entityMgr, 
                     register_type a_compsToRegister)
    : m_compRegistry(a_compsToRegister)
    , m_eventMgr(a_eventMgr)
    , m_entityMgr(a_entityMgr)
    , m_flags(k_count)
  { 
    m_eventMgr->AddListener(this, entity_events::insert_component);
    m_eventMgr->AddListener(this, entity_events::remove_component);
  }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  EntitySystemBase::
    ~EntitySystemBase()
  {
    m_eventMgr->RemoveListener
      (this, entity_events::insert_component);
    m_eventMgr->RemoveListener
      (this, entity_events::remove_component);
  }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  error_type
    EntitySystemBase::
    Initialize()
  {
    m_flags.Mark(k_systemInitialized);

    if (Pre_Initialize() == ErrorSuccess)
    {
      if (DoInitialize(m_activeEntities) == ErrorSuccess)
      {
        return Post_Initialize();
      }
    }

    return ErrorFailure;
  }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  bool
    EntitySystemBase::
    IsInitialized() const
  { return m_flags.IsMarked(k_systemInitialized); }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  void
    EntitySystemBase::
    ProcessActiveEntities(time_type a_deltaT)
  {
    TLOC_ASSERT(m_flags.IsMarked(k_systemInitialized),
      "Did you forget to call Initialize()?");

    if (CheckProcessing())
    {
      Pre_ProcessActiveEntities(a_deltaT);
      DoProcessActiveEntities(m_activeEntities, a_deltaT);
      Post_ProcessActiveEntities(a_deltaT);
    }
  }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  error_type
    EntitySystemBase::
    Shutdown()
  {
    if (Pre_Shutdown() == ErrorSuccess)
    {
      if (DoShutdown(m_activeEntities) == ErrorSuccess)
      {
        return Post_Shutdown();
      }
    }

    return ErrorFailure;
  }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  EventReturn
    EntitySystemBase::
    OnEvent(const EventBase& a_event)
  {
    typedef register_type::iterator                       register_itr_type;

    event_value_type type = a_event.GetType();

    EventReturn evtRet(false, false);

    switch(type)
    {
    case entity_events::insert_component:
      {
        const EntityComponentEvent& entEvent = a_event.GetAs<EntityComponentEvent>();

        // does the event have the component we are interested in?
        register_itr_type itr = 
          core::find_all(m_compRegistry.m_registeredComps, entEvent.GetComponent()->GetInfo());

        if (itr == m_compRegistry.m_registeredComps.end())
        { break; }

        entity_vptr ent = entEvent.GetEntity();

        for (register_itr_type itr = m_compRegistry.m_registeredComps.begin(), 
                               itrEnd = m_compRegistry.m_registeredComps.end(); 
                               itr != itrEnd; ++itr)
        {
          if (ent->HasComponent(*itr) )
          {
            evtRet.m_componentInSystem = true;

            entity_count_cont::iterator entItr = 
              core::find_if_all(m_activeEntities, 
              core::algos::pair::compare::MakeFirst(ent));

            if (entItr == m_activeEntities.end())
            {
              OnComponentInsert(entEvent);
              m_activeEntities.push_back(MakePair(ent, 1));
            }
            else
            {
              entItr->second++;
            }
          }
        }
        break;
      }
    case entity_events::remove_component:
      {
        const EntityComponentEvent& entEvent = a_event.GetAs<EntityComponentEvent>();
        entity_vptr ent = entEvent.GetEntity();

        for (register_itr_type itr = m_compRegistry.m_registeredComps.begin(), 
                               itrEnd = m_compRegistry.m_registeredComps.end(); 
                               itr != itrEnd; ++itr)
        {
          if (ent->HasComponent(*itr) )
          {
            entity_count_cont::iterator entItr = 
              core::find_if_all(m_activeEntities, 
              core::algos::pair::compare::MakeFirst(ent));

            if (entItr != m_activeEntities.end())
            {
              OnComponentRemove(entEvent);
              if (entItr->second == 1)
              {
                m_activeEntities.erase(entItr);
              }
              else
              {
                entItr->second--;
              }
            }
          }
        }
        break;
      }

    case entity_events::disable_component:
    case entity_events::enable_component:
      {
        const EntityComponentEvent& entEvent = a_event.GetAs<EntityComponentEvent>();
        component_sptr comp = entEvent.GetComponent();

        for (register_itr_type itr = m_compRegistry.m_registeredComps.begin(), 
                               itrEnd = m_compRegistry.m_registeredComps.end(); 
                               itr != itrEnd; ++itr)
        {
          if (comp->GetInfo() == *itr)
          {
            if (type == entity_events::disable_component)
            { OnComponentDisable(entEvent); }
            else
            { OnComponentEnable(entEvent); }
          }
        }
        break;
      }
    }

    return evtRet;
  }

  // -----------------------------------------------------------------------
  // algorithms

  namespace algos { namespace entity_system {

    // ///////////////////////////////////////////////////////////////////////
    // initialize

    void
      Initialize::
      operator()(value_type& a_system)
    { a_system.Initialize(); }

    // ///////////////////////////////////////////////////////////////////////
    // shutdown

    void
      ShutDown::
      operator()(value_type& a_system)
    { a_system.Shutdown(); }

    // ///////////////////////////////////////////////////////////////////////
    // process

    Process::
      Process(time_type a_deltaT)
      : m_deltaT(a_deltaT)
    { }

    // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

    void
      Process::
      operator()(value_type& a_system)
    { a_system.ProcessActiveEntities(m_deltaT); }

  };};

};};};

#include <tlocCore/smart_ptr/tlocVirtualPtr.inl.h>
TLOC_EXPLICITLY_INSTANTIATE_VIRTUAL_PTR(tloc::core_cs::EntitySystemBase);