#include "test.h"

#include <iostream>
#include <Eigen/Dense>
#include "src/model/component.h"
#include "src/model/kirchhoff.h"
#include "src/fileio/xml.h"

using namespace Ohmcha;
using namespace Eigen;

void simple_RE_example()
{
    // Declare nodes
    std::vector<Node> nodes(2);
    Node &n1 = nodes[0], &n2 = nodes[1];

    // Declare elements
    Branch R(new Resistor(10), &n1, &n2);
    Branch E(new Emf(20), &n1, &n2);

    std::cout << khoffSolve({R, E}, nodes);
    // Expected result: [ 20, 2, -2 ]
}

void E_2parallelR_example()
{
    // Declare nodes
    std::vector<Node> nodes(2);
    Node &n1 = nodes[0], &n2 = nodes[1];

    // Declare elements
    Branch R1(new Resistor(10), &n1, &n2);
    Branch R2(new Resistor(20), &n2, &n1);
    Branch E(new Emf(10), &n1, &n2);

    std::cout << khoffSolve({R1, R2, E}, nodes);
    // Expected: [ 10, 1, -0.5, -1.5 ]
}

void E_NparallelR_example()
{
    // Declare nodes
    std::vector<Node> nodes(2);
    Node &n1 = nodes[0], &n2 = nodes[1];

    // Declare elements
    std::vector<Branch> branches(1001, Branch(new Resistor(10), &n1, &n2));
    branches[0] = Branch(new Emf(10), &n1, &n2);

    std::cout << khoffSolve(branches, nodes);
    // Expected: [ 10, -1000, 1, 1,..., 1 ]
}

void daisy_chain_example()
{
    // Declare nodes
    std::vector<Node> nodes(1001);

    // Declare elements
    std::vector<Branch> branches(1001, Branch(new Resistor(1), &nodes[0], &nodes[1000]));
    branches[0] = new Emf(1);
    for (int i = 0; i < 1001; ++i)
    {
        branches[i].setNode1(nodes[i]);
        branches[i].setNode2(nodes[(i + 1) % 1001]);
    }

    auto result = khoffSolve(branches, nodes);
    std::cout << "Voltages: \n" << result.head(1001) << "Currents: \n" << result.tail(1001);
    // Expected: Voltages = [ 0.001, -0.999, -0.998, ..., -0.001 ] Currents = [ -0.001, ..., -0.001 ]
    // The results may not be exact
}


int test()
{
    return 0;
}
