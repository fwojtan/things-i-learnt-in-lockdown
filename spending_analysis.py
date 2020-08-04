import matplotlib.pyplot as plt
import numpy as np
import sys

np.set_printoptions(threshold=sys.maxsize)

array = np.load('spending_data.npy', allow_pickle=True)

grouped_data = np.zeros([144, 10])

def get_index_from_label(key): # there must be a better way of doing this with dictionaries or something
                                # (I should probably learn how to do that)
    if key == 'Food':
        return 1
    elif key == 'Social':
        return 2
    elif key == 'Bills':
        return 3
    elif key == 'Clubs':
        return 4
    elif key == 'Stash':
        return 5
    elif key == 'Transport':
        return 6
    elif key == 'Cash':
        return 7
    elif key == 'Amazon':
        return 8
    else:
        return 9


for i in range(len(array)):
    print('\rProgress: ' + str(i) + '/' + str(len(array)))
    week_index = array[i][0] - 41
    cat_index = get_index_from_label(array[i][2])
    grouped_data[week_index][cat_index] += array[i][3]

for i in range(len(grouped_data)):
    grouped_data[i][0] = i



# eliminate anomalaous purchases
grouped_data[80, 8] = 0
grouped_data[81, 6] = 0
grouped_data[86, 7] = 0

# drop unnecessary weeks
grouped_data = np.delete(grouped_data, range(51), axis=0)
grouped_data = np.delete(grouped_data, range(44, 51), axis=0)
grouped_data = np.delete(grouped_data, range(70, 86), axis=0)

# sum categories to find averages

data = np.delete(grouped_data, [0, 9], axis=1)
sums = np.zeros(len(data[0]))

for i in data:
    for k in range(len(i)):
        sums[k] += i[k]

explode = [0.01, 0, 0, 0, 0, 0, 0, 0]
labels = ['Food', 'Social', 'Bills', 'Clubs', 'Stash', 'Transport', 'Cash', 'Amazon']

for i in range(len(labels)):
    print(labels[i] + ': £' + str(round(sums[i], 2)))

plt.figure()
plt.pie(sums, explode=explode, labels=labels, autopct="%1.1f%%")
plt.show()

plt.figure()
plt.bar(grouped_data[:,0], grouped_data[:,1], label='Food')
plt.bar(grouped_data[:,0], grouped_data[:,2], bottom=grouped_data[:,1], label='Social')
plt.bar(grouped_data[:,0], grouped_data[:,3], bottom=grouped_data[:,1]+grouped_data[:,2], label='Bills')
plt.bar(grouped_data[:,0], grouped_data[:,4], bottom=grouped_data[:,1]+grouped_data[:,2]+grouped_data[:,3], label='Clubs')
plt.bar(grouped_data[:,0], grouped_data[:,5], bottom=grouped_data[:,1]+grouped_data[:,2]+grouped_data[:,3]+grouped_data[:,4], label='Stash')
plt.bar(grouped_data[:,0], grouped_data[:,6], bottom=grouped_data[:,1]+grouped_data[:,2]+grouped_data[:,3]+grouped_data[:,4]+grouped_data[:,5], label='Transport')
plt.bar(grouped_data[:,0], grouped_data[:,7], bottom=grouped_data[:,1]+grouped_data[:,2]+grouped_data[:,3]+grouped_data[:,4]+grouped_data[:,5]+grouped_data[:,6], label='Cash')
plt.bar(grouped_data[:,0], grouped_data[:,8], bottom=grouped_data[:,1]+grouped_data[:,2]+grouped_data[:,3]+grouped_data[:,4]+grouped_data[:,5]+grouped_data[:,6]+grouped_data[:,7], label='Amazon')
plt.ylim(0, 500)
plt.legend()
plt.show()
