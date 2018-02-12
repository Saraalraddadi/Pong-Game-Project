
#include <stdio.h>

#include "physics.h"
#include "utils.h"
#include "cab202_graphics.h"

int objectWithin(Object probe, Vector center, Vector diag);

/*
 * Check if object got inside the paddle and its direction is
 * the direction the paddle collides from.
 */
int doesCollideWithPaddle(Object probe, Paddle paddle) {
    Vector diag = v(0.5, paddle.width + 0.5); // vector from center to corner
	Vector pos = paddle.center.pos;
	if (paddle.facing == LEFT_PADDLE) {
		pos.x--;
	}
	else if (paddle.facing == RIGHT_PADDLE) {
		pos.x++;	
	}
    return objectWithin(probe, pos, diag)
        && (  (paddle.facing == LEFT_PADDLE  && probe.vel.x > 0)
           || (paddle.facing == RIGHT_PADDLE && probe.vel.x < 0)
           )
    ;
}

int doesCollideWithBlock(Object probe, Block block) {
    return block.on && objectWithin(probe, block.center.pos, v(0.5, 0.5));
}

int objectWithin(Object probe, Vector center, Vector diag) {
    Vector pt = probe.pos;
    Vector lu = minusV(center, diag); // left-up corner point
    Vector rd = plusV(center, diag);  // right-down corner point

    return pt.x >= lu.x
        && pt.x <= rd.x
        && pt.y >= lu.y
        && pt.y <= rd.y;
}


Object tryCollidingWithPaddle(Object probe, Paddle paddle) {
    if (doesCollideWithPaddle(probe, paddle)) {
        return collideWithPaddle(probe, paddle);
    }
    return probe;
}

Object collideWithPaddle(Object probe, Paddle paddle) {
    return bounceOffVerticalWall(probe);
}

/*
 * Check if object is located in affected part of the plane.
 */
int doesCollideWithWall(Object probe, Wall wall) {
    switch (wall.facing) {
        case UP:    return probe.pos.y <= wall.pivot.pos.y; 
        case DOWN:  return probe.pos.y >= wall.pivot.pos.y;
        case RIGHT: return probe.pos.x <= wall.pivot.pos.x;
        case LEFT:  return probe.pos.x >= wall.pivot.pos.x;
    }
    return 0;
}

Object collidingWithWall(Object probe, Wall wall) {
    switch (wall.facing) {
        case DOWN:
        case UP:
            return bounceOffHorisontalWall(probe);

        case RIGHT:
        case LEFT:
            return bounceOffVerticalWall(probe);
    }
    return probe;
}

Object collidingWithBlock(Object probe, Block wall) {
    return bounceOffHorisontalWall(probe);
}

Object tryCollidingWithWall(Object probe, Wall wall) {
    if (doesCollideWithWall(probe, wall))
        return collidingWithWall(probe, wall);
    return probe;
}

Object tryCollidingWithBlock(Object probe, Block * block) {
    if (doesCollideWithBlock(probe, *block)) {
        block->on = 0;
        return collidingWithBlock(probe, *block);
    }
    return probe;
}

/*
 * Recalculate acceleration.
 *
 * acc = mass * R / |R|^3,
 *   where
 *     R = sing.pos - probe.pos
 *
 *  (so |acc| = mass / |R|^2)
 */
Vector getForce(Object probe, Singularity black_hole) {
    Vector point       = probe.pos;
    Vector singularity = black_hole.center.pos;

    Vector dir = minusV(singularity, point);
    float dist = modulus(dir);

    return scaleV(dir, black_hole.mass / cube(dist));
}

Object getAffectedBySingularity(Object probe, Singularity black_hole) {
    probe.acc = getForce(probe, black_hole);
    return probe;
}

Paddle mkPaddle(PaddleFacing facing, Object center, float width) {
    return (Paddle) {
        facing = facing,
        center = center,
        width = width
    };
}
Wall mkWall(WallFacing facing, Object pivot) {
    return (Wall) {
        facing = facing,
        pivot = pivot
    };
}

Singularity mkSingularity(Object center, float mass) {
    return (Singularity) {
        center = center,
        mass = mass
    };
}

Block mkBlock(Object center, int on) {
    return (Block) {
        center = center,
        on = on
    };
}

Paddle movePaddleByY(float dy, Paddle paddle) {
    paddle.center = moveObj(v(0, dy), paddle.center);
    return paddle;
}

void print_paddle(Paddle paddle) {
    char *labels[2] = {
        [LEFT_PADDLE] = "LEFT",
        [RIGHT_PADDLE] = "RIGHT",
    };
    printf("Paddle facing %s at ",
        labels[paddle.facing]
    );
    print_object(paddle.center);
}

void print_wall(Wall wall) {
    char *labels[4] = {
        [UP] = "UP",
        [DOWN] = "DOWN",
        [LEFT] = "LEFT",
        [RIGHT] = "RIGHT"
    };
    printf("Wall facing %s at ",
        labels[wall.facing]
    );
    print_vector(wall.pivot.pos);
}

void print_singilarity(Singularity singularity) {
    printf("singularity at ");
    print_vector(singularity.center.pos);
}
