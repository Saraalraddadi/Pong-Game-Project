
#include <stdio.h>
#include <math.h>

#include "metrics.h"
#include "utils.h"

static float LIGHTSPEED = 1;

Vector v(float x, float y) {
    return (Vector) { x = x, y = y };
}

Vector integrate(int delta_time, Vector diff, Vector base) {
    return v(
        delta_time * diff.x + base.x,
        delta_time * diff.y + base.y
    );
}

/*
 * If vector length > required, return (vec / (length / required)).
 * This will produce vector of exact required length or less.
 */
Vector limitByModulus(Vector vec, float mod) {
    float now = modulus(vec);

    if (now > mod)
        return scaleV(vec, mod / now);

    return vec;
}

Object mkObject(Vector pos, Vector vel, Vector acc) {
    return (Object) { pos = pos, vel = vel, acc = acc };
}

Object mkStationaryObject(Vector pos) {
    return mkObject(pos, v(0, 0), v(0, 0));
}

/*
 * We update object position and velocity here.
 * We also make sure velocity doesn't exceed LIGHTSPEED.
 */
Object tick(float d_time, Object obj) {
    obj.pos = integrate(d_time, obj.vel, obj.pos);
    obj.vel = limitByModulus(integrate(d_time, obj.acc, obj.vel), LIGHTSPEED);
    return obj;
}

Object moveObj(Vector delta, Object obj) {
    obj.pos = plusV(delta, obj.pos);
    return obj;
}

Vector flipY(Vector vec) {
    return v(vec.x, -vec.y);
}

Vector flipX(Vector vec) {
    return v(-vec.x, vec.y);
}

Object bounceOffVerticalWall(Object obj) {
    obj.vel = flipX(obj.vel);
    return obj;
}

Object bounceOffHorisontalWall(Object obj) {
    obj.vel = flipY(obj.vel);
    return obj;
}

float modulus(Vector from) {
    return sqrtf(sqr(from.x) + sqr(from.y));
}

/*
 * v1 - v2 = v1 + (-v2)
 */
Vector minusV(Vector from, Vector to) {
    return plusV(from, scaleV(to, -1));
}

Vector plusV(Vector from, Vector to) {
    return v(
        from.x + to.x,
        from.y + to.y
    );
}

Vector scaleV(Vector from, float multiplier) {
    return v(
        from.x * multiplier,
        from.y * multiplier
    );
}

void print_vector(Vector v) {
    printf("{%.1f, %.1f}", v.x, v.y);
}
void print_object(Object obj) {
    printf("{ pos = ");
    print_vector(obj.pos);
    printf(", vel = ");
    print_vector(obj.vel);
    printf(", acc = ");
    print_vector(obj.acc);
    printf(" }");
}
