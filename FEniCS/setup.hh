#pragma once

#include <dolfin.h>
#include <cassert>

#include "LinearHeat.h"
#include "L2Functional.h"

#include <Spacy/Adapter/FEniCS/vectorSpace.hh>

inline auto single_hilbert_space_1D(int degrees_of_freedom)
{
    assert(degrees_of_freedom > 1);
    auto mesh = std::make_shared<dolfin::UnitIntervalMesh>(MPI_COMM_WORLD, degrees_of_freedom-1);
    auto V = std::make_shared<LinearHeat::FunctionSpace>(mesh);
    return Spacy::FEniCS::makeHilbertSpace(V);
}

inline auto product_hilbert_space_1D(int degrees_of_freedom)
{
    assert(degrees_of_freedom > 1);
    auto mesh = std::make_shared<dolfin::UnitIntervalMesh>(MPI_COMM_WORLD, degrees_of_freedom-1);
    auto V = std::make_shared<L2Functional::CoefficientSpace_x>(mesh);
    return Spacy::FEniCS::makeHilbertSpace(V,{0,1,2},{});
}

inline auto primal_dual_product_hilbert_space_1D(int degrees_of_freedom)
{
    assert(degrees_of_freedom > 1);
    auto mesh = std::make_shared<dolfin::UnitIntervalMesh>(MPI_COMM_WORLD, degrees_of_freedom-1);
    auto V = std::make_shared<L2Functional::CoefficientSpace_x>(mesh);
    return Spacy::FEniCS::makeHilbertSpace(V,{0,1},{2});
}
