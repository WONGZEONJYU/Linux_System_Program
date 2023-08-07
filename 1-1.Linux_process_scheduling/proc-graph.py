import sys

flag = sys.argv[1]
nproc = int(sys.argv[2])

data = []
colors = []

for fid in range(0,nproc):
    fname = str(fid) + '-proc.log'
    fd = open(fname,'r')
    lines = fd.readlines()

    for s in lines:
        s = s.strip().split('\t')
        data.append([int(s[0]),int(s[1]),int(s[2])])
        
    fd.close()
    print(data)