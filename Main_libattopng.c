#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "libattopng.h"

#define RGBA(r, g, b, a) ((r) | ((g) << 8) | ((b) << 16) | ((a) << 24))
#define RGB(r, g, b) RGBA(r, g, b, 0xff)


////////////Global constants ///////////////////////////////

//Filename of the output image
char pngname[100]="paramO2.png";

//Image size
int	pointsx=1000;
int	pointsy=1000; 

//Coordinate limits
double	R1=-55;
double	R2=85;
double	I1=-70;
double	I2=70;

//Maximum amount of iterates
int	maxit=150; 

//Convergence/divergence test. Close to infinity means bigger than esc. Close to 0 means smaller than 1/esc
double esc=10000;

//Number of critical points of the function
int	ncrit=2; 

////////Functions//////////

	//This function creates a palette of colors 
	//The first 201 colors are a scaling gray->blue->green->red
	//19 extra colors are left for other uses (mainly different convergence behaviours between critical points)
int** createcolors(void){
	int i;
	int** col = (int**)calloc(220, sizeof(int*));
	FILE *colormap;	
	for (i = 0; i < 220; i++)  
		col[i] = (int*)calloc(3, sizeof(int));
		
   colormap = fopen ("colourmap.map", "r");
   for(i=0;i<220;i++){
   			 fscanf(colormap, "%d %d %d", &col[i][0], &col[i][1], &col[i][2]);
    } 
   fclose(colormap);
	return col;
}

//Definition of the critical points of the method. crit is the vector of critical points. a is the parameter
void defcriticalpoints(complex* crit, complex a){ 
	complex auxsqrt;
	auxsqrt=csqrt(5*a*(4+a)) ;         
	crit[0]=(4+a-auxsqrt-csqrt(10*a*(6-a)-2*(4+a)*auxsqrt))/(4*(a-1));
	crit[1]=(4+a+auxsqrt-csqrt(10*a*(6-a)+2*(4+a)*auxsqrt))/(4*(a-1));
	//More critical points can be added below
	return;
}

//Definition of the iterating function. z is the variable. a is the parameter
complex evaluateFunction(complex z, complex a){ 
	complex z2, z3, z4;
	z2=z*z;   
	z3=z2*z;
	z4=z3*z;
	return (z4*(1-a+4*z+6*z2+4*z3+z4))/(1+4*z+6*z2+4*z3+(1-a)*z4);
}


/////////////////////////////////////////

int main(int argc, char *argv[]) {
	int y, x, i, j,k, col, controlescape, speedconv;
	double  eps, scalecol;
	complex z,  a;
	int** palette;
  	complex* crit=(complex*)calloc(ncrit, sizeof(complex));
	eps=1./esc;
	
	
	//the scale of colors is adapted to the maximum number of iterates given
	scalecol=200./maxit;  
  	//scaling of colors
	palette= createcolors();
	libattopng_t *png  = libattopng_new(pointsx, pointsy, PNG_RGB);
   

	//double for loop for the parameter a=x+iy
	for (x = 0; x < pointsx; x++) { 
		printf("%d\n",x);
		for (y = 0; y < pointsy; y++) { 	
		
        	a=x*(R2-R1)/pointsx+R1+ I*(y*(I1-I2)/pointsy+I2);//the (0,0) pixel is the upper left corner. This draws from the bottom up
        	
        	defcriticalpoints(crit, a); 
			
			controlescape=0;//counts the number of critical orbits escaping to the roots
			speedconv=0; //If all critical orbits converge to the roots this number will give the greatest convergence time among all the critical points
			
			for(k=0;k<ncrit;k++){
			
				z=crit[k];
				for(i=0; i<maxit;i++){
					z=evaluateFunction(z,a);
               		
				  	if(cabs(z)>esc||cabs(z)<eps){ //If the critical point escapes to the roots (0 or infinity)...
				    		if(speedconv<i) speedconv=i; //this is a greatest convergence time among all the critical points escaping to the roots
				    		controlescape++;
				    		break;				
				    }
				}
			}
			
			if(controlescape==ncrit){ //If all the critical points escape to the roots...
				col=200-speedconv*scalecol;
				libattopng_set_pixel(png, x, y, RGB(palette[col][0], palette[col][1], palette[col][2]));
			}else{//A different color is given according to the number of escaping critical points
				col=201+controlescape;//extra colors start at 201. If controlescape=0 we plot in black
				libattopng_set_pixel(png, x, y, RGB(palette[col][0], palette[col][1], palette[col][2]));
			}
        }
    }
    
    libattopng_save(png, pngname);

	//Next commands free all memory
    libattopng_destroy(png);
    for(i=0;i<220;i++) free(palette[i]);
    free(palette); 
	free(crit);   

    return 0;
}



