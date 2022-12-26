import math
class RegularPolygon:
    __n=3
    __side=1
    __x=0
    __y=0
    def __init__(self,n,side,x,y):
        self.__n=n
        self.__side=side
        self.__x=x
        self.__y=y
    def getPerimeter(self):
        return self.__n*self.__side
    def getArea(self):
        return ((self.__n*pow(self.__side,2)*math.sin((2*math.pi)/self.__n))/(4*(1-math.cos((2*math.pi)/self.__n))))
    def distanceToPolygon(self,a):
        return math.sqrt(pow(self.__x-a.getX(),2)+pow(self.__y-a.getY(),2))

    def getX(self):
        return self.__x
    def getY(self):
        return self.__y


a=RegularPolygon(3,1,1,1)
b=RegularPolygon(6,2,5,6)
print(a.distanceToPolygon(b))
print(a.getArea())
print(b.getArea())
print(a.getPerimeter())
print(b.getPerimeter())