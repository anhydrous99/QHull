import math
import numpy as np
import skgeom as sg
from typing import List, Tuple


def draw_point(plt, point, **kwargs):
    """
    Reimplementation of scikit-geometry's point drawing function where the plotting is done on a subplot instead
    :param plt:
    :param point:
    :param kwargs:
    :return:
    """
    plt.scatter([float(point.x())], [float(point.y())], **kwargs)


def draw_list(plt, point_list, **kwargs):
    """
    Reimplementation of scikit-geometry's point list drawing function where the plotting is done on a subplot instead
    :param plt:
    :param point_list:
    :param kwargs:
    :return:
    """
    for point in point_list:
        draw_point(plt, point, **kwargs)


def draw_segment(plt, segment, visible_point=True, **kwargs):
    """
    Reimplementation of scikit-geometry's segment drawing function where the plotting is done on a subplot instead
    :param plt:
    :param segment:
    :param visible_point:
    :param kwargs:
    :return:
    """
    plt.plot(
        [segment.source().x(), segment.target().x()],
        [segment.source().y(), segment.target().y()],
        **kwargs
    )
    if visible_point:
        draw_list(plt, [segment.source(), segment.target()], **kwargs)


def draw_segments(plt, segments, visible_point=True, **kwargs):
    """
    Plots a list of segments
    :param plt:
    :param segments: A List of segments
    :param visible_point:
    :param kwargs:
    :return:
    """
    for segment in segments:
        draw_segment(plt, segment, visible_point=visible_point, **kwargs)


def draw_circle(plt, circle, fill=False, **kwargs):
    """
    Reimplementation of scikit-geometry's circle drawing function where the plotting is done on a subplot instead
    :param plt:
    :param circle:
    :param fill:
    :param kwargs:
    :return:
    """
    fig, ax = plt.gcf(), plt.gca()
    center = circle.center()
    center_x = float(center.x())
    center_y = float(center.y())
    radius = math.sqrt(float(circle.squared_radius()))
    circle_displayed = plt.Circle((center_x, center_y), radius, fill=fill, **kwargs)
    ax.add_artist(circle_displayed)
    ax.set_aspect('equal')


def redraw_plot(plot, canvas, points, segments=None, xlim=(-1, 1), ylim=(-1, 1), redraw=True, point_color='r',
                line_color='b', **kwargs):
    """
    Redraws a plot from points, and segments, on a [xlim, ylim] interval
    :param plot:
    :param canvas:
    :param points:
    :param segments:
    :param xlim:
    :param ylim:
    :param redraw:
    :param point_color:
    :param line_color:
    :param kwargs:
    :return:
    """
    clear_plot(plot, canvas, xlim, ylim, redraw=False)
    if segments is not None:
        draw_segments(plot, segments, visible_point=False, c=line_color)
    draw_list(plot, points, c=point_color, **kwargs)
    if redraw:
        canvas.draw()


def clear_plot(plot, canvas, xlim=(-1, 1), ylim=(-1, 1), redraw=True):
    """
    A function that clears the canvas with the added benefit where the xlim and ylim is set for the canvas
    :param plot:
    :param canvas:
    :param xlim:
    :param ylim:
    :param redraw:
    """
    plot.clear()
    plot.set_xlim(*xlim)
    plot.set_ylim(*ylim)
    if redraw:
        canvas.draw()


def points_to_np(points: List[sg.Point2]) -> np.ndarray:
    """
    Converts a list of points to a 2D numpy array
    :param points:
    :return: A 2D numpy array
    """
    points_array = None
    if isinstance(points[0], sg.Point2):
        points_array = np.zeros((len(points), 2))
        for idx, point in enumerate(points):
            points_array[idx, 0] = point.x()
            points_array[idx, 1] = point.y()
    elif isinstance(points[0], sg.Point3):
        points_array = np.zeros((len(points), 3))
        for idx, point in enumerate(points):
            points_array[idx, 0] = point.x()
            points_array[idx, 1] = point.y()
            points_array[idx, 2] = point.z()
    return points_array


def index_to_segment(points: List[sg.Point2], indexes: List[int]) -> List[sg.Segment2]:
    """
    Converts a list of point indexes to a list of segments representing the Polygon
    :param points:
    :param indexes:
    :return: A list of line segments
    """
    segments = []
    for idx in range(len(indexes) - 1):
        segments.append(sg.Segment2(points[indexes[idx]], points[indexes[idx + 1]]))
    segments.append(sg.Segment2(points[indexes[0]], points[indexes[-1]]))
    return segments


def points_to_segment(points: List[sg.Point2]):
    segments = []
    for idx in range(len(points) - 1):
        segments.append(sg.Segment2(points[idx], points[idx + 1]))
    segments.append(sg.Segment2(points[0], points[-1]))
    return segments


def seg_side(segment: sg.Segment2, point: sg.Point2) -> int:
    """
    Returns 0 if on line, +1 on one side, and -1 on the other side
    :param segment:
    :param point:
    :return: 0, +1, and -1, depending on side or on point
    """
    return np.sign((segment.target().x() - segment.source().x()) * (point.y() - segment.source().y()) -
                   (segment.target().y() - segment.source().y()) * (point.x() - segment.source().x()))


def split_points(points: List[sg.Point2], segment: sg.Segment2) -> Tuple[List[sg.Point2], List[sg.Point2]]:
    a = []
    b = []
    for point in points:
        value = seg_side(segment, point)
        if value > 0:  # ignore points on the line
            a.append(point)
        elif value < 0:
            b.append(point)
    return (a, b)


def sign(trig: Tuple[sg.Point2, sg.Point2, sg.Point2]) -> float:
    return (trig[0].x() - trig[2].x()) * (trig[1].y() - trig[2].y()) - \
           (trig[1].x() - trig[2].x()) * (trig[0].y() - trig[2].y())


def triangle_isInside(trig: Tuple[sg.Point2, sg.Point2, sg.Point2], point: sg.Point2):
    d1 = sign((point, trig[0], trig[1]))
    d2 = sign((point, trig[1], trig[2]))
    d3 = sign((point, trig[2], trig[0]))
    has_neg = (d1 < 0) or (d2 < 0) or (d3 < 0)
    has_pos = (d1 > 0) or (d2 > 0) or (d3 > 0)
    return not (has_neg and has_pos)


def split_points_triangle(points: List[sg.Point2], trig: Tuple[sg.Point2, sg.Point2, sg.Point2]) -> Tuple[
    List[sg.Point2], List[sg.Point2]]:
    a = []
    b = []
    proj = sg.Line2(trig[0], trig[1]).projection(trig[2])
    proj_seg = sg.Segment2(proj, trig[2])
    for point in points:
        if not triangle_isInside(trig, point):
            value = seg_side(proj_seg, point)
            if value > 0:
                a.append(point)
            elif value < 0:
                b.append(point)
    return (a, b)
