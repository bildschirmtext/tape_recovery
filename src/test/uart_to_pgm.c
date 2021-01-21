#include <stdio.h>
#include <stdint.h>

int popcnt(const int c)
{
	int n;
	int cnt=0;
	for (n=0; n<8; n++) {
		cnt=cnt+((c>>n)&1);
	}
	return cnt;
}

int main(int argc, char *argv[])
{
	int n=-1;
	double t=0;
	double x=0;
	double bits[10];
	int bit=0;
	while (scanf("%lf%lf", &t, &x)==2) {
		if (n>16*9.5) {
			int ch=0;
			for (n=0; n<9; n++) if (bits[n+1]<0) ch=ch| (1<<n);
			if ((ch&0x100)==0) {
				printf("FRAMING ERROR");
			}
			int par=popcnt(ch)%1;
			if (par!=0) printf("PARITY ERROR! "); else {
				int frame=t*50;
				fprintf(stderr,"%d %d\n", frame, ch%0x7f);
			}
			printf("%03x ",ch);
			char c=ch&0x7f;
			if ((c>=0x20) && (c<0x7f)) printf("%c", c);
			printf("\n");
			n=-1; //after character was received go to idle
			bit=0;
		}
		if (n<0) { //idle state
			if (x<0) continue; //no start bit
			n=0;
		}
		if ((n+8)%16==0) {
			if ((x<0) && (bit==0)) {
				printf("FRAMING ERROR\n");
				n=-1;
				continue;
			}
			printf("|"); 
			if (bit<10) bits[bit]=x;
			bit=bit+1;
		}else {
			if (x<0) printf("#"); else printf(".");
		}
		n=n+1;
	}
}
