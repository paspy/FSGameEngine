import math

traveled = 0
upDown = 0
xspeed = 0
yspeed = 0
zspeed = 0
dist_max = 25
state = 0
speed_max = 0.05

def borg_behavior():
    sin_move()
    square_move()

def sin_move():
    global xspeed
    global yspeed
    global zspeed
    global traveled
    global upDown
    xspeed = math.sin(traveled) * 0.25
    yspeed = math.sin(traveled) * 0.1
    zspeed = math.cos(traveled) * 0.25
    traveled += 0.1


def square_move():
    global xspeed
    global zspeed
    global traveled
    global state
    global dist_max
    global speed_max

    traveled += (abs(xspeed) + abs(zspeed))
    if traveled > dist_max:
        traveled = 0
        state += 1
        state = state if state < 4 else 0

    if state == 0:
        xspeed = speed_max
        zspeed = 0
    elif state == 1:
        xspeed = 0
        zspeed = speed_max
    elif state == 2:
        xspeed = -speed_max
        zspeed = 0
    elif state == 3:
        xspeed = 0
        zspeed = -speed_max