import csv
import matplotlib.pyplot as plt
import numpy as np

def list_finder(list, time):
    for ind, sub_list in enumerate(list):
        if sub_list[0] == time:
            return ind
    return -1

list_of_time = []

with open('./1half.csv', newline='') as csvfile:
    f = csv.reader(csvfile, delimiter=',', quotechar=',')
    for row in f:
        index = list_finder(list_of_time, row[0][:5])
        if index != -1:
            list_of_time[index][1] += 1
        else:
            list_of_time.append([row[0][:5], 1])

time = [data[0] for data in list_of_time]
quantity = [data[1] for data in list_of_time]

fig, ax = plt.subplots()

bar_width = 0.8  
bar_positions = np.arange(len(time))
ax.bar(bar_positions, quantity, width=bar_width, align='center')

plt.xlabel('Tempo')
plt.ylabel('Quantidade de tweets')
plt.title('Tweets por minuto')

plt.xticks(bar_positions, time, rotation=90)

plt.show()


list_of_time2 = []

with open('./2half.csv', newline='') as csvfile:
    file = csv.reader(csvfile, delimiter=',', quotechar=',')
    for row in file:
        index = list_finder(list_of_time2, row[0][:5])
        if index != -1:
            list_of_time2[index][1] += 1
        else:
            list_of_time2.append([row[0][:5], 1])

time2 = [data[0] for data in list_of_time2]
quantity2 = [data[1] for data in list_of_time2]

fig2, ax2 = plt.subplots()

bar_width2 = 0.8  
bar_positions2 = np.arange(len(time2))
ax2.bar(bar_positions2, quantity2, width=bar_width2, align='center')

plt.xlabel('Tempo')
plt.ylabel('Quantidade de tweets')
plt.title('Tweets por minuto')

plt.xticks(bar_positions2, time2, rotation=90)

plt.show()
