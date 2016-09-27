/******************************************************************************
    Filename: FastLog.h
    Description: 
        This file contains the a simple way to log events while the system
		is running.  Basically the info is dumped into a circular buffer
		and then dumped after the event has been captured.
		
		ENABLE_FAST_LOG must be defined in your build system - for arduino
		I just put it in this file and then comment/uncomment when I want
		to include the logging code
    
    Written: Jon Norenberg, 2013 - 2016
    Copyright: My code is your code.  No warranties. 
******************************************************************************/
#define ENABLE_FAST_LOG		1
 
#ifndef FAST_LOG_H
#define FAST_LOG_H

//******************************************************************************
// Includes
//******************************************************************************


//******************************************************************************
// Constants
//******************************************************************************

// define how many events can be captured before events are thrown away
// oldest events are thrown away first
#define MAX_NUMBER_LOG_EVENTS  			32

// to keep things from being interrupted, define these
#define FL_ENTER_CRITICAL_SECTION
#define FL_EXIT_CRITICAL_SECTION

// create groups so that you can log and debug group by group
#define FL_GRP_I2C              ( 1 << 0 )
#define FL_GRP_UART           	( 1 << 1 )
#define FL_GRP_FW_UPDATE        ( 1 << 2 )
#define FL_GRP_VIDEO            ( 1 << 3 )
#define FL_GRP_TIMER            ( 1 << 4 )
// add more bits here

// when debugging, add the groups if you want to log in that subsystem
#define ACTIVE_TRACE_BITS 		( \
                                    FL_GRP_UART | \
                                    FL_GRP_TIMER | \
                                    FL_GRP_I2C | \
                                    0 \
                                )


#ifndef TRUE
#define TRUE                            1
#endif

#ifndef FALSE
#define FALSE                           0
#endif

//******************************************************************************
// Typedefs
//******************************************************************************

// modify as necessary for your system
typedef unsigned long uint32;
//typedef unsigned int uint32;
typedef unsigned short uint16;
//typedef unsigned char uint8;
//typedef unsigned char bool;

//******************************************************************************
// Function prototypes
//******************************************************************************

#if !ENABLE_FAST_LOG
// logging has not been enabled in the build system
#define FLGetNextEntry( firstEntry, pTag, value1, value2, value3, value4 )
#define FLGetEntryCount()
#define FLSetState( newTraceState )
#define FLGetState()
#define FLClear() 
#define FLInit()
#define FLAdd( traceFlag, entryName, param1, param2, param3, param4 )    

#else
// PROTOTYPES FOR THE MACROS
void FL_Init( void );
void FL_Add( const char *pName, uint32 value1, uint32 value2, uint32 value3, uint32 value4 );
bool FL_GetNextEntry( bool firstEntry, uint32 *pName, uint32 *value1, uint32 *value2, uint32 *value3, uint32 *value4 );
uint16 FL_GetEntryCount( void );
void FL_SetState( bool newTraceState );
bool FL_GetState( void );
void FL_Clear( void );

// USE THESE MACROS IN YOUR CODE
#define FLInit()    \
    do { \
        FL_Init();   \
    } while (0);

#define FLGetNextEntry( firstEntry, pName, value1, value2, value3, value4 )    \
    do { \
        FL_GetNextEntry( firstEntry, pName, value1, value2, value3, value4 );   \
    } while (0);

#define FLGetEntryCount()    FL_GetEntryCount()

#define FLSetState( newTraceState )    \
    do { \
        FL_SetState( newTraceState );   \
    } while (0);

#define FLGetState()    	FL_GetState()

#define FLClear()    \
    do { \
        FL_Clear();   \
    } while (0);

#define FLAdd( traceFlag, entryName, param1, param2, param3, param4 )   \
    do { \
        if ( ACTIVE_TRACE_BITS & traceFlag ) FL_Add( entryName, param1, param2, param3, param4 );   \
    } while (0);
    
#endif
    
#endif
