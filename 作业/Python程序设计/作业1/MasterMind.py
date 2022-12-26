def Process(ans,gus):
    count_num=0
    count_dig=0
    if len(gus)==5:
        for i in range(len(ans)):
            if gus[i] == ans[i]:
                count_dig += 1
            for n in range(len(gus)):
                if gus[n] == ans[i]:
                    count_num += 1
                    break
    return count_num,count_dig

def vaildGuess(str):
    if(len(str)!=5):
        return False
    else:
        for i in range(len(str)):
            for n in range(i+1,len(str)):
                if(str[i]==str[n]):
                    return False
    return True

try_num=int(input('Please set chances: '))
tot=try_num
answer=input('Please input answer: ')
sign=vaildGuess(answer)
while not sign:
    print('Invalid answer, please input again: ',end='')
    answer=input()
    sign=vaildGuess(answer)

print('You have %d chances to guess the correct answer. Input -1 to quit.'%tot)

guess=input("Please input your guess: ")
sign=vaildGuess(guess)
count_num,count_dig=Process(answer,guess)
while ((not sign) or count_dig!=5 or count_num!=5)and try_num>0 and guess!='-1':
    if not sign:
        print('Invalid try, please input again: ', end='')
        guess = input()
        sign = vaildGuess(guess)
        count_num, count_dig = Process(answer, guess)
    else:
        try_num-=1
        if(try_num>0):
            print('remain chances: %s, your answer: %s, correct numbers: %d, correct digits: %d. ' % (
            try_num, guess, count_num, count_dig))
            print('Please try again: ', end='')
            guess = input()
            sign = vaildGuess(guess)
            count_num, count_dig = Process(answer, guess)
        else:
            print('You fail. ')
            break
if sign and count_num==5 and count_dig==5 and guess!='-1':
    try_num-=1
    if tot-try_num>1:
        print('Correct answer is %s, you used %d chances. '%(guess,tot-try_num))
    else:
        print('Correct answer is %s, you used %d chance. '%(guess,tot-try_num))
if guess=='-1':
    print('You fail, ',end='')
    if tot-try_num>1:
        print('you used %d chances. '%(tot-try_num))
    else:
        print('you used %d chance. '%(tot-try_num))