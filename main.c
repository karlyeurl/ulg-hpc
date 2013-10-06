#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vectorlib.h"
#include "main.h"

int main(int argc, char* argv[]){

   if(argc < 3){
      printf("Incorrect input, use args : [input file] [output file]");
      return EXIT_FAILURE;
   }

   boolean verbatim = FALSE;

   if( argc > 3 && strcmp(argv[1],"-v")){
	verbatim = TRUE;
   }
   
   FILE* inputFile = fopen(argv[1],"r");
   FILE* outputFile = fopen(argv[2],"w+");

   if( inputFile == NULL || outputFile == NULL){
      printf("Error : could not open required files.");
      return EXIT_FAILURE;
   }
   
   // positions and speed at time n-1 (0) and n (1).
   vector p10,p20,v10,v20,p11,p21;
   // masses
   long double m1,m2;
   // times
   long double dt, t_final, dt_output;

   fscanf(inputFile, "%Lf\r\n%Lf %Lf %Lf\r\n%Lf %Lf %Lf\r\n%Lf\r\n%Lf %Lf %Lf\r\n%Lf %Lf %Lf\r\n%Lf\r\n%Lf\r\n%Lf", &m1, &(p10.x), &(p10.y), &(p10.z), &(v10.x), &(v10.y), &(v10.z), &m2, &(p20.x), &(p20.y), &(p20.z), &(v20.x), &(v20.y), &(v20.z), &dt, &t_final, &dt_output );
   
   if(verbatim){
      printf("Read parameters\r\n-------------\r\n");
      printf("m1 \t %Lf\r\n p1 X \t %Lf\r\n p1 Y \t %Lf\r\n p1 Z  \t %Lf\r\n p1 U  \t %Lf\r\n p1 V  \t %Lf\r\n p1 W \t %Lf\r\n m2 \t %Lf\r\n p2 X \t %Lf\r\n p2 Y \t %Lf\r\n p2 Z \t %Lf\r\n p2 U \t %Lf\r\n p2 V \t %Lf\r\n p2 W \t %Lf\r\n dt \t %Lf\r\n endT \t %Lf\r\n outT \t %Lf\r\n", m1, p10.x, p10.y, p10.z, v10.x, v10.y, v10.z, m2, p20.x, p20.y, p20.z, v20.x, v20.y, v20.z, dt,t_final,dt_output);
   }

   // Initial values determination
   p11 = addv(scalev(v10, dt), p10);
   p21 = addv(scalev(v20, dt), p20);
   
   if(verbatim){
      printf("\r\nInitial conditions\r\n------------\r\n");
      printf(" p1 X(1) \t %f \r\n p1 Y(1)) \t %f \r\n p1 Z(1)) \t %f \r\n p2 X(1)) \t %f \r\n p2 Y(1) \t %f \r\n p2 Z(1)) \t %f \r\n", p11.x, p11.y, p11.z, p21.x, p21.y, p21.z);
      printf("\r\nComputed Data\r\n-------------\r\n");
   }
   
   long double next_out = dt_output;
   for(long double t = dt * 2 ; t < t_final ; t+= dt ){
      vector dp = subv(p21,p11);
      long double norm = normv(dp);
      
      // compute F
      vector f = scalev(dp, - m1 * m2 * CONST_G / norm * norm * norm); 
      
      // compute positions at time t
      vector new_p1 = addv(subv(scalev(f, - dt * dt / m1), p10) , scalev(p11, 2));
      vector new_p2 = addv(subv(scalev(f, dt * dt / m2), p20) , scalev(p21, 2));
      
      p10 = p11;
      p20 = p21;
      p11 = new_p1;
      p21 = new_p2;
      
      if(next_out <= t + 0.00001){
         fprintf(outputFile, "%Lf\t%Lf\t%Lf\t%Lf\t%Lf\t%Lf\t%Lf\r\n", t, p11.x, p11.y, p11.z, p21.x,p21.y, p21.z);
         if(verbatim) printf("%Lf\t%Lf\t%Lf\t%Lf\t%Lf\t%Lf\t%Lf\r\n", t, p11.x, p11.y, p11.z, p21.x,p21.y, p21.z);
         next_out +=dt_output;
      }
   }
   
   fclose(inputFile);
   fclose(outputFile);
   
   return EXIT_SUCCESS;
}

