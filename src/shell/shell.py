"""
This program implements serial and parallel versions of shell sort. Shell sort involves sorting subarrays of elements in the array which are gapped by k = 2 array slots. At first, k = N/2 but gets further halved after each pass until k can no longer be subdivided, indicating the array is sorted. The parallel implementation involves splitting the set of subarrays among two threads at each level so that each thread works on sorting the subarrays assigned to it.

@author Jose Enrique R. Lopez
@date-created June 12, 2022

"""




from math import ceil
from threading import Thread
from random import randint
from time import time
array = []
SIZES = [10, 20, 30, 40]

def insertionSortHelper(indices):
	for p in range(1, len(indices)):
		q = p-1
		while (q>=0 and array[indices[q]]> array[indices[q+1]]):
			temp = array[indices[q]]
			array[indices[q]] = array[indices[q+1]]
			array[indices[q+1]] = temp
			q-=1

def shellSort():
	gap = len(array)//2
	while True:
		i = 0
		end_reached = False
		while not end_reached:
			indices = []
			index = i
			while (index < len(array)):
				indices.append(index)
				index+= gap
				if (index == len(array)-1): end_reached = True
			insertionSortHelper(indices)
			i+=1
			if (end_reached): break;
		if gap == 1: break
		gap = gap//2

def sortSubarrays(subarrays_indices):
	for indices in subarrays_indices:
		insertionSortHelper(indices)

def parallelShellSort():
	gap = len(array)//2
	while True:
		i = 0

		subarrays_indices = []
		end_reached = False
		while not end_reached:
			indices = []
			index = i
			while (index < len(array)):
				indices.append(index)
				index+= gap
				if (index == len(array)-1): end_reached = True
			subarrays_indices.append(indices)
			# threads.append(Thread(target = insertionSortHelper, args = (indices,)))
			i+=1
			if (end_reached): break;

		threads = []
		t1 = Thread(target = sortSubarrays, args = (subarrays_indices[0:len(subarrays_indices)//2],))
		t2 = Thread(target = sortSubarrays, args = (subarrays_indices[len(subarrays_indices)//2:],))
		threads.extend([t1,t2])
		for t in threads: t.start()
		for t in threads: t.join()

		if gap == 1: break
		gap = gap//2

def initArray(size):
	global array
	upper = size//2
	lower = -1 * upper
	array = [randint(lower, upper) for i in range(size)]

print("-------------SHELL SORT----------------")
print("SERIAL")
for size in SIZES:
	initArray(size)
	start = time()
	shellSort()
	end = time()
	print(f"N = {size} | {end-start} seconds")

print("PARALLEL")
for size in SIZES:
	initArray(size)
	start = time()
	parallelShellSort()
	end = time()
	print(f"N = {size} | {end-start} seconds")

