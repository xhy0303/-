# 引入头文件
from tkinter import *
from random import *

# 定义回调函数
def DisplayBall():
    # 清除画布上原有小球
    canvas.delete('all')
    # 利用循环生成10个小球
    for i in range(10):
        # 随机生成小球圆心坐标
        Pos_x = randint(50, 450)
        Pos_y = randint(50, 250)
        # 定义小球的半径
        radius = 7
        # 获取随机颜色
        r = lambda: randint(0, 255)
        color = "#%02x%02x%02x" % (r(), r(), r())
        # 将小球添加到画布上
        canvas.create_oval(Pos_x, Pos_y, Pos_x + radius, Pos_y + radius, fill=color, outline="black", tag="ball")


sample = Tk()
sample.title('Random balls')

Label(sample, text="click button to get balls!").pack()  # 在窗口中显示标签

sample.minsize(600, 400)  # 冻结窗口大小
sample.maxsize(600, 400)

canvas=Canvas(sample, width=500, height=300)
canvas.pack()  # 添加画布


Button(sample, text="Display", command=DisplayBall).pack()  # 在窗口中显示按钮

sample.mainloop()

