/**
 * Implementation of a basic vector manipulation library for the course
 * INFO-0939 during the academic year 2013-2014.
 *
 * Copyright 2013 Damien Smeets and Pierre Lorent.
 *
 * This code is licensed under the WTFPL (http://sam.zoy.org/wtfpl/).
**/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vectorlib.h"

vector addv(vector v1, vector v2) {
   vector v;
   v.x = v1.x + v2.x;
   v.y = v1.y + v2.y;
   v.z = v1.z + v2.z;
   return v;
}

vector subv(vector v1, vector v2) {
   vector v;
   v.x = v1.x - v2.x;
   v.y = v1.y - v2.y;
   v.z = v1.z - v2.z;
   return v;
}

vector scalev(vector v, long double scale) {
   vector w;
   w.x = v.x * scale;
   w.y = v.y * scale;
   w.z = v.z * scale;
   return w;
}

long double normv(vector v) {
   long double norm = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
   return norm;
}

long double productv(vector v1, vector v2) {
   long double prod = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
   return prod;
}

vector xproductv(vector v1, vector v2) {
   vector v;
   /* Sarrus' rule */
   v.x = v1.y * v2.z - v1.z * v2.y;
   v.y = v1.z * v2.x - v1.x * v2.z;
   v.z = v1.x * v2.y - v1.y * v2.x;
   return v;
}

void printv(vector v) {
   printf("%Lf %Lf %Lf", v.x, v.y, v.z);
}
