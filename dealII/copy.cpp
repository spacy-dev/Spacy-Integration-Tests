#include <gtest.hh>

#include <deal.II/grid/tria.h>
#include <deal.II/grid/grid_generator.h>
#include <deal.II/lac/vector.h>

#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Adapter/dealII/copy.hh>
#include <Spacy/Adapter/dealII/vector.hh>
#include <Spacy/Adapter/dealII/vectorSpace.hh>
#include <Spacy/Util/cast.hh>

namespace
{
    const int fe_order = 1;

    class dealII_CopyVector : public ::testing::Test
    {
    public:
        dealII_CopyVector()
        {
            dealii::GridGenerator::hyper_cube(triangulation, -1, 1);
            n_components = triangulation.n_vertices();
            V = Spacy::dealII::makeHilbertSpace(triangulation, fe_order);
        }

        auto dealII_test_vector()
        {
            auto v = dealii::Vector<double>(n_components);
            v(0) = 1;
            v(1) = 2;
            return v;
        }

        Spacy::Vector Spacy_test_vector()
        {
            return Spacy::dealII::Vector(dealII_test_vector(), V);
        }


        dealii::Triangulation<2> triangulation;
        unsigned int n_components;
        Spacy::VectorSpace V;
    };
}

TEST_F(dealII_CopyVector,dealIIVectorToSpacyVector_OneVariable)
{
    auto v_dealii = dealII_test_vector();
    auto v_Spacy = zero(V);

    Spacy::dealII::copy(v_dealii, v_Spacy);

    EXPECT_EQ(get(Spacy::cast_ref<Spacy::dealII::Vector>(v_Spacy)), v_dealii);
}

TEST_F(dealII_CopyVector,SpacyVectorTodealIIVector_OneVariable)
{
    auto v_Spacy = Spacy_test_vector();
    dealii::Vector<double> v_dealii(n_components);

    Spacy::dealII::copy(v_Spacy, v_dealii);
    EXPECT_EQ(v_dealii, get(Spacy::cast_ref<Spacy::dealII::Vector>(v_Spacy)));
}
