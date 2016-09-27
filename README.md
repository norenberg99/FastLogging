# FastLogging
Simple fast logging module which does not impact timing.

You configure how many entries to store in the logging array.
	#define MAX_NUMBER_LOG_EVENTS  			32

Then instrument your code where you want to look at variables at runtime.
	FLAdd( FL_GRP_TIMER, "time", tickCounter, ledOn, LED_PIN, HIGH ? 1 : 0 );

Each log entry has:

The group (GRP) define is just a culmination of the group bits that you want
to capture.  Change this and then recompile to debugging a different section
of code (group).  Create new trace bits by including the group you want to 
debug.

A 4 char identifier so that it is easier to trace your log entries.

4 parameters to find out what is going on with the code.

When Fast Logging is turned off (#define ENABLE_FAST_LOG 0) then no
logging code is added to your image.

#define ACTIVE_TRACE_BITS 		( \
                                    FL_GRP_UART | \
                                    FL_GRP_TIMER | \
                                    FL_GRP_I2C | \
                                    0 \
                                )

Run your code and then after the logging, go back and dump the log entries
using whatever interface is appropriate for your system.  The uart is the 
normal easy interface.

I created an Arduino project using the Fast Logging module because it is quick
and the uart is easy to use.  The log can be cleared or dumped at any time. 

Why not just use printfs?  They are slow, take up space, and need a big library.

Have fun,

jon
