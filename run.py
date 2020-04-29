from utils import redraw_plot, clear_plot, points_to_np, index_to_segment
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.backend_bases import button_press_handler
from matplotlib.figure import Figure
from scipy.spatial import ConvexHull
from random import random as r
from QHull_py import QHull as qh_py
import tkinter as tk
import skgeom as sg
import numpy as np
import QHull as qh
import time


def qh_scipy(points):
    array = points_to_np(points)
    hull = ConvexHull(array)
    segments = index_to_segment(points, hull.vertices)
    return segments


qh.calc(np.zeros((10, 2)))
points = []
root = tk.Tk()
root.resizable(False, False)
root.wm_title('Q-Hull Diagram')

fig = Figure(figsize=(5, 4), dpi=100)
subplot = fig.add_subplot(111)
subplot.set_xlim(-1, 1)
subplot.set_ylim(-1, 1)

canvas = FigureCanvasTkAgg(fig, master=root)
canvas.draw()
canvas.get_tk_widget().grid(row=0, column=0, columnspan=4)
v = tk.StringVar()
algo_impl = tk.StringVar()
v.set('100')
algo_impl.set('python')


def on_button_press(event):
    if 65 <= event.x <= 450 and 350 >= event.y >= 42:
        click_point = sg.Point2((float(event.x - 65) / (450 - 65)) * 2 - 1, (float(event.y - 42) / (350 - 42)) * 2 - 1)
        points.append(click_point)
        _draw()
    button_press_handler(event, canvas)


def _quit():
    root.quit()
    root.destroy()


def _generate():
    n_points = int(v.get())
    for i in range(n_points):
        points.append(sg.Point2(2 * r() - 1, 2 * r() - 1))
    _draw()


def _clear():
    points.clear()
    clear_plot(subplot, canvas)


def _draw():
    if len(points) != 0 and len(points) != 1 and len(points) != 2:
        des = algo_impl.get()
        t0 = time.time()
        if des == 'python':
            segments = qh_py(points)
        elif des == 'scipy':
            segments = qh_scipy(points)
        t1 = time.time()
        print(f'{(t1 - t0) * 1000} ms')
        redraw_plot(subplot, canvas, points, segments=segments)
    elif len(points) == 2:
        redraw_plot(subplot, canvas, points, segments=[sg.Segment2(points[0], points[1])])
    else:
        redraw_plot(subplot, canvas, points)


canvas.mpl_connect("button_press_event", on_button_press)

q_button = tk.Button(master=root, text='Quit', command=_quit)
q_button.grid(row=2, column=0)

c_button = tk.Button(master=root, text='clear', command=_clear)
c_button.grid(row=2, column=3)

g_button = tk.Button(master=root, text='Generate', command=_generate)
g_button.grid(row=1, column=3)

nplabel = tk.Label(master=root, text='N Random Points:')
nplabel.grid(row=1, column=1)

npentry = tk.Entry(master=root, textvariable=v)
npentry.grid(row=1, column=2)

python_radio = tk.Radiobutton(master=root, text='Python', variable=algo_impl, value='python', command=_draw)
scipy_radio = tk.Radiobutton(master=root, text='Scipy', variable=algo_impl, value='scipy', command=_draw)
python_radio.grid(row=2, column=1)
scipy_radio.grid(row=2, column=2)

tk.mainloop()
