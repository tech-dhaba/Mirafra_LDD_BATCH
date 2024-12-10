#include <time.h>
#include<stdio.h>
void timestamp();
int main(){
	timestamp();
	return 0;
}
void timestamp()
{
    time_t ltime; /* calendar time */
    ltime=time(NULL); /* get current cal time */
    printf("%s",asctime( localtime(&ltime) ) );
}

