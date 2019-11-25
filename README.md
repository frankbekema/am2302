# am2302
C++ Arduino library driver for the AM2302 or DHT22

## Usage
To use this driver you will need to make a folder in your Arduino libraries directory called "AM2302". After you have done that you can import the driver by using your Arduino libary manager.

## Example

```C
//Include libary
#include <AM2302.h>

//Initialize sensor on GPIO pin 14
AM2302 sensor(14);

void setup() {
  //Start serial input on baud rage 9600
  Serial.begin(9600);
}

void loop() {
  //Read the sensor
  AM2302_res result = sensor.read();
  //Print humidity in % RH
  Serial.print("Humidity ");
  Serial.println(result.humidity);
  //Print temperature in celcius
  Serial.print("Temperature ");
  Serial.println(result.temperature);
  //Boolean wether the tempature was read correctly
  Serial.print("Correct ");
  Serial.println(result.correct);
  //Wait 2 seconds
  delay(2000);
}
```

## License
This project is licensed under the [MIT license](https://opensource.org/licenses/MIT)

## Questions?
If you have any questions feel free to contact Frank Bekema on frankbekema10@gmail.com or on github.
