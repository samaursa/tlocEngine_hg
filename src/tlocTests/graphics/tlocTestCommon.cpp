#include "tlocTestCommon.h"
#include <tlocCore/platform/tlocPlatform.h>

#include <cstring>
#include "tlocAssetsPath.h"

#if defined(TLOC_OS_WIN)

const char* GetAssetPath()
{
  static const char* assetPath = TLOC_ASSETS_PATH;
  return assetPath;
}
#elif defined(TLOC_OS_IPHONE)
const char* GetAssetPath()
{
  static char assetPath[1024];
  strcpy(assetPath, [[[NSBundle mainBundle] resourcePath]
                        cStringUsingEncoding:[NSString defaultCStringEncoding]]);
  strcat(assetPath, "/assets/");

  return assetPath;
}
#endif