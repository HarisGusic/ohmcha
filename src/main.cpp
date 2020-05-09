#include <iostream>
#include <vector>

#include "model/branch.h"
#include "model/node.h"
#include "model/graph.h"

using namespace Ohmcha;

std::vector<Branch> branches;
std::vector<Node> nodes;

int main(int argc, char *argv[])
{
	Node n1, n2;
	BResistor R(10);
	BEmf E(20);
	R.setNode1(n1);
	R.setNode2(n2);
	E.setNode1(n1);
	E.setNode2(n2);
	return 0;
}
