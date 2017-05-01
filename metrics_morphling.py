import sys, re, xlsxwriter
from xlsxwriter.utility import xl_rowcol_to_cell

def read_cases(filename, case_list):
    lines = [line.rstrip('\n') for line in open(filename, 'r')]
    i = 0;
    while (i < len(lines)):
        #line  = [map(int,x) for x in lines[i].split()]
        line = lines[i]
        if (line.find('firebase.js:32') != -1):
            m = re.search('default: (\d*.\d*)ms', line)
            if m:
                 found = m.group(1)
                 #print found
                 case_list.append(found)
        i += 1

def process_cases(cases, worksheet):
    i = 1
    startCell = xl_rowcol_to_cell(i, 1)
    for case in cases:
        worksheet.write_number(i, 1, float(case))
        i += 1
    endCell = xl_rowcol_to_cell(i-1, 1)
    worksheet.write(i,0,'Average')
    worksheet.write(i,1, '=AVERAGE('+startCell+':'+endCell+')')
    worksheet.write(i+1,0,'P50')
    worksheet.write(i+1,1, '=PERCENTILE('+startCell+':'+endCell+',0.5)')
    worksheet.write(i+2,0,'P90')
    worksheet.write(i+2,1, '=PERCENTILE('+startCell+':'+endCell+',0.9)')
    worksheet.write(i+3,0,'P99')
    worksheet.write(i+3,1, '=PERCENTILE('+startCell+':'+endCell+',0.99)')

def main():
    workbook = xlsxwriter.Workbook('metrics_morphling.xlsx')
    for i in range(1,len(sys.argv)):
        filename = str(sys.argv[i])
        #print filename
        cases = []
        read_cases(filename, cases)
        #print len(cases)
        worksheet = workbook.add_worksheet()
        process_cases(cases, worksheet)

    workbook.close()

if __name__ == "__main__":
    main()
