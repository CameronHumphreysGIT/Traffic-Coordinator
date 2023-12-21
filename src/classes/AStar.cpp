#include <AStar.h>
#include <queue>
#include <Variables.h>
#include <MathHelper.h>
#include <iostream>

using namespace std;

AStar::AStar() {

}

AStar::~AStar() {

}

stack<Intersection*> AStar::findRoute(Intersection* start, Intersection* end, vector<vector<Intersection*>*>* intersections) {
    //since we want a stack, we'll have to sort out the whole algo, then copy it to a stack
    map<float, Node*> nodes;
    vector<Node*> path;
    //list of closed nodes so we don't revisit them.
    vector<Node*> closed;
    //calculate distance to end
    pair<int, int> dir = (start->getCenter() - end->getCenter());
    Vector2 direction2 = {(float)dir.first, (float)dir.second};
    nodes.insert({0.0f, new Node(start, NULL, 0.0f, direction2.Magnitude())});
    bool endFlag = false;
    while (!endFlag) {
        Node* topNode = nodes.begin()->second;
        Intersection* top = nodes.begin()->second->getIntersection();
        //add to closed
        closed.push_back(topNode);
        nodes.erase(nodes.begin());
        Node* minNeighbor = NULL;
        //search the neigbors of the top of the queue
        for (int side = Variables::TOP; side != Variables::END; side++) {
            if (top->getNeighbor((Variables::Side)side).first != -1 && top->getNeighbor((Variables::Side)side).second != -1) {
                Intersection* neighbor = intersections->at(top->getNeighbor((Variables::Side)side).first)->at(top->getNeighbor((Variables::Side)side).second);
                //make sure neigbor isn't the intersection we just came from on the path.
                if (!isClosed(neighbor, closed)) {
                    //figure out magnitude from top to neighbor, and from neigbor to end, add together for score.
                    pair<int, int> dir = (top->getCenter() - neighbor->getCenter());
                    Vector2 direction = {(float)dir.first, (float)dir.second};
                    dir = (neighbor->getCenter() - end->getCenter());
                    Vector2 direction2 = {(float)dir.first, (float)dir.second};
                    //now make it into a node:
                    Node* node = new Node(neighbor, topNode, (direction.Magnitude() + topNode->getDistToMe()), direction2.Magnitude());
                    //check if we've added this node:
                    auto prev = findNode(neighbor, nodes);
                    if (prev != nodes.end()) {
                        //check if path is shorter:
                        if (prev->second->getDistToMe() > node->getDistToMe()) {
                            //we need to erase and replace.
                            delete prev->second;
                            nodes.erase(prev);
                            nodes.insert({node->getScore(), node});
                            if (minNeighbor == NULL || node->getScore() < minNeighbor->getScore()) {
                                minNeighbor = node;
                            }
                        }
                    }else {
                        //add it to the nodes
                        nodes.insert({node->getScore(), node});
                        if (minNeighbor == NULL || node->getScore() < minNeighbor->getScore()) {
                            minNeighbor = node;
                        }
                    }
                }
            }
        }
       //cout<<"Top node: "<<topNode->getIntersection()->getId().first<<", "<<topNode->getIntersection()->getId().second;
       //for (auto node = nodes.begin(); node != nodes.end(); node++) {
       //    cout<<"score: "<<node->first<<" id "<<node->second->getIntersection()->getId().first<<", "<<node->second->getIntersection()->getId().second;
       //}
       //cout<<"==========================/n";
        bool flag = false;
        for (auto node = path.begin(); node != path.end(); node++) {
            if (topNode->getIntersection() == (*node)->getIntersection()) {
                //ahhh
               flag = true;
            }

        }
        path.push_back(topNode);
        if (top == end) {
            endFlag = true;
        }else {
            resetPath(path, nodes, minNeighbor);
        }
    }
    if (path.size() >= 3) {
        //
        Intersection* top = path.at(0)->getIntersection();
        if (top != start) {
            int i =0;
        }
        if (path.at(2)->getIntersection() == top) {
            //op[gredhjio
            bool flag = true;
        }
    }
    return makeStack(path);
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

stack<Intersection*> AStar::makeStack(vector<Node*> path) {
    //work backwards
    stack<Intersection*> stack;
    vector<Node*>::iterator iter = path.end() - 1;
    Node* parent = (*iter)->getParent();
    stack.push((*iter)->getIntersection());
    iter--;
    while (parent != (*path.begin())) {
        while ((*iter) != parent) {
            iter--;
        }
        //the iterator has reached the next member of the path
        parent = (*iter)->getParent();
        stack.push((*iter)->getIntersection());
        iter--;
    }
    //now push the begining of the path:
    stack.push(parent->getIntersection());
    return stack;
}

void AStar::resetPath(vector<Node *>& path, map<float, Node *> nodes, Node* minNeighbor) {
    Node* next = nodes.begin()->second;
    //basically check if the next node is a neighbor of the topnode... if not, we need to reset the path.
    if (next != minNeighbor) {
        //this means we are chosing a node that is not the neighbor of the topnode.
        //delete until we hit the parent of the next node
        while (!(path.empty()) && *(path.end() - 1) != next->getParent()) {
            path.pop_back();
        }
        if (path.empty()) {
            //repopulate up to next.
            //we are going to push in the reverse order and then reverse the list.
            Node* parent = next->getParent();
            //until we hit the start.
            while (parent != NULL) {
                path.push_back(parent);
                parent = parent->getParent();
            }
            //reverse it so we are in the right order
            reverse(path.begin(), path.end());
        }
    }
}

bool AStar::isClosed(Intersection* i, vector<Node*> closed) {
    for (auto iter = closed.begin(); iter != closed.end(); iter++) {
        if (i == (*iter)->getIntersection()) {
            return true;
        }
    }
    return false;
}