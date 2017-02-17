//
// time library
//

#include "time.h"

// Using struct to gettime
/*
typedef struct tag_TIMER
{
	unsigned int Timer_LB;
	unsigned int Timer_HB;
} Timer_Reg;

static volatile Timer_Reg *pTimer_Reg = (Timer_Reg*)TIMER_BASE;
*/

unsigned int
gettime( )
{
	unsigned int Time_low;
	Time_low = GET32(0x4000001C);
	return Time_low;

	//return pTimer_Reg-> Timer_LB;
}

unsigned int
timediff( unsigned int now, unsigned int then )
{
	if(now > then)
	 return (now - then);
	if(now < then) 
		return (now + 0xffffffff - then);
}

void
wait( unsigned int time )
{
	unsigned int now;
	unsigned int then;
	then = gettime();
	now = gettime();
	while(timediff(now, then)<time)
		{
		now = gettime();
		}
}

