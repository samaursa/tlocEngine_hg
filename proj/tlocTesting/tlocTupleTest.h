namespace TestingTuple
{

  #define CHECK_TUP(tup,x,y,z,w)   CHECK(tup[0] == x); \
                                   CHECK(tup[1] == y); \
                                   CHECK(tup[2] == z); \
                                   CHECK(tup[3] == w);

  TEST_CASE("Core/DataStructures/Tuple", "Testing the Tuple<T, size> class")
  {
    Tuple<int, 4> p(0);
    CHECK_TUP(p, 0, 0, 0, 0);
    
    p[0] = 0; p[1] = 1; p[2] = 2; p[3] = 3;
    CHECK_TUP(p, 0, 1, 2, 3);

    CHECK(p.Get(0) == 0);
    CHECK(p.Get(1) == 1);
    CHECK(p.Get(2) == 2);
    CHECK(p.Get(3) == 3);

    p.Set(5);
    CHECK_TUP(p, 5, 5, 5, 5);

    Tuple<int, 4> q(8);
    p.Swap(q);

    CHECK_TUP(p, 8, 8, 8, 8);
    CHECK_TUP(q, 5, 5, 5, 5);
  }

};