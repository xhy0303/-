from tkinter import *

sample=Tk()
canvas=Canvas(sample,width=1600,height=800)

image1=PhotoImage(file='pic1.png')
image2=PhotoImage(file='pic2.png')

image_1=canvas.create_image(425,400,image=image1)
image_2=canvas.create_image(425+750,400,image=image2)

canvas.pack()

sample.mainloop()