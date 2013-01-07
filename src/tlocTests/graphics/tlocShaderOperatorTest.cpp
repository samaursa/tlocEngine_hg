#include "tlocTestCommon.h"

#include <tlocGraphics/renderer/tlocRenderer.h>
#include <tlocGraphics/window/tlocWindow.h>
#include <tlocGraphics/opengl/tlocShader.h>
#include <tlocGraphics/opengl/tlocShaderProgram.h>
#include <tlocGraphics/opengl/tlocShaderOperator.h>
#include <tlocGraphics/opengl/tlocError.h>
#include <tlocGraphics/types/tlocColor.h>

#include <tlocMath/types/tlocVector2.h>
#include <tlocMath/types/tlocVector2.inl>
#include <tlocMath/types/tlocVector3.h>
#include <tlocMath/types/tlocVector3.inl>
#include <tlocMath/types/tlocVector4.h>
#include <tlocMath/types/tlocVector4.inl>

namespace TestingShaderOperator
{
  const char* vShaderStr =
    "#ifdef GL_ES                                                      \n"
    "#  version 100                                                    \n"
    "#else                                                             \n"
    "#  version 140                                                    \n"
    "#endif                                                            \n"
    "                                                                  \n"
    "  uniform float  u_float;                                         \n"
    "  uniform vec2   u_vec2;                                          \n"
    "  uniform vec3   u_vec3;                                          \n"
    "  uniform vec4   u_vec4;                                          \n"
    "  uniform int    u_int;                                           \n"
    "  uniform ivec2  u_ivec2;                                         \n"
    "  uniform ivec3  u_ivec3;                                         \n"
    "  uniform ivec4  u_ivec4;                                         \n"
    "  uniform uint   u_uint;                                          \n"
    "  uniform uvec2  u_uivec2;                                        \n"
    "  uniform uvec3  u_uivec3;                                        \n"
    "  uniform uvec4  u_uivec4;                                        \n"
    "  uniform mat2   u_mat2;                                          \n"
    "  uniform mat3   u_mat3;                                          \n"
    "  uniform mat4   u_mat4;                                          \n"
    "                                                                  \n"
    "void main(void)                                                   \n"
    "{                                                                 \n"
    "  gl_Position.x = u_float * u_vec2.x * u_vec3.x * u_vec4.x;       \n"
    "  gl_Position.y = u_int * u_ivec2.x * u_ivec3.x * u_ivec4.x;      \n"
    "  gl_Position.z = u_mat2[0].x + u_mat3[0].x + u_mat4[0].x;        \n"
    "  gl_Position.a = u_uint * u_uivec2.x * u_uivec3.x * u_uivec4.x;  \n"
    "}\n";

  using namespace tloc;
  using namespace graphics;
  using namespace core;

  struct fixture
  {
    typedef gl::UniformPtr          uniform_ptr_type;
    typedef gl::AttributePtr        attribute_ptr_type;
    typedef gl::ShaderOperatorPtr   shader_op_ptr;
  };

