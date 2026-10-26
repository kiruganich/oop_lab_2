#include "polynomial.hpp"
#include <cmath>
#include <iostream>
#include <string>

Polynomial::Polynomial() : coeffs_(nullptr), degree_(-1) {}

Polynomial::Polynomial(const double* coeffs, int degree) : coeffs_(nullptr), degree_(degree) {
    if (degree < 0) { degree_ = -1; coeffs_ = nullptr; return; }
    allocate_and_copy(degree, coeffs);
    normalize();
}

Polynomial::Polynomial(const Polynomial& other) : coeffs_(nullptr), degree_(other.degree_) {
    if (other.coeffs_ != nullptr && other.degree_ >= 0) allocate_and_copy(other.degree_, other.coeffs_);
}

Polynomial::Polynomial(Polynomial&& other) noexcept
    : coeffs_(other.coeffs_), degree_(other.degree_) {
    other.coeffs_ = nullptr; other.degree_ = -1;
}

Polynomial::~Polynomial() { delete[] coeffs_; }

Polynomial& Polynomial::operator=(Polynomial other) noexcept { swap(other); return *this; }

void Polynomial::allocate_and_copy(int new_degree, const double* src) {
    if (new_degree < 0 || src == nullptr) { delete[] coeffs_; coeffs_ = nullptr; degree_ = -1; return; }
    double* buf = new double[new_degree + 1]{};
    for (int i = 0; i <= new_degree; ++i) buf[i] = src[i];
    delete[] coeffs_; coeffs_ = buf; degree_ = new_degree;
}

void Polynomial::normalize() {
    while (degree_ > 0 && std::fabs(coeffs_[degree_]) < 1e-12) --degree_;
    if (degree_ == 0 && std::fabs(coeffs_[0]) < 1e-12) { delete[] coeffs_; coeffs_ = nullptr; degree_ = -1; }
}

void Polynomial::trim() { normalize(); }

int Polynomial::degree() const noexcept { return degree_; }
bool Polynomial::is_zero() const noexcept { return degree_ == -1; }

void Polynomial::swap(Polynomial& other) noexcept {
    using std::swap;
    swap(coeffs_, other.coeffs_);
    swap(degree_, other.degree_);
}

Polynomial& Polynomial::add(const Polynomial& other) {
    if (other.is_zero()) return *this;
    if (is_zero()) { *this = other; return *this; }
    int nd = std::max(degree_, other.degree_);
    double* buf = new double[nd + 1]{};
    for (int i = 0; i <= degree_; ++i) buf[i] += coeffs_[i];
    for (int i = 0; i <= other.degree_; ++i) buf[i] += other.coeffs_[i];
    delete[] coeffs_; coeffs_ = buf; degree_ = nd; normalize();
    return *this;
}

Polynomial& Polynomial::mul(const Polynomial& other) {
    if (is_zero() || other.is_zero()) { *this = Polynomial(); return *this; }
    int nd = degree_ + other.degree_;
    double* buf = new double[nd + 1]{};
    for (int i = 0; i <= degree_; ++i)
        for (int j = 0; j <= other.degree_; ++j) buf[i + j] += coeffs_[i] * other.coeffs_[j];
    delete[] coeffs_; coeffs_ = buf; degree_ = nd; normalize();
    return *this;
}

double Polynomial::evaluate(double x) const {
    if (is_zero()) return 0.0;
    double r = 0.0, p = 1.0;
    for (int i = 0; i <= degree_; ++i) { r += coeffs_[i] * p; p *= x; }
    return r;
}

void Polynomial::print(std::ostream& os) const {
    if (is_zero()) { os << "0"; return; }
    bool first = true;
    for (int i = 0; i <= degree_; ++i) {
        double c = coeffs_[i];
        if (std::fabs(c) < 1e-12) continue;
        if (!first) os << (c >= 0 ? " + " : " - "), c = std::fabs(c);
        else { if (c < 0) os << "-", c = -c; first = false; }
        if (i == 0) os << c;
        else if (i == 1) os << (std::fabs(c - 1.0) < 1e-12 ? "x" : (std::to_string(c) + "x"));
        else os << (std::fabs(c - 1.0) < 1e-12 ? "x^" : (std::to_string(c) + "x^")) << i;
    }
}

Polynomial Polynomial::derivative() const {
    if (is_zero() || degree_ == 0) return Polynomial();
    int nd = degree_ - 1; double* buf = new double[nd + 1]{};
    for (int i = 1; i <= degree_; ++i) buf[i - 1] = coeffs_[i] * static_cast<double>(i);
    Polynomial r; r.allocate_and_copy(nd, buf); delete[] buf; r.normalize(); return r;
}

Polynomial Polynomial::integral() const {
    if (is_zero()) return Polynomial();
    int nd = degree_ + 1; double* buf = new double[nd + 1]{};
    buf[0] = 0.0;
    for (int i = 0; i <= degree_; ++i) buf[i + 1] = coeffs_[i] / static_cast<double>(i + 1);
    Polynomial r; r.allocate_and_copy(nd, buf); delete[] buf; r.normalize(); return r;
}

std::ostream& operator<<(std::ostream& os, const Polynomial& p) { p.print(os); return os; }

Polynomial operator+(const Polynomial& a, const Polynomial& b) { Polynomial r = a; r.add(b); return r; }
Polynomial operator*(const Polynomial& a, const Polynomial& b) { Polynomial r = a; r.mul(b); return r; }

bool operator==(const Polynomial& a, const Polynomial& b) {
    if (a.is_zero() && b.is_zero()) return true;
    if (a.is_zero() || b.is_zero()) return false;
    if (a.degree() != b.degree()) return false;
    for (int x = -5; x <= 5; ++x)
        if (std::fabs(a.evaluate(static_cast<double>(x)) - b.evaluate(static_cast<double>(x))) > 1e-9) return false;
    return true;
}

bool operator!=(const Polynomial& a, const Polynomial& b) { return !(a == b); }
