/******************************************************************************
    Filename: FastLog.cpp
    Description: 
        This file contains the a simple way to log events while the system
		is running.  Basically the info is dumped into a circular buffer
		and then dumped after the event has been captured.
    
    Written: Jon Norenberg, 2013 - 2016
    Copyright: My code is your code.  No warranties. 
******************************************************************************/

//******************************************************************************
// Includes
//******************************************************************************

#include "FastLog.h"

// set this true in your build system if the logging mechanism is needed.
// enable to debug builds, but maybe not release builds
#if ENABLE_FAST_LOG


//******************************************************************************
// Constants
//******************************************************************************

//******************************************************************************
// Typedefs
//******************************************************************************

// log arrary members
typedef struct 
{
    uint32      entryName;
    uint32      param1;
    uint32      param2;
    uint32      param3;
    uint32      param4;
} LogEntry_t, *LogEntryPtr_t;

//******************************************************************************
// Global variables
//******************************************************************************

// log array
static LogEntry_t     	gLogArray[ MAX_NUMBER_LOG_EVENTS ];
// tracks where the next entry will be inserted
static uint16           gLogHeadIdx = 0;
// tracks how many entries are in the array
static uint16           gLogCount = 0;
// should the array accept new entries
static bool             gLogEnabled = TRUE;


/**************************************************************************************************
 * @fn      FL_Init
 *
 * @brief   Setup the log array for operation.
 *
 * @param   
 *
 * @return  
 **************************************************************************************************/
void FL_Init( void )
{
    FL_Clear();
}

/**************************************************************************************************
 * @fn      DbgLogInsert
 *
 * @brief   Add a log entry to the global log array.  Overwrite the last (oldest) value if the
 *          array is full.
 *
 * @param   eName - pointer to a string which should contain at least 4 chars
 *          value1 - first param to save in the log
 *          value2 - second param to save in the log
 *          value3 - third param to save in the log
 *          value4 - fourth param to save in the log
 *
 * @return  
 **************************************************************************************************/
void FL_Add( const char *eName, uint32 value1, uint32 value2, uint32 value3, uint32 value4 )
{
    FL_ENTER_CRITICAL_SECTION;
    
    if ( gLogEnabled )
    {
        // save the log entry data into the circular buffer
        gLogArray[ gLogHeadIdx ].entryName = *( (uint32 *)eName );
        gLogArray[ gLogHeadIdx ].param1 = value1;
        gLogArray[ gLogHeadIdx ].param2 = value2;
        gLogArray[ gLogHeadIdx ].param3 = value3;
        gLogArray[ gLogHeadIdx ].param4 = value4;

        // adjust the head index, see if it is past the end of the array
        if ( gLogHeadIdx < ( MAX_NUMBER_LOG_EVENTS - 1) )
        { 	// move to the next slot in the array  
            ++gLogHeadIdx;
        }
        else
        {	// reset to the top of the array
            gLogHeadIdx = 0;       
        }
        
        // check if the array is already full
        if ( gLogCount < MAX_NUMBER_LOG_EVENTS )
        {
            ++gLogCount;
        }
    }
    else
    {
        // throw the log entry on the floor since the log mechanism is disabled
    }
    
    FL_EXIT_CRITICAL_SECTION;
}

/**************************************************************************************************
 * @fn      FL_GetNextEntry
 *
 * @brief   Get a log entry.  Return the most recent entry first, the last entry returned
 *          is the oldest.  Must pass in true for firstEntry, so set the log buffer
 *          index.
 *
 * @param   firstEntry - set to true on first call to get the entire buffer, all following 
 *                       calls it must be false.
 *          pName - pointer to a string which should contain at least 4 chars
 *          pParam1 - put first param from log entry here
 *          pParam2 - put second param from log entry here
 *          pParam3 - put third param from log entry here
 *          pParam4 - put fourth param from log entry here
 *
 * @return  TRUE, if a valid entry was returned, FALSE if no entry could be found
 **************************************************************************************************/
bool FL_GetNextEntry( bool firstEntry, uint32 *pName, uint32 *pParam1, uint32 *pParam2, 
                 uint32 *pParam3, uint32 *pParam4 )
{
    static uint16 retrieveIndex = 0;
    static uint16 retrieveCount = 0;
    bool entryValid;

    FL_ENTER_CRITICAL_SECTION;
    
    if ( firstEntry )
    {   // set where to start retrieving the data
        // start at the 
        retrieveIndex = gLogHeadIdx;
        retrieveCount = 0;
    }
    
    // check if the request is within the buffer count
    if ( retrieveCount < gLogCount )
    {
        if ( retrieveIndex == 0 )
        {   // hit the start of the buffer, go to the end
            retrieveIndex = MAX_NUMBER_LOG_EVENTS - 1;
        }
        else
        {
            --retrieveIndex;
        }
        
        // copy the log entry data to the variables
        *pName = gLogArray[retrieveIndex].entryName;
        *pParam1 = gLogArray[retrieveIndex].param1;
        *pParam2 = gLogArray[retrieveIndex].param2;
        *pParam3 = gLogArray[retrieveIndex].param3;
        *pParam4 = gLogArray[retrieveIndex].param4;
        entryValid = TRUE;
        ++retrieveCount;
    }
    else
    {   // caller requested more entries than were in the buffer
        entryValid = FALSE;
    }
    
    FL_EXIT_CRITICAL_SECTION;
    
    return entryValid;
}

/**************************************************************************************************
 * @fn      FL_Clear
 *
 * @brief   Clear the log by setting the head to 0 and the count.
 *
 * @param   
 *
 * @return  
 **************************************************************************************************/
void FL_Clear( void )
{
    FL_ENTER_CRITICAL_SECTION;

    gLogHeadIdx = 0;
    gLogCount = 0;
        
    FL_EXIT_CRITICAL_SECTION;
}

/**************************************************************************************************
 * @fn      FL_GetEntryCount
 *
 * @brief   Return the number of log entries in the array.
 *
 * @param   
 *
 * @return 	number of log entries 
 **************************************************************************************************/
uint16 FL_GetEntryCount( void )
{
    uint16 entryCount;

    FL_ENTER_CRITICAL_SECTION;

    entryCount = gLogCount;
        
    FL_EXIT_CRITICAL_SECTION;
    
    return entryCount;
}

/**************************************************************************************************
 * @fn      FL_SetState
 *
 * @brief   Enable the log by setting global to TRUE/FALSE.
 *
 * @param	newLogState - set to TRUE to capture events, FALSE to stop capturing events 	  
 *
 * @return  
 **************************************************************************************************/
void FL_SetState( bool newLogState )
{
    FL_ENTER_CRITICAL_SECTION;
    
    gLogEnabled = newLogState;

    FL_EXIT_CRITICAL_SECTION;
}

/**************************************************************************************************
 * @fn      FL_GetState
 *
 * @brief   Return the current state of the log.
 *
 * @param   
 *
 * @return  TRUE if log is enabled, FALSE otherwise  
 **************************************************************************************************/
bool FL_GetState( void )
{
    bool logState;
    
    FL_ENTER_CRITICAL_SECTION;

    logState = gLogEnabled;
        
    FL_EXIT_CRITICAL_SECTION;
    
    return logState;  
}


#endif
