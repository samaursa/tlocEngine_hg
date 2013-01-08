#ifndef _TLOC_GRAPHICS_GL_DEFAULT_SHADERS_H_
#define _TLOC_GRAPHICS_GL_DEFAULT_SHADERS_H_

#include <tlocCore/string/tlocString.h>

#include <tlocGraphics/component_system/tlocMaterial.h>
#include <tlocGraphics/types/tlocColor.h>

namespace tloc { namespace graphics { namespace gl {

  namespace prefab_materials
  {
    enum MaterialName
    {
      lambert
    };

    component_system::Material
      Lambert(const types::Color& a_col)
    {
      using namespace graphics::types;

      String   vProg =
        "#ifdef GL_ES                \n\
        #  version 100               \n\
        #else                        \n\
        #  version 140               \n\
        #endif                       \n\
        \n\
        attribute vec4 a_vertex;     \n\
        attribute vec4 a_color;      \n\
        \n\
        varying lowp vec4 v_color;   \n\
        \n\
        void main(void)              \n\
        {                            \n\
          v_color = a_color;         \n\
          gl_Position   = a_vertex;  \n\
        }";

      String fProg =
        "#ifdef GL_ES                \n\
        #  version 100               \n\
        #else                        \n\
        #  version 140               \n\
        #endif                       \n\
        \n\
        varying lowp vec4 v_color;   \n\
        \n\
        void main(void)              \n\
        {                            \n\
          gl_FragColor = v_color;    \n\
        }";

      Material m;
      m.SetVertexSource(vProg);
      m.SetFragmentSource(fProg);

      Material::shader_prog_type& sp = m.GetShaderProgRef();

      Vec4f32 colVec;
      Color col(0.5f, 0.5f, 0.5f, 1.0f);
      col.GetAs<p_color::format::RGBA>(colVec);
      sp.AddAttribute(colVec);
    }


  };

};};};

#endif