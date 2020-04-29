//
// Created by Armando Herrera on 4/29/20.
//

#include "QHull.h"
#include "utils.h"
#include <algorithm>

void FindHull(std::vector<Eigen::Vector2d> Sk, const Eigen::Vector2d &P, const Eigen::Vector2d &Q,
              std::vector<Eigen::Vector2d> &convex_hull);

Eigen::MatrixXd QHull(Eigen::MatrixXd points) {
    std::vector<Eigen::Vector2d> convex_hull;

    // Find min and max index
    unsigned int maxIndex, minIndex;
    points.col(0).maxCoeff(&maxIndex);
    points.col(0).minCoeff(&minIndex);
    // Save those points
    convex_hull.emplace_back(points.row(minIndex));
    convex_hull.emplace_back(points.row(maxIndex));
    // Remove said points from matrix
    removeRow(points, maxIndex);
    removeRow(points, minIndex);
    // Create line segment from them
    LineSegment segment(convex_hull[0], convex_hull[1]);

    // Split points based on side from line
    std::vector<Eigen::Vector2d> S1, S2;
    split_along_line(S1, S2, segment, points);

    FindHull(S1, segment.source, segment.target, convex_hull);
    FindHull(S2, segment.target, segment.source, convex_hull);
    return points;
}

void FindHull(std::vector<Eigen::Vector2d> Sk, const Eigen::Vector2d &P, const Eigen::Vector2d &Q,
              std::vector<Eigen::Vector2d> &convex_hull) {
    // If empty return
    if (Sk.empty())
        return;
    // Find maximum element
    auto max_itr = std::max_element(Sk.begin(), Sk.end(), [](const Eigen::Vector2d &a, const Eigen::Vector2d &b) {
        return std::pow(b[0] - a[0] , 2.0) + std::pow(b[1] - b[0], 2.0);
    });
    Eigen::Vector2d max_point = *max_itr;
    convex_hull.insert(std::find(convex_hull.begin(), convex_hull.end(), P), max_point);
    Sk.erase(max_itr);

    // Split points based on side of line while not considering points in triangle
    std::vector<Eigen::Vector2d> S1, S2;
    split_along_line_trig(S1, S2, LineSegment(P, Q), max_point, Sk);
    FindHull(S1, P, max_point, convex_hull);
    FindHull(S2, max_point, Q, convex_hull);
}
