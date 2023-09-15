import sys
import openpyxl
import matplotlib.pyplot as plt

xlsx = sys.argv[1]
workbook = openpyxl.load_workbook(xlsx)
sheet = workbook['Proc-Data']

cols = ['B','C','D','E']
row_num = sheet.max_row

nproc_data = [None]

for c in cols:
    data = [0]
    for i in range(1,row_num):
        nproc = sheet['A' + str(i+1)].value
        delay = sheet[c + str(i+1)].value
        throughput = nproc / delay * 1000 ##吞吐量计算公式
        data.append(throughput)
    nproc_data.append(data)

plt.title('Process Throughput')
plt.style.use('bmh')
plt.xlabel('Proc')
plt.ylabel('TP')


for i in range(1,len(cols) + 1):
    x = [j for j in range(0,row_num)]
    y = nproc_data[i]
    l = str(i) + '-CPU'
    plt.plot(x,y,label=l)

plt.legend()
plt.show()