  TEST_CASE_METHOD(fixture, "Graphics/ShaderOperator/Uniforms", "")
  {
    typedef Window::graphics_mode       graphics_mode;

    Window win;
    win.Create(graphics_mode(graphics_mode::Properties(1, 1)),
               WindowSettings("Atom & Eve"));

    // Initialize glew
    REQUIRE(Renderer().Initialize() != common_error_types::error_initialize);

    gl::VertexShader  vShader;
    REQUIRE(vShader.Load(vShaderStr) == ErrorSuccess());
    REQUIRE(vShader.Compile() == ErrorSuccess());

    gl::ShaderProgram sp;
    sp.AttachShaders(gl::ShaderProgram::one_shader_component(&vShader));
    REQUIRE(sp.Link() == ErrorSuccess());
    CHECK(gl::Error().Succeeded());

    // Cache the attributes and uniforms
    sp.Enable();
    sp.LoadUniformInfo();
    sp.Disable();
    CHECK(gl::Error().Succeeded());

    shader_op_ptr so(new gl::ShaderOperator());

    //------------------------------------------------------------------------
    // Add all the uniforms
    {
      uniform_ptr_type    uniform(new gl::Uniform());
      uniform->SetName("u_float");
      uniform->SetValueAs(f32(5.0f));

      so->AddUniform(uniform);
    }
    {
      uniform_ptr_type    uniform(new gl::Uniform());
      uniform->SetName("u_vec2");
      uniform->SetValueAs(math::Vec2f32(0.1f, 0.2f));

      so->AddUniform(uniform);
    }
    {
      uniform_ptr_type    uniform(new gl::Uniform());
      uniform->SetName("u_vec3");
      uniform->SetValueAs(math::Vec3f32(0.1f, 0.2f, 0.3f));

      so->AddUniform(uniform);
    }
    {
      uniform_ptr_type    uniform(new gl::Uniform());
      uniform->SetName("u_vec4");
      uniform->SetValueAs(math::Vec4f32(0.1f, 0.2f, 0.3f, 0.4f));

      so->AddUniform(uniform);
    }
    {
      uniform_ptr_type    uniform(new gl::Uniform());
      uniform->SetName("u_int");
      uniform->SetValueAs(s32(5));

      so->AddUniform(uniform);
    }
    {
      uniform_ptr_type    uniform(new gl::Uniform());
      uniform->SetName("u_ivec2");
      uniform->SetValueAs(Tuple2s32(2));

      so->AddUniform(uniform);
    }
    {
      uniform_ptr_type    uniform(new gl::Uniform());
      uniform->SetName("u_ivec3");
      uniform->SetValueAs(Tuple3s32(3));

      so->AddUniform(uniform);
    }
    {
      uniform_ptr_type    uniform(new gl::Uniform());
      uniform->SetName("u_ivec4");
      uniform->SetValueAs(Tuple4s32(4));

      so->AddUniform(uniform);
    }
    {
      uniform_ptr_type    uniform(new gl::Uniform());
      uniform->SetName("u_uint");
      uniform->SetValueAs(u32(5));

      so->AddUniform(uniform);
    }
    {
      uniform_ptr_type    uniform(new gl::Uniform());
      uniform->SetName("u_uivec2");
      uniform->SetValueAs(Tuple2u32(2));

      so->AddUniform(uniform);
    }
    {
      uniform_ptr_type    uniform(new gl::Uniform());
      uniform->SetName("u_uivec3");
      uniform->SetValueAs(Tuple3u32(3));

      so->AddUniform(uniform);
    }
    {
      uniform_ptr_type    uniform(new gl::Uniform());
      uniform->SetName("u_uivec4");
      uniform->SetValueAs(Tuple4u32(4));

      so->AddUniform(uniform);
    }
    {
      uniform_ptr_type    uniform(new gl::Uniform());
      uniform->SetName("u_mat2");
      uniform->SetValueAs(math::Mat2f32(1, 0,
                                        0, 1));

      so->AddUniform(uniform);
    }
    {
      uniform_ptr_type    uniform(new gl::Uniform());
      uniform->SetName("u_mat3");
      uniform->SetValueAs(math::Mat3f32(1, 0, 0,
                                        0, 1, 0,
                                        0, 0, 1));

      so->AddUniform(uniform);
    }
    {
      uniform_ptr_type    uniform(new gl::Uniform());
      uniform->SetName("u_mat4");
      uniform->SetValueAs(math::Mat4f32(1, 0, 0, 0,
                                        0, 1, 0, 0,
                                        0, 0, 1, 0,
                                        0, 0, 0, 1));

      so->AddUniform(uniform);
    }

    sp.Enable();
    CHECK(gl::Error().Succeeded());
    CHECK(so->PrepareAllUniforms(sp) == ErrorSuccess());
    CHECK(gl::Error().Succeeded());
    sp.Disable();
  }

  const char* vShaderStr2 =
    "#ifdef GL_ES                                                      \n"
    "#  version 100                                                    \n"
    "#else                                                             \n"
    "#  version 140                                                    \n"
    "#endif                                                            \n"
    "                                                                  \n"
    "  uniform float u_float[2];                                       \n"
    "  uniform vec2  u_vec2[2];                                        \n"
    "  uniform vec3  u_vec3[2];                                        \n"
    "  uniform vec4  u_vec4[2];                                        \n"
    "  uniform int   u_int[2];                                         \n"
    "  uniform ivec2 u_ivec2[2];                                       \n"
    "  uniform ivec3 u_ivec3[2];                                       \n"
    "  uniform ivec4 u_ivec4[2];                                       \n"
    "  uniform uint  u_uint[2];                                        \n"
    "  uniform uvec2 u_uivec2[2];                                      \n"
    "  uniform uvec3 u_uivec3[2];                                      \n"
    "  uniform uvec4 u_uivec4[2];                                      \n"
    "                                                                  \n"
    "void main(void)                                                   \n"
    "{                                                                 \n"
    "  gl_Position.x = u_float[0] * u_vec2[0].x * u_vec3[0].x *        \n"
    "                  u_vec4[0].x;                                    \n"
    "  gl_Position.y = u_int[0] * u_ivec2[0].x * u_ivec3[0].x  *       \n"
    "                  u_ivec4[0].x;                                   \n"
    "  gl_Position.z = u_uint[0] + u_uivec2[0].x + u_uivec3[0].x *     \n"
    "                  u_uivec4[0].x;                                  \n"
    "}\n";

