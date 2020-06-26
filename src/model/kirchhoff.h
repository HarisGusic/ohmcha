#pragma once

#include <Eigen/Dense>
#include "component.h"
#include "schematic.h"

namespace Ohmcha
{

using namespace Eigen;

VectorXf khoffSolve(const std::vector<Branch> &branches, const std::vector<Node> &nodes);
VectorXf khoffSolve(Schematic *schematic);

}
