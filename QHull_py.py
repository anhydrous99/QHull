import copy
import skgeom as sg
from typing import List
from utils import split_points, split_points_triangle, points_to_segment


def QHull(points: List[sg.Point2]) -> List[sg.Segment2]:
    """
    Finds the convex hull via QuickHull
    :param points: A list of points to find the convex hull for
    :return: A list of line segments
    """
    point_list = copy.copy(points)
    hull_points = []
    points.sort(key=lambda point: point.x())
    mn = points[0]
    mx = points[-1]
    hull_points.append(mn)
    hull_points.append(mx)
    point_list.remove(mn)
    point_list.remove(mx)
    seg = sg.Segment2(mn, mx)
    # a line between the left most and right most point
    s1, s2 = split_points(point_list, seg)
    _FindHull(s1, mn, mx, hull_points)
    _FindHull(s2, mx, mn, hull_points)
    return points_to_segment(hull_points)


def _FindHull(s: List[sg.Point2], p: sg.Point2, q: sg.Point2, hull_points: List[sg.Point2]):
    """
    A helper function for the QuickHull, uses recursion
    :param s:
    :param p:
    :param q:
    :param hull_points:
    :return:
    """
    if len(s) == 0:
        return
    seg = sg.Segment2(p, q)
    c = max(s, key=lambda point: sg.squared_distance(seg, point))
    hull_points.insert(hull_points.index(p) + 1, c)
    s.remove(c)
    s1, s2 = split_points_triangle(s, (p, q, c))
    _FindHull(s1, p, c, hull_points)
    _FindHull(s2, c, q, hull_points)
