/* MIT License
   Copyright (C) 2013 questor(KaiJ)
   Permission is hereby granted, free of charge, to any person obtaining a copy of this 
   software and associated documentation files (the "Software"), to deal in the 
   Software without restriction, including without limitation the rights to use, copy, 
   modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
   and to permit persons to whom the Software is furnished to do so, subject to the following 
   conditions:
   The above copyright notice and this permission notice shall be included in all copies or 
   substantial portions of the Software.
   
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
   INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
   PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
   FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
   OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
   DEALINGS IN THE SOFTWARE. */

#ifndef RINGBUFFER_H__
#define RINGBUFFER_H__

#include <string.h>       //memcpy

/* a ring buffer of size n can only save n-1 elements! */
template<class T> class RingBuffer {
public:
   RingBuffer(int size) 
      : mSize(size)
   {
      mData = new T[size];
      clear();
   }
   ~RingBuffer() {
      delete[] mData;
   }
   int getSize() {
      return mSize;
   }

   //get all written data of the buffer
   int totalAvailableNumberElements() {
      ptrdiff_t numberElements = mPosToWrite-mOldestDataWritten;
      if(numberElements < 0) {
         //wrap around was between
         numberElements = (mData+mSize-mOldestDataWritten)+(mPosToWrite-mData);
      }
      return numberElements;
   }
   void totalGet(T* data, int numberElements) {
      if(mOldestDataWritten+numberElements >= mData+mSize) {
         //split read operation
         int numberGetFromTop = mData+mSize-mOldestDataWritten;
         memcpy(data, mOldestDataWritten, numberGetFromTop*sizeof(T));
         int remaining = numberElements-numberGetFromTop;
         memcpy(data+numberGetFromTop, mData,remaining*sizeof(T));
      } else {
         memcpy(data, mOldestDataWritten, numberElements*sizeof(T));
      }
   }

   //iterative read and write
   bool isEmpty() {
      return (mPosToRead == mPosToWrite);
   }
   int availableNumberElements() {
      ptrdiff_t numberElements = mPosToWrite-mPosToRead;
      if(numberElements < 0) {
         //wrap around was between
         numberElements = (mData+mSize-mPosToRead)+(mPosToWrite-mData);
      }
      return numberElements;
   }
   void clear() {
      mPosToRead = mPosToWrite = mOldestDataWritten = mData;
   }
   void resetReadPosition() {
      mPosToRead = mPosToWrite;
   }

   void put(T element) {
      put(&element, 1);
   }
   void put(T* data, int numberElements) {
      if(mPosToWrite+numberElements >= mData+mSize) {
         //split write operation
         int numberPutOnTop = mData+mSize-mPosToWrite;
         memcpy(mPosToWrite, data, numberPutOnTop*sizeof(T));
         int remaining = numberElements-numberPutOnTop;
         memcpy(mData, data+numberPutOnTop, remaining*sizeof(T));
         mPosToWrite = mData+remaining;

         if((mPosToRead > mData+mSize-numberPutOnTop) || (mPosToRead <= mPosToWrite)) {
            mPosToRead = mPosToWrite+1;
            if(mPosToRead == mData+mSize)
               mPosToRead = mData;
         }

         if((mOldestDataWritten > mData+mSize-numberPutOnTop) || (mOldestDataWritten <= mPosToWrite)) {
            mOldestDataWritten = mPosToWrite+1;
            if(mOldestDataWritten == mData+mSize)
               mOldestDataWritten = mData;
         }
      } else {
         memcpy(mPosToWrite, data, numberElements*sizeof(T));
         mPosToWrite += numberElements;
         if((mPosToRead > mPosToWrite-numberElements) && (mPosToRead <= mPosToWrite)) {
            mPosToRead = mPosToWrite+1;
            if(mPosToRead == mData+mSize)
               mPosToRead = mData;
         }
         if((mOldestDataWritten > mPosToWrite-numberElements) && (mOldestDataWritten <= mPosToWrite)) {
            mOldestDataWritten = mPosToWrite+1;
            if(mOldestDataWritten == mData+mSize)
               mOldestDataWritten = mData;
         }
      }
   }
   void get(T* data, int numberElements=1) {
      if(mPosToRead+numberElements >= mData+mSize) {
         //split read operation
         int numberGetFromTop = mData+mSize-mPosToRead;
         memcpy(data, mPosToRead, numberGetFromTop*sizeof(T));
         int remaining = numberElements-numberGetFromTop;
         memcpy(data+numberGetFromTop, mData,remaining*sizeof(T));
         mPosToRead = mData+remaining;
      } else {
         memcpy(data, mPosToRead, numberElements*sizeof(T));
         mPosToRead += numberElements;
      }
   }

protected:
   const int mSize;
   T* mData;
   T* mPosToRead;
   T* mPosToWrite;
   T* mOldestDataWritten;
};

#endif
