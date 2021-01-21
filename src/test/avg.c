#include <stdio.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
	int n;
	double t=0;
	double x=0;
	double sum=0;
	double samples[16];
	for (n=0; n<16; n++) samples[n]=0;
	n=0;
	while (scanf("%lf%lf", &t, &x)==2) {
		sum=sum+x-samples[n];
		samples[n]=x;
		n=(n+1)%16;
		printf("%lf %lf\n", t, sum);
	}
}
