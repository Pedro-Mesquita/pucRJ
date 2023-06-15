import csv
import matplotlib.pyplot as plt
import numpy as np

def list_finder(list, time):
    for ind, sub_list in enumerate(list):
        if sub_list[0] == time:
            return ind
    return -1

list_of_time = []
teste = []

with open('./1half.csv', newline='') as csvfile:
    f = csv.reader(csvfile, delimiter=',', quotechar=',')
    for row in f:
        index = list_finder(list_of_time, row[0][:5])
        if index != -1:
            list_of_time[index][1]+=1
        else:
            list_of_time.append([row[0][:5], 1])
       
time = [data[0] for data in list_of_time]
quantity = [data[1] for data in list_of_time]

plt.hist(time, bins=len(time), weights=quantity)

# Set the labels and title
plt.xlabel('Tempo')
plt.ylabel('Quantidade de tweets')
plt.title('Tweets por minuto')

tick_positions = np.arange(0, len(time)) + 10  # Adjust this value for desired spacing
tick_labels = [str(t) for t in time]
plt.xticks(tick_positions, tick_labels)
# Display the histogram
plt.show()