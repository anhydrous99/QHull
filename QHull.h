/*! QHull.h */
//
// Created by Armando Herrera on 4/29/20.
//

#ifndef QHULL_QHULL_H
#define QHULL_QHULL_H

#include <Eigen/Core>

/*!
 * Calculates the convex hull of some points
 * @param points The points
 * @return A list of line segments
 */
Eigen::MatrixXd QHull(Eigen::MatrixXd points);

#endif //QHULL_QHULL_H
