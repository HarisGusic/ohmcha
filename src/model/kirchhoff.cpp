#include "kirchhoff.h"
//TODO temporary tests
#include <QDebug>
#include <stack>

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

    return A.colPivHouseholderQr().solve(B);
}

bool find(const std::vector<int> vec, int x)
{
    return std::find(vec.begin(), vec.end(), x) != vec.end();
}

int findBranch(Node *node, Schematic *schematic, const std::vector<int> &tree, int nth = 0)
{
    auto branches = schematic->getBranches();
    for (int i = 0; i < branches.size(); ++i)
        if (branches[i]->hasNode(node) && !find(tree, i))
        {
            if (nth == 0)
                return i;
            --nth;
        }
    return -1;
}

/**
 * Find the nth branch that is not in tree, in a graph that has a total of nTotal branches
 * Indexation starts from zero.
 */
int findStartBranch(const std::vector<int> &tree, int nth, int nTotal)
{
    for (int i = 0; i < nTotal; ++i)
        if (!find(tree, i))
        {
            if (nth == 0)
                return i;
            --nth;
        }
}

int findBranchOtherThan(int node, int branch, const std::vector<int> &branches, int nth, Schematic *schematic)
{
    for (int i = 0; i < branches.size(); ++i)
    {
        if (i != branch && schematic->getBranches()[i]->hasNode(schematic->getNodes()[node]))
        {
            if (nth == 0)
                return i;
            --nth;
        }
    }
    return -1;
}

void nextLoop(int startBranch, int loopIndex, MatrixXf &loopMatrix, const std::vector<int> &tree, Schematic *schematic)
{
    auto branches = schematic->getBranches();
    auto nodes = schematic->getNodes();
    int startNode = schematic->getNodeId(branches[startBranch]->getNode1());

    loopMatrix(loopIndex, startBranch) = 1;

    std::stack<int> history;
    std::stack<int> indices;
    history.push(startBranch);
    indices.push(0);
    for (int node = schematic->getNodeId(branches[startBranch]->getNode2()); node != startNode;)
    {
        // Find branch connected to node that is not the previous branch
        auto nextBranch = findBranchOtherThan(node, history.top(), tree, indices.top(), schematic);
        if (nextBranch < 0)
        { // We have reached a dead end
            node = schematic->getNodeId(branches[history.top()]->getOtherNode(nodes[node]));
            // Remove the last branch from the loop matrix
            loopMatrix(loopIndex, history.top()) = 0;
            // Go back one step
            history.pop();
            indices.pop();
            indices.top()++;
            continue;
        }

        // We will take this branch
        history.push(nextBranch);
        indices.push(0);
        node = schematic->getNodeId(branches[nextBranch]->getOtherNode(nodes[node]));

        // Populate the loop matrix
        if (nodes[node] == branches[nextBranch]->getNode2())
            loopMatrix(loopIndex, nextBranch) = 1;
        else
            loopMatrix(loopIndex, nextBranch) = -1;
    }
}

VectorXf loopCurrentSolve(Schematic *schematic, std::vector<int> &tree)
{
    auto nodes = schematic->getNodes();
    auto branches = schematic->getBranches();
    int n_loops = branches.size() - nodes.size() + 1;

    // Loop matrix
    MatrixXf B(n_loops, branches.size());

    // Construct a fundamental tree
    tree = std::vector<int>(nodes.size() - 1, -1);
    for (int i = 0; i < nodes.size(); ++i)
    {
        // Find a branch connected to this node that is not already part of the tree
        int branch = findBranch(nodes[i], schematic, tree);
        if (i != nodes.size() - 1)
            tree[i] = branch;
    }

    // Find a branch that is not in the tree
    int startBranch = findStartBranch(tree, 0,  branches.size()); //TODO index

    // Populate the loop matrix
    for (int i = 0; i < branches.size() - tree.size(); ++i)
        nextLoop(startBranch, i, B, tree, schematic);

    // Populate the resistance and emf matrices

    VectorXf R(branches.size());
    VectorXf E(branches.size());

    for (int i = 0; i < branches.size(); ++i)
    {
        R(i) = -branches[i]->getA()(2);
        E(i) = branches[i]->getB();
    }

    MatrixXf _R = R.asDiagonal();

    MatrixXf lhs = B * _R * B.transpose();
    VectorXf rhs = B * E;

    return lhs.colPivHouseholderQr().solve(rhs);

}

}
