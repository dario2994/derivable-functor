// Public functors collection.
// List of nice wrappers around internal ugly-and-not-so-user-friendly
// functions and classes.

#ifndef DERIVABLE_FUNCTOR_PUBLIC_H
#define DERIVABLE_FUNCTOR_PUBLIC_H

#include "util.h"
#include "functors.h"
#include "derivatives.h"


// A syntax sugar to use in expressions.
// Example: X*X + X + 1 is possible only for this definition.
// It is on purpose out of the DerivableFunctor namespace. This way the user
// doesn't have to write "using namespace DerivableFunctor" or similar stuff
// as it is just working out of the box (at the cost of polluting the global
// namespace).
constexpr DerivableFunctor::Identity X;

namespace DerivableFunctor {
// Sum
PUBLIC_BINARY_OPERATOR(Sum, operator+)

// Difference
PUBLIC_BINARY_OPERATOR(Difference, operator-)

// Product
PUBLIC_BINARY_OPERATOR(Product, operator*)

// Division
PUBLIC_BINARY_OPERATOR(Division, operator/)

}  // namespace DerivableFunctor

#endif  // DERIVABLE_FUNCTOR_PUBLIC_H
