print([2 * num - 1 for num in range(1,5)])
sample=['apple', 'orange', 'pear']
print([str[0].upper() for str in sample])
print([sample[2*num] for num in range(2)])
sample1=["TA_parth", "student_poohbear", "TA_michael", "TA_guido", "student_htiek"]
print([str[3:] for str in sample1 if str[:2]=='TA'])
print([(str,len(str)) for str in sample])
print([{str:len(str)} for str in sample])