from time import sleep

from dealimage import getDistance
from shell import screen, jump


while True:
    screen()#截屏
    distance = getDistance()
    # time = (int)(distance*1.372)
    time = (int)(distance * 1.372)
    jump(time)
    print("distance:" + str(distance) + ",time:" + str(time))
    try:
        sleep(0.9)
    except Exception as e:
        print(e)