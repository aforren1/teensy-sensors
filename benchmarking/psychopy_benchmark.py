from psychopy import visual, core,logging
from psychopy.iohub import Computer
import math
import matplotlib.pyplot as plt

num_secs = 5
num_frames = round(num_secs/0.016666) #todo: get flip interval automatically?
#times = [None] * int(num_frames)
# (400,400),
win = visual.Window((400,400), fullscr = False,
                    allowGUI = False, winType = 'pyglet')
win.setRecordFrameIntervals(True)
win._refreshThreshold=1/60.0 + 0.004
logging.console.setLevel(logging.WARNING)

clock = core.Clock()
blackframe = visual.Rect(win = win, width = 2, height = 2,
                         fillColor = (-1, -1, -1), lineColor = (-1, -1, -1))
whiteframe = visual.Rect(win = win, width = 2, height = 2,
                         fillColor = (1, 1, 1), lineColor = (1, 1, 1))
whiteframe.draw()
win.flip()
Computer.enableHighPriority()
for frameN in range(int(num_frames)):
    second_cnt = math.floor((frameN/num_frames)*num_secs);
    if second_cnt%2 == 1:
        blackframe.draw()
    else:
        whiteframe.draw()
    win.flip()
    #times[frameN-1] = clock.getTime()


Computer.disableHighPriority()
win.close()
del win.frameIntervals[0]
plt.plot(win.frameIntervals)
plt.savefig('test')
