#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>


#define SRATE (1200*16)

#define MIN_PAUSE (SRATE/5)

void sno_to_string(char *s, const size_t ss, const int snum)
{
	int srem=snum%SRATE; //Remaining samples
	int fsec=snum/SRATE; //Full seconds
	int sec=(fsec%60); //Seconds
	int min=(fsec/60)%60; //Minutes
	int hour=(fsec/3600); //Hours
	int msec=(srem*1000)/SRATE; //Milliseconds
	snprintf(s, ss, "%01d-%02d-%02d.%03d", hour, min, sec, msec);
}


void print_message(const int snum, const char *message)
{
	char time[32];
	memset(time, 0, sizeof(time));
	sno_to_string(time, sizeof(time)-1, snum);
	printf("%s: %s\n", time, message);
}

int main(int argc, char *argv[])
{
	if (argc!=2) {
		printf("Usage: %s <prefix>\n", argv[0]);
		return 1;
	}
	char *prefix=argv[1];	
	printf("prefix: %s\n", prefix);
	FILE *f=NULL;
	long long int sampleno=0; //What time in samples is it?
	long long int framestart=-1; //When did this frame start?
	long long int lastframestart=-1; //When did the previous frame start?
	uint16_t byte=0;
	int framepos=-1;
	int blocknumber=0;
	while (!feof(stdin)) {
		char c=fgetc(stdin);
		if (framepos<0) {
			//We are in idle state
			//If there is a 0, we have a transition to the start bit
			if (c=='0') {
				framepos=0;
				byte=0;
				framestart=sampleno;
			}
		} else if (framepos%16==7) { 
			//We are at the center of a bit
			int bit=framepos/16;
			if (bit==0) { //Start Bit
				if (c=='1') { //Start Bit must be 0
					print_message(sampleno, "Framing Error, start bit invalid");
					framepos=-1;
				}
			} else if ( (bit>=1) && (bit<=8) ) {
				if (c=='1') byte=byte | (1<<(bit-1));
			} else { //Stop bit must be 1
				if (c=='1') {
					//A full frame was received
					if ((framestart-lastframestart>MIN_PAUSE) || (f==NULL) ) {
						print_message(framestart, "Starting new block");
						if (f!=NULL) {
							fclose(f);
							f=NULL;
						}
						char time[32];
						sno_to_string(time, sizeof(time)-1, framestart);
						char fn[256];
						memset(fn,0, sizeof(fn));
						snprintf(fn, sizeof(fn)-1,"%s%03d%s.cpt", prefix, blocknumber, time);
						f=fopen(fn, "w");
						if (f==NULL) {
							printf("Couldn't create file %s\n", fn);
							return 1;
						}
						blocknumber=blocknumber+1;
					}
					putc(byte,f);
					lastframestart=framestart;
					framestart=-1;
				} else {
					//If Stop bit is not 1, we have an error
					print_message(sampleno, "Framing error, Stop bit invalid");
					printf("Framing Error byte=0x%02x\n", byte);
				}
				framepos=-1;
			}
		}
		if (framepos>=0) framepos=framepos+1;
		sampleno=sampleno+1;
	}
	if (f!=NULL) {
		fclose(f);
	}
	return 0;
}
