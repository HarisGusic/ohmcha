#include <iostream>
#include <vector>
#include <Eigen/Dense>

#include "model/branch.h"
#include "model/node.h"
#include "model/kirchhoff.h"

using namespace Ohmcha;
using namespace Eigen;

void simple_RE_example()
{
	// Declare nodes
	std::vector<Node> nodes(2);
	Node &n1 = nodes[0], &n2 = nodes[1];

	// Declare elements
	BResistor R(10, n1, n2);
	BEmf E(20, n1, n2);

	std::cout << khoffSolve({R, E}, nodes);
	// Expected result: [ 20, 2, -2 ]
}

void E_2parallelR_example()
{
	// Declare nodes
	std::vector<Node> nodes(2);
	Node &n1 = nodes[0], &n2 = nodes[1];

	// Declare elements
	BResistor R1(10, n1, n2);
	BResistor R2(20, n2, n1);
	BEmf E(10, n1, n2);

	std::cout << khoffSolve({R1, R2, E}, nodes);
	// Expected: [ 10, 1, -0.5, -1.5 ]
}

void E_NparallelR_example()
{
	// Declare nodes
	std::vector<Node> nodes(2);
	Node &n1 = nodes[0], &n2 = nodes[1];

	// Declare elements
	std::vector<Branch> branches(1001, BResistor(10, n1, n2));
	branches[0] = BEmf(10, n1, n2);

	std::cout << khoffSolve(branches, nodes);
	// Expected: [ 10, -1000, 1, 1,..., 1 ]
}

void daisy_chain_example()
{
	// Declare nodes
	std::vector<Node> nodes(1001);

	// Declare elements
	std::vector<Branch> branches(1001, BResistor(1, nodes[0], nodes[1000]));
	branches[0] = BEmf(1);
	for (int i = 0; i < 1001; ++i)
	{
		branches[i].setNode1(nodes[i]);
		branches[i].setNode2(nodes[(i + 1) % 1001]);
	}

	auto result = khoffSolve(branches, nodes);
	std::cout << "Voltages: \n" << result.head(1001) << "Currents: \n" << result.tail(1001);
	// Expected: [ 0.001, -0.999, -0.998, ..., -0.001, -0.001, ..., -0.001 ]
	// The results may not be exact
}

int main(int argc, char *argv[])
{
	daisy_chain_example();
	return 0;
}
