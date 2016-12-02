#include <iostream>
#include <cmath>
#include <cassert>
#include "public.h"

template <typename T>
bool IsEqual(const T& x, const T& y) {
    return x == y;
}
bool IsEqual(const double& x, const double& y) {
    return std::abs(x-y) < 1e-5;
}
bool IsEqual(const float& x, const float& y) {
    return std::abs(x-y) < 1e-5;
}

template <typename Functor1, typename Functor2, typename T>
void ShouldBeEqual(
        const Functor1& f, const Functor2& g, const T& x, bool equal) {
    const T& value_f = f(x);
    const T& value_g = g(x);
    if (IsEqual(value_g, value_f) != equal) {
        std::cerr << "Expected " << (equal?"equal":"different")
                  << " values when x = " << x << std::endl;
        std::cerr << "\t f(x) = " << value_f << std::endl;
        std::cerr << "\t g(x) = " << value_g << std::endl;
        assert(0);
    }
}

template <typename Functor1, typename Functor2, typename T>
void ExpectEqualEvaluation(const Functor1& f, const Functor2& g, const T& x) {
    ShouldBeEqual(f, g, x, true);
}

template <typename Functor1, typename Functor2>
void ExpectDifferentEvaluation(const Functor1& f, const Functor2& g, double x) {
    ShouldBeEqual(f, g, x, false);
}

// Almost all the tests should have the following form:
// f = D( function_to_test );
// g = derivative_of_function_to_test_calculated_with_an_external_program
// sequence of checks like:
// ExpectEqualEvaluation(f, g, value_with_varying_type)
//
// It is not feasible to have a general function that takes f and g and checks
// their equality for the following reasong:
//  1. Really checking if the two functions are equal is a very _hard_ problem,
//     and I wouldn't be surprised if, given a large enough amount of elementary
//     functions, it is undecidable where to expressions with those functions
//     are the same function.
//  2. Checking various fixed values is very brittle as the functions may give
//     division by zero errors or overflow when evaluated in those points.
void TestingRationalFunctions() {
    const auto& f = D( (X*X + X - 5) / (X-X*X) );
    const auto& g = (2*X*X - 10*X + 5) / ((X-1)*(X-1)*X*X);
    ExpectEqualEvaluation(f, g, 3.0);
    ExpectEqualEvaluation(f, g, 5.0);
    ExpectDifferentEvaluation(f, g+1, 5.0);
}

int main() {
    TestingRationalFunctions();
}
