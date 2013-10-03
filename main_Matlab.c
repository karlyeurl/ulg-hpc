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

   FILE* input_file = fopen(argv[1],"r");
   FILE* output_file = fopen(argv[2],"w+");

   if( input_file == NULL || output_file == NULL)
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
   vector p10,p20,v1,v2,p11,p21;
   // masses
   double m1,m2;
   // times
   double dt, t_final, dt_output;

   fscanf(input_file, "%lf\r\n%lf %lf %lf\r\n%lf %lf %lf\r\n%lf\r\n%lf %lf %lf\r\n%lf %lf %lf\r\n%lf\r\n%lf\r\n%lf", &m1, &(p10.x), &(p10.y), &(p10.z), &(v1.x), &(v1.y), &(v1.z), &m2, &(p20.x), &(p20.y), &(p20.z), &(v2.x), &(v2.y), &(v2.z), &dt, &t_final, &dt_output );

   // Initial values determination
   p11 = addv(scalev(v1, dt), p10);
   p21 = addv(scalev(v2, dt), p20); 
   
   write_init(output_file, matlab_file, use_verbatim, m1, p10, v1, m2, p20, v2, dt, t_final, dt_output);

   write_output(output_file, matlab_file, use_verbatim, 0, p10, p20);
   if(dt_output <= dt + DOUBLE_EPSILON) write_output(output_file, matlab_file, use_verbatim, dt, p11, p21);
   
   double next_out = dt_output;
 
   for(double t = dt * 2 ; t <= t_final ; t+= dt ){
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
      
      if(t + dt - DOUBLE_EPSILON >= next_out){
         write_output(output_file, matlab_file, use_verbatim, t, p11, p21);
         next_out +=dt_output;
      }
   }
   
   write_end(output_file, matlab_file, use_verbatim);
   fclose(input_file);
   fclose(output_file);
   
   return EXIT_SUCCESS;
}

void write_init(FILE* out1, FILE* out2, boolean verb, double m1, vector p1, vector v1, double m2, vector p2, vector v2, double dt, double t_final, double dt_output){
   if(verb){
      printf("Read parameters\r\n-------------\r\n");
      printf("m1 \t %lf\r\n p1 X \t %lf\r\n p1 Y \t %lf\r\n p1 Z  \t %lf\r\n p1 U  \t %lf\r\n p1 V  \t %lf\r\n p1 W \t %lf\r\n m2 \t %lf\r\n p2 X \t %lf\r\n p2 Y \t %lf\r\n p2 Z \t %lf\r\n p2 U \t %lf\r\n p2 V \t %lf\r\n p2 W \t %lf\r\n dt \t %lf\r\n endT \t %lf\r\n outT \t %lf\r\n", m1, p1.x, p1.y, p1.z, v1.x, v1.y, v1.z, m2, p2.x, p2.y, p2.z, v2.x, v2.y, v2.z, dt,t_final,dt_output);
      printf("\r\nInitial conditions\r\n------------\r\n");
      printf(" p1 X(1) \t %f \r\n p1 Y(1) \t %f \r\n p1 Z(1) \t %f \r\n p2 X(1) \t %f \r\n p2 Y(1) \t %f \r\n p2 Z(1) \t %f \r\n", p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
      printf("\r\nComputed Data\r\n-------------\r\n");
   }
   if(out2 != NULL){
      fprintf(out2, "% Matlab simulation script\r\n\r\nt_end = %lf;\r\ndt_computation = %lf;\r\ndt_sample = %lf;\r\n\r\nm1 = %lf; m2 = %lf;\r\n\r\nsamples = [\r\n", t_final, dt, dt_output, m1, m2);
   }
}

void write_end(FILE* out1, FILE* out2, boolean verb){
   if(out2 != NULL){
      fseek(out2, -3, SEEK_CUR);
      fprintf(out2, "];\r\n\r\n");
      fprintf(out2, "figure; hold on; grid on;\r\naxis([ min(min(samples(:,2)),min(samples(:,5))), max(max(samples(:,2)),max(samples(:,5))), min(min(samples(:,3)),min(samples(:,6))), max(max(samples(:,3)),max(samples(:,6))), min(min(samples(:,4)),min(samples(:,7))), max(max(samples(:,4)),max(samples(:,7))) ]);\r\nfor sample = transpose(samples)\r\n   plot3(sample(2), sample(3), sample(4), 'rd:');\r\n   plot3(sample(5), sample(6), sample(7), 'bd:');\r\n   pause(0.05);\r\nend\r\n");
   }
}

void write_output(FILE* out1, FILE* out2, boolean verb, double t, vector p1, vector p2){
   fprintf(out1, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\r\n", t, p1.x, p1.y, p1.z, p2.x,p2.y, p2.z);
   if(verb) printf("%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\r\n", t, p1.x, p1.y, p1.z, p2.x,p2.y, p2.z);
   if(out2 != NULL) fprintf(out2, "%lf,%lf,%lf,%lf,%lf,%lf,%lf;\r\n", t, p1.x, p1.y, p1.z, p2.x,p2.y, p2.z);
}
