from PIL import Image
from PIL import ImageFont
from PIL import ImageDraw
from PIL import ImageFilter
import string
import random

characters=string.ascii_letters+string.digits
def randChar():
    return random.choice(characters)
def randColor():
    return (random.randint(0,255),random.randint(0,255),random.randint(0,255))

width=60*4
height=60

image=Image.new('RGB',(width,height),(255,255,255))
imageFinal = Image.new('RGB', (width, height), (255, 255, 255))

pixelsFinal = imageFinal.load()
pixelsTemp = image.load()

for y in range(0, height):
    offset = random.randint(-1,1)
    for x in range(0, width):
        newx = x+offset
        if newx > 0 and newx <width:
            pixelsFinal[newx, y] = pixelsTemp[x, y]

font=ImageFont.truetype('D:/arial.ttf',36)

draw=ImageDraw.Draw(image)

for i in range(4):
    draw.text((60*i+10,10),randChar(),font=font,fill=randColor())
for i in range(int(width * height * 0.01)):
    draw.point((random.randint(0, width), random.randint(0, height)), fill=randColor())


for i in range(4):
    draw.line(
        [(random.randint(0, width), random.randint(0, height)), (random.randint(0, width), random.randint(0, height)),
         (random.randint(0, width), random.randint(0, height)), (random.randint(0, width), random.randint(0, height))],
        fill=randColor(), width=random.randint(0, 3))
    draw.arc((0,0,width,height)
             ,start=0+25*i,end=180+15*i
              ,fill='black',width=random.randint(0,3))
image=image.filter(ImageFilter.BLUR)
image.show()
