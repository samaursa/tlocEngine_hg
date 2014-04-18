#include "tlocFreeType.h"

#include <tlocCore/logging/tlocLogger.h>
#include <tlocCore/containers/tlocArray.inl.h>
#include <tlocCore/tlocAlgorithms.h>

#include <tlocMath/tlocRange.h>
#include <tlocMath/utilities/tlocScale.h>

#include <tlocGraphics/error/tlocErrorTypes.h>

namespace tloc { namespace graphics { namespace media { namespace free_type {

  // ///////////////////////////////////////////////////////////////////////
  // FreeTypeGlyph

  FreeTypeGlyph::
    FreeTypeGlyph()
    : m_glyph(nullptr)
  { }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  FreeTypeGlyph::
    FreeTypeGlyph(ft_glyph_type a_glyphToCopy)
    : m_glyph(a_glyphToCopy)
  { }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  FreeTypeGlyph::
    FreeTypeGlyph(const this_type& a_other)
    : m_glyph(a_other.m_glyph)
  { }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  FreeTypeGlyph::this_type&
    FreeTypeGlyph::
    operator=(this_type a_other)
  {
    swap(a_other);
    return *this;
  }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  void
    FreeTypeGlyph::
    swap(this_type& a_other)
  {
    using core::swap;
    swap(m_glyph, a_other.m_glyph);
  }

  // ///////////////////////////////////////////////////////////////////////
  // FreeType

  FreeType::
    FreeType()
    : m_library(nullptr)
  { }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  FreeType::
    ~FreeType()
  {
    base_type::Destroy();
  }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  bool
    FreeType::
    SetCurrentSize(ft_ushort a_charSize) const
  {
    AssertIsInitialized();

    const ft_ushort currentSize = m_face->size->metrics.x_ppem;

    if (currentSize != a_charSize)
    {
      FT_Error err = FT_Set_Pixel_Sizes(m_face, 0, a_charSize);
      //FT_Error err = FT_Set_Char_Size(m_face, 0, a_charSize * 64, 500, 500);

      TLOC_LOG_GFX_WARN_IF(err != 0)
        << "Unable to set current font size to: " << a_charSize;

      return err == 0;
    }
    else
    { return true; }
  }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  FreeType::ft_vec
    FreeType::
    GetKerning(ft_ulong a_leftCharCode, ft_ulong a_charCode)
  {
    ft_vec delta;
    delta.x = 0;
    delta.y = 0;

    if (FT_HAS_KERNING(m_face))
    {
      LoadGlyph(a_leftCharCode);

      ft_ulong leftIndex = FT_Get_Char_Index(m_face, a_leftCharCode);
      ft_ulong index = FT_Get_Char_Index(m_face, a_charCode);

      FT_Error err = FT_Get_Kerning(m_face, leftIndex, index,
                                    FT_KERNING_UNFITTED, &delta);

      TLOC_LOG_GFX_ERR_IF(err != 0)
        << "Could not get kerning for characters (" << a_leftCharCode
        << ", " << a_charCode << ")";
    }

    return delta;
  }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  FreeTypeGlyph
    FreeType::
    LoadGlyph(ft_ulong a_charCode) const
  {
    AssertIsInitialized();

    FT_Error err = FT_Load_Char(m_face, a_charCode, FT_LOAD_RENDER);

    if (err)
    {
      TLOC_LOG_GFX_ERR_IF( err != 0 )
        << "Could not load glyph for character: " << a_charCode;

      return FreeTypeGlyph();
    }

    return FreeTypeGlyph(m_face->glyph);
  }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  FreeType::image_ptr
    FreeType::
    GetGlyphImage(ft_ulong a_charCode, 
                  gfx_t::Color a_fontColor,
                  gfx_t::Color a_backgroundColor) const
  {
    AssertIsInitialized();

    // scaling setup
    math::range_s32 r0to256 = math::Range0to256<s32, math::p_range::Inclusive>().Get();
    math::range_f32 r0to1 = math::Range0to1<f32, math::p_range::Inclusive>().Get();
    math_utils::scale_f32_s32 scale(r0to1, r0to256);

    FreeTypeGlyph g = LoadGlyph(a_charCode);

    FT_Bitmap bmp = g.GetGlyphSlot()->bitmap;
    Image::const_uchar8_ptr bmpBuff(bmp.buffer);

    Image::pixel_container_type pixelCont;
    for (tl_int i = 0; i < bmp.width * bmp.rows; ++i)
    {
      uchar8 gc = bmpBuff.get()[i];

      const f32 fontMulti = scale.ScaleDown(gc);
      const f32 bgMulti = 1 - fontMulti;

      const gfx_t::Color finalFontColor = a_fontColor * fontMulti;
      const gfx_t::Color finalBgColor = a_backgroundColor * bgMulti;

      const gfx_t::Color finalColor = finalFontColor + finalBgColor;

      pixelCont.push_back(finalColor);
    }

    image_ptr fontImg(new image_ptr::value_type());
    fontImg->LoadFromMemory(pixelCont, core_ds::MakeTuple(bmp.width, bmp.rows));

    return fontImg;
  }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  FreeType::error_type
    FreeType::
    DoInitialize(const data_type& a_data)
  {
    FT_Error error = FT_Init_FreeType( &m_library );
    if ( error )
    {
      TLOC_LOG_GFX_ERR() <<
        "Failed to initialize FreeType - FreeType error code: " << error;
      return TLOC_ERROR( gfx_err::error_free_type_initialize );
    }

    const FT_Byte* rawData =
      reinterpret_cast<const FT_Byte*>( a_data.begin() );

    error = FT_New_Memory_Face( m_library, rawData, a_data.size(), 0, &m_face );

    if ( error )
    {
      TLOC_LOG_GFX_ERR() <<
        "Failed to load font from memory - FreeType error code: " << error;
      return TLOC_ERROR( gfx_err::error_free_type_error_loading_font );
    }

    error = FT_Set_Char_Size(m_face, 0, 150*64, 300, 300);

    if ( error )
    {
      TLOC_LOG_GFX_ERR() <<
        "Failed to set character size - FreeType error code: " << error;
      return TLOC_ERROR( gfx_err::error_free_type_error_setting_size);
    }

    return ErrorSuccess;
  }

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

  FreeType::error_type
    FreeType::
    DoDestroy()
  {
    if (m_library)
    { FT_Done_FreeType(m_library); }

    return ErrorSuccess;
  }

};};};};

TLOC_EXPLICITLY_INSTANTIATE_ARRAY(tloc::char8);