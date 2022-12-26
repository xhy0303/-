import string
def removePunctuations(word):
    for ch in word:
         if ch in string.punctuation:
            word = word.replace(ch, " ")
    return word

count=dict.fromkeys(["and", "as", "assert", "break", "class",
"continue", "def", "del", "elif", "else",
"except", "False", "finally", "for", "from",
"global", "if", "import", "in", "is", "lambda",
"None", "nonlocal", "not", "or", "pass", "raise",
"return", "True", "try", "while", "with", "yield"])

for i in count:
    count[i]=0

with open('nARQ.py') as data:
     da=removePunctuations(data.read())
     for word in da.split():
         if word in count.keys():
             count[word]+=1;


count_ordered=sorted(count.items(),key=lambda x:x[1],reverse=True)

print(count_ordered)