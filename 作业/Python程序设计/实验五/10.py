import os
from collections import namedtuple

def fileCounter(dir):
    FileInfo = namedtuple('FileInfo', ['file_num', 'code_num', 'space_num', 'comments_num'])
    if os.path.isdir(dir):
        files = os.listdir(dir)
        file_num = len(files)
        code_num = 0
        space_num = 0
        comments_num = 0

        for f in files:
            with open(dir + '\\' + f, encoding='utf_8') as data:
                str = data.readline()
                while str:
                    if str.startswith('#'):
                        comments_num += 1
                    elif str == '\n':
                        space_num += 1
                    else:
                        code_num += 1
                    str = data.readline()
    else:
        file_num = 1
        code_num = 0
        space_num = 0
        comments_num = 0
        with open(dir , encoding='utf_8') as data:
            str = data.readline()
            while str:
                if str.startswith('#'):
                    comments_num += 1
                elif str == '\n':
                    space_num += 1
                else:
                    code_num += 1
                str = data.readline()
    info=FileInfo(file_num,code_num,space_num,comments_num)
    return info

print(('代码\实验'))
print(fileCounter('代码\实验'))
print(('代码\作业'))
print(fileCounter('代码\作业'))

dir=input("Please input the location of the file which you wish to process. \n")
print(fileCounter(dir))