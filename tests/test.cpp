#include <gtest/gtest.h>
#include "polynomial.hpp"
#include <cmath>
#include <sstream>

TEST(PolynomialTest, DefaultConstructorZero) {
    Polynomial p;
    EXPECT_TRUE(p.is_zero());
    EXPECT_EQ(p.degree(), -1);
}

TEST(PolynomialTest, FromCoeffsArray) {
    double c[] = {1.0, 2.0};
    Polynomial p(c, 1);
    EXPECT_FALSE(p.is_zero());
    EXPECT_EQ(p.degree(), 1);
    EXPECT_NEAR(p.evaluate(0.0), 1.0, 1e-9);
    EXPECT_NEAR(p.evaluate(1.0), 3.0, 1e-9);
}

TEST(PolynomialTest, NegativeDegreeZero) {
    double c[] = {1.0, 2.0};
    Polynomial p(c, -1);
    EXPECT_TRUE(p.is_zero());
    EXPECT_EQ(p.degree(), -1);
}

TEST(PolynomialTest, CopyConstructorDeepCopy) {
    double c[] = {1.0, 2.0, 3.0};
    Polynomial original(c, 2);
    Polynomial copy(original);
    EXPECT_EQ(copy.degree(), original.degree());
    EXPECT_NEAR(copy.evaluate(2.0), original.evaluate(2.0), 1e-9);
}

TEST(PolynomialTest, MoveConstructor) {
    double c[] = {1.0, 2.0};
    Polynomial original(c, 1);
    Polynomial moved(std::move(original));
    EXPECT_FALSE(moved.is_zero());
    EXPECT_TRUE(original.is_zero());
}

TEST(PolynomialTest, CopyAssignmentIndependentBuffers) {
    double c1[] = {1.0, 2.0};
    double c2[] = {3.0, 4.0, 5.0};
    Polynomial a(c1, 1), b(c2, 2);
    b = a;
    EXPECT_EQ(b.degree(), 1);
    EXPECT_NEAR(b.evaluate(1.0), 3.0, 1e-9);
    a = a;
    EXPECT_NEAR(a.evaluate(1.0), 3.0, 1e-9);
}

TEST(PolynomialTest, MoveAssignmentRvalue) {
    double c[] = {5.0, 0.0, 1.0};
    Polynomial a;
    a = Polynomial(c, 2);
    EXPECT_EQ(a.degree(), 2);
    EXPECT_NEAR(a.evaluate(0.0), 5.0, 1e-9);
}

TEST(PolynomialTest, AddPolynomials) {
    double c1[] = {1.0, 2.0};
    double c2[] = {0.0, 1.0, 1.0};
    Polynomial a(c1, 1), b(c2, 2);
    a.add(b);
    EXPECT_EQ(a.degree(), 2);
    EXPECT_NEAR(a.evaluate(1.0), 5.0, 1e-9);
}

TEST(PolynomialTest, MulPolynomials) {
    double c1[] = {1.0, 2.0};
    double c2[] = {0.0, 1.0, 1.0};
    Polynomial a(c1, 1), b(c2, 2);
    a.mul(b);
    EXPECT_EQ(a.degree(), 3);
    EXPECT_NEAR(a.evaluate(1.0), 6.0, 1e-9);
}

TEST(PolynomialTest, Evaluate) {
    double c[] = {1.0, 2.0, 3.0};
    Polynomial p(c, 2);
    EXPECT_NEAR(p.evaluate(0.0), 1.0, 1e-9);
    EXPECT_NEAR(p.evaluate(1.0), 6.0, 1e-9);
    EXPECT_NEAR(p.evaluate(-1.0), 2.0, 1e-9);
}

TEST(PolynomialTest, EqualityOperators) {
    double c1[] = {1.0, 2.0};
    double c2[] = {1.0, 2.0, 0.0};
    Polynomial a(c1, 1), b(c2, 2);
    EXPECT_TRUE(a == b);
    double c3[] = {1.0, 3.0};
    Polynomial c(c3, 1);
    EXPECT_TRUE(a != c);
}

TEST(PolynomialTest, Derivative) {
    double c[] = {1.0, 2.0, 3.0};
    Polynomial p(c, 2);
    Polynomial d = p.derivative();
    EXPECT_EQ(d.degree(), 1);
    EXPECT_NEAR(d.evaluate(0.0), 2.0, 1e-9);
    EXPECT_NEAR(d.evaluate(1.0), 8.0, 1e-9);
}

TEST(PolynomialTest, DerivativeOfConstant) {
    double c[] = {5.0};
    Polynomial p(c, 0);
    Polynomial d = p.derivative();
    EXPECT_TRUE(d.is_zero());
}

TEST(PolynomialTest, Integral) {
    double c[] = {0.0, 2.0};
    Polynomial p(c, 1);
    Polynomial integ = p.integral();
    EXPECT_EQ(integ.degree(), 2);
    EXPECT_NEAR(integ.evaluate(0.0), 0.0, 1e-9);
    EXPECT_NEAR(integ.evaluate(1.0), 1.0, 1e-9);
    EXPECT_NEAR(integ.evaluate(2.0), 4.0, 1e-9);
}

TEST(PolynomialTest, OperatorsDoNotModifyOperands) {
    double c1[] = {1.0, 1.0};
    double c2[] = {1.0, -1.0};
    Polynomial a(c1, 1), b(c2, 1);
    Polynomial c = a * b;
    EXPECT_NEAR(c.evaluate(1.0), 0.0, 1e-9);
    EXPECT_NEAR(a.evaluate(1.0), 2.0, 1e-9);
    EXPECT_NEAR(b.evaluate(1.0), 0.0, 1e-9);
}
