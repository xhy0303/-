import random

hole_num=int(input("Please set hole number: "))
day_num=int(input('Please set time: '))
count=0
fox=int(hole_num*random.random())

guess=int(input('Where is the fox? '))
while guess>=hole_num:
    guess=int(input('No such hole, try again. '))
count+=1

while guess!=fox and count<day_num:
    count+=1
    tmp=int(hole_num*random.random())
    while tmp==fox:
        tmp=int(hole_num * random.random())
    fox=tmp
    guess=int(input('No fox here, try again. '))
    while guess >= hole_num:
        guess = int(input('No such hole, try again. '))

if fox==guess:
    print('Congratulations on finding the fox. ')
if count==day_num:
    print('Sorry, you failed to find the fox. ')
    print('The fox hides in hole %d. '%fox)
