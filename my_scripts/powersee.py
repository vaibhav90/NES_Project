import csv
import matplotlib.pyplot as plt

# for P lines
#0-> str,
#1 -> clock_time(),2-> P, 3->rimeaddr_node_addr.u8[0],rimeaddr_node_addr.u8[1], 4-> seqno,
#5 -> all_cpu,6-> all_lpm,7-> all_transmit,8-> all_listen,9-> all_idle_transmit,10-> all_idle_listen,
#11->cpu,12-> lpm,13-> transmit,14-> listen, 15 ->idle_transmit, 16 -> idle_listen, [RADIO STATISTICS...]
from collections import defaultdict
cpuOverTime =  defaultdict(list)

with open('pow5.txt', 'rb') as f:
	reader = csv.reader(f,delimiter=' ')
	
	for row in reader:
		if row[2] is 'P':
			cpuOverTime[row[3]].append(row[5])
		else:
			print "data misalligned"	

for i in cpuOverTime:
	plt.plot(cpuOverTime[i])
plt.show()

#Energy_consumed = (cpu_time * 2.2 + lpm_time * 0.00169 + rx_time * 18.5 + rx_time * 33.6) * 3 / RTIMER_SECOND
#RTIMER_SECOND 32768
#power[row] = (((row[5]*2.2) + (row[6] * 0.00169) + (row[7] * 18.5) + (row[8] * 33.6)) * 3) / 32768
