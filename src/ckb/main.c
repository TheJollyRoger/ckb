#define _DEFAULT_SOURCE
#ifdef __linux
#include <features.h>
#endif
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>	//life
#include <errno.h>
#include <linux/input.h>


#define N_KEYS 144
FILE* output = 0;

// Keyboard LED positions, measured roughly in 16th inches. Most keys are 3/4" apart.
typedef struct {
    const char* name;
    int x, y;
} keypos;

#ifdef KEYMAP_UK

keypos positions[] = {
{"mr", 38, 0}, {"m1", 50, 0}, {"m2", 62, 0}, {"m3", 74, 0}, {"light", 222, 0}, {"lock", 234, 0}, {"mute", 273, 0},
{"g1", 0, 14}, {"g2", 11, 14}, {"g3", 22, 14}, {"esc", 38, 14}, {"f1", 58, 14}, {"f2", 70, 14}, {"f3", 82, 14}, {"f4", 94, 14}, {"f5", 114, 14}, {"f6", 126, 14}, {"f7", 138, 14}, {"f8", 150, 14}, {"f9", 170, 14}, {"f10", 182, 14}, {"f11", 194, 14}, {"f12", 206, 14}, {"prtscn", 222, 14}, {"scroll", 234, 14}, {"pause", 246, 14}, {"stop", 262, 14}, {"prev", 273, 14}, {"play", 285, 14}, {"next", 296, 14},
{"g4", 0, 25}, {"g5", 11, 25}, {"g6", 22, 25}, {"grave", 38, 27}, {"1", 50, 27}, {"2", 62, 27}, {"3", 74, 27}, {"4", 86, 27}, {"5", 98, 27}, {"6", 110, 27}, {"7", 122, 27}, {"8", 134, 27}, {"9", 146, 27}, {"0", 158, 27}, {"minus", 170, 27}, {"equal", 182, 27}, {"bspace", 200, 27}, {"ins", 222, 27}, {"home", 234, 27}, {"pgup", 246, 27}, {"numlock", 261, 27}, {"numslash", 273, 27}, {"numstar", 285, 27}, {"numminus", 297, 27},
{"g7", 0, 39}, {"g8", 11, 39}, {"g9", 22, 39}, {"tab", 42, 39}, {"q", 56, 39}, {"w", 68, 39}, {"e", 80, 39}, {"r", 92, 39}, {"t", 104, 39}, {"y", 116, 39}, {"u", 128, 39}, {"i", 140, 39}, {"o", 152, 39}, {"p", 164, 39}, {"lbrace", 178, 39}, {"rbrace", 190, 39}, {"del", 222, 39}, {"end", 234, 39}, {"pgdn", 246, 39}, {"num7", 261, 39}, {"num8", 273, 39}, {"num9", 285, 39}, {"numplus", 297, 33},
{"g10", 0, 50}, {"g11", 11, 50}, {"g12", 22, 50}, {"caps", 44, 51}, {"a", 59, 51}, {"s", 71, 51}, {"d", 83, 51}, {"f", 95, 51}, {"g", 107, 51}, {"h", 119, 51}, {"j", 131, 51}, {"k", 143, 51}, {"l", 155, 51}, {"colon", 167, 51}, {"quote", 179, 51}, {"hash", 191, 51}, {"enter", 198, 51}, {"num4", 273, 51}, {"num5", 273, 51}, {"num6", 285, 51},
{"g13", 0, 64}, {"g14", 11, 64}, {"g15", 22, 64}, {"lshift", 46, 63}, {"bslash", 53, 63}, {"z", 65, 63}, {"x", 77, 63}, {"c", 89, 63}, {"v", 101, 63}, {"b", 113, 63}, {"n", 125, 63}, {"m", 137, 63}, {"comma", 149, 63}, {"dot", 161, 63}, {"slash", 173, 63}, {"rshift", 195, 63}, {"up", 234, 63}, {"num1", 261, 63}, {"num2", 273, 63}, {"num3", 285, 63}, {"numenter", 297, 69},
{"g16", 0, 75}, {"g17", 11, 75}, {"g18", 22, 75}, {"lctrl", 42, 75}, {"lwin", 56, 75}, {"lalt", 70, 75}, {"space", 122, 75}, {"ralt", 164, 75}, {"rwin", 178, 75}, {"rmenu", 190, 75}, {"rctrl", 202, 75}, {"left", 222, 75}, {"down", 234, 75}, {"right", 246, 75}, {"num0", 267, 75}, {"numdot", 285, 75},
};

