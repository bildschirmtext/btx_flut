#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<strings.h>

#define MAXWIDTH (80)
#define MAXHEIGHT (24*3)


void gotoxy(int x, int y)
{
	printf("\x1f%c%c", y+'A', x+'A');
}

void set_fgcolor(int new, int *old)
{
	if (new<0) return;
	if (new>7) return;
	if (new==*old) return;
	printf("%c", 0x80+new);
	*old=new;
}

void set_bgcolor(int new, int *old)
{
	if (new<0) return;
	if (new>7) return;
	if (new==*old) return;
	printf("%c", 0x90+new);
	*old=new;
}


int main(int argc, char *argv[])
{
	uint8_t image[MAXWIDTH*MAXHEIGHT];
	memset(image, 0, sizeof(image));
	int width=0;
	int height=0;
	if (scanf("%d%d\n", &width, &height)!=2) {
		fprintf(stderr,"Syntax Error in Header\n");
		return 1;
	}
	int y=0;
	int x=0;
	char *line=NULL;
	size_t as=0;
	for (y=0; y<height; y++) {
		size_t s=getline(&line, &as, stdin);
		for (x=0; x<width; x++) {
			int p=0;
			if ((line[x]>='0') && (line[x]<='9')) p=line[x]-'0'; else
			if ((line[x]<='A') && (line[x]<='Z')) p=line[x]-'A'+10;
			image[y*MAXWIDTH+x]=p;
		}
	}

	fprintf(stderr, "Width: %d characters, Height: %d characters\n", width/2, height/3);

	for (y=0; y<height/3*3; y++) {
		for (x=0; x<width/2*2; x++) {
			fprintf(stderr, "%d", image[y*MAXWIDTH+x]);
		}
		fprintf(stderr, "\n");
	}

	printf("\x1f\x2f\x42"); //Reset Terminal to parallel attributes
	printf("\x1b\x7e"); //Graphical characters in right area

	for (y=0; y<height/3; y++){
		int fg=-1;
		int bg=-1;
		gotoxy(0,y);
		printf("%02d ", y);
		for (x=0; x<width/2; x++) {
			//Write sextet for character
			uint8_t pixels[6];
			pixels[0]=image[(y*3+0)*MAXWIDTH+(x*2+0)];
			pixels[1]=image[(y*3+0)*MAXWIDTH+(x*2+1)];
			pixels[2]=image[(y*3+1)*MAXWIDTH+(x*2+0)];
			pixels[3]=image[(y*3+1)*MAXWIDTH+(x*2+1)];
			pixels[4]=image[(y*3+2)*MAXWIDTH+(x*2+0)];
			pixels[5]=image[(y*3+2)*MAXWIDTH+(x*2+1)];

			//Count the number of pixels of a colour
			int counts[256];
			memset(counts, 0, sizeof(counts));
			int n;
			for (n=0; n<6; n++) {
				counts[pixels[n]]=counts[pixels[n]]+1;
			}

			//Find the most and second most common color
			int first=-1;
			int second=-1;
			int third=-1;
			for (n=0; n<256; n++) {
				if ( (first<0) || (counts[n]>=counts[first])) {
					third=second;
					second=first;
					first=n;
				}
			}
			//If there is a third colour, warn about colour clash
			if (third>=0) {
				fprintf(stderr, "Colour clash on Row %d Column %d ", y,x);
				fprintf(stderr, "Colours: %d %d %d\n", first, second, third);
			}
			//If there is no second colour, there's only one colour
			if ( (second<0) || (second>0xff) ) {
				second=bg;
				if (first==bg) second=fg;
				if (second<0) second=0; //If foreground or background colour is not set, use 0
			}
			//if the first color is smaller than the second one, swap
			if (first<second) {
				int t=second;
				second=first;
				first=t;
			}
			fprintf(stderr,"f=%02x s=%02x\n", first, second);
			set_fgcolor(first, &fg);
			set_bgcolor(second, &bg);

			uint8_t v=0;
			for (n=0; n<6; n++) {
				if (pixels[n]!=bg) v=v| (1<<n); 
			}
			fprintf(stderr, "%02x ", v);
			if (v==0x00) printf(" "); else
			if (v<0x20) printf("%c", (v&0x1f)+0xA0); else
			if (v==0x3f) printf("\xDF"); else
			printf("%c", (v&0x1f)+0xE0);
		}
		fprintf(stderr,"\n");
	}

}
