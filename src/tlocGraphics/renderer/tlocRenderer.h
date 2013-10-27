#ifndef TLOC_GRAPHICS_RENDERER_H
#define TLOC_GRAPHICS_RENDERER_H

#include <tlocGraphics/tlocGraphicsBase.h>

#include <tlocCore/utilities/tlocCheckpoints.h>
#include <tlocCore/utilities/tlocObjectCounter.h>
#include <tlocCore/error/tlocError.h>
#include <tlocCore/platform/tlocPlatform.h>
#include <tlocCore/smart_ptr/tlocSharedPtr.h>

#include <tlocGraphics/error/tlocErrorTypes.h>
#include <tlocGraphics/types/tlocColor.h>
#include <tlocGraphics/opengl/tlocFramebufferObject.h>

namespace tloc { namespace core { namespace base_classes {

  // ///////////////////////////////////////////////////////////////////////
  // InitializeAndDestroyOnce_TI (should go into core's base classes)

  template <typename T_Derived>
  class InitializeAndDestroyOnce_TI
  {
  public:
    enum
    {
      k_initialized,
      k_destroyed,

      k_count
    };

  public:
    typedef T_Derived                                   derived_type;
    typedef InitializeAndDestroyOnce_TI<derived_type>   this_type;
    typedef core::error::Error                          error_type;
    typedef core::utils::Checkpoints                    flags_type;

    error_type  Initialize()
    {
      TLOC_ASSERT(m_flags.IsUnMarked(k_initialized), "Already initialized");
      return static_cast<derived_type*>(this)->DoInitialize();
    }

    error_type  Destroy()
    {
      TLOC_ASSERT(m_flags.IsUnMarked(k_destroyed), "Already destroyed");
      return static_cast<derived_type*>(this)->DoDestroy();
    }

    bool        IsInitialized() const
    { return m_flags.IsMarked(k_initialized); }

    bool        IsDestroyed() const
    { return m_flags.IsMarked(k_destroyed); }

  protected:
    InitializeAndDestroyOnce_TI()
      : m_flags(k_count)
    { }

    ~InitializeAndDestroyOnce_TI()
    {
      TLOC_ASSERT(m_flags.IsMarked(k_destroyed), "Destroy was not called");
    }

  private:
     flags_type         m_flags;
  };

};};};

namespace tloc { namespace graphics { namespace renderer {

  // ///////////////////////////////////////////////////////////////////////
  // Renderer

  namespace p_renderer
  {
    namespace depth_function
    {
      typedef s32                     value_type;

      struct Never        { static const value_type s_glParamName; };
      struct Less         { static const value_type s_glParamName; };
      struct Equal        { static const value_type s_glParamName; };
      struct LessEqual    { static const value_type s_glParamName; };
      struct Greater      { static const value_type s_glParamName; };
      struct NotEqual     { static const value_type s_glParamName; };
      struct GreaterEqual { static const value_type s_glParamName; };
      struct Always       { static const value_type s_glParamName; };
    };

    namespace blend_function
    {
      typedef s32                     value_type;

      struct Zero                     { static const value_type s_glParamName; };
      struct One                      { static const value_type s_glParamName; };
      struct SourceColor              { static const value_type s_glParamName; };
      struct OneMinusSourceColor      { static const value_type s_glParamName; };
      struct DestinationColor         { static const value_type s_glParamName; };
      struct OneMinusDestinationColor { static const value_type s_glParamName; };
      struct SourceAlpha              { static const value_type s_glParamName; };
      struct OneMinusSourceAlpha      { static const value_type s_glParamName; };
      struct DestinationAlpha         { static const value_type s_glParamName; };
      struct OneMinusDestinationAlpha { static const value_type s_glParamName; };
      struct ConstantColor            { static const value_type s_glParamName; };
      struct OneMinusConstantColor    { static const value_type s_glParamName; };
      struct ConstantAlpha            { static const value_type s_glParamName; };
      struct OneMinusConstantAlpha    { static const value_type s_glParamName; };
      struct SourceAlphaSaturate      { static const value_type s_glParamName; };
      struct Source1Color             { static const value_type s_glParamName; };
      struct OneMinusSource1Color     { static const value_type s_glParamName; };
      struct Source1Alpha             { static const value_type s_glParamName; };
      struct OneMinusSource1Alpha     { static const value_type s_glParamName; };
    };

    namespace enable
    {
      typedef s32                     value_type;

      struct Blend                    { static const value_type s_glParamName; };
      struct DepthTest                { static const value_type s_glParamName; };
      struct CullFace                 { static const value_type s_glParamName; };
      struct LineSmooth               { static const value_type s_glParamName; };
      struct PolygonSmooth            { static const value_type s_glParamName; };
    };

    namespace clear
    {
      typedef s32                     value_type;

      struct ColorBufferBit           { static const value_type s_glParamName; };
      struct DepthBufferBit           { static const value_type s_glParamName; };
      struct StencilBufferBit         { static const value_type s_glParamName; };

    };

  };

  // ///////////////////////////////////////////////////////////////////////
  // Renderer_T<>

  template <typename T_DepthPrecision = f32>
  class Renderer_T
  {
  public:
    typedef Renderer_T<T_DepthPrecision>              this_type;