#else

keypos positions[] = {
{"mr", 38, 0}, {"m1", 50, 0}, {"m2", 62, 0}, {"m3", 74, 0}, {"light", 222, 0}, {"lock", 234, 0}, {"mute", 273, 0},
{"g1", 0, 14}, {"g2", 11, 14}, {"g3", 22, 14}, {"esc", 38, 14}, {"f1", 58, 14}, {"f2", 70, 14}, {"f3", 82, 14}, {"f4", 94, 14}, {"f5", 114, 14}, {"f6", 126, 14}, {"f7", 138, 14}, {"f8", 150, 14}, {"f9", 170, 14}, {"f10", 182, 14}, {"f11", 194, 14}, {"f12", 206, 14}, {"prtscn", 222, 14}, {"scroll", 234, 14}, {"pause", 246, 14}, {"stop", 262, 14}, {"prev", 273, 14}, {"play", 285, 14}, {"next", 296, 14},
{"g4", 0, 25}, {"g5", 11, 25}, {"g6", 22, 25}, {"grave", 38, 27}, {"1", 50, 27}, {"2", 62, 27}, {"3", 74, 27}, {"4", 86, 27}, {"5", 98, 27}, {"6", 110, 27}, {"7", 122, 27}, {"8", 134, 27}, {"9", 146, 27}, {"0", 158, 27}, {"minus", 170, 27}, {"equal", 182, 27}, {"bspace", 200, 27}, {"ins", 222, 27}, {"home", 234, 27}, {"pgup", 246, 27}, {"numlock", 261, 27}, {"numslash", 273, 27}, {"numstar", 285, 27}, {"numminus", 297, 27},
{"g7", 0, 39}, {"g8", 11, 39}, {"g9", 22, 39}, {"tab", 42, 39}, {"q", 56, 39}, {"w", 68, 39}, {"e", 80, 39}, {"r", 92, 39}, {"t", 104, 39}, {"y", 116, 39}, {"u", 128, 39}, {"i", 140, 39}, {"o", 152, 39}, {"p", 164, 39}, {"lbrace", 178, 39}, {"rbrace", 190, 39}, {"bslash", 202, 39}, {"del", 222, 39}, {"end", 234, 39}, {"pgdn", 246, 39}, {"num7", 261, 39}, {"num8", 273, 39}, {"num9", 285, 39}, {"numplus", 297, 33},
{"g10", 0, 50}, {"g11", 11, 50}, {"g12", 22, 50}, {"caps", 44, 51}, {"a", 59, 51}, {"s", 71, 51}, {"d", 83, 51}, {"f", 95, 51}, {"g", 107, 51}, {"h", 119, 51}, {"j", 131, 51}, {"k", 143, 51}, {"l", 155, 51}, {"colon", 167, 51}, {"quote", 179, 51}, {"enter", 198, 51}, {"num4", 273, 51}, {"num5", 273, 51}, {"num6", 285, 51},
{"g13", 0, 64}, {"g14", 11, 64}, {"g15", 22, 64}, {"lshift", 46, 63}, {"z", 65, 63}, {"x", 77, 63}, {"c", 89, 63}, {"v", 101, 63}, {"b", 113, 63}, {"n", 125, 63}, {"m", 137, 63}, {"comma", 149, 63}, {"dot", 161, 63}, {"slash", 173, 63}, {"rshift", 195, 63}, {"up", 234, 63}, {"num1", 261, 63}, {"num2", 273, 63}, {"num3", 285, 63}, {"numenter", 297, 69},
{"g16", 0, 75}, {"g17", 11, 75}, {"g18", 22, 75}, {"lctrl", 42, 75}, {"lwin", 56, 75}, {"lalt", 70, 75}, {"space", 122, 75}, {"ralt", 164, 75}, {"rwin", 178, 75}, {"rmenu", 190, 75}, {"rctrl", 202, 75}, {"left", 222, 75}, {"down", 234, 75}, {"right", 246, 75}, {"num0", 267, 75}, {"numdot", 285, 75},
};

