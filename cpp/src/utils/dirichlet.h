//
// Created by 刘也宽 on 2020/11/18.
//

#ifndef ALPHAZERO_RENJU_DIRICHLET_H
#define ALPHAZERO_RENJU_DIRICHLET_H

#include <random>
#include <vector>

template<class RNG>
class dirichlet_distribution {
public:
    dirichlet_distribution(const std::vector<double> &);

    void set_params(const std::vector<double> &);

    std::vector<double> get_params();

    std::vector<double> operator()(RNG &);

private:
    std::vector<double> alpha;
    std::vector<std::gamma_distribution<>> gamma;
};

template<class RNG>
dirichlet_distribution<RNG>::dirichlet_distribution(const std::vector<double> &alpha) {
    set_params(alpha);
}

template<class RNG>
void dirichlet_distribution<RNG>::set_params(const std::vector<double> &new_params) {
    alpha = new_params;
    std::vector<std::gamma_distribution<>> new_gamma(alpha.size());
    for (int i = 0; i < alpha.size(); ++i) {
        std::gamma_distribution<> temp(alpha[i], 1);
        new_gamma[i] = temp;
    }
    gamma = new_gamma;
}

template<class RNG>
std::vector<double> dirichlet_distribution<RNG>::get_params() {
    return alpha;
}

template<class RNG>
std::vector<double> dirichlet_distribution<RNG>::operator()(RNG &generator) {
    std::vector<double> x(alpha.size());
    double sum = 0.0;
    for (int i = 0; i < alpha.size(); ++i) {
        x[i] = gamma[i](generator);
        sum += x[i];
    }
    for (double &xi : x) xi = xi / sum;
    return x;
}

#endif //ALPHAZERO_RENJU_DIRICHLET_H
