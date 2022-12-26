import numpy as np
import matplotlib.pyplot as plt
from PIL import Image

def GuassAverageMaskProcessing(pixels):
    Mask=[1,2,1,
          2,4,2,
          1,2,1]
    mid=np.array(pixels)
    Total=16
    k=1
    for i in range(k,690-k):
        for j in range(k,690-k):
            tmp=0
            for m in range(-k,k+1):
                for n in range(-k,k+1):
                    tmp=tmp+(int(pixels[i+m][j+n])*Mask[(m+k)*3+(n+k)])
            mid[i][j]=(tmp//Total)
    mid=np.reshape(mid,(690,690))
    result=Image.fromarray(mid)
    result.save('GuassProcessed.png')

image=Image.open('sample.jpg')
pixels=np.array(image.convert('L'))
result=Image.fromarray(pixels)
result.save('img_gray.png')

result=Image.fromarray(pixels[0:int(len(pixels)/2):])
result.save('img_crop.png')

for i in range(690):
    for j in range(690//2):
        tmp=pixels[i][j]
        pixels[i][j]=pixels[i][689-j]
        pixels[i][689-j]=tmp
result=Image.fromarray(pixels)
result.save('img_flip_vert.png')

GuassAverageMaskProcessing(pixels)
#高斯模糊



