//
// Created by Armando Herrera on 4/29/20.
//

#include "QHull.h"
#include "utils.h"
#include <algorithm>
#include <iostream>

void FindHull(const std::vector<Eigen::Vector2d> &Sk, const Eigen::Vector2d &P, const Eigen::Vector2d &Q,
              std::vector<Eigen::Vector2d> &convex_hull);

Eigen::MatrixXd QHull(Eigen::MatrixXd points) {
    std::vector<Eigen::Vector2d> convex_hull;
    std::vector<Eigen::Vector2d> check_points;
    for (unsigned int i = 0; i < points.rows(); i++)
        check_points.emplace_back(points.row(i));
//    std::sort(check_points.begin(), check_points.end(), [](const Eigen::Vector2d &a, const Eigen::Vector2d &b) {
//        return a[0] < b[0];
//    });
//    convex_hull.push_back(check_points[0]);
//    convex_hull.push_back(check_points.back());
//    check_points.erase(check_points.begin());
//    check_points.erase(check_points.end() - 1);
    auto [min_it, max_it] = std::minmax_element(check_points.begin(), check_points.end(), [](const Eigen::Vector2d &a, const Eigen::Vector2d &b) {
        return a.x() < b.x();
    });
    convex_hull.push_back(*min_it);
    convex_hull.push_back(*max_it);
    // Create line segment from them
    LineSegment segment(convex_hull[0], convex_hull[1]);

    // Split points based on side from line
    std::vector<Eigen::Vector2d> S1, S2;
    split_along_line(S1, S2, segment, check_points);

    FindHull(S1, segment.source, segment.target, convex_hull);
    FindHull(S2, segment.target, segment.source, convex_hull);

    Eigen::MatrixXd hull_matrix = Eigen::MatrixXd::Zero(convex_hull.size(), 2);
    for (unsigned int i = 0; i < convex_hull.size(); i++) {
        hull_matrix(i, 0) = convex_hull[i][0];
        hull_matrix(i, 1) = convex_hull[i][1];
    }
    return hull_matrix;
}

void FindHull(const std::vector<Eigen::Vector2d> &Sk, const Eigen::Vector2d &P, const Eigen::Vector2d &Q,
              std::vector<Eigen::Vector2d> &convex_hull) {
    // If empty return
    if (Sk.empty())
        return;
    // Find maximum element
    /*double max_element = 0;
    auto max_it = Sk.begin();
    for (auto it = Sk.begin(); it < Sk.end(); it++) {
        auto element = distance_line(P, Q, *it);
        if (max_element < element) {
            max_element = element;
            max_it = it;
        }
    }*/
    auto max_it = std::max_element(Sk.begin(), Sk.end(), [&](const Eigen::Vector2d &a, const Eigen::Vector2d &b) {
        return distance_line(P, Q, a) < distance_line(P, Q, b);
    });
    Eigen::Vector2d max_point = *max_it;
    //Sk.erase(max_it);

    convex_hull.insert(std::find(convex_hull.begin(), convex_hull.end(), Q), max_point);

    // Split points based on side of line while not considering points in triangle
    std::vector<Eigen::Vector2d> S1, S2;
    split_along_line_trig(S1, S2, LineSegment(P, Q), max_point, Sk);
    FindHull(S1, P, max_point, convex_hull);
    FindHull(S2, max_point, Q, convex_hull);
}
