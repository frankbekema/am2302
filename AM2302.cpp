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

#include "Arduino.h"
#include "AM2302.h"

/*
    Constructor

    int inputPin = The data pin from the AM2302
*/
AM2302::AM2302(int inputPin) {
    pin = inputPin;
}

/*
    Function for listening for a bit change. Listens for every microsecond.

    int check = Value that should be checked for
    int tries = The amount of tries to check for

    Returns the amount of microsecond it took to listen for
*/
int AM2302::bitChangeListener(int check, int tries) {
    int usAmount = 0;
    for(int i = 0; i < tries; i++) {
        if(digitalRead(pin) == check) {
            break;
        }
        usAmount++;
        delayMicroseconds(1);
    }
    return usAmount;
}

/*
    Listen for a single bit
*/
int AM2302::listenForBit() {
    bitChangeListener(0, 50);
    bitChangeListener(1, 50);
    int msAmount = bitChangeListener(0, 70); 
    if(msAmount > 28) {
        return 1;
    } else {
        return 0;
    }
}

/*
    Convert an bit array to an byte array

    int intArr[] = Array aan nummers die naar een byte omgezet te worden
    int bitLength = De lengte van de bits

    Returns byte*, an array of bytes that were merged from the bits
*/
byte* AM2302::bitArrToByteArr(int intArr[], int bitLength) {
    int rounded = 8 - bitLength % 8;
    int byteAmount;
    if(rounded == 8) {
        byteAmount = (bitLength) / 8;
    }
    else {
        byteAmount = (bitLength + (rounded)) / 8;
    }
    byte result[byteAmount];
    for(int byte = 0; byte < byteAmount; byte++) {
        result[byte] = 0;
        for(int bit = 0; bit < 8; bit++) {
            result[byte] = result[byte] + (intArr[byte * 8 + bit] << (7 - bit)); 
        }
    }
    return result;
}

/*
    Add two bytes together to one

    byte byte1 = Most significant byte 
    byte byte2 = Least significant byte
    
    Returns an integer with the two merged bytes
*/
int AM2302::mergeBytes(byte byte1, byte byte2) {
    int result = 0;
    for(int i = 7; i >= 0; i--) {
        result = result + (bitRead(byte1, i) << i);
    }
    result = result << 8;
    for(int i = 7; i >= 0; i--) {
        result = result + (bitRead(byte2, i) << i);
    }
    return result;
}

/*
    Compare two bits

    int number1 = Number 1 that you would like to compare
    int number2 = Number 2 that you would like to compare
    int checkLength = The length of the bits you want to compare starting from the least significant bit 

    Returns boolean based on if the bits were equal
*/
bool AM2302::compareBits(int number1, int number2, int checkLength) {
    for(int i = 0; i < checkLength; i++) {
        if(bitRead(number1, i) != bitRead(number2, i)) {
            return false;
        }
    }
    return true;
}

/*
    Read the temperature
*/
AM2302_res AM2302::read() {
    //Send an request for getting the bits from the sensor
    pinMode(pin, OUTPUT);
    //10 ms low
    digitalWrite(pin, 0);
    delay(10);
    //40 us high
    digitalWrite(pin, 1);
    delayMicroseconds(40);
    //Start listening for bits
    pinMode(pin, INPUT);
    //Wait 40 microseconds until the bit is low
    bitChangeListener(0, 40);
    //Wait 80 microseconds until the bit is high
    bitChangeListener(1, 80);
    //START OF RECEIVING BITS
    int res[40];
    for(int i = 0; i < 40; i++) {
        res[i] = listenForBit();
    }
    byte* byteRes = bitArrToByteArr(res, 40);
    AM2302_res result;
    result.humidity = mergeBytes(byteRes[0], byteRes[1]) / 10.0;
    result.temperature = mergeBytes(byteRes[2], byteRes[3]) / 10.0;
    result.correct = compareBits(byteRes[0] + byteRes[1] + byteRes[2] + byteRes[3], byteRes[4], 8);
    return result;
}