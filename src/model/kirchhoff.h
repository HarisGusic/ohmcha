#pragma once

#include <Eigen/Dense>
#include "component.h"

namespace Ohmcha
{

using namespace Eigen;

VectorXf khoffSolve(const std::vector<Branch> &branches, const std::vector<Node> &nodes);

}
