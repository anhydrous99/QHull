/*! utils.h */
//
// Created by constexpr_dog on 4/29/20.
//

#ifndef QHULL_UTILS_H
#define QHULL_UTILS_H

#include <Eigen/StdVector>
#include <Eigen/Core>
#include <ostream>

// Make sure vectors of eigen vectors are memory aligned
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Eigen::Vector2d)

/*!
 * A struct representing a line segment
 */
struct LineSegment {
    Eigen::Vector2d source, target;

    LineSegment() = default;
    LineSegment(Eigen::Vector2d s, Eigen::Vector2d t);

    /*!
     * Calculates the projection of a point on the line
     * @param p The point
     * @return The projected point
     */
    Eigen::Vector2d projection(const Eigen::Vector2d &p);
};

/*!
 * Splits a vector of points based on which side of a line the points are
 * @param S1 Output vector 1
 * @param S2 Output vector 2
 * @param line The line to separate on
 * @param points The list of points to separate
 */
void split_along_line(std::vector<Eigen::Vector2d> &S1, std::vector<Eigen::Vector2d> &S2, const LineSegment &line,
                      const std::vector<Eigen::Vector2d> &points);

/*!
 * Splits a vector of points based on which side of a line the points are, ignoring the triangle
 * @param S1 Output vector 1
 * @param S2 Output vector 2
 * @param line The line to separate on
 * @param max_point The max point that forms the triangle
 * @param points The list of points to separate
 */
void split_along_line_trig(std::vector<Eigen::Vector2d> &S1, std::vector<Eigen::Vector2d> &S2, LineSegment line,
                           const Eigen::Vector2d &max_point, const std::vector<Eigen::Vector2d> &points);

/*!
 * Find the distance between a point and a line segment
 * @param p1 A point on the line
 * @param p2 Another point on the line
 * @param b The point to find the distance to
 * @return The distance
 */
double distance_line(const Eigen::Vector2d &p1, const Eigen::Vector2d &p2, const Eigen::Vector2d &b);
std::ostream &operator<<(std::ostream &o, const std::vector<Eigen::Vector2d> &v);

// From https://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

#endif //QHULL_UTILS_H
