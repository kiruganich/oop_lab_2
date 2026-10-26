#include <iostream>
#include <limits>
#include <string>
#include "polynomial.hpp"

void clear_input() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    Polynomial current;
    bool running = true;

    while (running) {
        std::cout << "1. Create zero polynomial\n"
                 << "2. Create from coefficients array\n"
                 << "3. Print current polynomial\n"
                 << "4. Polynomial info (degree / is_zero)\n"
                 << "5. Copy constructor (deep copy)\n"
                 << "6. Move constructor (source becomes zero)\n"
                 << "7. Copy assignment (copy-and-swap)\n"
                 << "8. Add another polynomial\n"
                 << "9. Multiply by another polynomial\n"
                 << "10. Evaluate at point x\n"
                 << "11. Variant 8: derivative()\n"
                 << "12. Variant 8: integral()\n"
                 << "0. Exit\n"
                 << "Enter 0-12: ";
        int choice;
        if (!(std::cin >> choice)) { clear_input(); std::cout << "Input error\n"; continue; }

        switch (choice) {
            case 0: running = false; break;
            case 1: current = Polynomial(); std::cout << "Zero polynomial created\n"; break;
            case 2: {
                std::cout << "Enter degree: "; int deg;
                if (!(std::cin >> deg) || deg < 0) { clear_input(); std::cout << "Input error\n"; break; }
                clear_input();
                std::cout << "Enter " << (deg + 1) << " coefficients:\n";
                double* coeffs = new double[deg + 1]{};
                bool ok = true;
                for (int i = 0; i <= deg; ++i) if (!(std::cin >> coeffs[i])) { ok = false; break; }
                if (!ok) { clear_input(); delete[] coeffs; std::cout << "Input error\n"; break; }
                current = Polynomial(coeffs, deg); delete[] coeffs;
                std::cout << "Polynomial created\n"; break;
            }
            case 3: {
                if (current.is_zero()) std::cout << "0\n";
                else { std::cout << "P(x) = "; current.print(std::cout); std::cout << "\n"; }
                break;
            }
            case 4: std::cout << "degree() = " << current.degree() << ", is_zero() = " << (current.is_zero() ? "true" : "false") << "\n"; break;
            case 5: {
                Polynomial copy(current);
                std::cout << "Copy created: "; copy.print(std::cout); std::cout << "\n"; break;
            }
            case 6: {
                Polynomial moved(std::move(current));
                std::cout << "Moved: "; moved.print(std::cout); std::cout << "\n";
                std::cout << "Source after move: "; if (current.is_zero()) std::cout << "0\n"; else { current.print(std::cout); std::cout << "\n"; }
                current = Polynomial(); break;
            }
            case 7: {
                std::cout << "Enter degree to assign: "; int deg;
                if (!(std::cin >> deg) || deg < 0) { clear_input(); std::cout << "Input error\n"; break; }
                clear_input();
                std::cout << "Enter " << (deg + 1) << " coefficients:\n";
                double* coeffs = new double[deg + 1]{};
                bool ok = true;
                for (int i = 0; i <= deg; ++i) if (!(std::cin >> coeffs[i])) { ok = false; break; }
                if (!ok) { clear_input(); delete[] coeffs; std::cout << "Input error\n"; break; }
                current = Polynomial(coeffs, deg); delete[] coeffs;
                std::cout << "Assigned\n"; break;
            }
            case 8: {
                std::cout << "Enter degree of polynomial to add: "; int deg;
                if (!(std::cin >> deg) || deg < 0) { clear_input(); std::cout << "Input error\n"; break; }
                clear_input();
                std::cout << "Enter " << (deg + 1) << " coefficients:\n";
                double* coeffs = new double[deg + 1]{};
                bool ok = true;
                for (int i = 0; i <= deg; ++i) if (!(std::cin >> coeffs[i])) { ok = false; break; }
                if (!ok) { clear_input(); delete[] coeffs; std::cout << "Input error\n"; break; }
                Polynomial other(coeffs, deg); delete[] coeffs;
                current.add(other); std::cout << "Added\n"; break;
            }
            case 9: {
                std::cout << "Enter degree of polynomial to multiply: "; int deg;
                if (!(std::cin >> deg) || deg < 0) { clear_input(); std::cout << "Input error\n"; break; }
                clear_input();
                std::cout << "Enter " << (deg + 1) << " coefficients:\n";
                double* coeffs = new double[deg + 1]{};
                bool ok = true;
                for (int i = 0; i <= deg; ++i) if (!(std::cin >> coeffs[i])) { ok = false; break; }
                if (!ok) { clear_input(); delete[] coeffs; std::cout << "Input error\n"; break; }
                Polynomial other(coeffs, deg); delete[] coeffs;
                current.mul(other); std::cout << "Multiplied\n"; break;
            }
            case 10: {
                if (current.is_zero()) { std::cout << "P(x) = 0 for all x\n"; break; }
                std::cout << "Enter x: "; double x;
                if (!(std::cin >> x)) { clear_input(); std::cout << "Input error\n"; break; }
                std::cout << "P(" << x << ") = " << current.evaluate(x) << "\n"; break;
            }
            case 11: {
                if (current.is_zero() || current.degree() == 0) { std::cout << "Derivative: 0\n"; break; }
                Polynomial d = current.derivative();
                std::cout << "P'(x) = "; d.print(std::cout); std::cout << "\n"; break;
            }
            case 12: {
                Polynomial integ = current.integral();
                std::cout << "Integral P(x)dx = "; integ.print(std::cout); std::cout << " + C\n"; break;
            }
            default: std::cout << "Unknown command\n"; break;
        }
    }
    return 0;
}
