

#include <Arduino.h>
#include <FastLog.h>


void setup() 
{
  Serial.begin( 115200 );

  // put your setup code here, to run once:
  FL_Init();

  FL_Add( "Tes1", 1, 2, 3, 4 );

  FL_Add( "Tes2", 5, 6, 7, 8 );

  FL_Add( "Tes3", 30, 40, 50, 60 );

  uint16 logCount = FL_GetEntryCount();

  Serial.println( "log count" );
  Serial.println( logCount, DEC );

    uint32 *name;
    char nameStr[5];
    uint32 value1;
    uint32 value2;
    uint32 value3;
    uint32 value4;

    name = (uint32 *)nameStr;
    nameStr[4] = 0; // null terminate the string

  for (uint16 i = 0; i < logCount; i++)
  {

    bool entryValid = FL_GetNextEntry( (i == 0), name, &value1, &value2, &value3, &value4 );

    if ( entryValid )
    {
      Serial.println( "entry valid" );
      //Serial.println( name, HEX );
      Serial.println( nameStr );
      Serial.println( value1, DEC );
      Serial.println( value2, DEC );
      Serial.println( value3, DEC );
      Serial.println( value4, DEC );
    }
  }

}

void loop() 
{
  // put your main code here, to run repeatedly:





}

