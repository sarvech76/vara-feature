#include "vara/Solver/SolverFactory.h"

#include "vara/Feature/ConstraintBuilder.h"
#include "vara/Feature/FeatureModelBuilder.h"
#include "gtest/gtest.h"

namespace vara::solver {

TEST(SolverFactory, EmptyZ3SolverTest) {
  auto S = SolverFactory::initializeSolver(SolverType::Z3);
  auto E = S->getNumberValidConfigurations();
  EXPECT_TRUE(E);
  EXPECT_EQ(E.extractValue(), 1);
}

TEST(SolverFactory, GeneralZ3Test) {
  vara::feature::FeatureModelBuilder B;
  B.makeRoot("root");
  B.makeFeature<vara::feature::BinaryFeature>("Foo", true)
      ->addEdge("root", "Foo");
  B.makeFeature<vara::feature::BinaryFeature>("alt", false)
      ->addEdge("root", "alt");
  B.makeFeature<feature::BinaryFeature>("a", true)->addEdge("alt", "a");
  B.makeFeature<feature::BinaryFeature>("b", true)->addEdge("alt", "b");

  vara::feature::ConstraintBuilder CB;
  CB.feature("a").implies().lNot().feature("b");
  B.addConstraint(
      std::make_unique<vara::feature::FeatureModel::BooleanConstraint>(
          CB.build()));

  B.makeFeature<vara::feature::BinaryFeature>("A", false)->addEdge("root", "A");
  B.makeFeature<vara::feature::BinaryFeature>("A1", true)->addEdge("A", "A1");
  B.makeFeature<vara::feature::BinaryFeature>("A2", true)->addEdge("A", "A2");
  B.makeFeature<vara::feature::BinaryFeature>("A3", true)->addEdge("A", "A3");
  B.emplaceRelationship(
      vara::feature::Relationship::RelationshipKind::RK_ALTERNATIVE, "A");
  B.makeFeature<vara::feature::BinaryFeature>("B", false)->addEdge("root", "B");
  B.makeFeature<vara::feature::BinaryFeature>("B1", true)->addEdge("B", "B1");
  B.makeFeature<vara::feature::BinaryFeature>("B2", true)->addEdge("B", "B2");
  B.makeFeature<vara::feature::BinaryFeature>("B3", true)->addEdge("B", "B3");
  B.emplaceRelationship(
      vara::feature::Relationship::RelationshipKind::RK_ALTERNATIVE, "B");
  B.makeFeature<vara::feature::BinaryFeature>("C", false)->addEdge("root", "C");
  B.makeFeature<vara::feature::BinaryFeature>("C1", true)->addEdge("C", "C1");
  B.makeFeature<vara::feature::BinaryFeature>("C2", true)->addEdge("C", "C2");
  B.makeFeature<vara::feature::BinaryFeature>("C3", true)->addEdge("C", "C3");
  B.emplaceRelationship(vara::feature::Relationship::RelationshipKind::RK_OR,
                        "C");

  auto FM = B.buildFeatureModel();
  auto S = SolverFactory::initializeSolver(*FM, SolverType::Z3);

  auto E = S->getNumberValidConfigurations();
  EXPECT_TRUE(E);
  EXPECT_EQ(E.extractValue(), 6 * 63);
}

} // namespace vara::solver
