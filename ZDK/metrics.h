
#pragma once

/*
 * Here we declare our space-time things, vectors, objects, etc.
 */

typedef struct {
    float x, y;
} Vector;

/*
 * Construct Vector out of X and Y.
 */
Vector v(float, float);

/*
 * Simulate timeflow for some "base" value from its derivative ("diff").
 *
 * integrate(dt, v, x) = x + dt * v
 *
 * Add a `diff` scaled by `delta_time` to `base`.
 */
Vector integrate(int delta_time, Vector diff, Vector base);

/*
 * Make sure that modulus (length) of the vector doesn't exceed limit.
 *
 * Used for speed limiting.
 */
Vector limitByModulus(Vector, float);

/*
 * We model object up to second derivative of its position
 */
typedef struct {
    Vector pos, vel, acc;
} Object;

/*
 * Construct object out of position, velocity and acceleration.
 */
Object mkObject(Vector pos, Vector vel, Vector acc);

/*
 * Construct stationary object at given position.
 */
Object mkStationaryObject(Vector pos);

/*
 * Add "delta" vector to the "obj"ect position.
 */
Object moveObj(Vector delta, Object obj);

/*
 * Simulate timeflow on object.
 */
Object tick(float d_time, Object obj);

/*
 * Change sign of given vector component.
 */
Vector flipY(Vector vec);
Vector flipX(Vector vec);

/*
 * Flip X or Y component of object velocity.
 */
Object bounceOffVerticalWall(Object obj);
Object bounceOffHorisontalWall(Object obj);

/*
 * Length of the vector.
 */
float modulus(Vector);

/*
 * Some useful arithmetics on vectors.
 */
Vector minusV(Vector from, Vector to);
Vector plusV(Vector from, Vector to);
Vector scaleV(Vector from, float mutliplier);

/*
 * For debug.
 */
void print_vector(Vector v);
void print_object(Object v);