#endif

#define WIDTH 298
#define HEIGHT 76
#define N_POSITIONS (sizeof(positions)/sizeof(keypos))

//life

static const char *const evval[3] = {
    "RELEASED",
    "PRESSED ",
    "REPEATED"
};

/** A matrix representing neghbouring keys. The first number of a set is the key number, second is the number of neighbours and after that come 1-10 neighbouring key numbers. **/
typedef struct { 
	const char* name;
	int code;
	int n[12];
} neighbour;
neighbour sus[] = {
{"esc",1 ,7, 34, 46, 1, 13, 12, 11, 23 }, 
{"grave", 41, 6, 70, 106, 2, 13 }, 
{"tab", 15, 7, 106, 71, 3, 14, 13, 1, 70 }, 
{"caps", 58, 7, 71, 107, 4, 15, 14, 2, 106 }, 
{"lshift", 42, 8, 107, 143, 5, 17, 28, 15, 3, 71 }, 
{"lctrl", 29, 4, 143, 17, 4, 107 }, 
{"f12", 88, 4, 132, 31, 18, 54 }, 
{"equal", 13, 7, 133, 134, 90, 31, 132, 120, 108 }, 
{"lock", -1, 4, 137, 18, 30, 42 }, 
{"num7", 71, 7, 67, 57, 69, 21, 92, 80, 78 }, 
{"g1", -1, 3, 22, 46, 58 }, 
{"mr", -1, 3, 34 }, 
{"f1", 59, 6 }, 
{"1", 2, 5, 1, 2, 14, 25, 12 }, 
{"q", 16, 6, 2, 3, 15, 26, 25, 13 }, 
{"a", 30, 7, 3, 4, 28, 27, 26, 14, 2 }, 
{NULL }, 
{"lwin", 125, 4, 5, 29, 28, 4 }, 
{"prtscn", 99, 6, 6, 31, 54, 66, 30, 137 }, 
{NULL }, 
{"mute", 113, 3, 32, 44, 56 }, 
{"num8", 72, 8, 9, 57, 69, 81, 33, 104, 92, 80 }, 
{"g2", -1, 5, 10, 46, 58, 70, 34 }, 
{"m1", -1, 4, 11 }, 
{"f2", 60, 6, 12, 25, 37, 36, 47, 35 }, 
{"2", 3, 6, 13, 14, 26, 37, 24, 12 }, 
{"w", 17, 6, 14, 15, 27, 38, 37, 25 }, 
{"s", 31, 6, 15, 28, 40, 39, 38, 26 }, 
{"z", 44, 6, 4, 17, 29, 40, 27, 15 }, 
{"lalt", 56, 4, 17, 53, 40, 28 }, 
{"scroll", 70, 7, 18, 54, 66, 78, 42, 8, 137 }, 
{"bspace", 14, 9, 7, 90, 102, 43, 54, 18, 6, 132, 120 }, 
{"stop", 128, 6, 42, 78, 80, 92, 44, 20 }, 
{"num9", 73, 7, 21, 69, 81, 128, 116, 104, 92 }, 
{"g3", -1, 6, 22, 58, 70, 1 }, 
{"m2", -1, 4, 23, 12, 24, 47 }, 
{"f3", 61, 5, 24, 37, 49, 48, 47 }, 
{"3", 4, 6, 25, 26, 38, 49, 36, 24 }, 
{"e", 18, 6, 26, 27, 39, 50, 49, 37 }, 
{"d", 32, 6, 27, 40, 52, 51, 50, 38 }, 
{"x", 45, 6, 28, 29, 53, 52, 39, 27 }, 
{NULL }, 
{"pause", 119, 6, 30, 66, 78, 80, 32, 8 }, 
{"del", 111, 6, 102, 126, 55, 66, 54, 31 }, 
{"prev", 165, 6, 32, 80, 92, 104, 56, 20 }, 
{NULL }, 
{"g4", -1, 4, 82, 94, 58, 22, 10 }, 
{"m3", -1, 3, 35, 24, 36 }, 
{"f4", 62, 3, 36, 49, 61 }, 
{"4", 5, 6, 37, 38, 50, 61, 48, 36 }, 
{"r", 19, 6, 38, 39, 51, 62, 61, 49 }, 
{"f", 33, 6, 39, 52, 64, 63, 62, 50 }, 
{"c", 46, 5, 40, 53, 64, 51, 39 }, 
{"space", 57, 10, 29, 89, 124, 112, 100, 88, 76, 64, 52, 40 }, 
{"ins", 110, 8, 31, 102, 43, 55, 66, 30, 18, 6 }, 
{"end", 107, 5, 43, 67, 78, 66, 54 }, 
{"play", 164, 6, 44, 92, 104, 116, 68, 20 }, 
{"num4", 75, 5, 93, 105, 69, 21, 9 }, 
{"g5", -1, 8, 46, 82, 94, 106, 70, 34, 22, 10 }, 
{"g11", -1, 8, 118, 83, 95, 107, 71, 106, 94, 82 }, 
{"f5", 63, 3, 73, 85, 72 }, 
{"5", 6, 5, 49, 50, 62, 73, 48 }, 
{"t", 20, 6, 50, 51, 63, 74, 73, 61 }, 
{"g", 34, 6, 51, 64, 76, 75, 74, 62 }, 
{"v", 47, 5, 52, 53, 76, 63, 51 }, 
{NULL }, 
{"home", 102, 8, 54, 43, 55, 67, 78, 42, 30, 18 }, 
{"pgdn", 109, 6, 55, 57, 9, 80, 78, 66 }, 
{"next", 163, 3, 56, 104, 116 }, 
{"num5", 76, 8, 57, 93, 105, 117, 81, 33, 21, 9 }, 
{"g6", -1, 9, 70, 58, 94, 106, 2, 1 }, 
{"g12", -1, 8, 59, 95, 107, 4, 3, 2, 106, 94 }, 
{"f6", 64, 4, 60, 85, 97, 84 }, 
{"6", 7, 5, 61, 62, 28, 85, 60 }, 
{"y", 21, 6, 62, 63, 75, 86, 85, 73 }, 
{"h", 35, 6, 63, 76, 88, 87, 86, 74 }, 
{"b", 48, 5, 64, 53, 88, 75, 63 }, 
{NULL }, 
{"pgup", 104, 8, 66, 55, 67, 9, 80, 32, 42, 30 }, 
{"rshift", 54, 7, 136, 101, 113, 91, 115, 126, 135 }, 
{"numlock", 69, 8, 78, 67, 9, 21, 92, 32, 44, 42 }, 
{"num6", 77, 7, 69, 105, 117, 140, 128, 33, 21 }, 
{"g7", -1, 5, 118, 59, 94, 58, 46 }, 
{"g13", -1, 5, 119, 131, 95, 59, 118 }, 
{"f7", 65, 4, 72, 97, 109, 96 }, 
{"7", 8, 6, 73, 74, 86, 97, 72, 60 }, 
{"u", 22, 6, 74, 75, 87, 98, 97, 85 }, 
{"j", 36, 6, 75, 88, 100, 99, 98, 86 }, 
{"n", 49, 5, 76, 53, 100, 87, 75 }, 
{"ralt", 100, 5, 53, 101, 136, 124, 112 }, 
{"rbrace", 27, 6, 134, 135, 126, 102, 31, 7 }, 
{"rctrl", 97, 3, 113, 115, 79 }, 
{"numslash", 98, 8, 80, 9, 21, 33, 104, 56, 44, 32 }, 
{"num1", 79, 4, 129, 105, 69, 57 }, 
{"g8", -1, 8, 82, 118, 59, 71, 106, 70, 58, 46 }, 
{"g14", -1, 8, 83, 119, 131, 143, 107, 71, 59, 118 }, 
{"f8", 66, 3, 84, 109, 121 }, 
{"8", 9, 6, 85, 86, 98, 109, 84, 72 }, 
{"i", 23, 6, 86, 87, 99, 110, 109, 97 }, 
{"k", 37, 6, 87, 100, 112, 111, 110, 98 }, 
{"m", 50, 5, 88, 53, 112, 99, 87 }, 
{"rwin", 126, 4, 89, 113, 79, 136 }, 
{"bslash", 43, 5, 134, 126, 43, 54, 31 }, 
{"up", 103, 3, 115, 127, 139 }, 
{"numstar", 55, 8, 92, 21, 33, 128, 116, 68, 56, 44 }, 
{"num2", 80, 7, 93, 129, 141, 117, 81, 69, 57 }, 
{"g9", -1, 8, 94, 59, 71, 3, 2, 1, 70, 58 }, 
{"g15", -1, 8, 95, 131, 143, 5, 4, 3, 71, 59 }, 
{"f9", 67, 4, 121, 133, 7, 120 }, 
{"9", 10, 6, 97, 98, 110, 121, 96, 84 }, 
{"o", 24, 6, 98, 99, 111, 122, 121, 109 }, 
{"l", 38, 7, 99, 100, 112, 124, 123, 122, 110 }, 
{"comma", 51, 6, 100, 53, 89, 124, 111, 99 }, 
{"rmenu", 127, 3, 101, 91, 79 }, 
{NULL }, 
{"left", 105, 4, 91, 127, 103, 79 }, 
{"numminus", 74, 5, 104, 33, 128, 68, 56 }, 
{"num3", 81, 7, 105, 129, 141, 140, 128, 81, 69 }, 
{"g10", -1, 5, 83, 95, 59, 94, 82 }, 
{"g16", -1, 3, 131, 95, 83 }, 
{"f10", 68, 5, 108, 133, 7, 31, 132 }, 
{"0", 11, 6, 109, 110, 122, 133, 108, 96 }, 
{"p", 25, 6, 110, 111, 123, 134, 133, 121 }, 
{"colon", 39, 6, 111, 124, 136, 135, 134, 122 }, 
{"dot", 52, 6, 112, 53, 89, 136, 123, 111 }, 
{NULL }, 
{"enter", 28, 5, 135, 79, 43, 102, 90 }, 
{"down", 108, 3, 115, 139, 103 }, 
{"numplus", 78, 6, 33, 81, 117, 140, 116, 104 }, 
{"num0", 82, 5, 139, 141, 117, 105, 93 }, 
{NULL }, 
{"g17", -1, 5, 119, 143, 107, 95, 83 }, 
{"f11", 87, 4, 120, 7, 31, 6 }, 
{"minus", 12, 6, 121, 122, 134, 7, 120, 108 }, 
{"lbrace", 26, 6, 122, 123, 135, 90, 7, 133 }, 
{"quote", 40, 6, 123, 136, 79, 126, 90, 134 }, 
{"slash", 53, 6, 124, 29, 101, 79, 135, 123 }, 
{"light", -1, 4, 6, 18, 30, 8 }, 
{NULL }, 
{"right", 106, 4, 127, 129, 93, 103 }, 
{"numenter", 96, 4, 117, 141, 128, 81 }, 
{"numdot", 83, 4, 129, 105, 117, 140 },
{NULL }, 
{"g18", -1, 5, 131, 5, 4, 107, 95 }, 
	};


