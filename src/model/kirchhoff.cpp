#include "kirchhoff.h"

namespace Ohmcha
{

VectorXf khoffSolve(const std::vector<Branch> &branches, const std::vector<Node> &nodes)
{
    int b = branches.size(), n = nodes.size();
	// Allocate A and B of the system of equations
	MatrixXf A(b + n - 1, b + n - 1);
	VectorXf B(b + n - 1);
	A.setZero();
	B.setZero();

	// Iterate through all branches
	for (int i = 0; i < b; ++i)
	{
		// Some aliases
        const Branch &branch = branches[i];
		int n1_index = branch.getNode1() - &nodes[0],
				n2_index = branch.getNode2() - &nodes[0];

		// The last node (at index n-1) is chosen as reference
		if (n1_index != n - 1)
			A(i, n1_index) = branch.getA()[0];
		if (n2_index != n - 1)
			A(i, n2_index) = branch.getA()[1];
		A.row(i).tail(b)[i] = branch.getA()[2];
		B[i] = branch.getB();

		// Kirchhoff's current law

		// Bottom-right quadrant of A
		auto br_quad = A.bottomRightCorner(n - 1, b);

		// The current goes out of node1
		if (n1_index != n - 1)
			br_quad(n1_index, i) = 1;
		// The current goes into node2
		if (n2_index != n - 1)
			br_quad(n2_index, i) = -1;
	}

	// TODO: choose the right algorithm
    return A.colPivHouseholderQr().solve(B);
}

VectorXf khoffSolve(Schematic *schematic)
{
    auto branches = schematic->getBranches();
    auto nodes = schematic->getNodes();

    int b = branches.size(), n = nodes.size();
    // Allocate A and B of the system of equations
    MatrixXf A(b + n - 1, b + n - 1);
    VectorXf B(b + n - 1);
    A.setZero();
    B.setZero();

    // Iterate through all branches
    for (int i = 0; i < b; ++i)
    {
        // Some aliases
        const Branch *branch = branches[i];
        int n1_index = schematic->getNodeId(branch->getNode1()),
                n2_index = schematic->getNodeId(branch->getNode2());

        // The last node (at index n-1) is chosen as reference
        if (n1_index != n - 1)
            A(i, n1_index) = branch->getA()[0];
        if (n2_index != n - 1)
            A(i, n2_index) = branch->getA()[1];
        A.row(i).tail(b)[i] = branch->getA()[2];
        B[i] = branch->getB();

        // Kirchhoff's current law

        // Bottom-right quadrant of A
        auto br_quad = A.bottomRightCorner(n - 1, b);

        // The current goes out of node1
        if (n1_index != n - 1)
            br_quad(n1_index, i) = 1;
        // The current goes into node2
        if (n2_index != n - 1)
            br_quad(n2_index, i) = -1;
    }

    // TODO: choose the right algorithm
    return A.colPivHouseholderQr().solve(B);
}

}
