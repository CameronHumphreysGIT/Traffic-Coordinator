#include <AStar.h>
#include <queue>
#include <Variables.h>
#include <MathHelper.h>

using namespace std;

AStar::AStar() {

}

AStar::~AStar() {

}

stack<Intersection*> AStar::findRoute(Intersection* start, Intersection* end, vector<vector<Intersection*>*>* intersections) {
    //since we want a stack, we'll have to sort out the whole algo, then copy it to a stack
    map<float, Node*> nodes;
    vector<Intersection*> path;
    //calculate distance to end
    pair<int, int> dir = (start->getCorners().at(0) - end->getCorners().at(0));
    Vector2 direction2 = {(float)dir.first, (float)dir.second};
    nodes.insert({0.0f, new Node(start, NULL, 0.0f, direction2.Magnitude())});
    bool endFlag = false;
    while (!endFlag) {
        Node* topNode = nodes.begin()->second;
        Intersection* top = nodes.begin()->second->getIntersection();
        nodes.erase(nodes.begin());
        //search the neigbors of the top of the queue
        for (int side = Variables::TOP; side != Variables::END; side++) {
            if (top->getNeighbor((Variables::Side)side).first != -1 && top->getNeighbor((Variables::Side)side).second != -1) {
                Intersection* neighbor = intersections->at(top->getNeighbor((Variables::Side)side).first)->at(top->getNeighbor((Variables::Side)side).second);
                //make sure neigbor isn't the intersection we just came from on the path.
                if (path.empty() || neighbor != path.at(path.size() - 1)) {
                    //figure out magnitude from top to neighbor, and from neigbor to end, add together for score.
                    pair<int, int> dir = (top->getCorners().at(0) - neighbor->getCorners().at(0));
                    Vector2 direction = {(float)dir.first, (float)dir.second};
                    dir = (neighbor->getCorners().at(0) - end->getCorners().at(0));
                    Vector2 direction2 = {(float)dir.first, (float)dir.second};
                    //now make it into a node:
                    Node* node = new Node(neighbor, topNode, (direction.Magnitude() + topNode->getDistToMe()), direction2.Magnitude());
                    //check if we've added this node:
                    auto prev = findNode(neighbor, nodes);
                    if (prev != nodes.end()) {
                        //we need to erase and replace.
                        nodes.erase(prev);
                        delete prev->second;
                    }
                    //add it to the nodes
                    nodes.insert({node->getScore(), node});
                }
            }
        }
        path.push_back(top);
        if (top == end) {
            endFlag = true;
        }
    }
    stack<Intersection*> s;
    for (int i = (int)path.size() - 1; i >=0; i--) {
        s.push(path.at(i));
    }

    return s;
}

map<float, Node*>::iterator AStar::findNode(Intersection* i, map<float, Node*>& nodes) {
    map<float, Node*>::iterator iter = nodes.begin();
    while (iter != nodes.end()) {
        if (iter->second->getIntersection() == i) {
            //found the node.
            break;
        }
        iter++;
    }
    return iter;
}