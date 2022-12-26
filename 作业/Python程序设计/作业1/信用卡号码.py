
def vaildCreadit(cardNum):
    length=len(cardNum)
    sum1=sum2=0
    for i in range(length):
        if (i+1) % 2 != 0:
            sum2 += int(cardNum[length-i-1])
        else:
            mid = 2 * int(cardNum[length-i-1])
            while mid >= 10:
                sum1 += (mid // 10)
                mid %= 10
            sum1 += mid
    if (sum1+sum2)%10==0 and (length==13 or length == 15 or length==16):
        valid=True
        if (length == 15):
            name = 'American Express'
        elif (length == 13):
            name = 'Visa'
        elif (length == 16):
            if int(cardNum[0])==4:
                name='Visa'
            else:
                name='MasterCard'
        print(name+', '+'Vaild')
    else:
        vaild=False
        print('Invalid')
        return
vaildCreadit("378282246310005")
vaildCreadit("371449635398431")
vaildCreadit("5555555555554444")
vaildCreadit("4111111111111111")
vaildCreadit("4012888888881881")
vaildCreadit("4222222222222")
vaildCreadit("1111111")
