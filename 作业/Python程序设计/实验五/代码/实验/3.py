class Stack:
    __content=[]
    __size=0
    __current= len(__content)
    def __init__(self,size):
        self.__size=size
    def isempty(self):
        if self.__current==0:
            return True
        else:
            return False
    def empty(self):
        self.__content.clear()
        self.__current=0
    def setSize(self,size):
        self.__size=size
        for i in range(len(self.__content)):
            if i>self.__size:
                self.__content.remove(self.__content[i])
    def isFull(self):
        if(self.__size==self.__current):
            return True
        else:
            return False

    def push(self,value):
        self.__current+=1
        self.__content.append(value)
    def pop(self):
        self.__content.remove(self.__content[self.__current-1])
        self.__current-=1

    def show(self):
        print(self.__content)


s=Stack(5);
while(not s.isFull()):
    print("Stack is not full yet. ")
    print("To input more value, input your value. Or type -1 to pop. ")
    tmp=int(input())
    if(tmp==-1):
        s.pop()
    else:
        s.push(tmp)
    s.show()
print("Stack is full now. ")