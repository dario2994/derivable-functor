// Contains all built-in functors.
//
// Here the least amount of boilerplate code is present and no checks
// on the types is done (the checks are done in the exposed functions).
// Furthermore all types are and should stay explicit (use of the auto
// keyword is forbidden here) as this makes debugging much easier and is
// not a big deal while adding new functors.
// All the functor operators taking one or two functors as parameters should
// call the macro *_OPERATOR_MAKE(Name).

#ifndef DERIVABLE_FUNCTOR_FUNCTORS_H
#define DERIVABLE_FUNCTOR_FUNCTORS_H

#include "util.h"

namespace DerivableFunctor {

// Constant
template <typename S>
struct Constant : DerivableFunctor {
    Constant(const S& value) :
      value(value) {};
    template <typename T>
    T operator()(const T& x) const {
        // Casting to T the value is a good way to evaluate this functor even
        // if S != T, given that it is possible to cast S to T.
        // TODO: There are situations when this is not the expected behavior.
        // For example when multiplying real_number * space_vector. It is not
        // possible to cast a real_number to a space_vector but it is
        // meaningful to multiply them (and the result is a space_vector).
        // The point is that the problem might be that the real_number should
        // not have been enclosed in a Constant functor in the first place.
        return static_cast<T>(value);
    }
    
    const S value;
};

// Identity
struct Identity : DerivableFunctor {
    template <typename T>
    T operator()(const T& x) const {
        return x;
    }
};

// Sum
template <typename Functor1, typename Functor2>
struct Sum : BinaryFunctorOperator<Functor1, Functor2> {
    USE_STANDARD_BINARY_OPERATOR_CONSTRUCTOR

    template <typename T>
    T operator ()(const T& x) const {
        return this->f(x) + this->g(x);
    }
};
BINARY_OPERATOR_MAKE(Sum)

// Difference
template <typename Functor1, typename Functor2>
struct Difference : BinaryFunctorOperator<Functor1, Functor2> {
    USE_STANDARD_BINARY_OPERATOR_CONSTRUCTOR

    template <typename T>
    T operator ()(const T& x) const {
        return this->f(x) - this->g(x);
    }
};
BINARY_OPERATOR_MAKE(Difference)

// Product
template <typename Functor1, typename Functor2>
struct Product : BinaryFunctorOperator<Functor1, Functor2> {
    USE_STANDARD_BINARY_OPERATOR_CONSTRUCTOR

    template <typename T>
    T operator ()(const T& x) const {
        return this->f(x) * this->g(x);
    }
};
BINARY_OPERATOR_MAKE(Product)

// Division
template <typename Functor1, typename Functor2>
struct Division : BinaryFunctorOperator<Functor1, Functor2> {
    USE_STANDARD_BINARY_OPERATOR_CONSTRUCTOR

    template <typename T>
    T operator ()(const T& x) const {
        return this->f(x) / this->g(x);
    }
};
BINARY_OPERATOR_MAKE(Division)

}  // namespace DerivableFunctor

#endif  // DERIVABLE_FUNCTOR_FUNCTORS_H

