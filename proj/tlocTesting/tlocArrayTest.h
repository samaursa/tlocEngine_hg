namespace TestingArray
{
  struct ArrayFixture
  {
    int a, b, c, d, e, f;
    Array<s32> ints, ints2, ints3;
  };

#define FILL_INT_ARRAY_BY_PUSH(arrayName, nFrom, nTo) \
  for (u32 i = nFrom; i < nTo; ++i)\
  {\
    arrayName.push_back(i);\
  }\

#define FILL_INT_ARRAY_BY_INDEX(arrayName, nFrom, nTo) \
  for (u32 i = nFrom; i < nTo; ++i)\
  {\
    arrayName[i] = i;\
  }\

  TEST_CASE_METHOD(ArrayFixture, "Core/Containers/Array/General",
                                                   "Test General functionality")
  {
    TLOC_ASSERT(NULL == NULL, "Test");

    ints.push_back(5);
    a = ints[0];
    CHECK(a == 5);

    ints.push_back(10);
    a = ints[0];
    CHECK(a == 5);
    b = ints[1];
    CHECK(b == 10);
  }

  TEST_CASE_METHOD(ArrayFixture, "Core/Containers/Array/Accessors",
                                                              "Test accessors")
  {
    FILL_INT_ARRAY_BY_PUSH(ints, 0, 100);

    Array<s32>::iterator itr = ints.begin();
    Array<s32>::iterator itrEnd = ints.end();

    s32 count = 0;
    while (itr != itrEnd)
    {
      CHECK(count++ == *itr);
      ++itr;
    }

    CHECK(ints.size() == 100);
  }

  TEST_CASE_METHOD(ArrayFixture, "Core/Containers/Array/Erase",
    "Test all erase functions")
  {
    //------------------------------------------------------------------------
    // Erase at position

    FILL_INT_ARRAY_BY_PUSH(ints, 0, 10);

    CHECK(ints.size() == 10);
    ints.erase(ints.begin() + 4);
    CHECK(ints.size() == 9);

    for (u32 i = 4; i < 9; ++i)
    {
      CHECK(ints[i] == (s32)i + 1);
    }

    //------------------------------------------------------------------------
    // Erase range

    ints.push_back(0); // Make the array size 10
    FILL_INT_ARRAY_BY_INDEX(ints, 0, 10);

    CHECK(ints.size() == 10);
    u32 currCapacity = ints.capacity();
    ints.erase(ints.begin(), ints.end());
    CHECK(ints.size() == 0);
    CHECK(ints.capacity() == currCapacity);
  }

  TEST_CASE_METHOD(ArrayFixture, "Core/Containers/Array/Clear",
    "Test the clear function")
  {
    u32 sizeOfInts = 1000;
    FILL_INT_ARRAY_BY_PUSH(ints, 0, sizeOfInts);

    u32 currCapacity = ints.capacity();
    CHECK(ints.size() == sizeOfInts);
    ints.clear();
    CHECK(ints.size() == 0);
    CHECK(ints.capacity() == currCapacity);
  }

  TEST_CASE_METHOD(ArrayFixture, "Core/Containers/Array/Insert",
    "Test the insert methods")
  {
    //------------------------------------------------------------------------
    // Insert single values

    FILL_INT_ARRAY_BY_PUSH(ints, 0, 10);

    for (u32 i = 10; i < 20; ++i)
    {
      ints.insert(ints.end(), i);
    }

    CHECK(ints.size() == 20);

    for (u32 i = 0; i < 20; ++i)
    {
      CHECK(ints[i] == (s32)i);
    }

    CHECK(ints.capacity() == 32);

    ints.insert(ints.begin() + 2, 90);
    CHECK(ints.size() == 21);
    CHECK(ints[0] == 0);
    CHECK(ints[1] == 1);
    CHECK(ints[2] == 90);
    CHECK(ints[3] == 2);

    CHECK(ints.capacity() == 32);

    //------------------------------------------------------------------------
    // Insert several values

    FILL_INT_ARRAY_BY_PUSH(ints2, 0, 10);

    CHECK(ints2.size() == 10);

    ints2.insert(ints2.begin() + 2, static_cast<tl_sizet>(10), 90);

    CHECK(ints2[0] == 0);
    CHECK(ints2[1] == 1);

    for (u32 i = 2; i < 10 + 2; ++i)
    {
      CHECK(ints2[i] == 90);
    }

    CHECK(ints2[12] == 2);
  }

  TEST_CASE_METHOD(ArrayFixture, "Core/Containers/Array/Resize",
    "Test the resize methods")
  {
    /*FILL_INT_ARRAY_BY_PUSH(ints, 0, 10);

    CHECK(ints.size() == 10);
    ints.resize(5);
    CHECK(ints.size() == 5);*/
  }

  TEST_CASE_METHOD(ArrayFixture, "Core/Containers/Array/Assign",
    "Test the assign function")
  {
    /*int someArray[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int* itrBegin = someArray;
    int* itrEnd = someArray + 9;

    ints.assign(itrBegin, itrEnd);*/
  }

#undef FILL_INT_ARRAY_BY_PUSH
};
