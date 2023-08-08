import sys
import numpy as np
import matplotlib.pyplot as plt

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
    #print(data)

    color = '#'
    
    for c in np.random.randint(0, 255, 3):
        color += format(c, '02X')
    
    colors.append(color)

#print(colors)

x_value = []
y_value = []
c_value = []

if flag == 'id-time':
    for d in data:
        y_value.append(d[0])
        x_value.append(d[1])
        c_value.append(colors[d[0]])

    plt.scatter(x_value,y_value,c = c_value)
    plt.title('Data Analysis')
    plt.ylabel('Process No.')
    plt.xlabel('TIme(ms)')
    plt.show()

if flag == 'work-time':
    for d in data:
        y_value.append(d[2])
        x_value.append(d[1])
        c_value.append(colors[d[0]])

    plt.scatter(x_value,y_value,c = c_value)
    plt.title('Data Analysis')
    plt.ylabel('Work Load')
    plt.xlabel('TIme(ms)')
    plt.show()