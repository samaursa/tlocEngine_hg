#include "tlocTestCommon.h"

#include <tlocCore/tlocRange.h>

namespace TestingRange
{
  using namespace tloc;
  using namespace core;

  // Tests are taken from https://bitbucket.org/AraK/range and modified where
  // appropriate

  template <typename T_RangeType>
  void TestCtors()
  {
    typedef T_RangeType range_type;

    CHECK(range_type().empty());

    range_type r;
    r = range_type(0, 10);
    r = range_type(10, 10);
    r = range_type(0, 0);

    r = range_type(0, 10, range_type::step_size(1));
    r = range_type(0, 0, range_type::step_size(2));

    range_type r2(r);
    CHECK(r2 == r);
  }

  TEST_CASE("core/range/ctors", "")
  {
    TestCtors<Range>();
    TestCtors<Rangef>();
  }

  template <typename T_RangeType>
  void TestAccessors()
  {
    typedef T_RangeType range_type;

    range_type r(0, 10);
    CHECK(r.front() == 0);
    CHECK(r.back() == 9);

    r = range_type(0, 0);
    CHECK(r.front() == 0);
    CHECK(r.back() == 0);

    r = range_type(5, 10);
    CHECK(r.front() == 5);
    CHECK(r.back() == 9);

    r = range_type(0, 10, range_type::step_size(2));
    CHECK(r.front() == 0);
    CHECK(r.back() == 8);

    r = range_type(0, 10, range_type::step_size(3));
    CHECK(r.front() == 0);
    CHECK(r.back() == 9);

    r = range_type(0, 10, range_type::step_size(4));
    CHECK(r.front() == 0);
    CHECK(r.back() == 8);

    r = range_type(10, 50, range_type::step_size(10));
    CHECK(r.front() == 10);
    CHECK(r.back() == 40);

    r = range_type(10, 51, range_type::step_size(10));
    CHECK(r.front() == 10);
    CHECK(r.back() == 50);
  }

  TEST_CASE("core/range/accessors", "")
  {
    TestAccessors<Range>();
    TestAccessors<Rangef>();
  }

  template <typename T_RangeType>
  void TestSize()
  {
    typedef T_RangeType range_type;

    range_type r(0, 10);
    CHECK(r.size() == 10);
    r = range_type(0, 10, range_type::step_size(2));
    CHECK(r.size() == 5);
    r = range_type(0, 10, range_type::step_size(10));
    CHECK(r.size() == 1);
    r = range_type(0, 10, range_type::step_size(11));
    CHECK(r.size() == 1);
  }

  TEST_CASE("core/range/size", "")
  {
    TestSize<Range>();
    TestSize<Rangef>();
  }

  template <typename T_RangeType>
  void TestEmptyFull()
  {
    typedef T_RangeType range_type;

    CHECK(range_type().empty());
    range_type r(150, 200);
    CHECK_FALSE(r.empty());
    r = range_type(150, 150);
    CHECK(r.empty());
  }

  TEST_CASE("core/range/empty_full", "")
  {
    TestEmptyFull<Range>();
    TestEmptyFull<Rangef>();
  }

  template <typename T_RangeType>
  void TestIterators()
  {
    typedef T_RangeType range_type;

    range_type r(0, 10);
    range_type::iterator itr = r.begin();
    range_type::reverse_iterator rItr = r.rbegin();

    bool testPassed = true;
    for (tl_int i = 0; i < 10; ++i)
    {
      if (*(itr++) != i)
      { testPassed = false; break; }
    }
    CHECK(testPassed);

    testPassed = true;
    for (tl_int i = 10; i > 0; --i)
    {
      if (*(rItr++) != i-1)
      { testPassed = false; break; }
    }
    CHECK(testPassed);

    rItr -= 10; // we were at rend, so we can go rback 10 indices
    CHECK(*rItr == 9);
    rItr += 9; // we were at rbegin, we can go rforward 9 indices
    CHECK(*rItr == 0);

    CHECK(*rItr-- == 0);
    CHECK(*rItr   == 1);
    CHECK(*--rItr == 2);

    CHECK(*rItr++ == 2);
    CHECK(*rItr   == 1);
    CHECK(*++rItr == 0);

    CHECK( *(rItr - 9) == 9);
    CHECK( *( (--rItr) + 1) == 0);

    r = range_type(0, 10, range_type::step_size(2));
    itr = r.begin();
    testPassed = true;
    for (tl_int i = 0; i < 10; i = i+2)
    {
      if (*(itr++) != i)
      { testPassed = false; break; }
    }
    CHECK(testPassed);

    r = range_type(5, 6);
    itr = r.begin();
    testPassed = true;
    for (tl_int i = 5; i < 6; ++i)
    {
      if (*(itr++) != i)
      { testPassed = false; break; }
    }
    CHECK(testPassed);

    r = range_type(5, 6, range_type::step_size(10));
    itr = r.begin();
    testPassed = true;
    for (tl_int i = 5; i < 6; i = i + 10)
    {
      if (*(itr++) != i)
      { testPassed = false; break; }
    }
    CHECK(testPassed);

    r = range_type(0, 50, range_type::step_size(2));
    itr = r.begin();
    range_type::iterator itrEnd = r.end();
    tl_int count = 0;
    for (; itr < itrEnd; ++itr)
    {
      ++count;
    }
    CHECK(count == 50/2);

    itr -= 25;
    CHECK(*itr == 0);
    itr += 10;
    CHECK(*itr == 20);

    CHECK(*itr++ == 20);
    CHECK(*itr   == 22);
    CHECK(*++itr == 24);

    CHECK(*itr == 24);

    CHECK(*itr-- == 24);
    CHECK(*itr   == 22);
    CHECK(*--itr == 20);

    CHECK( *(itr - 10) == 0);
    CHECK( *( (--itr) + 1) == 20);
  }

  TEST_CASE("core/range/iterators", "")
  {
    TestIterators<Range>();
    TestIterators<Rangef>();
  }
}
