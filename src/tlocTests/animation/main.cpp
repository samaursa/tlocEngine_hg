#if defined (__APPLE__) && defined (__OBJC__)
# import <Foundation/Foundation.h>
#endif

#include <tlocCore/tlocBase.h>
#include <tlocCore/memory/tlocLinkMe.cpp>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE("", "")
{
  std::cout << "Testing tlocAnimation..." << std::endl;
}
