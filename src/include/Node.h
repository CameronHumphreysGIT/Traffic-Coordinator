#ifndef NODE_H
#define NODE_H
#include <Intersection.h>

class Node {
    private:
        Intersection* intersection;
        Node* parent;
        float score;
        float distToMe;
        float distToEnd;
    public:
        Node(Intersection* , Node* , float, float );
        ~Node();
        Intersection* getIntersection();
        float getDistToMe();
        float getScore();
        Node* getParent();
};

#endif