int reader() {
	static int firstrun = 1;
	int i;
	char c[30], d[2];
	static int fd;
	struct input_event ev;
	ssize_t n;
	if (firstrun == -1) return -1;
	if (firstrun) {					//open the keyboard input file for reading
		printf("Scanning for keyboard input dir, press some random key repeatedly please. \n");
		for (i=0; i<10; i++) {			// checks if files /dev/input/event0-10 are the keyboard
			close(fd);			// input file, and if they are starts to read from them
			sprintf(c, "/dev/input/event%d", i);
 			// debug printf("\n%s\n", c);
			fd = open(c, O_RDONLY);
			int flags = fcntl(fd, F_GETFL, 0);	//instruct the read function not to wait
			fcntl(fd, F_SETFL, flags | O_NONBLOCK);	//for input, just to read
			if (fd == -1) { printf("Cannot open %s. Please run the program as root or give everyone read permissions on it. \n", c); } else {
				usleep(1116667);
				n = read(fd, &ev, sizeof ev);
				if (ev.type == EV_KEY) break; 
				};
			};
		if (i == 10) { firstrun = -1; return -1; };
		firstrun = 0;
		printf("\n%s is the keyboard input directory. \nLife has Begun. \n", c);
		};

	int flags = fcntl(fd, F_GETFL, 0);		
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);    	
	n = read(fd, &ev, sizeof ev);

	// if the event type is a key and if value is pressed, find the key number and return it
	if (ev.type == EV_KEY && ev.value >= 0 && ev.value == 1) {
			// debug printf("%s %i (%d)\n", evval[ev.value], (int)ev.code, (int)ev.code); 
			for (i=0; i<N_KEYS; i++) if ( ev.code == sus[i].code ) break;
			return i;
		} else	return -1;  
}
	
	
void mainloop_life(float fr, float fg, float fb, float br, float bg, float bb){
	static char og[N_KEYS], tog[N_KEYS], ng[N_KEYS];
    	static float r[N_KEYS], g[N_KEYS], b[N_KEYS], rn, gn, bn;
    	static float rspeed, gspeed, bspeed;
    	static int firstrun = 1;
    	static int frame = 0;
	int pressed, count, k;
	srand(time(NULL));
	
	//on first run make the first generation randomly
	if(firstrun) for(int i = 0; i < N_KEYS; i++) og[i] = ng[i] = rand() % 2;

    	// On first run, fill colors for first generation
    	if(firstrun){ 
        	srand(time(NULL));
        	for(int i = 0; i < N_KEYS; i++){
			if (og[i]) {
				r[i] = fr;
            			g[i] = fg;
            			b[i] = fb;
				} else {
				r[i] = br;
            			g[i] = bg;
            			b[i] = bb;
				}
        		}
		//pick itteration speeds
		rspeed = (fr - br) / 20.f;
        	gspeed = (fg - bg) / 20.f;
        	bspeed = (fb - bb) / 20.f;
        	firstrun = 0;
    		}

	//on frame 0 generate a new generation, store it in ng[] nad set itteration speeds
	if (frame == 50) for(int i = 0; i < N_KEYS; i++) og[i] = ng[i];
	if (frame == 50) for(int i = 0; i < N_KEYS; i++){
			count=0;	//live neighbours count
			if ( sus[i].name == NULL ) continue;
			for (int j = 0; j < sus[i].n[0]; j++) if (og[ sus[i].n[1 + j] ]) count++;
			if (og[i] == 1 && count < 2) 	ng[i] = 0;
			if (og[i] == 1 && (count == 3 || count == 2 ))  ng[i] = 1;
			if (og[i] == 1 && count > 3) 	ng[i] = 0;
			if (og[i] == 0 && count == 3) 	ng[i] = 1;
			}
			
	//on other frames, itterate colors
	if (frame > 50) for(int i = 0; i < N_KEYS; i++){
			if ((og[i] == 1) && (ng[i] == 0)) {
							r[i] -= rspeed;
							g[i] -= gspeed;
							b[i] -= bspeed;
							}
			if ((og[i] == 0) && (ng[i] == 1)) {
							rn = r[i] += rspeed;
							gn = g[i] += gspeed;
							bn = b[i] += bspeed;
							}
			if ((og[i] == 0) && (ng[i] == 0)) {
							r[i] = br;
            						g[i] = bg;
            						b[i] = bb;
							}
			if ((og[i] == 1) && (ng[i] == 1)) {
							r[i] = fr;
            						g[i] = fg;
            						b[i] = fb;
							}
			}
			
	//check if keys where pressed	
	pressed = reader();
	if (pressed > 0) { 
		ng[pressed] = 1;
		og[pressed] = 1;
		if (frame != 0) {  
			r[pressed] = rn;
			g[pressed] = gn;
			b[pressed] = bn;
			};
				  
		};
		
	// Update and output the keys
	fprintf(output, "rgb ");
	for(int i = 0; i < N_KEYS; i++) if ( sus[i].name != NULL ) fprintf(output, " %s:%02x%02x%02x", sus[i].name, (int)(r[i]), (int)(g[i]), (int)(b[i])); 
	frame = (frame + 1) % 70;
	fputc('\n', output);
	fflush(output);
}


