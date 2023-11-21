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
    public:
        AStar();
        ~AStar();
        std::stack<Intersection*> findRoute(Intersection* ,Intersection* , std::vector<std::vector<Intersection*>*>* );
        
        

};


#endif