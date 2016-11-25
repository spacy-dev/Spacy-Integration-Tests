#include <gtest.hh>
#include <dolfin.h>
#include <Spacy/vectorSpace.hh>
#include <Spacy/Spaces/productSpace.hh>
#include <Spacy/Adapter/FEniCS/vectorSpace.hh>
#include <mockSetup.hh>
#include "LinearHeat.h"
#include "setup.hh"

using namespace Spacy;

TEST(FEniCS,SingleSpaceCreator)
{
    constexpr int degrees_of_freedom = 2;
    auto W = single_hilbert_space_1D(degrees_of_freedom);
    const auto& V = creator<FEniCS::VectorCreator>(W);
    EXPECT_EQ( V.size(), degrees_of_freedom );
    EXPECT_EQ( V.dofmap(0), 0 );
    EXPECT_EQ( V.dofmap(1), 1 );
    EXPECT_EQ( V.inverseDofmap(0), 0 );
    EXPECT_EQ( V.inverseDofmap(1), 1 );
}

TEST(FEniCS,ProductSpaceCreator)
{
    constexpr int scalar_degrees_of_freedom = 2;
    constexpr int degrees_of_freedom = scalar_degrees_of_freedom * scalar_degrees_of_freedom;
    auto W = product_hilbert_space_1D(scalar_degrees_of_freedom);
    const auto& X = creator<ProductSpace::VectorCreator>(W);
    const auto& Y = creator<FEniCS::VectorCreator>(X.subSpace(0));
    const auto& U = creator<FEniCS::VectorCreator>(X.subSpace(1));
    const auto& P = creator<FEniCS::VectorCreator>(X.subSpace(2));

    EXPECT_EQ( Y.size(), degrees_of_freedom );
    EXPECT_EQ( U.size(), degrees_of_freedom );
    EXPECT_EQ( P.size(), degrees_of_freedom );
}

TEST(FEniCS,PrimalDualProductSpaceCreator)
{
    constexpr int scalar_degrees_of_freedom = 2;
    constexpr int degrees_of_freedom = scalar_degrees_of_freedom * scalar_degrees_of_freedom;
    auto W = primal_dual_product_hilbert_space_1D(scalar_degrees_of_freedom);
    const auto& X = creator<ProductSpace::VectorCreator>(W);
    const auto& X_primal = creator<ProductSpace::VectorCreator>(X.subSpace(0));
    const auto& X_dual = creator<ProductSpace::VectorCreator>(X.subSpace(1));
    const auto& Y = creator<FEniCS::VectorCreator>(X_primal.subSpace(0));
    const auto& U = creator<FEniCS::VectorCreator>(X_primal.subSpace(1));
    const auto& P = creator<FEniCS::VectorCreator>(X_dual.subSpace(0));

    EXPECT_EQ( Y.size(), degrees_of_freedom );
    EXPECT_EQ( U.size(), degrees_of_freedom );
    EXPECT_EQ( P.size(), degrees_of_freedom );
}
