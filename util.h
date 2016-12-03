// Collection of base classes and macros.

#ifndef DERIVABLE_FUNCTOR_UTIL_H
#define DERIVABLE_FUNCTOR_UTIL_H

#include <type_traits>

namespace DerivableFunctor {

// A type T is a Derivable Functor if it has the following property (property
// 3. cannot be fully abstracted in C++ through inheritance, templating or
// concepts as discussed in http://stackoverflow.com/q/40835599):
// 1. It must be copyable.
// 2. It must be immutable.
// 3. It must have an operator () accepting whatever type S and returning the
//    same type ( operator() S -> S).
// 4. It must exist a function D that takes T and returns a Functor.

// Every derivable functor should inherit from this one.
struct DerivableFunctor {};

// Check whether a type is inheriting from DerivableFunctor.
#define IS_FUNCTOR(Type) std::is_base_of<DerivableFunctor, Type>::value
// This static_assert will never be really needed as the real check on the
// type is done on public functions only. We use it anyway as static_asserts
// are cheap and can be useful to debug.
#define CHECK_FUNCTOR(Type) static_assert(IS_FUNCTOR(Type));

// Removes surrounding parenthesis from a value.
// Useful to remove the parenthesis added to a macro's parameter with commas.
// Example: If you need a macro that take a type and a name and prints
//      type xxx name
// you cannot simply use
//      #define print(type, name) type xxx name
// as it will not work if the type contain commas.
// Therefore you should instead use
//      #define print(type, name) UNPACK type xxx name
// and call the macro with surrounding parenthesis:
//      print((map<int,int>), foobar).
// It was copied from: http://stackoverflow.com/a/35999754/
#define UNPACK( ... ) __VA_ARGS__

// These are the base classes for functor operators taking one or two
// functors as parameters.
template <typename Functor>
struct UnaryFunctorOperator : DerivableFunctor {
    UnaryFunctorOperator(const Functor& f) :
        f(f) {
        CHECK_FUNCTOR(Functor)
    }
        
    const Functor f;
};
template <typename Functor1, typename Functor2>
struct BinaryFunctorOperator : DerivableFunctor {
    BinaryFunctorOperator(const Functor1& f, const Functor2& g) :
        f(f),
        g(g) {
        CHECK_FUNCTOR(Functor1)
        CHECK_FUNCTOR(Functor2)
    }
        
    const Functor1 f;
    const Functor2 g;
};

// Use the costructor of the base class in an child functor.
#define USE_STANDARD_UNARY_OPERATOR_CONSTRUCTOR \
using UnaryFunctorOperator<Functor>::UnaryFunctorOperator;
#define USE_STANDARD_BINARY_OPERATOR_CONSTRUCTOR \
using BinaryFunctorOperator<Functor1, Functor2>::BinaryFunctorOperator;

// Creates a utility function that construct the functor with the given
// parameters. This is necessary as a class costructor needs to have explicit
// template specialization and it might be an excruciating pain to write the
// types explicitly in given situations (namely when deriving a functor).
// In internal code only this utility functions should be used (avoid using
// public APIs or class constructors).
// Example:
// Instead of calling Sum<Type1, Type2>(f, g) we can call MakeSum(f, g).
#define UNARY_OPERATOR_MAKE(Name) \
template <typename Functor> \
Name <Functor> Make##Name(const Functor& f) { \
    return Name <Functor>(f); \
}
#define BINARY_OPERATOR_MAKE(Name) \
template <typename Functor1, typename Functor2> \
Name <Functor1, Functor2> Make##Name(const Functor1& f, const Functor2& g) { \
    return Name <Functor1, Functor2>(f, g); \
}

// If you have survived reading the code until here, it's time to surrender!
//
// Here we have macros for enforcing that templated types are functors.
// To achieve that we (ab)use SFINAE: substitution failure is not an error.
// Let's say that we have
//      template <typename T> int f(T t) { ... }
// but we would like to instantiate this function only when T is a functor
// (i.e. IS_FUNCTOR(T) = true). The syntax to do that is:
//      template <typename T> enable_if<IS_FUNCTOR(T), int>::type f(T t) { ... }
// This macros are implementing that for various kinds of possible
// signatures (multiple parameters, some should be functors and some should
// not).
// The macros' naming adheres to the following standard:
// XXX_YYY_TEMPLATE: first argument is a functor? XXX
//                   second argument is a functor? YYY
// Therefore YES_NO_TEMPLATE means that the first argument should be a functor
// whereas the second should no.
// Not all these macros are used, but for the sake of completeness we define
// all the possible YES/NO combinations.
//
// Calling these macros the ReturnType should always be surrounded by additional
// parenthesis (see UNPACK macros for further details).
#define YES_TEMPLATE(ReturnType) \
template <typename Functor> \
typename std::enable_if<IS_FUNCTOR(Functor), UNPACK ReturnType>::type

#define NO_TEMPLATE(ReturnType) \
template <typename Functor> \
typename std::enable_if<!IS_FUNCTOR(Functor), UNPACK ReturnType>::type

#define YES_YES_TEMPLATE(ReturnType) \
template <typename Functor1, typename Functor2> \
typename std::enable_if<IS_FUNCTOR(Functor1) && IS_FUNCTOR(Functor2), \
                        UNPACK ReturnType>::type
#define YES_NO_TEMPLATE(ReturnType) \
template <typename Functor1, typename T> \
typename std::enable_if<IS_FUNCTOR(Functor1) && !IS_FUNCTOR(T), \
                        UNPACK ReturnType>::type
#define NO_YES_TEMPLATE(ReturnType) \
template <typename T, typename Functor2> \
typename std::enable_if<!IS_FUNCTOR(T) && IS_FUNCTOR(Functor2), \
                        UNPACK ReturnType>::type
#define NO_NO_TEMPLATE(ReturnType) \
template <typename S, typename T> \
typename std::enable_if<!IS_FUNCTOR(S) && !IS_FUNCTOR(T), \
                        UNPACK ReturnType>::type

// Macros to define functions exposed to the user.
// What this macros are doing is enforcing that the parameters are functors and
// if they are not (and is feasible to do so) transform them into constant
// functors. This macros are needed to have implicit cast with operator between
// functors and any other type. The rationale behind the cast is that is you
// are operating with two values and one is a functor, the other should be
// treated as a constant functor.
//
// Example: The expression
//      Identity() + 1.0
// is correctly interpreted as
//      Identity() + Constant<float>(1.0)
// the public exposed operator+ has different a different behaviors depending
// on whether the second (or first) parameter is a functor.
#define PUBLIC_UNARY_OPERATOR(Name, PublicName) \
YES_TEMPLATE((Name<Functor>)) \
PublicName(const Functor& f) { \
    return Make##Name(f); \
}
#define PUBLIC_BINARY_OPERATOR(Name, PublicName) \
YES_YES_TEMPLATE((Name<Functor1, Functor2>)) \
PublicName(const Functor1& f, const Functor2& g) { \
    return Make##Name(f, g); \
} \
YES_NO_TEMPLATE((Name<Functor1, Constant<T>>)) \
PublicName(const Functor1& f, const T& value) { \
    return PublicName(f, Constant<T>(value)); \
} \
NO_YES_TEMPLATE((Name<Constant<T>, Functor2>)) \
PublicName(const T& value, const Functor2& g) { \
    return PublicName(Constant<T>(value), g); \
}

}  // namespace DerivableFunctor

#endif  // DERIVABLE_FUNCTOR_UTIL_H

