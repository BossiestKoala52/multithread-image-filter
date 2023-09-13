/**
* (basic description of the program or class)
*
* Completion time: (estimation of hours spent on this program)
*
* @author (your name), (anyone else, e.g., Acuna, whose code you used)
* @version (a version number or a date)
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include "BmpProcessor.h"
#include "PixelProcessor.h"

//UNCOMMENT BELOW LINE IF USING SER334 LIBRARY/OBJECT FOR BMP SUPPORT
//#include "BmpProcessor.h"


////////////////////////////////////////////////////////////////////////////////
//MACRO DEFINITIONS

//problem assumptions
#define BMP_HEADER_SIZE 14
#define BMP_DIB_HEADER_SIZE 40
#define MAXIMUM_IMAGE_SIZE 4096
#define THREAD_COUNT 4

////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES
typedef struct BMP_Header BMP_Header;
typedef struct DIB_Header DIB_Header;
typedef struct Pixel Pixel;
typedef struct holeData{
    int y, x, radius;
}holeData;
typedef struct filterData {
    int startCol, endCol, height;
    int threadNum;
    int numHoles;
    struct Pixel** pArr;
    struct holeData** holeDataArr;
} filterData;


struct holeData** globalHoleData;
struct Pixel** globalImage;

////////////////////////////////////////////////////////////////////////////////
//MAIN PROGRAM CODE
void* blur(void* param) {
    unsigned char red, green, blue;
    int startCol, endCol, height, width;

    filterData* filterData1 = (filterData*) param;
    startCol = filterData1->startCol;
    endCol = filterData1->endCol;
    height = filterData1->height;
    printf("Thread number: %d\n", filterData1->threadNum);
    printf("startCol: %d, endCol: %d\n\n", startCol, endCol);



    for (int i = 0; i < height; i++) {
        for (int j = startCol; j < endCol; j++) {
            filterData1->pArr[i][j].red = globalImage[i][j].red;
            filterData1->pArr[i][j].green = globalImage[i][j].green;
            filterData1->pArr[i][j].blue = globalImage[i][j].blue;
        }
    }

    for (int i = 0; i < height; i++) {
            for (int j = startCol; j < endCol; j++) {
                red = 0;
                green = 0;
                blue = 0;

                if(i != 0 && j != startCol && (i+1) != height && (j+1) != endCol){ //3x3
                    red = (filterData1->pArr[i][j].red + filterData1->pArr[i-1][j-1].red + filterData1->pArr[i-1][j].red + filterData1->pArr[i-1][j+1].red + filterData1->pArr[i][j-1].red + filterData1->pArr[i][j+1].red + filterData1->pArr[i+1][j-1].red + filterData1->pArr[i+1][j].red + filterData1->pArr[i+1][j+1].red) / 9;
                    green = (filterData1->pArr[i][j].green + filterData1->pArr[i-1][j-1].green + filterData1->pArr[i-1][j].green + filterData1->pArr[i-1][j+1].green + filterData1->pArr[i][j-1].green + filterData1->pArr[i][j+1].green + filterData1->pArr[i+1][j-1].green + filterData1->pArr[i+1][j].green + filterData1->pArr[i+1][j+1].green) / 9;
                    blue = (filterData1->pArr[i][j].blue + filterData1->pArr[i-1][j-1].blue + filterData1->pArr[i-1][j].blue + filterData1->pArr[i-1][j+1].blue + filterData1->pArr[i][j-1].blue + filterData1->pArr[i][j+1].blue + filterData1->pArr[i+1][j-1].blue + filterData1->pArr[i+1][j].blue + filterData1->pArr[i+1][j+1].blue) / 9;

                }
                if(i == 0 && j != startCol && (j+1) != endCol){ //2x3 top edge
                    red = (filterData1->pArr[i][j].red + filterData1->pArr[i+1][j].red + filterData1->pArr[i][j-1].red + filterData1->pArr[i][j+1].red + filterData1->pArr[i+1][j+1].red + filterData1->pArr[i+1][j-1].red) / 6;
                    green = (filterData1->pArr[i][j].green + filterData1->pArr[i+1][j].green + filterData1->pArr[i][j-1].green + filterData1->pArr[i][j+1].green + filterData1->pArr[i+1][j+1].green + filterData1->pArr[i+1][j-1].green) / 6;
                    blue = (filterData1->pArr[i][j].blue + filterData1->pArr[i+1][j].blue + filterData1->pArr[i][j-1].blue + filterData1->pArr[i][j+1].blue + filterData1->pArr[i+1][j+1].blue + filterData1->pArr[i+1][j-1].blue) / 6;
                }
                if(i == height - 1 && j != startCol && (j+1) != endCol){ //2x3 bottom edge
                    red = (filterData1->pArr[i][j].red + filterData1->pArr[i-1][j].red + filterData1->pArr[i][j-1].red + filterData1->pArr[i][j+1].red + filterData1->pArr[i-1][j+1].red + filterData1->pArr[i-1][j-1].red) / 6;
                    green = (filterData1->pArr[i][j].green + filterData1->pArr[i-1][j].green + filterData1->pArr[i][j-1].green + filterData1->pArr[i][j+1].green + filterData1->pArr[i-1][j+1].green + filterData1->pArr[i-1][j-1].green) / 6;
                    blue = (filterData1->pArr[i][j].blue + filterData1->pArr[i-1][j].blue + filterData1->pArr[i][j-1].blue + filterData1->pArr[i][j+1].blue + filterData1->pArr[i-1][j+1].blue + filterData1->pArr[i-1][j-1].blue) / 6;
                }
                if(i != 0 && j == startCol && (i+1) != height){ //2x3 left edge
                    red = (filterData1->pArr[i][j].red + filterData1->pArr[i-1][j].red + filterData1->pArr[i+1][j].red + filterData1->pArr[i][j+1].red + filterData1->pArr[i-1][j+1].red + filterData1->pArr[i+1][j+1].red) / 6;
                    green = (filterData1->pArr[i][j].green + filterData1->pArr[i-1][j].green + filterData1->pArr[i+1][j].green + filterData1->pArr[i][j+1].green + filterData1->pArr[i-1][j+1].green + filterData1->pArr[i+1][j+1].green) / 6;
                    blue = (filterData1->pArr[i][j].blue + filterData1->pArr[i-1][j].blue + filterData1->pArr[i+1][j].blue + filterData1->pArr[i][j+1].blue + filterData1->pArr[i-1][j+1].blue + filterData1->pArr[i+1][j+1].blue) / 6;
                }
                if(i != 0 && j == endCol - 1 && (i+1) != height){ //2x3 right edge
                    red = (filterData1->pArr[i][j].red + filterData1->pArr[i-1][j-1].red + filterData1->pArr[i-1][j].red + filterData1->pArr[i][j-1].red + filterData1->pArr[i+1][j-1].red + filterData1->pArr[i+1][j].red)  / 6;
                    green = (filterData1->pArr[i][j].green + filterData1->pArr[i-1][j-1].green + filterData1->pArr[i-1][j].green + filterData1->pArr[i][j-1].green + filterData1->pArr[i+1][j-1].green + filterData1->pArr[i+1][j].green ) / 6;
                    blue = (filterData1->pArr[i][j].blue + filterData1->pArr[i-1][j-1].blue + filterData1->pArr[i-1][j].blue +  filterData1->pArr[i][j-1].blue + filterData1->pArr[i+1][j-1].blue + filterData1->pArr[i+1][j].blue) / 6;
                }
                if(i == 0 && j == startCol){ //2x2 left top corner
                    red = (filterData1->pArr[i][j].red + filterData1->pArr[i+1][j].red + filterData1->pArr[i][j+1].red + filterData1->pArr[i+1][j+1].red) / 4;
                    green = (filterData1->pArr[i][j].green + filterData1->pArr[i+1][j].green + filterData1->pArr[i][j+1].green + filterData1->pArr[i+1][j+1].green) / 4;
                    blue = (filterData1->pArr[i][j].blue +  filterData1->pArr[i+1][j].blue + filterData1->pArr[i][j+1].blue + filterData1->pArr[i+1][j+1].blue) / 4;
                }
                if(i == height - 1 && j == startCol){ //2x2 left bottom corner
                    red = (filterData1->pArr[i][j].red + filterData1->pArr[i-1][j].red + filterData1->pArr[i][j+1].red + filterData1->pArr[i-1][j+1].red) / 4;
                    green = (filterData1->pArr[i][j].green + filterData1->pArr[i-1][j].green + filterData1->pArr[i][j+1].green + filterData1->pArr[i-1][j+1].green) / 4;
                    blue = (filterData1->pArr[i][j].blue +  filterData1->pArr[i-1][j].blue + filterData1->pArr[i][j+1].blue + filterData1->pArr[i-1][j+1].blue) / 4;
                }
                if(i == 0 && j == endCol - 1){ //2x2 right top corner
                    red = (filterData1->pArr[i][j].red + filterData1->pArr[i][j-1].red + filterData1->pArr[i+1][j-1].red + filterData1->pArr[i+1][j].red)  / 4;
                    green = (filterData1->pArr[i][j].green + filterData1->pArr[i][j-1].green + filterData1->pArr[i+1][j-1].green + filterData1->pArr[i+1][j].green ) / 4;
                    blue = (filterData1->pArr[i][j].blue + filterData1->pArr[i][j-1].blue + filterData1->pArr[i+1][j-1].blue + filterData1->pArr[i+1][j].blue) / 4;
                }
                if(i == height - 1 && j == endCol - 1){ //2x2 right bottom corner
                    red = (filterData1->pArr[i][j].red + filterData1->pArr[i][j-1].red + filterData1->pArr[i-1][j-1].red + filterData1->pArr[i-1][j].red)  / 4;
                    green = (filterData1->pArr[i][j].green + filterData1->pArr[i][j-1].green + filterData1->pArr[i-1][j-1].green + filterData1->pArr[i-1][j].green ) / 4;
                    blue = (filterData1->pArr[i][j].blue + filterData1->pArr[i][j-1].blue + filterData1->pArr[i-1][j-1].blue + filterData1->pArr[i-1][j].blue) / 4;
                }
                globalImage[i][j].red = red;
                globalImage[i][j].green = green;
                globalImage[i][j].blue = blue;
            }
        }
        return 0;
    }
void colorShiftPixels(struct Pixel** pArr, int width, int height, int rShift, int gShift, int bShift){
    for(int i=0; i<height; i++) {
        for (int j = 0; j < width; j++) {
            if( (pArr[i][j].blue + bShift) > 255){
                pArr[i][j].blue = 255;
            }
            else if( (pArr[i][j].blue + bShift) < 0){
                pArr[i][j].blue = 0;
            }
            else{
                pArr[i][j].blue += bShift;
            }
            if( (pArr[i][j].green + gShift) > 255){
                pArr[i][j].green = 255;
            }
            else if((pArr[i][j].green + gShift) < 0){
                pArr[i][j].green = 0;
            }
            else{
                pArr[i][j].green += gShift;
            }
            if( (pArr[i][j].red + rShift) > 255){
                pArr[i][j].red = 255;
            }
            else if( (pArr[i][j].red + rShift) < 0){
                pArr[i][j].red = 0;
            }
            else{
                pArr[i][j].red += rShift;
            }
        }
    }
}

void* swissCheese(void* param){
    int startCol, endCol, height, width, x, y, radius, numHoles;

    filterData* cheeseData1 = (filterData*) param;
    startCol = cheeseData1->startCol;
    endCol = cheeseData1->endCol;
    height = cheeseData1->height;
    numHoles = cheeseData1->numHoles;

    printf("Thread number: %d\n", cheeseData1->threadNum);
    printf("startCol: %d, endCol: %d\n\n", startCol, endCol);

    for(int k = 0; k<numHoles;k++) {
        x = cheeseData1->holeDataArr[k]->x;
        y = cheeseData1->holeDataArr[k]->y;
        radius = cheeseData1->holeDataArr[k]->radius;
        for (int i = 0; i < height; i++) {
            for (int j = startCol; j < endCol; j++) {
                double d = sqrt(((i - y)*(i - y)) + ((j-x)*(j-x)));
                if(d < radius){
                    globalImage[i][j].red = 0;
                    globalImage[i][j].green = 0;
                    globalImage[i][j].blue = 0;
                }
            }
        }
    }

}

double randNormalDist(int width){
    return (int)(width * 0.1) + 5*((sqrt(-2 * log(((rand() + 1.0)/(RAND_MAX+1.0)))) * cos(2*M_PI*(rand() + 1.0)/(RAND_MAX+1.0))));
}
int randNum(){
   return rand();
}


int main(int argc, char* argv[]) {
    char* fileNameIn = "";
    char* fileNameOut = "default";
    char* filter;

    int c = 0;
    opterr = 0;
    while((c = getopt(argc, argv, "i:o:f:")) != -1){
        switch(c)
        {
            case'i':
                fileNameIn = optarg;
            case 'o':
                fileNameOut = optarg;
                break;
            case 'f':
                filter = optarg;
                if(strcmp(optarg, "b") != 0 && strcmp(optarg, "c") != 0){
                    printf("Not a valid filter option. 'c' for swiss cheese filter or 'b' for blur filter");
                    return 1;
                }
                break;
            case '?':
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                return 1;
            default:
                abort();
        }
    }
    struct BMP_Header* bmpHeader = (BMP_Header*)malloc(sizeof(BMP_Header));
    struct DIB_Header* dibHeader = (DIB_Header*)malloc(sizeof(DIB_Header));


    FILE* file_input = fopen(fileNameIn, "rb");

    readBMPHeader(file_input, bmpHeader);
    readDIBHeader(file_input, dibHeader);

    globalImage = (struct Pixel **) malloc(sizeof(struct Pixel *) * dibHeader->width);


    readPixelsBMP(file_input, globalImage, dibHeader->width, dibHeader->height);
    fclose(file_input);

if(strcmp(filter, "b") == 0) {
    filterData *filterData1 = (struct filterData*)malloc(sizeof(struct filterData) * THREAD_COUNT);
    pthread_t tids[THREAD_COUNT];
    int colWidth = dibHeader->width / THREAD_COUNT;

    for (int i = 0; i < THREAD_COUNT; i++) {
        filterData1[i].height = dibHeader->height;
        filterData1[i].threadNum = i;
        filterData1[i].startCol = i * colWidth;
        if (filterData1[i].startCol > 0) {
            filterData1[i].startCol -= 1;
        }
        filterData1[i].endCol = ((i + 1) * colWidth);
        filterData1[i].pArr = (struct Pixel **) malloc(sizeof(struct Pixel *) * dibHeader->height);
        for (int j = 0; j < dibHeader->height; j++) {
            filterData1[i].pArr[j] = malloc(sizeof(struct Pixel) * (dibHeader->width));
        }
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        //pthread_attr_init(&attr[i]);
        pthread_create(&tids[i], NULL, blur, &filterData1[i]);
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(tids[i], NULL);
    }
    free(filterData1);
}
else if(strcmp(filter, "c") == 0){
    int numHoles1 = (int)dibHeader->width * 0.1;
    globalHoleData = (struct holeData **) malloc(sizeof(struct holeData *) * numHoles1);
    filterData *filterData1 = (struct filterData*) malloc(sizeof(struct filterData) * THREAD_COUNT);


int radius = (int)randNormalDist(dibHeader->width);
printf("numholes: %d\n sample radius: %d\n", numHoles1, radius);
    srand(time(0));
    for(int i = 0; i<numHoles1; i++){
        globalHoleData[i] = malloc(sizeof (struct holeData));

        globalHoleData[i]->y = randNum() % (dibHeader->height + 1);
        globalHoleData[i]->x = randNum() % (dibHeader->width + 1);
        globalHoleData[i]->radius = (int)randNormalDist(dibHeader->width);

    }
    colorShiftPixels(globalImage, dibHeader->width, dibHeader->height, 50, 50, 0);
    pthread_t tids[THREAD_COUNT];
    int colWidth = dibHeader->width / THREAD_COUNT;

    for (int i = 0; i < THREAD_COUNT; i++) {
        filterData1[i].height = dibHeader->height;
        filterData1[i].threadNum = i;
        filterData1[i].startCol = i * colWidth;


        filterData1[i].endCol = ((i + 1)  * colWidth);
        filterData1[i].holeDataArr = (struct holeData **) malloc(sizeof(struct holeData*) * numHoles1);
        filterData1[i].numHoles = 0;
        for (int j = 0; j < numHoles1; j++) {
           if(((globalHoleData[j]->radius + globalHoleData[j]->x) >= filterData1->startCol && globalHoleData[j]->x <= filterData1->endCol) || ((globalHoleData[j]->radius - globalHoleData[j]->x) <= filterData1->endCol && globalHoleData[j]->x >= filterData1->startCol)){
               filterData1[i].holeDataArr[j] = globalHoleData[j];
               filterData1[i].numHoles++;
           }
        }
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_create(&tids[i], NULL, swissCheese, &filterData1[i]);
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(tids[i], NULL);
    }
    free(filterData1);
}

    FILE* file_output = fopen(fileNameOut, "wb");

    writeBMPHeader(file_output, bmpHeader);
    writeDIBHeader(file_output, dibHeader);
    writePixelsBMP(file_output, globalImage, dibHeader->width, dibHeader->height);
    fclose(file_output);
    free(dibHeader);
    free(bmpHeader);
    free(globalImage);
    free(globalHoleData);


}