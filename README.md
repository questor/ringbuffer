ringbuffer
==========

simple ringbuffer with possibility to get all data and/or only updates

the idea is to provide a ring buffer that can be queried for all inserted data, reset the read pointer and get incremental updates. I've used this approach in a system monitoring application where in the background the free memory is queried every second and put into the ringbuffer (put). When the reporting tool is started I get the total number of values inserted into the buffer (totalAvailableNumberElements and totalGet), reset the pointer after I've got all values (resetReadPosition) and afterwards can make live-reports with incremental updates (availableNumberElements and get) of the values instead of updating all values every updatestep.

the following sequence should explain well how the buffer works:
- ringbuffer(size 10) has 0 elements(totalAvailable and available elements return 0)
- put 1,2,3,4,5 into the ringbuffer
- totalAvailable and available elements return 5
- get 2 values (return the 1 and 2)
- totalAvailable will return 5, available elements return 3
- call resetReadPosition
- totalAvailable will return 5, available elements return 0
- put 6,7,8,9,10 into the ringbuffer
- totalAvailable will return 9, available elements return 5 (a ringbuffer of size n can save at most n-1 elements!)
- put 11,12,13,14,15,16,17,18,19 into the ringbuffer
- totalAvailable and available elements will return 9

no error detection is done, for example don't insert more values than the size of the ringbuffer-1 and don't get more elements than inserted and stuff like that.

MIT License

Copyright (C) 2013 questor(KaiJ)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
