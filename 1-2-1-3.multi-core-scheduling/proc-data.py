import sys
import openpyxl

ncpu = int(sys.argv[1])

proc_data = [None]

def get_data(line):
    ret = None
    if('nproc' in line ) and ('end' in line):
        begin = line.find('=') + 1
        end = line.find('end')
        nproc = int(line[begin:end].strip())
        begin = line.find('-->') + 3
        end = line.find('ms')
        delay = int(line[begin:end].strip())
        ret = (nproc,delay)
    return ret

max_proc = 0

for i in range(1,ncpu+1):
    fname = str(i) + '-cpu.log'
    fd  = open(fname,'r')
    lines = fd.readlines()
    data = []

    for s in lines:
        r = get_data(s)
        if r != None:
            max_proc = r[0]
            data.append(r)
    proc_data.append(data)
    fd.close()

col = ['A','B','C','D','E']
book = openpyxl.Workbook()
book.active.title = 'Proc-Data'

for i in range(1,max_proc + 1):
    book.active[col[0] + str(i+1)] = i

for i in range(1,ncpu + 1):
    book.active[col[i] + '1'] = str(i) + '-CPU'
    data = proc_data[i]
    for d in data:
        book.active[col[i] + str(d[0] + 1)] = d[1]

book.save('./data.xlsx')