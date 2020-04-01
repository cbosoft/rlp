#pragma once

#define EPSILON 1e-10
#define FLOAT_EQ(L, R) (std::fabs(L-R) < EPSILON)
#define FLOAT_APPROX(L, R, T) (std::fabs(L-R) < T)
