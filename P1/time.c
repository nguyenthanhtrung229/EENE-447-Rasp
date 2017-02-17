//
// time library
//

#include "time.h"

typedef struct tag_TIMER
{
	unsigned int Timer_LB;
	unsigned int Timer_HB;
} *Timer_Reg;

unsigned int
gettime( )
{
	unsigned int Time_low;
	Time_low = GET32(0x4000001C);
	return Time_low;
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
		
	
	/*else*/	
		/*while(timediff(now + 0xffffffff , then)<time)
		{
			now = gettime();
		}*/
}

