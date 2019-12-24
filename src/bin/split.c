#include <stdio.h>

#define BUFFLEN (512)

double buffer[BUFFLEN];

int main(int argc, char *argv[])
{
	double t=0;
	double x=0;

	int n;
	for (n=0; n<BUFFLEN; n++) buffer[n]=-0.5;
	
	FILE *f=fopen("start.dat","w");

	int cnt=0;
	int pause=0;
	int fileno=0;

	int p=0;
	while (scanf("%lf%lf", &t, &x)==2) {
	       fprintf(f, "%lf %lf\n", t, buffer[p]);
	       buffer[p]=x;
	       p=(p+1)%BUFFLEN;
	       if (x<0) {
		       cnt=cnt+1;
	       } else {
		       if (cnt>BUFFLEN) {
		       		printf("cnt=%d\n", cnt);
			       char fn[128];
			       sprintf(fn, "%03d.dat", fileno);
			       fileno=fileno+1;
			       if (f!=NULL) fclose(f);
			       f=fopen(fn, "w");
		       }
		       cnt=0;
	       }
	}	       
}