void mainloop_random(float fr, float fg, float fb, float br, float bg, float bb){
    static float r[N_KEYS], g[N_KEYS], b[N_KEYS];
    static float rspeed[N_KEYS], gspeed[N_KEYS], bspeed[N_KEYS];
    static int firstrun = 1;
    static int frame = 0;
    // On first run, fill all colors randomly
    if(firstrun){
        srand(time(NULL));
        for(int i = 0; i < N_KEYS; i++){
            r[i] = rand() % 256;
            g[i] = rand() % 256;
            b[i] = rand() % 256;
        }
        firstrun = 0;
    }
    // On frame 0, pick a new set of colors to cycle to and choose the appropriate speeds to get there by frame 120
    if(frame == 0){
        for(int i = 0; i < N_KEYS; i++){
            float r2 = rand() % 256;
            float g2 = rand() % 256;
            float b2 = rand() % 256;
            rspeed[i] = (r2 - r[i]) / 120.f;
            gspeed[i] = (g2 - g[i]) / 120.f;
            bspeed[i] = (b2 - b[i]) / 120.f;
        }
    }
    // Update and output the keys
    fprintf(output, "rgb on ");
    for(int i = 0; i < N_KEYS; i++){
        r[i] += rspeed[i];
        g[i] += gspeed[i];
        b[i] += bspeed[i];
        fprintf(output, " #%d:%02x%02x%02x", i, (int)(r[i]), (int)(g[i]), (int)(b[i]));
    }
    fputc('\n', output);
    fflush(output);
    frame = (frame + 1) % 60;
}


