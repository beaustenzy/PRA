/* File:     PRA.c
 *
 * Author:   Beau Stensgard
 *
 * Purpose:  Simulation of the Page Replacement Algorithms
 *
 * Compile:  gcc -o PRA PRA.c -lm
 *
 * Run:      ./PRA textfile minimumframes maximumframes pages
 *
 * Note: 
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
void fifo(int numFrames, int* numPages, int pages[]) {
	int faults = 0, i, a, b;
	int found;
	int temp[numFrames];
	
    for(i = 0; i < numFrames; i++) {
        temp[i] = -1;
    }
	int c = 0;
	for(a = 0; a < *numPages; a++) {
        found = 0;
        for(b = 0; b < numFrames; b++) {
            if(pages[a] == temp[b]) {
                found = 1;
            }
        }
		if (found == 0)
			faults++;
        if((faults <= numFrames) && (found == 0)) {
            temp[c] = pages[a];
			c++;
        }   
        else if(found == 0) {
            temp[(faults - 1) % numFrames] = pages[a];
        }
        //for(b = 0; b < numFrames; b++) {     
            //printf("%d\t", temp[b]);
        //}
		//printf("\n");
    } 
    printf("%d faults\n", faults);
}
void lru(int numFrames, int* numPages, int pages[]) {
	int faults = 0, i, a, b, c, d, e = 0;;
	int temp[numFrames];
	for (i = 0; i < numFrames; i++) {
        temp[i] = -1;
    }
	for (a = 0; a < *numPages; a++) {
		int flag = 0;
		for(b=0;b<numFrames;b++) {
			if (pages[a]==temp[b]) {
				flag = 1; 
				break;
			}
		}
		if(flag == 0) {
			faults++;
			if (e < numFrames) { 
				temp[e++] = pages[a];
			} 
			else {
				int index, prev, leastuse = -32767;
				for (c = 0; c < numFrames; c++) {
					prev = a;
					for (d = a - 1; d >= 0; d--) {
						if(pages[d] == temp[c])
						{
							prev = d;
							break;
						}
					}
					if (a - prev > leastuse) {
						index = c;
						leastuse = a - prev;
					}
				}
				temp[index] = pages[a];
			}
		//for (int z = 0; z < numFrames; z++) {     
        //    printf("%d\t", temp[z]);
        //}
		//printf("\n");   
		}
		//else {
		//	for(int z = 0; z < numFrames; z++) {     
        //    printf("%d\t", temp[z]);
		//	}
		//printf("\n");   
		//}

   }
   printf("%d faults\n", faults);
}
void opt(int numFrames, int* numPages, int pages[]) {
	int faults = 0, i, a, b, c, flag1, flag2, flag3, position, max;
	int temp[numFrames];
	int temp2[numFrames];
    for(i = 0; i < numFrames; i++) {
        temp[i] = -1;
    }
	for(a = 0; a < *numPages; ++a) {
		flag1 = flag2 = 0;

		for(b = 0; b < numFrames; ++b) {
			if(temp[b] == pages[a]) {
				flag1 = flag2 = 1;
				break;
			}
		}
		if(flag1 == 0) {
			for(b = 0; b < numFrames; ++b) {
				if(temp[b] == -1) {
					faults++;
					temp[b] = pages[a];
					flag2 = 1;
					break;
				}
			}
		}
		if(flag2 == 0) {
			flag3 = 0;
			for(b = 0; b < numFrames; ++b) {
				temp2[b] = -1;

				for(c = a + 1; c < *numPages; ++c) {
					if(temp[b] == pages[c]) {
						temp2[b] = c;
						break;
					}
				}
			}
			for(b = 0; b < numFrames; ++b) {
				if(temp2[b] == -1) {
					position = b;
					flag3 = 1;
					break;
				}
			}
			if(flag3 == 0) {
				max = temp2[0];
				position = 0;

				for(b = 1; b < numFrames; ++b){ 
					if(temp2[b] > max) {
						max = temp2[b];
						position = b;
					}
				}
			}
			temp[position] = pages[a];
			faults++;
		}
		//printf("\n");
		//for(b = 0; b < numFrames; ++b) {
		//	printf("%d\t", temp[b]);
		//}
	}
	//printf("\n"); 	
    printf("%d faults\n", faults);
}
int main(int argc, char* argv[]) {
	char* str = argv[1];
	int i;
	int numFramesMin = atoi(argv[2]);
	int numFramesMax = atoi(argv[3]);
	int numPages = atoi(argv[4]);
	FILE *myFile;
	myFile = fopen(str, "r");
    int pages[numPages];
	
    if (myFile == NULL) {
        printf("Could not read text file.\n");
        exit (0);
    }
	
    for (i = 0; i < numPages; i++) 
        fscanf(myFile, "%d,", &pages[i] );
    fclose(myFile);
	
	printf("====== First-In-First-Out  =======\n");
	for (int i = numFramesMin; i <= numFramesMax; i++) {
		printf("For frame %d - ", i);
		fifo(i,&numPages,pages);
	}
	printf("====== Least Recently Used =======\n");
	for (int i = numFramesMin; i <= numFramesMax; i++) {
		printf("For frame %d - ", i);
        lru(i,&numPages,pages);
	}
	printf("=========== Optimal ==============\n");
	for (int i = numFramesMin; i <= numFramesMax; i++) {
		printf("For frame %d - ", i);
        opt(i,&numPages,pages);
	}
    return 0;
}
