#include <gtest.hh>

#include "setup.hh"

#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Adapter/Kaskade/vector.hh>
#include <Spacy/Adapter/Kaskade/vectorSpace.hh>
#include <mockSetup.hh>

namespace
{
  template <class Descriptions, int i>
  void testExtractedSpace(const std::vector<std::shared_ptr< Spacy::VectorSpace > >& spaces, const std::vector<std::string>& names)
  {
    using Creator = Spacy::Kaskade::VectorCreator<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,i>>;
    ASSERT_TRUE( Spacy::is<Creator>(spaces[i]->creator()) );
    ASSERT_EQ( Spacy::creator<Creator>(*spaces[i]).get().names[0] , names[i] );
  }
}

TEST(Kaskade,MakeSpaces)
{
  KASKADE_PRODUCT_SPACE_SETUP

  constexpr int n = boost::fusion::result_of::size<typename Descriptions::Variables>::value;
  std::vector<std::shared_ptr< Spacy::VectorSpace > > newSpaces( n );

  Spacy::Kaskade::Detail::MakeSpaces<Descriptions,0,n>::apply(descriptions,newSpaces);
  testExtractedSpace<Descriptions,0>(newSpaces,names);
  testExtractedSpace<Descriptions,1>(newSpaces,names);
  testExtractedSpace<Descriptions,2>(newSpaces,names);
}

TEST(Kaskade,MakeSpaces_PermutedVariableOrder)
{
  KASKADE_PRODUCT_SPACE_SETUP_PERM_VARIABLE_ORDER

  constexpr int n = boost::fusion::result_of::size<typename Descriptions::Variables>::value;
  std::vector<std::shared_ptr< Spacy::VectorSpace > > newSpaces( n );

  Spacy::Kaskade::Detail::MakeSpaces<Descriptions,0,n>::apply(descriptions,newSpaces);
  testExtractedSpace<Descriptions,0>(newSpaces,names);
  testExtractedSpace<Descriptions,1>(newSpaces,names);
  testExtractedSpace<Descriptions,2>(newSpaces,names);
}
