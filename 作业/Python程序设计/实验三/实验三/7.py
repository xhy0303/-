
def match_pattern(list1,list2):
    for i in range(len(list1)):
        if list1[i:i+len(list2)]==list2:
            print(True)
            return
    print(False)
list1 = [4, 10, 2, 3, 50, 100]
list2 = [3, 2, 50]
list3 = [2, 3, 50]
list4 = [2, 3, 40]
match_pattern(list1, list2) # return False
match_pattern(list1, list3) # return True
match_pattern(list1, list4) # return False
