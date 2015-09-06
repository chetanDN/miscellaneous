/*
looking at the "resolution" entries in
       /proc/timer_list , to know you kernel supported resolution
*/
#include <stdio.h>
#include <time.h>

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>


int kbhit(void)
{
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	if(ch != EOF)
	{
		ungetc(ch, stdin);
		return ch;
	}

	return 0;
}



//A function for the seconds tick.
void wait_for_sec(int sec)	//default wait for one second
{
	clock_t endCpuCount;	//end count marker of required cpu clock ticks
	endCpuCount = clock() + sec * CLOCKS_PER_SEC; 	//$ man 3 clock -> (present cpu clockticks + required more cpu clock ticks) to make it 1(n) second 
	while ( clock() < endCpuCount) 	//wait till end clock ticks reached
		;
}

int main(void)
{
	char ch;	//kbhit
//	int hour,min,sec;
	long unsigned int totalSec=0;



	printf("\npress   'p' -> to pause and\n\t'r' -> to resume\n\n");	
	while(1)
	{
		while((kbhit() ) )
		{
			ch=getchar();
			while( ch == 'p' )
			{
				do
				{
					ch=kbhit();
					if( ch == 'r')
						break;
				}while( ! kbhit() );
				break;
			}
		}
		
		printf("\b\b%02d:%02d:%02d\r",(int)totalSec/3600 ,(int) (totalSec%3600)/60 ,(int)(totalSec%3600)%60);

		++totalSec;
		wait_for_sec(1);

	}

	return 0;
}
