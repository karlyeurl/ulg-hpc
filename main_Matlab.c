#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vectorlib.h"
#include "main.h"

int main(int argc, char* argv[]){

   if(argc < 3)
      error_exit("Syntax error : Incorrect arguments, use : input_file output_file [-v] [-m filename].");

   boolean use_verbatim = FALSE;
   FILE* matlab_file = NULL;

   FILE* inputFile = fopen(argv[1],"r");
   FILE* outputFile = fopen(argv[2],"w+");

   if( inputFile == NULL || outputFile == NULL)
      error_exit("Runtime error : could not open required i/o files.");
   
   for(int i=3 ; i < argc; i++){
      if( argv[i][0] != '-' )
         error_exit("Syntax error : Unknown parameter : %s",argv[i]);

      switch(argv[i][1]){
         case 'v' :
            use_verbatim = TRUE;
            break;
         case 'm' :
            if(i+1 >= argc)
               error_exit("Syntax error : Filename required with -m option");
            
            matlab_file = fopen(argv[i+1], "w+");

            if(matlab_file == NULL)
               error_exit("Runtime error : Cannot open file '%s' for matlab output.",argv[i+1]);

            i++;
            break;
      }
   }
   
   // positions and speed at time n-1 (0) and n (1).
   vector p10,p20,v10,v20,p11,p21;
   // masses
   double m1,m2;
   // times
   double dt, t_final, dt_output;

   fscanf(inputFile, "%lf\r\n%lf %lf %lf\r\n%lf %lf %lf\r\n%lf\r\n%lf %lf %lf\r\n%lf %lf %lf\r\n%lf\r\n%lf\r\n%lf", &m1, &(p10.x), &(p10.y), &(p10.z), &(v10.x), &(v10.y), &(v10.z), &m2, &(p20.x), &(p20.y), &(p20.z), &(v20.x), &(v20.y), &(v20.z), &dt, &t_final, &dt_output );

   // Initial values determination
   p11 = addv(scalev(v10, dt), p10);
   p21 = addv(scalev(v20, dt), p20);
   
   if(use_verbatim){
      printf("Read parameters\r\n-------------\r\n");
      printf("m1 \t %lf\r\n p1 X \t %lf\r\n p1 Y \t %lf\r\n p1 Z  \t %lf\r\n p1 U  \t %lf\r\n p1 V  \t %lf\r\n p1 W \t %lf\r\n m2 \t %lf\r\n p2 X \t %lf\r\n p2 Y \t %lf\r\n p2 Z \t %lf\r\n p2 U \t %lf\r\n p2 V \t %lf\r\n p2 W \t %lf\r\n dt \t %lf\r\n endT \t %lf\r\n outT \t %lf\r\n", m1, p10.x, p10.y, p10.z, v10.x, v10.y, v10.z, m2, p20.x, p20.y, p20.z, v20.x, v20.y, v20.z, dt,t_final,dt_output);
      printf("\r\nInitial conditions\r\n------------\r\n");
      printf(" p1 X(1) \t %f \r\n p1 Y(1)) \t %f \r\n p1 Z(1)) \t %f \r\n p2 X(1)) \t %f \r\n p2 Y(1) \t %f \r\n p2 Z(1)) \t %f \r\n", p11.x, p11.y, p11.z, p21.x, p21.y, p21.z);
      printf("\r\nComputed Data\r\n-------------\r\n");
   }
   
   double next_out = dt_output;
   for(double t = dt * 2 ; t < t_final ; t+= dt ){
      vector dp = subv(p21,p11);
      double norm = normv(dp);
      
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
         fprintf(outputFile, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\r\n", t, p11.x, p11.y, p11.z, p21.x,p21.y, p21.z);
         if(use_verbatim) printf("%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\r\n", t, p11.x, p11.y, p11.z, p21.x,p21.y, p21.z);
         if(matlab_file != NULL) fprintf(matlab_file, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\r\n", t, p11.x, p11.y, p11.z, p21.x,p21.y, p21.z);
         next_out +=dt_output;
      }
   }
   
   fclose(inputFile);
   fclose(outputFile);
   
   return EXIT_SUCCESS;
}
