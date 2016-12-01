#include <iostream>
#include "public.h"

using namespace DerivableFunctor;

int main() {
    // const auto& f = X*X;
    const auto& f = X*X + X;
    // const auto& f = Sum<Product<Identity, Identity>, Identity>(Product<Identity, Identity>(Identity(), Identity()),  Identity());
    // D(f);
    // const auto& f = ((X*X)+X+1) * ((X*X)+X);
    // const auto& g = D(f);
    const auto g = D(f);
    // std::cout << g(5);
}
