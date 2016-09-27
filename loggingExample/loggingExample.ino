/******************************************************************************
    Filename: loggingExample.ino
    Description: 
        Demontration code for the Fast Logging module.  Use the serial port to 
		accept commands to interact with the logging module.
		Log some entries and then dump them later at a slower speed.
		The log can capture the data very quickly with little impact to the
		timing of the code.  Then later dump the log to help debug the
		instrumented code.
    
    Written: Jon Norenberg, 2013 - 2016
    Copyright: My code is your code.  No warranties. 
******************************************************************************/
 
 
#include <TimerOne.h>
#include "FastLog.h"

//******************************************************************************
// Constants
//******************************************************************************
#define LED_PIN			13

//******************************************************************************
// Globals
//******************************************************************************
int ledOn = 1;
uint32 tickCounter = 0;

char tempString[ 120 ];


/******************************************************************************
    @fn     fireLed

    @brief  This routine is called by the timer interrupt which allows the
			led to be turned on and off at a certain interval.

    @param  
            
    @return 
******************************************************************************/
void fireLed( void )
{
	if ( ledOn == 0 )
	{
		ledOn = 1;
		digitalWrite( LED_PIN, HIGH );   	
	}
	else
	{
		ledOn = 0;
		digitalWrite( LED_PIN, LOW );   	
	}

	++tickCounter;
	
	// add a log entry with a 4 character name and 4 data values
	FLAdd( FL_GRP_TIMER, "time", tickCounter, ledOn, LED_PIN, HIGH ? 1 : 0 );
}

/******************************************************************************
    @fn     setup

    @brief  Setup function runs once when you press reset or power the board.

    @param  
            
    @return 
******************************************************************************/
void setup() 
{
	// start the serial point
	Serial.begin( 9600 );
    while ( !Serial );

    Serial.println( "Fast Logging module - test program" );

	// initialize digital pin x as an output.
	pinMode( LED_PIN, OUTPUT );

    // set timer for 500 milliseconds
	Timer1.initialize( 500000 );
	
	// attach the timer service routine here
	Timer1.attachInterrupt( fireLed ); 
	
	// initialize the Fast Logging module
	FLInit();
}

/******************************************************************************
    @fn     setup

    @brief  Loop function runs over and over again forever.
			Accept input from the serial console and interact with the
			logging mechanism
    @param  
            
    @return 
******************************************************************************/
void loop() 
{
	char sInput;
	char entryName[5];
	uint16 entryCount, i;
	uint32 param1, param2, param3, param4, eName;
	
    if ( Serial.available() == 0 ) 
    {   // nothing to do
        return;
    }
    // read a character from serial console
    sInput = Serial.read();

	// add a log entry with a 4 character name and 4 data values
	FLAdd( FL_GRP_UART, "sdat", sInput, 0, 0, 0 );

    if ( sInput == 'c' ) 
    {   // clear out all the log entries
#if ENABLE_FAST_LOG
        FLClear();
		Serial.println( "Clear the logging buffer" );
#endif
    } 
    if ( sInput == 's' ) 
    {   // clear out all the log entries
#if ENABLE_FAST_LOG
        if ( FLGetState() )
		{	// logging is on, but now turn it off
			Serial.println( "Logging was ON, turning it OFF" );
			FLSetState( FALSE );
		}
		else
		{	// logging is off, but now turn it on
			Serial.println( "Logging was OFF, turning it ON" );
			FLSetState( TRUE );
		}
#endif
    } 
    else if ( sInput == 'd' ) 
    {   // dump the log entries
#if ENABLE_FAST_LOG
		entryCount = FLGetEntryCount();
		
        sprintf( tempString, "Total log entries = %d", entryCount );
        Serial.println( tempString );

		if ( entryCount )
		{	// get all the entries from the log, one entry at a time
			for ( i = 0; i < entryCount; ++i )
			{
				FLGetNextEntry( i == 0 ? TRUE : FALSE, &eName, &param1, &param2, &param3, &param4 );
				entryName[0] = ((char *)&eName)[0];
				entryName[1] = ((char *)&eName)[1];
				entryName[2] = ((char *)&eName)[2];
				entryName[3] = ((char *)&eName)[3];
				entryName[4] = 0;
				sprintf( tempString, "Entry %d: %s -> %ld, %ld, %ld, %ld", i + 1, entryName, param1, param2, param3, param4 );
				Serial.println( tempString );
			}
		}
		else
		{
			Serial.println( "No entries in the log." );
		}
#endif
	} 



}
