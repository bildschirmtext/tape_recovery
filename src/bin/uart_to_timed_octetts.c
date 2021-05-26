#include <stdio.h>
#include <string.h>

#define SRATE (19200)
#define BLEN (16)

#define BUFLEN (SRATE*1800)


char bits[BUFLEN];

int main(int argc, char *argv[])
{
	memset(bits, 0, sizeof(bits));
	int fs=fread(bits, 1, sizeof(bits), stdin);
	char c;
	int octett=0;
	int pos=1;
	int start=-1;
	int nstart=-1;
	while ((pos<fs) && (bits[pos]>0)) {
		int bit=bits[pos]%2;
		if (start<0) { //idle state
			if (bit==0) {
				start=pos; 
				octett=0;
				nstart=-1;
			}
		} else {
			if ( (nstart<0) && (bits[pos-1]=='1') && (bits[pos]=='0') ) { //Potential new start bit
				nstart=pos;
			}
			int bitnum=(pos-start)/BLEN;
			int bitpos=(pos-start)%BLEN;
			if (bitpos==BLEN/2) {
				if ( ( (bitnum==0) && (bit==1) ) ||
				     ( (bitnum==9) && (bit==0) ) ) {
					//error => backtrack
					start=nstart;
					octett=0;
					if (nstart>0) pos=nstart;
					nstart=-1;
				} else  if (bitnum==9) {
					printf("%d %d\n", start, octett);
					start=-1;
					nstart=-1;
				} else {
					octett=octett | (bit<<(bitnum-1));
				}
			}
		}
		pos=pos+1;
	}
}
