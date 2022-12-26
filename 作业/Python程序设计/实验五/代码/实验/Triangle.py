import math

def isValid(side1, side2, side3):
    if side1+side2>side3 and side1+side3>side2 and side2+side3>side1 and abs(side1-side2)<side3 and abs(side1-side3)<side2 and abs(side2-side3)<side1:
        print("%d,%d,%d可以构成三角形"%(side1,side2,side3))
        return True
    else:
        print("%d,%d,%d不可以构成三角形"%(side1,side2,side3))
        return False

def perimeter(side1, side2, side3):
    print("该三角形周长为：%d"%(side1+side3+side2))

def area(side1, side2, side3):
    p=(side1+side3+side2)/2
    S=math.sqrt(p*(p-side2)*(p-side3)*(p-side1))
    print("该三角形面积为：%.2f"%S)