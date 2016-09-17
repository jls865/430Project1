#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct{
	unsigned char r,g,b;

}pixel;

typedef struct{
	unsigned char magic_num;
	int width;
	int height;
	int max_val;
	pixel *pixels;

}ppm_data;


void read_file(char* file_name, ppm_data* pdata);
void write_file(char* destination, ppm_data data);



int main(){
ppm_data data;
strcpy(&data.magic_num, "P3");
data.width = 2;
data.height= 2;
data.max_val = 255;
ppm_data* p3_data;
read_file("P3.ppm", p3_data);
write_file("p3wrote.ppm", data);
}


//fgetc and atoi
void read_file(char* file_name, ppm_data* pdata){
FILE* fh;
int temp;
int i = 0;
char buffer[256];
fh = fopen(file_name, "rb");
if(!fh){
	fprintf(stderr, "Error: unable to open file given\n");
	exit(1);
}
fscanf(fh, "%s", buffer);
strcpy(&pdata->magic_num, buffer);

if(strcmp(buffer, "P3") ==0){

	pdata = (ppm_data*)malloc(sizeof(ppm_data));
	fscanf(fh, "%s", buffer);
	while(buffer[0] == '#'){
		fgets(buffer, 255, fh);
		fscanf(fh, "%s", buffer);
	}

	sscanf(buffer, "%d", &pdata->width);
	fscanf(fh, "%d", &pdata->height);
	fscanf(fh, "%d", &pdata->max_val);
	

	fscanf(fh, "%d", &temp);
	pdata->pixels[0].r = temp;
	printf("%d \n", temp);
	for(i =0; i< (pdata->width * pdata->height);i++){

		fscanf(fh, "%d", &temp);
		pdata->pixels[i].r = temp;

		fscanf(fh, "%d", &temp);
		pdata->pixels[i].g = temp;

		fscanf(fh, "%d", &temp);
		pdata->pixels[i].b = temp;



	}

}


else if(strcmp(buffer, "P6") ==0){
	printf("hello");
	pdata = malloc(sizeof(ppm_data));
	temp =getc(fh);
	while(temp== '#') {
		while(getc(fh) != '\n');
			temp = getc(fh);
	}
	ungetc(temp,fh);
	if(fscanf(fh, "%d %d", &pdata->width, &pdata->height) !=2){
		fprintf(stderr, "Error: Invalid size");
		exit(1);
	}
	
	fscanf(fh, "%d", &pdata->max_val);
	if(pdata->max_val != 255){
		fprintf(stderr, "ERROR: does not have 8 bit components\n");
	}
	
	while (fgetc(fh) != '\n');
	pdata->pixels = (pixel*)malloc(pdata->width* pdata->height * sizeof(pixel));
	
	if(fread(pdata->pixels, 3 * pdata->width, pdata->height, fh) != pdata->height){
		fprintf(stderr, "Error loading image data");
		exit(1);
	}
	fclose(fh);
		

}else{fprintf(stderr, "Error: Invalid file type, must be P3 or P6");}


}

//fprintf
void write_file(char* destination, ppm_data data){

FILE* fh = fopen(destination, "wb");
if(!fh) {
	fprintf(stderr, "Unable to open file");
	exit(1);
}

if(strcmp(&data.magic_num, "P3") == 0){
	fprintf(fh, "P3\n");
}
else if(strcmp(&data.magic_num, "P6") == 0){
	fprintf(fh,"P6\n");
}else{fprintf(stderr, "Error: Invalid PPM Type(magic number)");

fprintf(fh, "%d %d\n", data.width, data.height);

fprintf(fh, "%d\n", data.max_val);

fwrite(data.pixels, 3 * data.width, data.height, fh);
fclose(fh);
}


}

