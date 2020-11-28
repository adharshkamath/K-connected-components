ip = open('../Email-Enron.txt', 'r')
op = open('op.txt', 'w')
curr = None
arr = []
for line in ip:
    # print(line.strip().split())
    if line.startswith('#'):
        continue
    a, b = line.strip().split()
    if a == curr:
        arr.append(b)
    else:
        if curr:
            for x in arr:
                print(x, end=" ", file=op)
            print('', end="\n", file=op)
        arr = [b]
        curr = a
for x in arr:
    print(x, end=" ", file=op)
print('', end="\n", file=op)
op.close()
ip.close()
