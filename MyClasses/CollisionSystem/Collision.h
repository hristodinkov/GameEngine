//
// Created by Hristo Dinkov on 11.3.2026 г..
//

#ifndef RAWENGINE_COLLISION_H
#define RAWENGINE_COLLISION_H


class ConvexCollider;

class Collision {
    public:
    bool SATCollision(const ConvexCollider& A, const ConvexCollider& B);
};


#endif //RAWENGINE_COLLISION_H