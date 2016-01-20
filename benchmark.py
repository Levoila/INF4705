import subprocess as sp
import os
import itertools
import sys
import csv

#Runs strassenSeuil and returns the time it took to run the algorithm
def runProgram(m1, m2, threshold):
	return float(sp.check_output(['./strassenSeuil', '-f', str(m1), str(m2), '-s', str(threshold)]))
	
def average(l):
	return sum(l) / len(l)

def runBenchmark(baseThreshold):
	thresholds = [baseThreshold >> 2, baseThreshold, baseThreshold << 2, 1 << 30, 1]
	matSizes = [7, 8, 9, 10, 11]
	matNumber = 5
	
	print('Generating matrices...')
	filenames = [['mat_' + str(size) + '_' + str(i) + '.txt' for i in range(0, matNumber)] for size in matSizes]
	for index, matGroup in enumerate(filenames):
		for filename in matGroup:
			sp.call(['./Gen', str(matSizes[index]), filename])
	print('Matrix generation done.')
	
	print('Running benchmark...')
	
	with open('results.csv', 'wb') as csvFile:
		writer = csv.writer(csvFile, delimiter=',')
		writer.writerow(['Seuil', 'Matrix size'])
		writer.writerow([' '] + matSizes)
		for threshold in thresholds:
			row = [average([runProgram(combination[0], combination[1], threshold) for combination in itertools.combinations(matGroup, 2)]) for matGroup in filenames]
			writer.writerow([threshold] + row)
			print 'Threshold =', threshold, ':', row
			
	print('Benchmark done.')
			
	print("Removing generated matrices...")
	os.system("rm mat_*")
	
def findThreshold():
	matSize = 11
	bestTreshold = 0
	bestTime = sys.float_info.max
	
	#Generate a test matrix
	sp.call(['./Gen', str(matSize), 'mat.txt'])
	
	print('Determining best threshold...')
	sys.stdout.flush()
	
	for i in [(1 << n) for n in range(2, matSize+1)]:
		time = runProgram('mat.txt', 'mat.txt', i);
		print 'Threshold =', i, ':', time, 'seconds'
		sys.stdout.flush()
		
		if time < bestTime:
			bestTime = time
			bestThreshold = i
			
	print 'Best threshold : ', bestThreshold
	
	#Removing the test matrix
	sp.call(['rm', 'mat.txt'])
	
	return bestThreshold
	
runBenchmark(findThreshold())
