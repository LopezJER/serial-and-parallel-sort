"""
This program implements serial and parallel versions of bottom-up merge sort. Bottom-up merge sort involves first sorting subarrays of length 2 then merging them to larger and larger subarrays to be sorted. When the subarray length is of N/2, a final sort is performed, generating the sorted array. The parallel implementation involves splitting the whole array in 2 so that two threads each work on merge-sorting one half of the array. 

@author Jose Enrique R. Lopez
@date-created June 12, 2022

"""


from math import log2, ceil
from random import randint
from time import time
from threading import Thread

SERIAL = -1
PARALLEL = -2
ARRAY = []
SORTED_ARRAY = []
SUBARRAYS = []
SORTED_SUBARRAYS = []
SIZES = [10, 20, 30, 40]

def verify():
	for i in range(len(ARRAY)-1):
		if ARRAY[i]>ARRAY[i]+1: return False
	return True

def mergeSort(mode, index):
	if (mode == SERIAL):
		global ARRAY, SORTED_ARRAY
		array = ARRAY
		sorted_array = SORTED_ARRAY
	else:
		array = SUBARRAYS[index]
		sorted_array = SORTED_SUBARRAYS[index]
	length = len(array)
	stop = int(log2(length-1))+1
	temp = []
	for i in range(0, stop):
		offset = 2**i
		for j in range (0, len(array), offset):
			subarray = []
			subarray.extend(array[j:offset+j])
			temp.append(subarray)
			if (len(temp) == 2):
				merged = []
				while (temp[0] or temp[1]):
					if (temp[0] and not temp[1]):
						merged.extend(temp[0])
						temp[0] = []
					elif (temp[1] and not temp[0]):
						merged.extend(temp[1])
						temp[1] = []
					elif (temp[0] and temp[1] and temp[0][0] <= temp[1][0]):
						merged.append(temp[0].pop(0))
					elif (temp[0] and temp[1] and temp[0][0] >= temp[1][0]):
						merged.append(temp[1].pop(0))
				# print(merged)
				temp.clear()
				sorted_array.extend(merged)
				# print(temp)
				# merged=[]
		if(temp): 
			sorted_array.extend(temp[0])
			temp.clear()
		array = sorted_array.copy()
		sorted_array=[]
	if (mode == SERIAL): 
		ARRAY = array
		SORTED_ARRAY = ARRAY
	if (mode == PARALLEL): 
		SUBARRAYS[index] = array
		SORTED_SUBARRAYS[index] = SUBARRAYS[index]
		
def merge():
	global ARRAY, SUBARRAYS, SORTED_ARRAY
	while (len(SUBARRAYS)>=2):
		SUBARRAYS.insert(0, [])
		while (SUBARRAYS[1] or SUBARRAYS[2]):
			if (SUBARRAYS[1] and not SUBARRAYS[2]):
				SUBARRAYS[0].extend(SUBARRAYS[1])
				SUBARRAYS[1] = []
			elif (SUBARRAYS[2] and not SUBARRAYS[1]):
				SUBARRAYS[0].extend(SUBARRAYS[2])
				SUBARRAYS[2] = []
			elif (SUBARRAYS[1] and SUBARRAYS[2] and SUBARRAYS[1][0] <= SUBARRAYS[2][0]):
				SUBARRAYS[0].append(SUBARRAYS[1].pop(0))
			elif (SUBARRAYS[1] and SUBARRAYS[2] and SUBARRAYS[1][0] >= SUBARRAYS[2][0]):
				SUBARRAYS[0].append(SUBARRAYS[2].pop(0))
		SUBARRAYS.pop(1)
		SUBARRAYS.pop(1)	
	ARRAY = SUBARRAYS[0]
	SORTED_ARRAY = ARRAY


def parallelMergeSort(k=2):
	for i in range(k):
		SUBARRAYS.append(ARRAY[i*(round(len(ARRAY)/k)):(i+1)*(round(len(ARRAY)/k))])
		if (i==k-1): SUBARRAYS[-1].extend(ARRAY[(i+1)*(round(len(ARRAY)/k)):])
		SORTED_SUBARRAYS.append([])
	threads = []

	for i in range(k):
		thread = Thread(target = mergeSort, args = (PARALLEL, i))
		thread.start()
		threads.append(thread)
	for i in range(k):
		threads[i].join()
	merge()

def refresh():
	global ARRAY, SORTED_ARRAY, SUBARRAYS, SORTED_SUBARRAYS
	ARRAY=[]
	SORTED_ARRAY = []
	SUBARRAYS = []
	SORTED_SUBARRAYS = []
	temp = []


print("-------------MERGE SORT----------------")
print("SERIAL")
for size in SIZES:
	ARRAY = [randint(-size//2+1, size//2+1) for i in range (size)]
	start = time()
	mergeSort(SERIAL, 0)
	end = time()
	print(f"N = {size} | {end-start} seconds")
	refresh()

print("PARALLEL")
for size in SIZES:
	ARRAY = [randint(-size//2+1, size//2+1) for i in range (size)]
	start = time()
	parallelMergeSort()
	end = time()
	print(f"N = {size} | {end-start} seconds")
	refresh()
