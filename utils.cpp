//
// Created by constexpr_dog on 4/29/20.
//

#include "utils.h"
#include <algorithm>

double sign(const Eigen::Vector2d &p1, const Eigen::Vector2d &p2, const Eigen::Vector2d &p3) {
    return (p1.x() - p3.x()) * (p2.y() - p3.y()) - (p2.x() - p3.x()) * (p1.y() - p3.y());
}

LineSegment::LineSegment(Eigen::Vector2d s, Eigen::Vector2d t) : source(std::move(s)), target(std::move(t)) {}

Eigen::Vector2d LineSegment::projection(const Eigen::Vector2d &p) {
    double bax = target.x() - source.x();
    double bay = target.y() - source.y();
    double bax2 = bax * bax;
    double bay2 = bay * bay;
    double t = ((p.x() - source.x()) * bax + (p.y() - source.y()) * bay) / (bax2 + bay2);
    Eigen::Vector2d output = {source.x() + t * bax, source.y() + t * bay};
    return output;
}

void split_along_line(std::vector<Eigen::Vector2d> &S1, std::vector<Eigen::Vector2d> &S2, const LineSegment &line,
                      const std::vector<Eigen::Vector2d> &points) {
    for (const auto &point : points) {
        auto value = sign(point, line.source, line.target);
        if (value > 0)
            S1.push_back(point);
        else if (value < 0)
            S2.push_back(point);
    }
}

inline bool inTriangle(const Eigen::Vector2d &p, const Eigen::Vector2d &t1, const Eigen::Vector2d &t2, const Eigen::Vector2d &t3) {
    double d1 = sign(p, t1, t2);
    double d2 = sign(p, t2, t3);
    double d3 = sign(p, t3, t1);
    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
    return !(has_neg && has_pos);
}

void split_along_line_trig(std::vector<Eigen::Vector2d> &S1, std::vector<Eigen::Vector2d> &S2, LineSegment line,
                           const Eigen::Vector2d &max_point, const std::vector<Eigen::Vector2d> &points) {
    // Calculate projection
    LineSegment projline(max_point, line.projection(max_point));
    for (const auto &point : points) {
        if (!inTriangle(point, line.source, line.target, max_point)) {
            auto value = sign(point, projline.source, projline.target);
            if (value < 0)
                S1.push_back(point);
            else if (value > 0)
                S2.push_back(point);
        }
    }
}

double distance_line(const Eigen::Vector2d &p1, const Eigen::Vector2d &p2, const Eigen::Vector2d &b) {
    const double l2 = std::pow(p2.x() - p1.x(), 2.0) + std::pow(p2.y() - p1.y(), 2.0);
    if (l2 == 0) return (p1 - b).norm();
    const float t = std::max(0.0, std::min(1.0, (b - p1).dot(p2 - p1) / l2));
    const Eigen::Vector2d projection = p1 + t * (p2 - p1);
    return (projection - b).norm();
}

std::ostream &operator<<(std::ostream &o, const std::vector<Eigen::Vector2d> &v) {
    o << "[";
    for (unsigned int i = 0; i < v.size(); i++) {
        o << v[i] << ",";
    }
    o << "]";
    return o;
}