  TEST_CASE_METHOD(fixture, "Graphics/ShaderOperator/UniformArrays", "")
  {
    typedef Window::graphics_mode       graphics_mode;

    Window win;
    win.Create(graphics_mode(graphics_mode::Properties(1, 1)),
               WindowSettings("Atom & Eve"));

    // Initialize glew
    REQUIRE(Renderer().Initialize() != common_error_types::error_initialize);

    gl::VertexShader  vShader;
    REQUIRE(vShader.Load(vShaderStr2) == ErrorSuccess());
    REQUIRE(vShader.Compile() == ErrorSuccess());

    gl::ShaderProgram sp;
    sp.AttachShaders(gl::ShaderProgram::one_shader_component(&vShader));
    REQUIRE(sp.Link() == ErrorSuccess());
    CHECK(gl::Error().Succeeded());

    // Cache the attributes and uniforms
    sp.Enable();
    sp.LoadUniformInfo();
    sp.Disable();
    CHECK(gl::Error().Succeeded());

    shader_op_ptr so(new gl::ShaderOperator());

    //------------------------------------------------------------------------
    // Add all the uniforms
    {
      uniform_ptr_type    uniform(new gl::Uniform());
      uniform->SetName("u_float");

      core::Array<f32>  floats(2, 2.0f);
      uniform->SetValueAs(floats, gl::p_shader_variable_ti::SwapArray());

      so->AddUniform(uniform);
    }
    {
      uniform_ptr_type    uniform(new gl::Uniform());
      uniform->SetName("u_vec2");

      core::Array<math::Vec2f32>  floats(2, math::Vec2f32(5.0f, 6.0f));
      uniform->SetValueAs(floats, gl::p_shader_variable_ti::SwapArray());

      so->AddUniform(uniform);
    }
    {
      uniform_ptr_type    uniform(new gl::Uniform());
      uniform->SetName("u_vec3");

      core::Array<math::Vec3f32>  floats(2, math::Vec3f32(1, 2, 3));
      uniform->SetValueAs(floats, gl::p_shader_variable_ti::SwapArray());

      so->AddUniform(uniform);
    }
    {
      uniform_ptr_type    uniform(new gl::Uniform());
      uniform->SetName("u_vec4");

      core::Array<math::Vec4f32>  floats(2, math::Vec4f32(1, 2, 3, 4));
      uniform->SetValueAs(floats, gl::p_shader_variable_ti::SwapArray());

      so->AddUniform(uniform);
    }
    {
      uniform_ptr_type    uniform(new gl::Uniform());
      uniform->SetName("u_int");

      core::Array<s32>  ints(2, 1);
      uniform->SetValueAs(ints, gl::p_shader_variable_ti::SwapArray());

      so->AddUniform(uniform);
    }
    {
      uniform_ptr_type    uniform(new gl::Uniform());
      uniform->SetName("u_ivec2");

      core::Array<Tuple2s32>  ints(2, Tuple2s32(2));
      uniform->SetValueAs(ints, gl::p_shader_variable_ti::SwapArray());

      so->AddUniform(uniform);
    }
    {
      uniform_ptr_type    uniform(new gl::Uniform());
      uniform->SetName("u_ivec3");

      core::Array<Tuple3s32>  ints(2, Tuple3s32(3));
      uniform->SetValueAs(ints, gl::p_shader_variable_ti::SwapArray());

      so->AddUniform(uniform);
    }
    {
      uniform_ptr_type    uniform(new gl::Uniform());
      uniform->SetName("u_ivec4");

      core::Array<Tuple4s32>  ints(2, Tuple4s32(4));
      uniform->SetValueAs(ints, gl::p_shader_variable_ti::SwapArray());

      so->AddUniform(uniform);
    }

    {
      uniform_ptr_type    uniform(new gl::Uniform());
      uniform->SetName("u_uint");

      core::Array<u32>  ints(2, 1);
      uniform->SetValueAs(ints, gl::p_shader_variable_ti::SwapArray());

      so->AddUniform(uniform);
    }
    {
      uniform_ptr_type    uniform(new gl::Uniform());
      uniform->SetName("u_uivec2");

      core::Array<Tuple2u32>  ints(2, Tuple2u32(2));
      uniform->SetValueAs(ints, gl::p_shader_variable_ti::SwapArray());

      so->AddUniform(uniform);
    }
    {
      uniform_ptr_type    uniform(new gl::Uniform());
      uniform->SetName("u_uivec3");

      core::Array<Tuple3u32>  ints(2, Tuple3u32(3));
      uniform->SetValueAs(ints, gl::p_shader_variable_ti::SwapArray());

      so->AddUniform(uniform);
    }
    {
      uniform_ptr_type    uniform(new gl::Uniform());
      uniform->SetName("u_uivec4");

      core::Array<Tuple4u32>  ints(2, Tuple4u32(4));
      uniform->SetValueAs(ints, gl::p_shader_variable_ti::SwapArray());

      so->AddUniform(uniform);
    }

    sp.Enable();
    CHECK(gl::Error().Succeeded());
    CHECK(so->PrepareAllUniforms(sp) == ErrorSuccess());
    CHECK(gl::Error().Succeeded());
    sp.Disable();
  }

