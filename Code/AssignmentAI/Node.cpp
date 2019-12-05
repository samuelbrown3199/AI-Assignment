#include "Node.h"


Node::Node()
{

}

Node::Node(int _x, int _y, int _type, int i, AStar* _ai)
{
	posInList = i;
	ai = _ai;

	x = _x;
	y = _y;
	type = _type;

	nodeRect.x = 12 + (x * 50);
	nodeRect.y = 12 + (y * 50);
	nodeRect.w = 25;
	nodeRect.h = 25;
}


Node::~Node()
{
}

void Node::SetupNeighbours()
{
	neighbours[0] = ai->FindNode(x, y + 1);
	neighbours[1] = ai->FindNode(x + 1, y + 1);
	neighbours[2] = ai->FindNode(x + 1, y);
	neighbours[3] = ai->FindNode(x + 1, y - 1);
	neighbours[4] = ai->FindNode(x, y - 1);
	neighbours[5] = ai->FindNode(x - 1, y - 1);
	neighbours[6] = ai->FindNode(x - 1, y);
	neighbours[7] = ai->FindNode(x - 1, y + 1);
}

void Node::ChangeParent(Node* _newParent)
{
	parent = _newParent;
}

void Node::CalculateCost()
{
	float distance = sqrt((ai->endNode->x - x)*(ai->endNode->x - x) + (ai->endNode->y - y)*(ai->endNode->y - y));
	h = distance;

	distance = sqrt((ai->startNode->x - x)*(ai->startNode->x - x) + (ai->startNode->y - y)*(ai->startNode->y - y));
	g = distance;

	f = h + g;
}