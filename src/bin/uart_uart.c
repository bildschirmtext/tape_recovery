#include <stdio.h>
#include <stdint.h>


int popcnt(uint8_t b)
{
	int cnt=0;
	int n;
	for (n=0; n<8; n++)
		if ((b>>n)!=0) cnt=cnt+1;
	return cnt;
}

int main(int argc, char *argv[])
{
	int bpos=0;
	uint16_t byte=0;
	int framepos=-1;
	while (!feof(stdin)) {
		char c=fgetc(stdin);
		if (framepos<0) {
			//We are in idle state
			//If there is a 0, we have a transition to the start bit
			if (c=='0') {
				framepos=0;
				byte=0;
			}
		} else if (framepos%16==7) { 
			//We are at the center of a bit
			int bit=framepos/16;
			if (bit==0) { //Start Bit
				if (c=='1') { //Start Bit must be 0
					fprintf(stderr, "Framing Error, start bit invalid\n");
					framepos=-1;
				}
			} else if ( (bit>=1) && (bit<=8) ) {
				if (c=='1') byte=byte | (1<<(bit-1));
			} else { //Stop bit must be 1
				if (c=='1') {
					if (popcnt(byte)%2==0) {
						printf("%lf\t%d\n", bpos/19200.0, byte);
					} else {
						fprintf(stderr, "Parity Error 0x%02x\n", byte);
					}
				} else {
					//If Stop bit is not 1, we have an error
					fprintf(stderr, "Framing Error byte=0x%02x\n", byte);
				}
				framepos=-1;
			}
		}
		if (framepos>=0) framepos=framepos+1;
		bpos=bpos+1;
	}
	return 0;
}
