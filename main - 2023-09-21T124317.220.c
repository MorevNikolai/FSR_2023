#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lodepng.h"

char* Downloadpng(const char*inputfile, int*w, int*h) {
  unsigned char*picture = NULL;
  int ERROR = lodepng_decode32_file(&picture, w, h, inputfile);
  if(ERROR){
    printf("ERROR %u: %s\n", ERROR, lodepng_error_text(ERROR));
  }
  return (picture);
}

void print(const char*inputfile, const unsigned char*image, unsigned width, unsigned height) {
  unsigned char*png;
  long unsigned int pngsize;

  int error = lodepng_encode32(&png, &pngsize, image, width, height);
  if(!error) {
    lodepng_save_file(png, pngsize, inputfile);
  }
  
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
  
  free(png);
  
}

void brightness(int w, int h,unsigned char*p,unsigned char *picture1){
    for(int i=0;i<w*h;i++){
         
    picture1[i]=0.2*p[4*i]+0.7*p[4*i+1]+0.4*p[4*i+2];
      
    }
    return;
}

void b_w(int w,int h,unsigned char*picture1){
    for(int i=2;i<h-1;i++){
        for(int j=2;j<w-1;j++){
          
            if(picture1[w*i+j]<60){
                picture1[w*i+j]=0;
            }
            if(picture1[w*i+j]>180){
                picture1[w*i+j]=255;
            }
        }
    }
    return;
}

void color(int w, int h,unsigned char*picture2,unsigned char*picture4){
    for(int i=0;i<w*h;i++) {
        picture4[i*4] = 270;
        picture4[i*4+1] = 1*picture2[i];
        picture4[i*4+2] = 1.5*picture2[i];
        picture4[i*4+3] = 255;
    }
    return;
}


void Gauss(int w,int h,unsigned char*picture1, unsigned char*picture2){
     for(int i = 2;i < h-1 ;i++){
        for(int j = 2;j < w-1 ;j++){
           picture2[w*i+j]=
              0.09*picture1[w*(i+1)+(j-1)]
             +0.121*picture1[w*(i+1)+j]
             +0.012*picture1[w*(i+1)+(j+1)]
             +0.092*picture1[w*i+(j-1)]
             +0.146*picture1[w*i+j]
             +0.092*picture1[w*i+(j+1)]
             +0.121*picture1[w*(i-1)+(j-1)]
             +0.192*picture1[w*(i-1)+j]
             +0.192*picture1[w*(i-1)+(j+1)];
        }
     }
   return;
}

int main() {
    char*inputfile = "skull.png";
    int w,h;
    char*p = Downloadpng(inputfile, &w, &h);
    if (p == NULL){
        printf("I can't read %s. Error.\n", inputfile);
        return -1;
    }
    unsigned char *picture1=(unsigned char*)malloc(h*w*sizeof(unsigned char));
    unsigned char *picture2=(unsigned char*)malloc(h*w*sizeof(unsigned char));
    unsigned char *picture4=(unsigned char*)malloc(h*w*4*sizeof(unsigned char));
    b_w(w,h,picture1);
    brightness(w,h,p,picture1);
    Gauss(w,h,picture1,picture2);
    color(w,h,picture2,picture4);
    char*new_image="skull-after-test.png";
    print(new_image,picture4,w,h);
    return 0;
}