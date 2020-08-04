import pandas as pd
import math
import numpy as np
import sys
import os

clear = lambda: os.system('cls')

np.set_printoptions(threshold=sys.maxsize)

df = pd.read_excel(r'spending.xlsx', sheet_name = 'to_clean')

df.rename(columns={'Unnamed: 0' : 'Date', 'Unnamed: 1' : 'Week', 'Unnamed: 2' : 'Description', 'Unnamed: 3' : 'Money IN',
                   'Unnamed: 4' : 'Money OUT', 'Unnamed: 5' : 'Balance'}, inplace=True)

pd.set_option('display.max_columns', 7)
pd.options.display.width=None

df.drop(axis=0, index=[0, 1, 2], inplace=True)
df.drop(axis=1, labels=['Date', 'Balance'], inplace=True)

array = df.to_numpy()
income_indices = []

for i in range(len(array)):
    if not math.isnan(array[i][2]):
        income_indices.append(i)
array = np.delete(array, income_indices, axis=0)

num_unclassified = 0
for i in range(len(array)):
    if 'Amazon' in array[i][1]:
        array[i][2] = 'Amazon'
    elif 'AMZN' in array[i][1]:
        array[i][2] = 'Amazon'
    elif 'AMAZON' in array[i][1]:
        array[i][2] = 'Amazon'
    elif 'PETROL' in array[i][1]:
        array[i][2] = 'Transport'
    elif 'SAINSBURY' in array[i][1]:
        array[i][2] = 'Food'
    elif 'TESCO' in array[i][1]:
        array[i][2] = 'Food'
    elif 'CASH' in array[i][1]:
        array[i][2] = 'Cash'
    elif 'Cash' in array[i][1]:
        array[i][2] = 'Cash'
    elif 'Stash' in array[i][1]:
        array[i][2] = 'Stash'
    elif 'STASH' in array[i][1]:
        array[i][2] = 'Stash'
    elif 'CCBC' in array[i][1]:
        array[i][2] = 'Clubs'
    elif 'Subs' in array[i][1]:
        array[i][2] = 'Clubs'
    elif 'TRAIN' in array[i][1]:
        array[i][2] = 'Transport'
    elif 'METROLINK' in array[i][1]:
        array[i][2] = 'Transport'
    elif 'TFW RAIL' in array[i][1]:
        array[i][2] = 'Transport'
    elif 'trainline' in array[i][1]:
        array[i][2] = 'Transport'
    elif 'MCDONALDS' in array[i][1]:
        array[i][2] = 'Social'
    elif 'Collingwood Coll' in array[i][1]:
        array[i][2] = 'Social'
    elif 'BISHOPS' in array[i][1]:
        array[i][2] = 'Social'
    elif 'POST OFFICE' in array[i][1]:
        array[i][2] = 'IGNORE'
    elif 'LABOUR PARTY' in array[i][1]:
        array[i][2] = 'Bills'
    elif 'MOWBRAY' in array[i][1]:
        array[i][2] = 'Bills'
    elif 'CAFE' in array[i][1]:
        array[i][2] = 'Social'
    elif 'Cafe' in array[i][1]:
        array[i][2] = 'Social'
    elif 'MR FINLAY ALEXANDER LOWE WOJTAN' in array[i][1]:
        array[i][2] = 'IGNORE'
    elif 'WAITROSE' in array[i][1]:
        array[i][2] = 'Food'
    elif 'ONE STOP' in array[i][1]:
        array[i][2] = 'Food'
    elif 'FOOD' in array[i][1]:
        array[i][2] = 'Food'
    elif 'STGCOACH' in array[i][1]:
        array[i][2] = 'Transport'
    elif 'SUBWAY' in array[i][1]:
        array[i][2] = 'Social'
    elif 'GREGGS' in array[i][1]:
        array[i][2] = 'Social'
    elif 'Park' in array[i][1]:
        array[i][2] = 'Transport'
    elif 'PARK' in array[i][1]:
        array[i][2] = 'Transport'
    elif 'Meal' in array[i][1]:
        array[i][2] = 'Social'
    elif 'MEAL' in array[i][1]:
        array[i][2] = 'Social'
    elif 'FIXR' in array[i][1]:
        array[i][2] = 'Social'
    elif 'EVENT DURHAM' in array[i][1]:
        array[i][2] = 'Social'
    elif 'Present' in array[i][1]:
        array[i][2] = 'Amazon'
    elif 'Shopping' in array[i][1]:
        array[i][2] = 'Amazon'
    elif 'Pizza' in array[i][1]:
        array[i][2] = 'Social'
    elif 'PIZZA' in array[i][1]:
        array[i][2] = 'Social'
    elif 'PC/ENDSLEIGH' in array[i][1]:
        array[i][2] = 'Bills'
    elif 'HIPPO' in array[i][1]:
        array[i][2] = 'Social'
    elif 'COLLINGWOOD JCR' in array[i][1]:
        array[i][2] = 'Social'
    elif 'JS ONLINE' in array[i][1]:
        array[i][2] = 'Food'
    elif 'BOARDROOM' in array[i][1]:
        array[i][2] = 'Social'
    elif 'Vue' in array[i][1]:
        array[i][2] = 'Social'
    elif 'CINEWORLD' in array[i][1]:
        array[i][2] = 'Social'
    else:
        array[i][2] = 'Unclassified'
        num_unclassified += 1

def setID(id):
    if id == 1:
        return 'Food'
    elif id == 2:
        return 'Social'
    elif id == 3:
        return 'Bills'
    elif id == 4:
        return 'Clubs'
    elif id == 5:
        return 'Stash'
    elif id == 6:
        return 'Transport'
    elif id == 7:
        return 'Cash'
    elif id == 8:
        return 'Amazon'
    elif id == 9:
        return 'IGNORE'
    else:
        return 'Unclassified'
saved_data = []
if os.path.isfile('spending_data.npy'):
    saved_data = np.load('spending_data.npy', allow_pickle=True)
    print(len(saved_data))
    print(saved_data)
    for i in range(len(saved_data)):
        array[i] = saved_data[i]
    print(len(saved_data))

#save_list = []

for i in range(len(saved_data), len(array), 1):
    print('\n\n\n\n\n\n\n\n\n\n')
    print('Categorise the spending into: Food (1), Social (2), Bills (3), Clubs (4), Stash (5), Transport (6), Cash (7), Amazon (8)', 'IGNORE (9)          ' + str(i) + '/' + str(len(array)) + ' progress  -  ' + str(round((num_unclassified / len(array)*100))) + '% unclassified  -  ' + str(num_unclassified) + ' to classify')
    print('\x1b[96m' + array[i][1] + '\x1b[m  /  \x1b[91m' + str(array[i][3]) + '\x1b[m  /  Categorization: \x1b[93m' + array[i][2] + '\x1b[m\n\n\n\n\n\n')

    if array[i][2] != 'Unclassified':
        response = input('\x1b[42m\x1b[30mAre you satisfied with the classification? (y/n) \x1b[m')
        if response == 'y':
            pass
        else:
            id = int(input('Enter the number for the desired classification: '))
            array[i][2] = setID(id)
    else:
        id = int(input('Enter the number for the desired classification: '))
        array[i][2] = setID(id)

    #save_list.append(array[i])

    if i % 10 == 0:
        save_array = array[:i]
        #print('Saving file of length: ' + str(len(save_array)))
        np.save('spending_data.npy', save_array)

np.save('spending_data.npy', array)


## may want to change classification of flights at around 390