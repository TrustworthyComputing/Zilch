#ifndef CREATE_AFFINE_HPP
#define CREATE_AFFINE_HPP

#include <NTL/GF2XFactoring.h>
#include <NTL/matrix.h>
#include <NTL/GF2EX.h>
#include <vector>

using namespace std;
using namespace NTL;

void fillAc(GF2X&);
void fillAffineVec(Vec<GF2X>&);
void fillInvAffineVec(Vec<GF2X>&);

#endif // CREATE_AFFINE_HPP
