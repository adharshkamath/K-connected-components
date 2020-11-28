import sys

if len(sys.argv) == 1:
    print("Usage: python3 read_file.py [input filename] [optional output filename]")
    exit(-1)

ip = open(sys.argv[1], 'r')

if len(sys.argv) > 2:
    op = open(sys.argv[2], 'w')
else:
    op = open("adj.txt", 'w')

curr = None
arr = []
for line in ip:
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