void mainloop_wave(float fr, float fg, float fb, float br, float bg, float bb){
    float size = WIDTH + 36.f;
    static float wavepos = -36.f;
    fprintf(output, "rgb on ");
    for(int i = 0; i < N_POSITIONS; i++){
        float r = br;
        float g = bg;
        float b = bb;
        float distance = fabs(positions[i].x - wavepos);
        if(distance <= 36.f){
            r = r * distance / 36.f + fr * (1.f - distance / 36.f);
            g = g * distance / 36.f + fg * (1.f - distance / 36.f);
            b = b * distance / 36.f + fb * (1.f - distance / 36.f);
        }
        fprintf(output, " %s:%02x%02x%02x", positions[i].name, (int)r, (int)g, (int)b);
    }
    fputc('\n', output);
    fflush(output);
    wavepos += (size + 36.f) / 2.f / 60.f;
    if(wavepos >= size)
        wavepos = -36.f;
}

void mainloop_ripple(float fr, float fg, float fb, float br, float bg, float bb){
    float size = sqrt(WIDTH*WIDTH/2. + HEIGHT*HEIGHT/2.);
    float cx = WIDTH / 2.f, cy = HEIGHT / 2.f;
    static float ringpos = -36.f;
    fprintf(output, "rgb on");
    for(int i = 0; i < N_POSITIONS; i++){
        float r = br;
        float g = bg;
        float b = bb;
        float distance = fabs(sqrt(pow(positions[i].x - cx, 2.) + pow(positions[i].y - cy, 2.)) - ringpos);
        if(distance <= 36.f){
            r = r * distance / 36.f + fr * (1.f - distance / 36.f);
            g = g * distance / 36.f + fg * (1.f - distance / 36.f);
            b = b * distance / 36.f + fb * (1.f - distance / 36.f);
        }
        fprintf(output, " %s:%02x%02x%02x", positions[i].name, (int)r, (int)g, (int)b);
    }
    fputc('\n', output);
    fflush(output);
    ringpos += (size + 36.f) / 60.f;
    if(ringpos >= size)
        ringpos = -36.f;
}

