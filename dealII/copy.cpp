#include <gtest.hh>

#include <deal.II/grid/tria.h>
#include <deal.II/grid/grid_generator.h>
#include <deal.II/lac/vector.h>
#include <deal.II/lac/block_vector.h>

#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Adapter/dealII/copy.hh>
#include <Spacy/Adapter/dealII/vector.hh>
#include <Spacy/Adapter/dealII/vectorSpace.hh>
#include <Spacy/Util/cast.hh>

namespace
{
    const int fe_order = 1;
    const int n_variables = 3u;

    class dealII_CopySingleVector : public ::testing::Test
    {
    public:
        dealII_CopySingleVector()
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

    class dealII_CopyProductSpaceVector : public ::testing::Test
    {
    public:
        dealII_CopyProductSpaceVector()
        {
            dealii::GridGenerator::hyper_cube(triangulation, -1, 1);
            n_components = triangulation.n_vertices();
            V = Spacy::dealII::makeHilbertSpace(triangulation, n_variables, fe_order);
        }

        auto dealII_test_vector()
        {
            auto v = dealii::BlockVector<double>(n_variables, n_components);

            for(auto i=0; i<n_variables; ++i)
                for(auto j=0; j<n_components; ++j)
                    v.block(i)(j) = pow(10,i+1) + j;
            return v;
        }

        void check_dealII_vector(const dealii::BlockVector<double>& v)
        {
            for(auto i=0; i<n_variables; ++i)
                for(auto j=0; j<n_components; ++j)
                    EXPECT_DOUBLE_EQ(v.block(i)(j), pow(10,i+1) + j);
        }

        Spacy::Vector Spacy_test_vector()
        {
            return Spacy::dealII::Vector(dealII_test_vector(), V);
        }


        dealii::Triangulation<2> triangulation;
        unsigned int n_components;
        Spacy::VectorSpace V;
    };

    class dealII_CopyPermutedProductSpaceVector : public ::testing::Test
    {
    public:
        dealII_CopyPermutedProductSpaceVector()
        {
            dealii::GridGenerator::hyper_cube(triangulation, -1, 1);
            n_components = triangulation.n_vertices();
            V = Spacy::dealII::makeHilbertSpace(triangulation, {2, 0, 1}, fe_order);
        }

        auto dealII_test_vector()
        {
            auto v = dealii::BlockVector<double>(n_variables, n_components);

            for(auto i=0; i<n_variables; ++i)
                for(auto j=0; j<n_components; ++j)
                    v.block(i)(j) = pow(10,i+1) + j;
            return v;
        }

        void check_dealII_vector(const dealii::BlockVector<double>& v)
        {
            for(auto i=0; i<n_variables; ++i)
                for(auto j=0; j<n_components; ++j)
                    EXPECT_DOUBLE_EQ(v.block(i)(j), pow(10,i+1) + j);
        }

        Spacy::Vector Spacy_test_vector()
        {
            return Spacy::dealII::Vector(dealII_test_vector(), V);
        }


        dealii::Triangulation<2> triangulation;
        unsigned int n_components;
        Spacy::VectorSpace V;
    };

    class dealII_CopyPrimalDualProductSpaceVector : public ::testing::Test
    {
    public:
        dealII_CopyPrimalDualProductSpaceVector()
        {
            dealii::GridGenerator::hyper_cube(triangulation, -1, 1);
            n_components = triangulation.n_vertices();
            V = Spacy::dealII::makeHilbertSpace(triangulation, n_variables, fe_order);
        }

        auto dealII_test_vector()
        {
            auto v = dealii::BlockVector<double>(n_variables, n_components);

            for(auto i=0; i<n_variables; ++i)
                for(auto j=0; j<n_components; ++j)
                    v.block(i)(j) = pow(10,i+1) + j;
            return v;
        }

        void check_dealII_vector(const dealii::BlockVector<double>& v)
        {
            for(auto i=0; i<n_variables; ++i)
                for(auto j=0; j<n_components; ++j)
                    EXPECT_DOUBLE_EQ(v.block(i)(j), pow(10,i+1) + j);
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


TEST_F(dealII_CopySingleVector,dealIIVectorToSpacyVector)
{
    auto v_dealii = dealII_test_vector();
    auto v_Spacy = zero(V);

    Spacy::dealII::copy(v_dealii, v_Spacy);

    EXPECT_EQ(get(Spacy::cast_ref<Spacy::dealII::Vector>(v_Spacy)), v_dealii);
}

TEST_F(dealII_CopySingleVector,SpacyVectorTodealIIVector)
{
    auto v_Spacy = Spacy_test_vector();
    dealii::Vector<double> v_dealii(n_components);

    Spacy::dealII::copy(v_Spacy, v_dealii);
    EXPECT_EQ(v_dealii, get(Spacy::cast_ref<Spacy::dealII::Vector>(v_Spacy)));
}


TEST_F(dealII_CopyProductSpaceVector,dealIIVectorToSpacyVector)
{
    auto v_dealii = dealII_test_vector();
    auto v_Spacy = zero(V);

    Spacy::dealII::copy(v_dealii, v_Spacy);

    check_dealII_vector(get(Spacy::cast_ref<Spacy::dealII::Vector>(v_Spacy)));
}

TEST_F(dealII_CopyProductSpaceVector,SpacyVectorTodealIIVector)
{
    auto v_Spacy = Spacy_test_vector();
    dealii::BlockVector<double> v_dealii(n_variables, n_components);

    Spacy::dealII::copy(v_Spacy, v_dealii);
    check_dealII_vector(v_dealii);
}


TEST_F(dealII_CopyPermutedProductSpaceVector,dealIIVectorToSpacyVector)
{
    auto v_dealii = dealII_test_vector();
    auto v_Spacy = zero(V);

    Spacy::dealII::copy(v_dealii, v_Spacy);

    check_dealII_vector(get(Spacy::cast_ref<Spacy::dealII::Vector>(v_Spacy)));
}

TEST_F(dealII_CopyPermutedProductSpaceVector,SpacyVectorTodealIIVector)
{
    auto v_Spacy = Spacy_test_vector();
    dealii::BlockVector<double> v_dealii(n_variables, n_components);

    Spacy::dealII::copy(v_Spacy, v_dealii);
    check_dealII_vector(v_dealii);
}
