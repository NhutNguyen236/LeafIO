// Compatible with the Arduino IDE 1.0
// Library version:1.1
// Code by Dana Dietz with the help of Jeremy Blum, Sunfounder and others who wrote lcd code
// My first code using I2C
// LCD 1602A with backpack PCF85724T (I2C)
// This example shows you how to use the LCD with an I2C backpack
// It shows you how the I2C only accepts one bit at a time by printing only the first character of an array
// It then shows you how to print several characters at once either staying in one position, or scrolling, in this case, to the left

#include <Wire.h>              //wire library
#include <LiquidCrystal_I2C.h> //liquid crystal with I2C library

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

char array1[] = "Nhiet do vuon     "; // an array to print on line 0
char array2[] = "27oC"; // an array to print on line 1
char array3[] = "Oops!           ";
char array4[] = "1 Bit at a time!";

void setup()
{
    lcd.init(); // initialize the lcd

    lcd.backlight(); // Turn on backlight
}

void loop()
{
    // show that I2C only accepts one byte at a time
    // lcd.setCursor(0, 0); // set cursor at top left
    // lcd.print(array1);   // will only print the first letter of the array
    // lcd.setCursor(0, 1); // set cursor at left space on 2nd row
    // lcd.print(array2);   // will only print the first letter of the array
    // delay(2000);

    // lcd.clear();
    lcd.setCursor(0, 0);
    for (int positionCounter1 = 0; positionCounter1 < 17; positionCounter1++)
    {

        lcd.print(array3[positionCounter1]); // prints a 17 character array without scrolling
        delay(250);
    }

    {
        lcd.setCursor(0, 1);
        for (int positionCounter2 = 0; positionCounter2 < 17; positionCounter2++)
        {
            lcd.print(array4[positionCounter2]); // prints a 17 character or array without scrolling
        }
        delay(3000);
        lcd.clear();
    }
    {
        lcd.setCursor(15, 0); // set the cursor to column 15, line 0
        for (int positionCounter1 = 0; positionCounter1 < 16; positionCounter1++)
        {
            lcd.scrollDisplayLeft();             // Scrolls the contents of the display one space to the left.
            lcd.print(array1[positionCounter1]); // Print 12 character array
            delay(500);
        }
        delay(1000);
        lcd.clear(); // Clears the LCD screen and positions the cursor in the upper-left corner.

        lcd.setCursor(15, 1); // set the cursor to column 15, line 1
        for (int positionCounter2 = 0; positionCounter2 < 11; positionCounter2++)
        {
            lcd.scrollDisplayLeft();             // Scrolls the contents of the display one space to the left.
            lcd.print(array2[positionCounter2]); // Print a message to the LCD.
            delay(500);
        }
        delay(5000);
    }
    lcd.clear(); // Clears the LCD screen and positions the cursor in the upper-left corner.
}
