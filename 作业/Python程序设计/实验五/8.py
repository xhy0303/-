import numpy as np
from PIL import Image

def GuassAverageMaskProcessing(pixels):
    Mask=[1,2,1,
          2,4,2,
          1,2,1]#权重
    mid=np.array(pixels)#用于保存处理后的图像
    Total=16#权重和
    k=1
    for i in range(k,690-k):
        for j in range(k,690-k):#遍历
            tmp=0
            for m in range(-k,k+1):
                for n in range(-k,k+1):
                    tmp=tmp+(int(pixels[i+m][j+n])*Mask[(m+k)*3+(n+k)])
            mid[i][j]=(tmp//Total)#求均值后修改该点像素
    mid=np.reshape(mid,(690,690))
    result=Image.fromarray(mid)
    result.save('GuassProcessed.png')#保存图像

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