    typedef gfx_t::Color                              color_type;
    typedef p_renderer::depth_function::value_type    depth_function_value_type;
    typedef p_renderer::blend_function::value_type    blend_function_value_type;
    typedef p_renderer::enable::value_type            enable_value_type;
    typedef p_renderer::clear::value_type             clear_value_type;

    typedef core_conts::tl_array
            <enable_value_type>::type                 enable_cont;
    typedef core_conts::tl_array
            <clear_value_type>::type                  clear_cont;

    typedef T_DepthPrecision                          depth_value_type;
    typedef s32                                       stencil_value_type;

    typedef gl::FramebufferObject                     fbo_type;
    typedef core_err::Error                           error_type;

  public:
    struct Params
    {
      Params();

      template <typename T_DepthFunction>
      this_type& DepthFunction();

      template <typename T_BlendFunction>
      this_type& BlendFunction();

      template <typename T_Enable>
      this_type& Enable();

      template <typename T_ClearValue>
      this_type& Clear();

      TLOC_DECL_AND_DEF_GETTER
        (depth_function_value_type, GetDepthFunction, m_depthFunction);
      TLOC_DECL_AND_DEF_GETTER
        (blend_function_value_type, GetBlendFunction, m_blendFunction);
      TLOC_DECL_AND_DEF_GETTER_CONST_DIRECT
        (enable_cont, GetFeaturesToEnable, m_enableFeatures);
      TLOC_DECL_AND_DEF_GETTER_CONST_DIRECT
        (clear_cont, GetClearBits, m_clearBits);

      TLOC_DECL_PARAM_VAR(color_type, ClearColor, m_clearColor);
      TLOC_DECL_PARAM_VAR(fbo_type, FBO, m_fbo);

    private:
      depth_function_value_type   m_depthFunction;
      blend_function_value_type   m_blendFunction;
      enable_cont                 m_enableFeatures;
      clear_cont                  m_clearBits;
    };

  public:
    struct RenderOneFrame
    {
      RenderOneFrame();
      explicit RenderOneFrame(this_type& a_renderer);
      ~RenderOneFrame();

      this_type& m_renderer;
    }; friend struct RenderOneFrame;

  public:
    Renderer_T(const Params& a_params);

    error_type ApplyRenderSettings() const;

  private:
    error_type  DoStart() const;
    error_type  DoEnd() const;

  private:
    Params                    m_params;
    fbo_type::Bind            m_fboBinder;
  };

  // -----------------------------------------------------------------------
  // template definitions

  template <typename T_DepthPrecision>
  template <typename T_DepthFunction>
  typename Renderer_T<T_DepthPrecision>::this_type&
    Renderer_T<T_DepthPrecision>::Params::
    DepthFunction()
  {
    using namespace p_renderer::depth_function;

    tloc::type_traits::AssertTypeIsSupported
      <T_DepthFunction,
      Never, Less, Equal, LessEqual, Greater, NotEqual, GreaterEqual,
      Always>();

    m_depthFunction = T_DepthFunction::s_glParamName;
    return *this;
  }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  template <typename T_DepthPrecision>
  template <typename T_BlendFunction>
  typename Renderer_T<T_DepthPrecision>::this_type&
    Renderer_T<T_DepthPrecision>::Params::
    BlendFunction()
  {
    using namespace p_renderer::blend_function;

    tloc::type_traits::AssertTypeIsSupported
      <T_BlendFunction,
      Zero, One, SourceColor, OneMinusSourceColor, DestinationColor,
      OneMinusDestinationColor, SourceAlpha, OneMinusSourceAlpha,
      DestinationAlpha, OneMinusDestinationAlpha, ConstantColor,
      OneMinusConstantColor, ConstantAlpha,
      OneMinusConstantAlpha, SourceAlphaSaturate, Source1Color,
      OneMinusSource1Color, Source1Alpha, OneMinusSourceAlpha>();

    m_blendFunction = T_BlendFunction::s_glParamName;
    return *this;
  }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  template <typename T_DepthPrecision>
  template <typename T_Enable>
  typename Renderer_T<T_DepthPrecision>::this_type&
    Renderer_T<T_DepthPrecision>::Params::
    Enable()
  {
    using namespace p_renderer::enable;

    tloc::type_traits::AssertTypeIsSupported
      <T_Enable,
      Blend, DepthTest, CullFace, LineSmooth, PolygonSmooth>();

    m_enableFeatures.push_back(T_Enable::s_glParamName);
    return *this;
  }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  template <typename T_DepthPrecision>
  template <typename T_ClearValue>
  typename Renderer_T<T_DepthPrecision>::this_type&
    Renderer_T<T_DepthPrecision>::Params::
    Clear()
  {
    using namespace p_renderer::clear;

    tloc::type_traits::AssertTypeIsSupported
      <T_ClearValue,
       ColorBufferBit, DepthBufferBit, StencilBufferBit>();

    m_clearBits.push_back(T_ClearValue::s_glParamName);
    return *this;
  }

  // -----------------------------------------------------------------------
  // typedefs

  typedef Renderer_T<f32>               Renderer;
  typedef Renderer_T<f32>               Renderer_depth32;
  typedef Renderer_T<f64>               Renderer_depth64;

  TLOC_TYPEDEF_SHARED_PTR(Renderer, renderer);
  TLOC_TYPEDEF_SHARED_PTR(Renderer_depth32, renderer_depth32);
  TLOC_TYPEDEF_SHARED_PTR(Renderer_depth64, renderer_depth64);

};};};

#endif