  const char* vShaderStr3 =
    "#ifdef GL_ES                                                      \n"
    "#  version 100                                                    \n"
    "#else                                                             \n"
    "#  version 140                                                    \n"
    "#endif                                                            \n"
    "                                                                  \n"
    "  attribute float u_float;                                        \n"
    "  attribute vec2  u_vec2;                                         \n"
    "  attribute vec3  u_vec3;                                         \n"
    "  attribute vec4  u_vec4;                                         \n"
    "  attribute int   u_int;                                          \n"
    "  attribute ivec2 u_ivec2;                                        \n"
    "  attribute ivec3 u_ivec3;                                        \n"
    "  attribute ivec4 u_ivec4;                                        \n"
    "  attribute uint  u_uint;                                         \n"
    "  attribute uvec2 u_uivec2;                                       \n"
    "  attribute uvec3 u_uivec3;                                       \n"
    "  attribute uvec4 u_uivec4;                                       \n"
    "                                                                  \n"
    "void main(void)                                                   \n"
    "{                                                                 \n"
    "  gl_Position   = u_vec4;                                         \n"
    "  gl_Position.x = u_float * u_vec2.x * u_vec3.x;                  \n"
    "  gl_Position.y = u_int * u_ivec2.x * u_ivec3.x * u_ivec4.x;      \n"
    "  gl_Position.z = u_uint * u_uivec2.x * u_uivec3.x * u_uivec4.x;  \n"
    "}\n";