void mainloop_gradient(float fr, float fg, float fb, float br, float bg, float bb){
    static float grad = 1.f;
    int r = fr * grad + br * (1.f - grad);
    int g = fg * grad + bg * (1.f - grad);
    int b = fb * grad + bb * (1.f - grad);
    fprintf(output, "rgb on %02x%02x%02x\n", r, g, b);
    fflush(output);
    if(grad == 0.f)
        exit(0);
    grad -= 1.f / 2.f / 60.f;
    if(grad < 0.f)
        grad = 0.f;
}

void mainloop_solid(float fr, float fg, float fb, float br, float bg, float bb){
    fprintf(output, "rgb on %02x%02x%02x\n", (int)fr, (int)fg, (int)fb);
    fflush(output);
    exit(0);
}

int readcolor(const char* clr){
    if(!strcmp(clr, "red"))
        return 0xff0000;
    if(!strcmp(clr, "green"))
        return 0x00ff00;
    if(!strcmp(clr, "blue"))
        return 0x0000ff;
    if(!strcmp(clr, "cyan"))
        return 0x00ffff;
    if(!strcmp(clr, "magenta"))
        return 0xff00ff;
    if(!strcmp(clr, "yellow"))
        return 0xffff00;
    if(!strcmp(clr, "white"))
        return 0xffffff;
    if(!strcmp(clr, "black"))
        return 0x000000;
    int rgb;
    if(sscanf(clr, "%6x", &rgb) == 1)
        return rgb;
    return 0;
}

