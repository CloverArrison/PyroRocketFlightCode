#!/usr/bin/python3

'''
Copyright (C) 2026 Simon D. Levy

MIT License
'''

import argparse
from argparse import ArgumentDefaultsHelpFormatter


def beginclass(name):
    print('    class %s {\n' % name)
    print('        public:\n')


def writedefault(name):
    print('\n            %s() = default;' % name)
    print('\n            %s(' % name, end='')


def beginmethod():
    print('            {')


def endmethod():
    print('            }\n')

def endclass():
    print('    };\n')

def declare_vector(n):

    beginclass('Vector')

    print('            float', end=' ')
    for i in range(n):
        print('_%d%s' % (i, ';\n' if i == n - 1 else ', '), end='')

    writedefault('Vector')

    for i in range(n):
        print('float _%d%s' % (i, ')\n' if i == n - 1 else ', '), end='')

    print('               : ', end='')

    for i in range(n):
        print('_%d(_%d)%s' % (i, i, ' {}\n' if i == n - 1 else ', '), end='')

    endclass()


def declare_matrix(n):

    beginclass('Matrix')

    print('            float')
    for i in range(n):
        print('              ', end='')
        for j in range(n):
            print('_%d%d' % (i, j), end=', ' if i*n+j < n*n-1 else ';')
        print()

    writedefault('Matrix')

    print()

    for i in range(n):
        print('              ', end='')
        for j in range(n):
            print('float _%d%d' % (i, j), end=', ' if i*n+j < n*n-1 else ')')
        print()

    print('            :')

    for i in range(n):
        print('              ', end='')
        for j in range(n):
            print('_%d%d(_%d%d)' %
                  (i, j, i, j), end=', ' if i*n+j < n*n-1 else ' {}')
        print()

    endclass()


def declare_predict():

    print('            // P_k = F_{k-1} P_{k-1} F^T_{k-1} -------------------')
    print('            auto predict(const Matrix & F) -> Matrix')
    beginmethod()
    print('                Matrix FP;')
    print('                dot(F, P, FP);\n')
    print('                Matrix Ft;')
    print('                trans(F, Ft);\n')
    print('                dot(FP, Ft, P);')
    endmethod()


def declare_dot_ax(n):

    print('            // y = A * x')
    print('            static void dot(const Matrix & A, const Vector &x, '
          'Vector &y)')
    beginmethod()
    for i in range(n):
        print('                y._%d = ' % i, end='')
        for j in range(n):
            print('A._%d%d%s ' % (i, j, ';' if j == n - 1 else ' + '), end='')
        print()
    endmethod()


def declare_dot_ab(n):

    print('            // C = A * B')
    print('            static void dot(const Matrix & A, ' +
          'const Matrix &B, Matrix &C)')
    beginmethod()
    for i in range(n):
        for j in range(n):
            print('                C._%d%d = ' % (i, j), end='')
            for k in range(n):
                print('A._%d%d*B._%d%d%s' %
                      (i, k, k, j, ';\n' if k == n - 1 else ' + '), end='')
    endmethod()


def declare_outer(n):

    print('            // A = x * y')
    print('            static void outer(const Vector & x, ' +
          'const Vector &y, Matrix &A)')
    beginmethod()
    for i in range(n):
        print('                ', end='')
        for j in range(n):
            print('A._%d%d = x._%d*y._%d; ' % (i, j, i, j), end='')
        print()
    endmethod()


def declare_trans(n):

    print('            // At = A^T')
    print('            static void trans(const Matrix & A, Matrix & At)')
    beginmethod()
    for i in range(n):
        print('              ', end='')
        for j in range(n):
            print('At._%d%d=A._%d%d;' % (i, j, j, i), end=' ')
        print()
    endmethod()


def declare_core(n):

    beginclass('Core')

    print('            Vector x;')
    print('            Matrix P;')

    writedefault('Core')

    print('Vector & x, Matrix & P) : x(x), P(P) {}')

    # declare_predict()

    print('\n        private:\n')

    declare_dot_ax(n)
    declare_dot_ab(n)
    declare_outer(n)
    declare_trans(n)
    endclass()


def main():

    parser = argparse.ArgumentParser(
            formatter_class=ArgumentDefaultsHelpFormatter)
    parser.add_argument('n', help='Stae vector dimensionality')
    args = parser.parse_args()

    n = int(args.n)

    print('#pragma once\n')

    print('namespace tinyekf {\n')

    declare_vector(n)

    declare_matrix(n)

    declare_core(n)

    print('}\n')


main()
