/*
    MIT License

    Copyright (c) 2019 Frank Bekema

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#ifndef AM2302_H
#define AM2302_H

#include "Arduino.h"

//Structs
struct AM2302_res {
    float humidity;
    float temperature;
    bool correct;
};

class AM2302 {
private:
    //Variables
    int pin;

    //Functions
    int bitChangeListener(int check, int tries);
    int listenForBit();
    byte* bitArrToByteArr(int intArr[], int bitLength);
    int mergeBytes(byte byte1, byte byte2);
    bool compareBits(int number1, int number2, int checkLength);
public:
    //Constructor
    AM2302(int inputPin);

    //Functions
    AM2302_res read();
};

#endif