int main(int argc, char** argv){
    if(argc < 2){
        printf("Usage: ckb (solid | gradient | ripple | wave | random) [foreground] [background]\n");
        exit(0);
    }
    void (*mainloop)(float,float,float,float,float,float);
    if(!strcmp(argv[1], "solid"))
        mainloop = mainloop_solid;
    else if(!strcmp(argv[1], "gradient"))
        mainloop = mainloop_gradient;
    else if(!strcmp(argv[1], "ripple"))
        mainloop = mainloop_ripple;
    else if(!strcmp(argv[1], "wave"))
        mainloop = mainloop_wave;
    else if(!strcmp(argv[1], "random"))
        mainloop = mainloop_random;
    else if(!strcmp(argv[1], "life"))
        mainloop = mainloop_life;
    else {
        printf("Usage: ckb (solid | gradient | ripple | wave | random) [foreground] [background]\n");
        exit(0);
    }

#ifdef __linux
    output = fopen("/dev/input/ckb1/cmd", "w");
#else
    output = fopen("/tmp/ckb1/cmd", "w");
#endif
    if(!output){
        printf("Unable to open input device\n");
        exit(-1);
    }
    int foreground = 0xffffff, background = 0xffffff;
    if(argc >= 3)
        foreground = background = readcolor(argv[2]);
    if(argc >= 4)
        background = readcolor(argv[3]);
    float fr = (foreground >> 16) & 0xff, fg = (foreground >> 8) & 0xff, fb = foreground & 0xff;
    float br = (background >> 16) & 0xff, bg = (background >> 8) & 0xff, bb = background & 0xff;

    while(1){
        mainloop(fr, fg, fb, br, bg, bb);
        usleep(16667);
    }
    return 0;
}