  TEST_CASE_METHOD(fixture, "Graphics/ShaderOperator/ConstantAttributes", "")
  {
    typedef Window::graphics_mode       graphics_mode;

    Window win;
    win.Create(graphics_mode(graphics_mode::Properties(1, 1)),
               WindowSettings("Atom & Eve"));

    // Initialize glew
    REQUIRE(Renderer().Initialize() != common_error_types::error_initialize);

    gl::VertexShader  vShader;
    REQUIRE(vShader.Load(vShaderStr3) == ErrorSuccess());
    REQUIRE(vShader.Compile() == ErrorSuccess());

    gl::ShaderProgram sp;
    sp.AttachShaders(gl::ShaderProgram::one_shader_component(&vShader));
    REQUIRE(sp.Link() == ErrorSuccess());
    CHECK(gl::Error().Succeeded());

    // Cache the attributes and uniforms
    sp.Enable();
    sp.LoadAttributeInfo();
    sp.Disable();
    CHECK(gl::Error().Succeeded());

    shader_op_ptr so(new gl::ShaderOperator());

    {
      attribute_ptr_type attribute(new gl::Attribute());
      attribute->SetName("u_float");
      attribute->SetValueAs(f32(5.0f));

      so->AddAttribute(attribute);
    }
    {
      attribute_ptr_type attribute(new gl::Attribute());
      attribute->SetName("u_vec2");
      attribute->SetValueAs(math::Vec2f32(0.1f, 0.2f));

      so->AddAttribute(attribute);
    }
    {
      attribute_ptr_type attribute(new gl::Attribute());
      attribute->SetName("u_vec3");
      attribute->SetValueAs(math::Vec3f32(0.1f, 0.2f, 0.3f));

      so->AddAttribute(attribute);
    }
    {
      attribute_ptr_type attribute(new gl::Attribute());
      attribute->SetName("u_vec4");
      attribute->SetValueAs(math::Vec4f32(0.1f, 0.2f, 0.3f, 0.4f));

      so->AddAttribute(attribute);
    }
    {
      attribute_ptr_type attribute(new gl::Attribute());
      attribute->SetName("u_int");
      attribute->SetValueAs(s32(5));

      so->AddAttribute(attribute);
    }
    {
      attribute_ptr_type attribute(new gl::Attribute());
      attribute->SetName("u_ivec2");
      attribute->SetValueAs(Tuple2s32(2));

      so->AddAttribute(attribute);
    }
    {
      attribute_ptr_type attribute(new gl::Attribute());
      attribute->SetName("u_ivec3");
      attribute->SetValueAs(Tuple3s32(3));

      so->AddAttribute(attribute);
    }
    {
      attribute_ptr_type attribute(new gl::Attribute());
      attribute->SetName("u_ivec4");
      attribute->SetValueAs(Tuple4s32(4));

      so->AddAttribute(attribute);
    }
    {
      attribute_ptr_type attribute(new gl::Attribute());
      attribute->SetName("u_uint");
      attribute->SetValueAs(u32(5));

      so->AddAttribute(attribute);
    }
    {
      attribute_ptr_type attribute(new gl::Attribute());
      attribute->SetName("u_uivec2");
      attribute->SetValueAs(Tuple2u32(2));

      so->AddAttribute(attribute);
    }
    {
      attribute_ptr_type attribute(new gl::Attribute());
      attribute->SetName("u_uivec3");
      attribute->SetValueAs(Tuple3u32(3));

      so->AddAttribute(attribute);
    }
    {
      attribute_ptr_type attribute(new gl::Attribute());
      attribute->SetName("u_uivec4");
      attribute->SetValueAs(Tuple4u32(4));

      so->AddAttribute(attribute);
    }

    sp.Enable();
    CHECK(gl::Error().Succeeded());
    CHECK(so->PrepareAllAttributes(sp) == ErrorSuccess());
    CHECK(gl::Error().Succeeded());
    sp.Disable();
  }

  const char* vShaderStr4 =
    "#ifdef GL_ES                                                      \n"
    "#  version 100                                                    \n"
    "#else                                                             \n"
    "#  version 140                                                    \n"
    "#endif                                                            \n"
    "                                                                  \n"
    "  attribute float u_float;                                        \n"
    "  attribute vec2  u_vec2;                                         \n"
    "  attribute vec3  u_vec3;                                         \n"
    "  attribute vec4  u_vec4;                                         \n"
    "  attribute int   u_int;                                          \n"
    "  attribute ivec2 u_ivec2;                                        \n"
    "  attribute ivec3 u_ivec3;                                        \n"
    "  attribute ivec4 u_ivec4;                                        \n"
    "  attribute uint  u_uint;                                         \n"
    "  attribute uvec2 u_uivec2;                                       \n"
    "  attribute uvec3 u_uivec3;                                       \n"
    "  attribute uvec4 u_uivec4;                                       \n"
    "                                                                  \n"
    "void main(void)                                                   \n"
    "{                                                                 \n"
    "  gl_Position   = u_vec4;                                         \n"
    "  gl_Position.x = u_float * u_vec2.x * u_vec3.x;                  \n"
    "  gl_Position.y = u_int * u_ivec2.x * u_ivec3.x * u_ivec4.x;      \n"
    "  gl_Position.z = u_uint * u_uivec2.x * u_uivec3.x * u_uivec4.x;  \n"
    "}\n";

