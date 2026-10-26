#pragma once
#include <cstddef>
#include <ostream>
#include <utility>

class Polynomial {
public:
    Polynomial();
    Polynomial(const double* coeffs, int degree);
    Polynomial(const Polynomial& other);
    Polynomial(Polynomial&& other) noexcept;
    ~Polynomial();
    Polynomial& operator=(Polynomial other) noexcept;

    Polynomial& add(const Polynomial& other);
    Polynomial& mul(const Polynomial& other);
    double evaluate(double x) const;
    void print(std::ostream& os) const;

    Polynomial derivative() const;
    Polynomial integral() const;

    int degree() const noexcept;
    bool is_zero() const noexcept;
    void trim();
    void swap(Polynomial& other) noexcept;

private:
    double* coeffs_;
    int degree_;

    void allocate_and_copy(int new_degree, const double* src);
    void normalize();
};

std::ostream& operator<<(std::ostream& os, const Polynomial& p);
Polynomial operator+(const Polynomial& a, const Polynomial& b);
Polynomial operator*(const Polynomial& a, const Polynomial& b);
bool operator==(const Polynomial& a, const Polynomial& b);
bool operator!=(const Polynomial& a, const Polynomial& b);
