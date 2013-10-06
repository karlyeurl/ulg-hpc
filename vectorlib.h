/**
 * This file contains the interface for a basic vector manipulation library.
 */

#ifndef VECTORLIB_ULG
#define VECTORLIB_ULG

struct vector_t {
   long double x;
   long double y;
   long double z;
};

typedef struct vector_t vector;

/**
 * Adds two vectors.
 *
 * @param v1 The first vector. Order doesn't matter.
 *
 * @param v2 The second vector. Order doesn't matter.
 * 
 * @return A vector containing the result.
 */
vector addv(vector v1, vector v2);

/**
 * Subtracts two vectors.
 *
 * @param v1 The first vector. Order doesn't matter.
 *
 * @param v2 The second vector. Order doesn't matter.
 * 
 * @return A vector containing the result.
 */
vector subv(vector v1, vector v2);

/**
 * Multiplies a vector by a scalar.
 *
 * @param v1 The vector.
 *
 * @param scale The scalar.
 * 
 * @return A vector containing the result.
 */
vector scalev(vector v, long double scale);

/**
 * Scalar product of two vectors.
 *
 * @param v1 The first vector. Order doesn't matter.
 *
 * @param v2 The second vector. Order doesn't matter.
 * 
 * @return A long double containing the result.
 */
long double productv(vector v1, vector v2);

/**
 * Cross product of two vectors. Order matters!
 *
 * @param v1 The first vector.
 *
 * @param v2 The second vector.
 * 
 * @return A vector containing the result.
 */
vector xproductv(vector v1, vector v2);

/**
 * Computes the norm of a vector.
 *
 * @param v The vector.
 * 
 * @return The norm of the vector.
 */
long double normv(vector v);

/**
 * Prints a vector to the stdout.
 *
 * @param v The vector.
 * 
 * @return Nothing.
 */
void printv(vector v);
#endif
