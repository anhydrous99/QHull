//
// Created by constexpr_dog on 4/29/20.
//

#include "utils.h"

LineSegment::LineSegment(Eigen::Vector2d s, Eigen::Vector2d t) : source(std::move(s)), target(std::move(t)) {}

Eigen::Vector2d LineSegment::projection(const Eigen::Vector2d &p) {
    double ts1 = target[1] - source[1];
    double ts0 = target[0] - source[0];

    double t = ((p[0] - source[0]) * (ts0) + (p[1] - source[1]) * ts1) / (std::pow(ts0, 2) + std::pow(ts1, 2));
    return {source[0] + t * (ts0), source[1] + t * ts1};
}

////
// From: https://stackoverflow.com/questions/13290395/how-to-remove-a-certain-row-or-column-while-using-eigen-library-c
void removeRow(Eigen::MatrixXd &matrix, unsigned int rowToRemove) {
    unsigned int numRows = matrix.rows() - 1;
    unsigned int numCols = matrix.cols();

    if (rowToRemove < numRows)
        matrix.block(rowToRemove, 0, numRows - rowToRemove, numCols) = matrix.bottomRows(numRows - rowToRemove);

    matrix.conservativeResize(numRows, numCols);
}

void removeColumn(Eigen::MatrixXd &matrix, unsigned int colToRemove) {
    unsigned int numRows = matrix.rows();
    unsigned int numCols = matrix.cols() - 1;

    if (colToRemove < numCols)
        matrix.block(0, colToRemove, numRows, numCols - colToRemove) = matrix.rightCols(numCols - colToRemove);

    matrix.conservativeResize(numRows, numCols);
}
////

void split_along_line(std::vector<Eigen::Vector2d> &S1, std::vector<Eigen::Vector2d> &S2, const LineSegment &line,
                      const Eigen::MatrixXd &points) {
    // Compute all signs at one, makes use of vectorization
    Eigen::VectorXd signs = Eigen::sign((line.target[0] - line.source[0]) * (points.col(1).array() - line.source[1]) -
                                        (line.target[1] - line.source[1]) * (points.col(0).array() - line.source[0]));
    for (unsigned int i = 0; i < points.rows(); i++) {
        if (signs[i] > 0)
            S1.emplace_back(points.row(i));
        else if (signs[i] < 0)
            S2.emplace_back(points.row(i));
    }
}

// return (trig[0].x() - trig[2].x()) * (trig[1].y() - trig[2].y()) - \
//           (trig[1].x() - trig[2].x()) * (trig[0].y() - trig[2].y())
inline double sign(const Eigen::Vector2d &p1, const Eigen::Vector2d &p2, const Eigen::Vector2d &p3) {
    return (p1[0] - p3[0]) * (p2[1] - p3[1]) - (p2[0] - p3[0]) * (p1[1] - p3[1]);
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
    LineSegment projline(line.projection(max_point), max_point);
    double ts0 = line.target[0] - line.source[0];
    double ts1 = line.target[1] - line.source[1];
    for (const auto &point : points) {
        if (!inTriangle(line.source, line.target, max_point, point)) {
            double value = sgn(ts0 * (point[1] - line.source[1]) - ts1 * (point[0] - line.source[0]));
            if (value > 0)
                S1.emplace_back(point);
            else if (value < 0)
                S2.emplace_back(point);
        }
    }
}
