#include <stdio.h>
#include <stdlib.h>

/*This program does a majority vote on the input bits
 * this should kill short glitches
 */

#define MAX_LEN (255)

int main(int argc, char **argv)
{
	int l=7;
	if (argc==2) {
		l=atoi(argv[1]);
	}
	double t;
	double x;
	int bits[MAX_LEN];
	int n=0;
	for (n=0; n<MAX_LEN; n++) bits[n]=1; //set buffer to idle state
	int sum=l;
	while (scanf("%lf%lf", &t, &x)==2) {
		int bit=0;
		if (x<0) bit=1;
		sum=sum+bit-bits[n%l];
		bits[n%l]=bit;
		n=n+1;
		if (sum>l/2) printf("1"); else printf("0");
	}
}
