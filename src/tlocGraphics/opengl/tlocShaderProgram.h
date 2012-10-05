#ifndef TLOC_SHADER_PROGRAM_H
#define TLOC_SHADER_PROGRAM_H

#include <tlocCore/utilities/tlocCheckpoints.h>
#include <tlocCore/data_structures/tlocVariadic.h>

#include <tlocGraphics/opengl/tlocObject.h>
#include <tlocGraphics/opengl/tlocShaderComponent.h>

namespace tloc { namespace graphics { namespace gl {

  class ShaderProgram : public Object
  {
  public:
    // Supported number of shader components
    typedef core::Variadic<ShaderComponent*, 1>     one_shader_component;
    typedef core::Variadic<ShaderComponent*, 2>     two_shader_components;
    typedef core::Variadic<ShaderComponent*, 3>     three_shader_components;
    typedef core::Variadic<ShaderComponent*, 4>     four_shader_components;

    typedef Object                      base_type;
    using base_type::object_handle;

    typedef tl_size                     size_type;

    ShaderProgram();
    ~ShaderProgram();

    template <size_type T_Size>
    bool AttachShaders(core::Variadic<ShaderComponent*, T_Size>
                       a_shaderComponents);

    bool Link();
    void Enable();

  private:

    core::utils::Checkpoints    m_flags;
  };

};};};

#endif