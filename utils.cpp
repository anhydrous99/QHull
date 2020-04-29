//
// Created by constexpr_dog on 4/29/20.
//

#include "utils.h"

// return (trig[0].x() - trig[2].x()) * (trig[1].y() - trig[2].y()) - \
//           (trig[1].x() - trig[2].x()) * (trig[0].y() - trig[2].y())
inline double sign(const Eigen::Vector2d &p1, const Eigen::Vector2d &p2, const Eigen::Vector2d &p3) {
    return (p1[0] - p3[0]) * (p2[1] - p3[1]) - (p2[0] - p3[0]) * (p1[1] - p3[1]);
}

LineSegment::LineSegment(Eigen::Vector2d s, Eigen::Vector2d t) : source(std::move(s)), target(std::move(t)) {}

Eigen::Vector2d LineSegment::projection(const Eigen::Vector2d &p) {
    double py = target[1] - source[1];
    double px = target[0] - source[0];
    double dAB = px * px + py * py;
    double u = ((p[0] - source[0]) * px + (p[1] - source[1]) * py) / dAB;
    return {source[0] + u * px, source[1] + u * py};
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

inline bool  inTriangle(const Eigen::Vector2d &t1, const Eigen::Vector2d &t2, const Eigen::Vector2d &t3, const Eigen::Vector2d &p) {
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
        if (!inTriangle(line.source, line.target, max_point, point)) {
            auto value = sign(point, line.source, line.target);
            if (value > 0)
                S1.push_back(point);
            else if (value < 0)
                S2.push_back(point);
        }
    }
}

double distance_line(const Eigen::Vector2d &p1, const Eigen::Vector2d &p2, const Eigen::Vector2d &b) {
    double y2y1 = p2[1] - p1[1];
    double x2x1 = p2[0] - p1[0];
    return std::abs(y2y1*b[0] - x2x1*b[1] + p2[0]*p1[1] + p2[1]*p1[0]) / std::sqrt(std::pow(y2y1, 2) + std::pow(x2x1, 2));
}

std::ostream &operator<<(std::ostream &o, const std::vector<Eigen::Vector2d> &v) {
    o << "[";
    for (unsigned int i = 0; i < v.size(); i++) {
        o << v[i] << ",";
    }
    o << "]";
    return o;
}
