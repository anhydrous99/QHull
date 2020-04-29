//
// Created by constexpr_dog on 4/29/20.
//

#ifndef QHULL_UTILS_H
#define QHULL_UTILS_H

#include <Eigen/StdVector>
#include <Eigen/Core>
#include <vector>

// Make sure vectors of eigen vectors are memory aligned
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Eigen::Vector2d)

struct LineSegment {
    Eigen::Vector2d source, target;

    LineSegment() = default;
    LineSegment(Eigen::Vector2d s, Eigen::Vector2d t);
    Eigen::Vector2d projection(const Eigen::Vector2d &p);
};

void removeRow(Eigen::MatrixXd& matrix, unsigned int rowToRemove);
void removeColumn(Eigen::MatrixXd& matrix, unsigned int colToRemove);
void split_along_line(std::vector<Eigen::Vector2d> &S1, std::vector<Eigen::Vector2d> &S2, const LineSegment &line,
                      const Eigen::MatrixXd &points);
void split_along_line_trig(std::vector<Eigen::Vector2d> &S1, std::vector<Eigen::Vector2d> &S2, LineSegment line,
                           const Eigen::Vector2d &max_point, const std::vector<Eigen::Vector2d> &points);

// From https://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

#endif //QHULL_UTILS_H
