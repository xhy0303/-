import string
def readability(str):
    let_num=0
    word_num=1
    sect_num=0
    punc='.!?'
    for ch in str:
        if ch not in string.punctuation and ch!=' ':
            let_num+=1
        if ch in punc:
            sect_num+=1
        if ch==' ':
            word_num+=1
    L=let_num/word_num*100
    S=sect_num/word_num*100
    index=int(0.0588*L-0.296*S-15.8)
    if index<1:
        print("Before Grade 1")
    elif index>16:
        print("Grade 16+")
    else:
        print('Grade {}'.format(index))

with open('readability.txt') as data:
    str=data.read()
readability(str)