  TEST_CASE_METHOD(fixture, "Graphics/ShaderOperator/AttributesArrays", "")
  {
    typedef Window::graphics_mode       graphics_mode;

    Window win;
    win.Create(graphics_mode(graphics_mode::Properties(1, 1)),
               WindowSettings("Atom & Eve"));

    // Initialize glew
    REQUIRE(Renderer().Initialize() != common_error_types::error_initialize);

    gl::VertexShader  vShader;
    REQUIRE(vShader.Load(vShaderStr4) == ErrorSuccess());
    REQUIRE(vShader.Compile() == ErrorSuccess());

    gl::ShaderProgram sp;
    sp.AttachShaders(gl::ShaderProgram::one_shader_component(&vShader));
    REQUIRE(sp.Link() == ErrorSuccess());
    CHECK(gl::Error().Succeeded());

    // Cache the attributes and uniforms
    sp.Enable();
    sp.LoadAttributeInfo();
    sp.Disable();
    CHECK(gl::Error().Succeeded());

    shader_op_ptr so(new gl::ShaderOperator());

    {
      attribute_ptr_type attribute(new gl::Attribute());
      attribute->SetName("u_float");

      core::Array<f32>  floats(2, 2.0f);
      attribute->SetVertexArray(floats, gl::p_shader_variable_ti::SwapArray());

      so->AddAttribute(attribute);
    }
    {
      attribute_ptr_type attribute(new gl::Attribute());
      attribute->SetName("u_vec2");

      core::Array<math::Vec2f32>  floats(2, math::Vec2f32(5.0f, 6.0f));
      attribute->SetVertexArray(floats, gl::p_shader_variable_ti::SwapArray());

      so->AddAttribute(attribute);
    }
    {
      attribute_ptr_type attribute(new gl::Attribute());
      attribute->SetName("u_vec3");

      core::Array<math::Vec3f32>  floats(2, math::Vec3f32(1, 2, 3));
      attribute->SetVertexArray(floats, gl::p_shader_variable_ti::SwapArray());

      so->AddAttribute(attribute);
    }
    {
      attribute_ptr_type attribute(new gl::Attribute());
      attribute->SetName("u_vec4");

      core::Array<math::Vec4f32>  floats(2, math::Vec4f32(1, 2, 3, 4));
      attribute->SetVertexArray(floats, gl::p_shader_variable_ti::SwapArray());

      so->AddAttribute(attribute);
    }
    {
      attribute_ptr_type attribute(new gl::Attribute());
      attribute->SetName("u_int");

      core::Array<s32>  ints(2, 1);
      attribute->SetVertexArray(ints, gl::p_shader_variable_ti::SwapArray());

      so->AddAttribute(attribute);
    }
    {
      attribute_ptr_type attribute(new gl::Attribute());
      attribute->SetName("u_ivec2");

      core::Array<Tuple2s32>  ints(2, Tuple2s32(2));
      attribute->SetVertexArray(ints, gl::p_shader_variable_ti::SwapArray());

      so->AddAttribute(attribute);
    }
    {
      attribute_ptr_type attribute(new gl::Attribute());
      attribute->SetName("u_ivec3");

      core::Array<Tuple3s32>  ints(2, Tuple3s32(3));
      attribute->SetVertexArray(ints, gl::p_shader_variable_ti::SwapArray());

      so->AddAttribute(attribute);
    }
    {
      attribute_ptr_type attribute(new gl::Attribute());
      attribute->SetName("u_ivec4");

      core::Array<Tuple4s32>  ints(2, Tuple4s32(4));
      attribute->SetVertexArray(ints, gl::p_shader_variable_ti::SwapArray());

      so->AddAttribute(attribute);
    }

    {
      attribute_ptr_type attribute(new gl::Attribute());
      attribute->SetName("u_uint");

      core::Array<u32>  ints(2, 1);
      attribute->SetVertexArray(ints, gl::p_shader_variable_ti::SwapArray());

      so->AddAttribute(attribute);
    }
    {
      attribute_ptr_type attribute(new gl::Attribute());
      attribute->SetName("u_uivec2");

      core::Array<Tuple2u32>  ints(2, Tuple2u32(2));
      attribute->SetVertexArray(ints, gl::p_shader_variable_ti::SwapArray());

      so->AddAttribute(attribute);
    }
    {
      attribute_ptr_type attribute(new gl::Attribute());
      attribute->SetName("u_uivec3");

      core::Array<Tuple3u32>  ints(2, Tuple3u32(3));
      attribute->SetVertexArray(ints, gl::p_shader_variable_ti::SwapArray());

      so->AddAttribute(attribute);
    }
    {
      attribute_ptr_type attribute(new gl::Attribute());
      attribute->SetName("u_uivec4");

      core::Array<Tuple4u32>  ints(2, Tuple4u32(4));
      attribute->SetVertexArray(ints, gl::p_shader_variable_ti::SwapArray());

      so->AddAttribute(attribute);
    }

    sp.Enable();
    CHECK(gl::Error().Succeeded());
    CHECK(so->PrepareAllAttributes(sp) == ErrorSuccess());
    CHECK(gl::Error().Succeeded());
    sp.Disable();
  }
};
