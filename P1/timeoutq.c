//
// timeout queue
//

#include "os.h"
#include "llist.h"
#include "time.h"
#include "led.h"

struct event {
	LL_PTRS;
	int timeout;
	int repeat_interval;
	pfv_t go;
	unsigned int data;
};

#define MAX_EVENTS	8
struct event queue[ MAX_EVENTS ];

// list anchors -- important, but ignore them; they are never used directly
llobject_t TQ;
llobject_t FL;

struct event *timeoutq;
struct event *freelist;

unsigned int then_usec;

//
// sets up concept of local time
// initializes the timeoutq and freelist
// fills the freelist with entries
// timeoutq is empty
//
void
init_timeoutq()
{
	int i;

	then_usec = now_usec();

	timeoutq = (struct event *)&TQ;
	LL_INIT(timeoutq);
	freelist = (struct event *)&FL;
	LL_INIT(freelist);

	for (i=0; i<MAX_EVENTS; i++) {
		struct event *ep = &queue[i];
		LL_PUSH(freelist, ep);
	}

	return;
}


//
// account for however much time has elapsed since last update
// return timeout or MAX
//
#define MAX_WAIT 15*ONE_SECOND
int
bring_timeoutq_current()
{
	unsigned int time_to_wait;
	unsigned int last_update = then_usec;
	
	struct event* tmp;
	
	tmp = (struct event*) LL_FIRST(timeoutq);
	time_to_wait = tmp->timeout;
	time_to_wait -= usec_diff(now_usec(), last_update);
	if(time_to_wait >= 0)
	{
		return (time_to_wait < MAX_WAIT) ? time_to_wait : MAX_WAIT;
		last_update = now_usec();
	} 
	else
	{
		
		return 0;
	}

	//then_usec = now_usec();
	//debug OK time_to_wait - time_to_fire_event <= 0 handle

	// unsigned int time_to_wait = 2*ONE_SECOND;
	// unsigned int last_update = then_usec;
	// time_to_wait -= usec_diff(now_usec(), last_update);

	
}


//
// get a new event structure from the freelist,
// initialize it, and insert it into the timeoutq
// 
void
create_timeoutq_event(int timeout, int repeat, pfv_t function, unsigned int data)
{

	//taking an event structure off the free list
	struct event* new_event = (struct event*)LL_POP(freelist);
		if (new_event == NULL){
			return;
		}
	//initializing its values
	new_event -> timeout = timeout;
	new_event -> repeat_interval = repeat;
	new_event -> go = function;
	new_event -> data = data;
	//inserting it into the timeout queue
	LL_PUSH(timeoutq, new_event);

}



//
// bring the time current
// check the list to see if anything expired
// if so, do it - and either return it to the freelist 
// or put it back into the timeoutq
//
// return whether or not you handled anything
//
int
handle_timeoutq_event( )
{
	struct event* tmp;
	int ret = 0;

	//

	LL_EACH(timeoutq, tmp, struct event)
	{
		/*Handle null pointer*/
		if (tmp == NULL)
			return 0;
		
		/*Check expired timer*/
		if (usec_diff(now_usec(), then_usec) >= tmp->timeout ) {
			/*Handle evnet*/
			tmp->go(tmp->data);
			if (tmp->repeat_interval) {
				tmp->timeout += tmp->repeat_interval;
			} else {
				tmp = (struct event*)LL_DETACH(timeoutq, tmp);
				LL_PUSH(freelist, tmp);
			}
		}

			ret = 1;
		
	}
	return ret;
}

unsigned int 
now_usec( void )
{
	return GET32(0x4000001C);
}

unsigned int
usec_diff( unsigned int now, unsigned int then )
{
    return timediff(now,then);
}