#coding:utf-8
from time import sleep

from shell import calcDistance, jump, screen

try:
    # for Python2
    import Tkinter as tk   ## notice capitalized T in Tkinter
except ImportError:
    # for Python3
    import tkinter as tk   ## notice lowercase 't' in tkinter here
from PIL import Image, ImageTk

isFirst=True
preX=0
preY=0

def callBack(event):
    global isFirst,preX,preY,label,tk_image
    # print(event.x_root, event.y_root)
    if isFirst:
        preX = event.x_root
        preY = event.y_root
        isFirst= False
    else:
        print(event.x_root,preX,event.y_root,preY)
        distance = calcDistance(event.x_root,preX,event.y_root,preY)

        time = (int)(distance / 0.35)
        jump(time)
        isFirst = True
        print("distance:" + str(distance) + ",time:" + str(time))
        try:
            sleep(1)
        except Exception as e:
            print(e)
        screen()
        pil_image = Image.open("D:\\jump.png")
        # 获取图像的原始大小
        w, h = pil_image.size
        # 缩放图像让它保持比例，同时限制在一个矩形框范围内
        pil_image_resized = resize(w, h, w_box, h_box, pil_image)

        # 也可以显示缩放后的图像信息，获取大小
        wr, hr = pil_image_resized.size
        # convert PIL image object to Tkinter PhotoImage object
        # 把PIL图像对象转变为Tkinter的PhotoImage对象
        tk_image = ImageTk.PhotoImage(pil_image_resized)
        label.configure(image=tk_image,width=w_box, height=h_box)
        # label = tk.Label(root, image=tk_image, width=w_box, height=h_box)



        # padx,pady是图像与窗口边缘的距离
        # label.pack(padx=5, pady=5)






def resize(w, h, w_box, h_box, pil_image):
    '''''
    resize a pil_image object so it will fit into
    a box of size w_box times h_box, but retain aspect ratio
    对一个pil_image对象进行缩放，让它在一个矩形框内，还能保持比例
    '''

    f1 = 1.0 * w_box / w  # 1.0 forces float division in Python2
    f2 = 1.0 * h_box / h
    factor = min([f1, f2])
    # print(f1, f2, factor) # test
    # use best down-sizing filter
    width = int(w * factor)
    height = int(h * factor)
    return pil_image.resize((width, height), Image.ANTIALIAS)


root = tk.Tk()
root.title("跳一跳游戏辅助")
# size of image display box you want
# 期望图像显示的大小

screen()#截屏
pil_image = Image.open("D:\\jump.png")
# 获取图像的原始大小
w, h = pil_image.size
w_box = w/2
h_box = h/2
# 缩放图像让它保持比例，同时限制在一个矩形框范围内
pil_image_resized = resize(w, h, w_box, h_box, pil_image)

# 也可以显示缩放后的图像信息，获取大小
wr, hr = pil_image_resized.size
# convert PIL image object to Tkinter PhotoImage object
# 把PIL图像对象转变为Tkinter的PhotoImage对象
tk_image = ImageTk.PhotoImage(pil_image_resized)
# put the image on a widget the size of the specified display box
# Label: 这个小工具，就是个显示框，小窗口，把图像大小显示到指定的显示框
label = tk.Label(root, image=tk_image, width=w_box, height=h_box)

# padx,pady是图像与窗口边缘的距离
# label.pack(padx=5, pady=5)

label.pack()
label.bind("<Button-1>",callBack)

root.mainloop()