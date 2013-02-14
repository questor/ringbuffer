
#include "ringbuffer.h"
#include "gtest.h"

TEST(RingBuffer, simpleTestGetPut) {
   RingBuffer<int> intTest(10);
   EXPECT_EQ(0, intTest.availableNumberElements());
   EXPECT_EQ(true, intTest.isEmpty());
   intTest.put(10);
   EXPECT_EQ(1, intTest.availableNumberElements());
   EXPECT_EQ(false, intTest.isEmpty());
   intTest.put(20);
   EXPECT_EQ(2, intTest.availableNumberElements());
   int tmp;
   intTest.get(&tmp);
   EXPECT_EQ(10, tmp);
   EXPECT_EQ(1, intTest.availableNumberElements());
   intTest.get(&tmp);
   EXPECT_EQ(20, tmp);
   EXPECT_EQ(0, intTest.availableNumberElements());
   EXPECT_EQ(true, intTest.isEmpty());

   intTest.put(10);
   intTest.put(20);
   intTest.put(30);
   intTest.clear();
   EXPECT_EQ(0, intTest.availableNumberElements());
   EXPECT_EQ(true, intTest.isEmpty());
}

TEST(RingBuffer, simpleTestArrayGetPut) {
   RingBuffer<int> intTest(10);
   EXPECT_EQ(0, intTest.availableNumberElements());

   int tmpArray[] = {1,2,3,4,5,6,7,8,9};
   intTest.put(tmpArray, 9);

   EXPECT_EQ(9, intTest.availableNumberElements());

   int tmpArray2[9];
   intTest.get(tmpArray2, 9);

   EXPECT_EQ(1, tmpArray2[0]);
   EXPECT_EQ(2, tmpArray2[1]);
   EXPECT_EQ(3, tmpArray2[2]);
   EXPECT_EQ(4, tmpArray2[3]);
   EXPECT_EQ(5, tmpArray2[4]);
   EXPECT_EQ(6, tmpArray2[5]);
   EXPECT_EQ(7, tmpArray2[6]);
   EXPECT_EQ(8, tmpArray2[7]);
   EXPECT_EQ(9, tmpArray2[8]);
}

TEST(RingBuffer, simpleOverflow) {
   RingBuffer<int> intTest(10);
   int tmpArray[] = {1,2,3,4,5,6,7,8,9};
   intTest.put(tmpArray, 9);
   EXPECT_EQ(9, intTest.availableNumberElements());
   intTest.put(10);
   EXPECT_EQ(9, intTest.availableNumberElements());
   intTest.put(11);
   EXPECT_EQ(9, intTest.availableNumberElements());
   for(int i=0; i<100; ++i) {
      intTest.put(12+i);
      EXPECT_EQ(9, intTest.availableNumberElements());
   }
}

TEST(RingBuffer, simpleArrayOverflow) {
   RingBuffer<int> intTest(10);
   int tmpArray[] = {1,2,3};
   intTest.put(tmpArray,3);
   EXPECT_EQ(3, intTest.availableNumberElements());
   intTest.put(tmpArray,3);
   EXPECT_EQ(6, intTest.availableNumberElements());
   intTest.put(tmpArray,3);
   EXPECT_EQ(9, intTest.availableNumberElements());
   for(int i=0; i<100; ++i) {
      intTest.put(tmpArray,3);
      EXPECT_EQ(9, intTest.availableNumberElements());
   }
}

TEST(RingBuffer, simpleOverflow2) {
   RingBuffer<int> intTest(10);
   int tmpArray[] = {1,2,3,4,5,6,7,8,9};
   intTest.put(tmpArray, 9);
   EXPECT_EQ(9, intTest.availableNumberElements());
   intTest.put(10);
   EXPECT_EQ(9, intTest.availableNumberElements());
   int tmp;
   intTest.get(&tmp);
   EXPECT_EQ(2, tmp);
   EXPECT_EQ(8, intTest.availableNumberElements());
   intTest.put(11);
   EXPECT_EQ(9, intTest.availableNumberElements());
   intTest.put(12);
   EXPECT_EQ(9, intTest.availableNumberElements());
   intTest.get(&tmp);
   EXPECT_EQ(4, tmp);
}

TEST(RingBuffer, simpleOverflowTotalAvailable) {
   RingBuffer<int> intTest(10);
   int tmpArray[] = {1,2,3,4,5,6,7,8,9};
   intTest.put(tmpArray, 9);
   EXPECT_EQ(9, intTest.totalAvailableNumberElements());
   intTest.put(10);
   EXPECT_EQ(9, intTest.totalAvailableNumberElements());
   int tmp;
   intTest.get(&tmp);
   EXPECT_EQ(9, intTest.totalAvailableNumberElements());
   intTest.put(11);
   EXPECT_EQ(9, intTest.totalAvailableNumberElements());
   intTest.put(12);
   intTest.get(&tmp);
   EXPECT_EQ(9, intTest.totalAvailableNumberElements());
}

TEST(RingBuffer, simpleReset) {
   RingBuffer<int> intTest(10);
   int tmpArray[] = {1,2,3,4,5,6,7};
   intTest.put(tmpArray, 7);
   EXPECT_EQ(7, intTest.availableNumberElements());
   intTest.resetReadPosition();
   EXPECT_EQ(0, intTest.availableNumberElements());
   intTest.put(10);
   int tmp;
   intTest.get(&tmp);
   EXPECT_EQ(10, tmp);
}

TEST(RingBuffer, simpleReset2) {
   RingBuffer<int> intTest(10);
   int tmpArray[] = {1,2,3,4,5,6,7,8,9};
   intTest.put(tmpArray, 9);
   EXPECT_EQ(9, intTest.availableNumberElements());
   intTest.resetReadPosition();
   EXPECT_EQ(0, intTest.availableNumberElements());
   intTest.put(10);
   int tmp;
   intTest.get(&tmp);
   EXPECT_EQ(10, tmp);
}

TEST(RingBuffer, simpleRead) {
   RingBuffer<int> intTest(10);
   int tmpArray[] = {1,2,3,4,5,6,7,8,9};
   intTest.put(tmpArray, 9);
   intTest.put(10);   //overwrites 1
   intTest.put(11);   //overwrites 2
   intTest.put(12);   //overwrites 3
   intTest.put(13);   //overwrites 4

   int tmpArray2[9];
   EXPECT_EQ(9, intTest.totalAvailableNumberElements());
   intTest.totalGet(tmpArray2, 9);
   EXPECT_EQ(5, tmpArray2[0]);
   EXPECT_EQ(6, tmpArray2[1]);
   EXPECT_EQ(7, tmpArray2[2]);
   EXPECT_EQ(8, tmpArray2[3]);
   EXPECT_EQ(9, tmpArray2[4]);
   EXPECT_EQ(10, tmpArray2[5]);
   EXPECT_EQ(11, tmpArray2[6]);
   EXPECT_EQ(12, tmpArray2[7]);
   EXPECT_EQ(13, tmpArray2[8]);
}
