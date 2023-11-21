#include <Node.h>

Node::Node(Intersection* inter, Node* pa, float dstToMe, float dstToEnd) {
    distToMe = dstToMe;
    distToEnd = dstToEnd;
    score = distToMe + distToEnd;
    parent = pa;
    intersection = inter;
}

Node::~Node() {

}

Intersection* Node::getIntersection() {
    return intersection;
}

float Node::getDistToMe() {
    return distToMe;
}

float Node::getScore() {
    return score;
}

Node* Node::getParent() {
    return parent;
}