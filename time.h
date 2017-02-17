//
// time.h
//
// information regarding the time library
//

#define	ONE_SECOND	(0x1 << 20)	// you define this however you want/need
#define TIMER_BASE	0x4000001C

extern unsigned int gettime( void );
extern unsigned int timediff( unsigned int now, unsigned int then );
extern void wait( unsigned int time );
