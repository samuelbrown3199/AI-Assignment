#include "Node.h"


Node::Node()
{

}

Node::Node(int _x, int _y, int _type, int i, AStar* _ai) //constructor for the node class
{
	posInList = i; //used to keep a constant position in lists
	ai = _ai; //so we can reference the start and end nodes later

	x = _x; //sets its x and y pos
	y = _y;
	type = _type; //sets its type eg. wall, end, start, empty

	nodeRect.x = 12 + (x * 50); //sets up the render square for the node
	nodeRect.y = 12 + (y * 50);
	nodeRect.w = 25;
	nodeRect.h = 25;
}


Node::~Node()
{
}

void Node::SetupNeighbours() //used to find the neighbour nodes of the node
{
	neighbours[0] = ai->FindNode(x, y + 1); //these functions look at all the nodes around this node
	neighbours[1] = ai->FindNode(x - 1, y);
	neighbours[2] = ai->FindNode(x + 1, y);
	neighbours[3] = ai->FindNode(x, y - 1);
	neighbours[4] = ai->FindNode(x - 1, y - 1);
	neighbours[5] = ai->FindNode(x + 1, y + 1);
	neighbours[6] = ai->FindNode(x + 1, y - 1);
	neighbours[7] = ai->FindNode(x - 1, y + 1);
}

void Node::ChangeParent(Node* _newParent) //sets the pointer of the parent node to the passed parameter
{
	parent = _newParent;
}

void Node::CalculateCost() //calculates the f cost of the node
{
	h = GetDistanceBetweenNodes(ai->endNode); //gets the heuristic value of the node
	g = GetDistanceBetweenNodes(ai->startNode); //gets the g value of the node

	f = h + g; //sets the f value
}

int Node::GetDistanceBetweenNodes(Node* a) //works out the distance between the pass node and this node
{
	int disX = x - a->x; //gets the x distance
	int disY = y - a->y; //gets the y distance

	if (disX > disY)
		return 14 * disY + 10 * (disX - disY);

	return 14 * disX + 10 * (disY - disX); //calculates the full distance between the nodes
}