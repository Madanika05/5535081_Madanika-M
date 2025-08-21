no_ops = int(input())
ops = []
s = []
for i in range(no_ops):
    one_op = input().split(' ')
    ops.append(one_op)
    
from copy import copy
history = []
for op in ops:
    #print(s)
    #print(op)
    if op[0] == '1':
        to_append = op[1]
        history.append(copy(s))
        s.extend(to_append)
    elif op[0] == '2':
        k = int(op[1])
        history.append(copy(s))
        del s[-k:]
    elif op[0] == '3':
        k = int(op[1])
        print(s[k-1])
    elif op[0] == '4':
        s = history.pop()