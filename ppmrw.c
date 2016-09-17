#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Struct that will hold the pixel values from the ppm file
typedef struct{
	unsigned char r,g,b;

}pixel;
//Struct that holds the ppm files data
typedef struct{
	char magic_num;
	int width;
	int height;
	int max_val;
	pixel *pixels;

}ppm_data;

//function that reads the ppm file, and checks whether its p3 or p6 and acts accordingly.
void read_file(char* file_name, ppm_data* pdata);

//function that reads the struct passed into it and writes it into a ppm file.
void write_file(char* destination, ppm_data data);


int main(){
ppm_data data;
ppm_data* p3_data;
read_file("P3.ppm", p3_data);
write_file("p3wrote.ppm", data);
}



void read_file(char* file_name, ppm_data* pdata){
FILE* fh;
int temp;
int i = 0;
char buffer[256];
//opens the file with input filename
fh = fopen(file_name, "rb");

//fails to open
if(!fh){
	fprintf(stderr, "Error: unable to open file given\n");
	exit(1);
}
//scan in the magic number and store it in the struct
fscanf(fh, "%s", buffer);
strcpy(&pdata->magic_num, buffer);

//checks magic number if P3 this executes
if(strcmp(buffer, "P3") ==0){
	//allocates memory for the struct ppm data
	pdata = (ppm_data*)malloc(sizeof(ppm_data));
	
	temp = fgetc(fh);
	
	//skips comments in the ppm file
	fscanf(fh, "%s", buffer);
	//checks for the # comment symbol
	while(buffer[0] == '#'){
		fgets(buffer, 255, fh);
		fscanf(fh, "%s", buffer);
	}
	//scans in the width height and maxval for the struct and converts it to int and stored
	sscanf(buffer, "%d", &pdata->width);
	fscanf(fh, "%d", &pdata->height);
	fscanf(fh, "%d", &pdata->max_val);
	
	//gets the pixel data for the first pixel and prints it to check if correct.
	fscanf(fh, "%d", &temp);
	pdata->pixels[0].r = temp;
	printf("%d \n", temp);
	
	//loops through the data to get each pixels value
	for(i =0; i< (pdata->width * pdata->height);i++){

		fscanf(fh, "%d", &temp);
		pdata->pixels[i].r = temp;

		fscanf(fh, "%d", &temp);
		pdata->pixels[i].g = temp;

		fscanf(fh, "%d", &temp);
		pdata->pixels[i].b = temp;



	}
	fclose(fh);

}

//checks if the magic number is p6
else if(strcmp(buffer, "P6") ==0){
	//creates the memory space to store the files data in the struct
	pdata = (ppm_data*)malloc(sizeof(ppm_data));
	temp =getc(fh);
	//skips the comments in the ppm file
	while(temp== '#') {
		while(getc(fh) != '\n');
			temp = getc(fh);
	}
	//scans in the width and height and makes sure size is valid
	if(fscanf(fh, "%d %d", &pdata->width, &pdata->height) !=2){
		fprintf(stderr, "Error: Invalid size");
		exit(1);
	}
	
	//gets the max value and checks to make sure its correct
	fscanf(fh, "%d", &pdata->max_val);
	if(pdata->max_val != 255){
		fprintf(stderr, "ERROR: does not have 8 bit components\n");
	}
	//allocates space for the pixels
	pdata->pixels = (pixel*)malloc(pdata->width* pdata->height * sizeof(pixel));
	
	//loops to after newline
	while (fgetc(fh) != '\n');

	//reads the pixeldata into the struct
	if(fread(pdata->pixels, 3 * pdata->width, pdata->height, fh) != pdata->height){
		fprintf(stderr, "Error loading image data");
		exit(1);
	}
	fclose(fh);
		
//if neither p3 or p6 entered then an error is thrown
}else{fprintf(stderr, "Error: Invalid file type, must be P3 or P6");}


}

void write_file(char* destination, ppm_data data){
	
//opens the destination folder specified to write
FILE* fh = fopen(destination, "wb");
if(!fh) {
	fprintf(stderr, "Unable to open file");
	exit(1);
}
//checks whether its p3 or p6 and adds it to the file accordingly
if(strcmp(&data.magic_num, "P3") == 0){
	fprintf(fh, "P3\n");
}
else if(strcmp(&data.magic_num, "P6") == 0){
	fprintf(fh,"P6\n");
	
//if it is not p3 or p6 error is thrown
}else{fprintf(stderr, "Error: Invalid PPM Type(magic number)");

//writes the data width/ height to the file with newline
fprintf(fh, "%d %d\n", data.width, data.height);

fprintf(fh, "%d\n", data.max_val);

//writes the pixeldata from the struct to the file
fwrite(data.pixels, 3 * data.width, data.height, fh);
fclose(fh);
}


}

