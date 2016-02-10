from psychopy import visual, core

num_secs = 5
# (400,400),
win = visual.Window((400,400),fullscr = False, allowGUI = False, winType = 'pyglet')
clock = core.Clock()
blackframe = visual.Rect(win = win, width = 2, height = 2,
                         fillColor = (-1, -1, -1), lineColor = (-1, -1, -1))
whiteframe = visual.Rect(win = win, width = 2, height = 2,
                         fillColor = (1, 1, 1), lineColor = (1, 1, 1))
whiteframe.draw()
win.flip()
num_frames = round(num_secs/0.016666)
for frameN in range(int(num_frames)):
    second_cnt = round((frameN/num_frames)*num_secs);
    if second_cnt%2 == 1:
        blackframe.draw()
    else:
        whiteframe.draw()
    win.flip()

win.close()
