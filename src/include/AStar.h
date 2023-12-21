#ifndef ASTAR_H
#define ASTAR_H
#include <Intersection.h>
#include <Node.h>
#include <stack>
#include <map>

class AStar {
    private:
        //helpers
        std::map<float, Node*>::iterator findNode(Intersection* , std::map<float, Node*>&);
        std::stack<Intersection*> makeStack(std::vector<Node*> );
        void resetPath(std::vector<Node *>& path, std::map<float, Node *> nodes, Node* minNeighbor);
        bool isClosed(Intersection* i, vector<Node*> closed);
    public:
        AStar();
        ~AStar();
        std::stack<Intersection*> findRoute(Intersection* ,Intersection* , std::vector<std::vector<Intersection*>*>* );
        
        

};


#endif