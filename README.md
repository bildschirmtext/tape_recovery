# tape_recovery
Some scripts and little C programs to recover v.23 modulated data from tapes

# Steps to convert tapes:

1. Go to src/bin and execute compile.sh to complile the C programs
2. Go to src and place your .wav files there.
3. Execute process.sh

You need to have sox installed.

Note that this software has no security provisions so be careful how your files are named.

Files are split by sections of unmodulated data. Interruptions in the carrier will most likely cause short garbage files.

You can change the extensions of the resulting binary files by setting the variable extension in process.sh

# Guide to the programs
demod.c: Demodulates audio with a center frequency of a quarter of the sampling rate. So for 1700 Hz use 6800 Hz. Output rate will be half of input rate.

split.c: Looks at the demodulated audio and splits the data into chunks separated by longish unmodulated sections.

uart.c: Takes demodulated data at 16 times the data rate and spits out the octets. Currenty it does not check framing.
