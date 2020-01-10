#include <stdio.h>


/* This program takes samples in the "dat" format of sox at 4 times the medium frequency
 * The output is half of the intput sample rate.
 */

int main(int argc, char **argv)
{
	double t;
	double x;
	int bits[3];
	int n=0;
	for (n=0; n<3; n++) bits[n]=0;
	int sum=0;
	while (scanf("%lf%lf", &t, &x)==2) {
		int bit=0;
		if (x<0) bit=1;
		sum=sum+bit-bits[n%3];
		bits[n%3]=bit;
		n=n+1;
		if (sum>1) printf("1"); else printf("0");
	}
}
