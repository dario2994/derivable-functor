// Public functors collection.
// List of nice wrappers around internal ugly-and-not-so-user-friendly
// functions and classes.

#ifndef DERIVABLE_FUNCTOR_PUBLIC_H
#define DERIVABLE_FUNCTOR_PUBLIC_H

#include "util.h"
#include "functors.h"
#include "derivatives.h"

namespace DerivableFunctor {

// A syntax sugar to use in expressions.
// Example: X*X + X + 1 is possible only for this definition.
constexpr Identity X;

// Sum
PUBLIC_BINARY_OPERATOR(Sum, operator+)

// Product
PUBLIC_BINARY_OPERATOR(Product, operator*)

}  // namespace DerivableFunctor

#endif  // DERIVABLE_FUNCTOR_PUBLIC_H