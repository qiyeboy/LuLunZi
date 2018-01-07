#coding:utf-8
import subprocess

import math


def execute_command(cmd):
    print('start executing cmd...')
    s = subprocess.Popen(str(cmd), stderr=subprocess.PIPE, stdout=subprocess.PIPE, shell=True)
    stderrinfo, stdoutinfo = s.communicate()
    # print('stderrinfo is -------> %s and stdoutinfo is -------> %s' % (stderrinfo, stdoutinfo))
    # print('finish executing cmd....')
    return s.returncode



def jump(time):
    execute_command("adb shell input touchscreen swipe 170 187 170 187 " + str(time))


def screen():
    execute_command('adb shell screencap -p /sdcard/jump.png')
    execute_command('adb pull /sdcard/jump.png D:\\jump.png')


def calcDistance(x1,x2,y1,y2):
    return math.sqrt((x1 - x2) * (x1 - x2) + (y1-y2) * (y1-y2))


def calcRGB(pixel,last_pixel):
    return abs(int(pixel[0]) - int(last_pixel[0])) + abs(int(pixel[1]) - int(last_pixel[1])) + abs(int(pixel[2]) - int(last_pixel[2]))
