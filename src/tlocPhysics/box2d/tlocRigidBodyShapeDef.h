#pragma once
#ifndef _TLOC_PHYSICS_BOX2D_RIGID_BODY_SHAPE_H_
#define _TLOC_PHYSICS_BOX2D_RIGID_BODY_SHAPE_H_

#include <tlocCore/tlocBase.h>
#include <tlocCore/utilities/tlocUtils.h>
#include <tlocCore/component_system/tlocEntity.h>

#include <tlocPhysics/box2d/tlocShape.h>

#include <Box2D/Dynamics/b2Fixture.h>

namespace tloc { namespace physics { namespace box2d {

  ///-------------------------------------------------------------------------
  /// @brief  RigidBodyShapeDef is used in conjunction with RigidBodyShape
  /// component. Used to define "visible" physical attributes of the RigidBody.
  ///-------------------------------------------------------------------------
  class RigidBodyShapeDef
  {
  public:
    friend class RigidBody;

  public:
    typedef RigidBodyShapeDef   this_type;
    typedef Shape               shape_type;

    typedef shape_type::shape_internal_type_ptr   shape_internal_type_ptr;

    typedef f32 float_type;

  public:
    RigidBodyShapeDef(const shape_type& a_shape);

    void SetShape(const shape_type& a_shape);

    TLOC_DECL_AND_DEF_GETTER(float_type, GetFriction, m_fixtureDef.friction);
    TLOC_DECL_AND_DEF_GETTER(float_type, GetRestitution, m_fixtureDef.restitution);
    TLOC_DECL_AND_DEF_GETTER(float_type, GetDensity, m_fixtureDef.density);
    TLOC_DECL_AND_DEF_GETTER(bool, IsSensor, m_fixtureDef.isSensor);
    //TODO: Get collision filter
    
    TLOC_DECL_AND_DEF_SETTER(float_type, SetFriction, m_fixtureDef.friction);
    TLOC_DECL_AND_DEF_SETTER(float_type, SetRestitution, m_fixtureDef.restitution);
    TLOC_DECL_AND_DEF_SETTER(float_type, SetDensity, m_fixtureDef.density);
    TLOC_DECL_AND_DEF_SETTER(bool, SetSensor, m_fixtureDef.isSensor);
    //TODO: Set collision filter

  protected:
    typedef b2FixtureDef                      fixture_def_internal_type;
    typedef core::component_system::Entity    entity_type;

    TLOC_DECL_AND_DEF_GETTER_CONST_DIRECT
      (fixture_def_internal_type, DoGetFixtureDef, m_fixtureDef);

    TLOC_DECL_AND_DEF_SETTER
      (entity_type*, DoSetParent, m_fixtureDef.userData);

  private:
    shape_internal_type_ptr   m_internalShape;
    fixture_def_internal_type m_fixtureDef;
  };

};};};

#endif