// Contains derivatives of all functors in functors.h.
//
// As in functors.h, the least amount of boilerplate code is present.
// Differently from functors.h, the auto keyword should be used as much as
// possible as otherwise writing explicitly all types is a good way to
// go crazy.

#ifndef DERIVABLE_FUNCTOR_DERIVATIVES_H
#define DERIVABLE_FUNCTOR_DERIVATIVES_H

#include "util.h"
#include "functors.h"

namespace DerivableFunctor {
// ACHTUNG:
// If we work in an additional namespace (i.e. namespace Derivative) the
// compiler is going to complain. The reason is that ADL (argument dependent
// lookup) will not work but derivative functions needs it as they
// are calling each other without respecting the order of declaration.

// Constant
template <typename S>
auto D(const Constant<S> /* constant value is ininfluent */) {
    // The chosen type is int as almost anything that someone might want
    // to derive should be constructible given an integer.
    // Furthermore there is absolutely no way to choose this type any better,
    // so we just stick with it.
    return Constant<int>(0);
}

// Identity
auto D(Identity /* empty struct */) {
    // See the derivatives of a constant for an explanation of the 'int' type.
    return Constant<int>(1);
}

// Sum
template <typename Functor1, typename Functor2>
auto D(const Sum<Functor1, Functor2>& s) {
    return MakeSum(D(s.f), D(s.g)); 
}

// Difference
template <typename Functor1, typename Functor2>
auto D(const Difference<Functor1, Functor2>& s) {
    return MakeDifference(D(s.f), D(s.g)); 
}

// Product
template <typename Functor1, typename Functor2>
auto D(const Product<Functor1, Functor2>& s) {
    return MakeSum(MakeProduct(D(s.f), s.g), MakeProduct(s.f, D(s.g))); 
}

// Division
template <typename Functor1, typename Functor2>
auto D(const Division<Functor1, Functor2>& s) {
    return MakeDivision(
        MakeDifference(MakeProduct(D(s.f), s.g), MakeProduct(s.f, D(s.g))),
        MakeProduct(s.g, s.g)); 
}

// Composition
template <typename Functor1, typename Functor2>
auto D(const Composition<Functor1, Functor2>& s) {
    return MakeProduct(
        MakeComposition(D(s.f), s.g),
        D(s.g)); 
}

}  // namespace DerivableFunctor

#endif  // DERIVABLE_FUNCTOR_DERIVATIVES_H

