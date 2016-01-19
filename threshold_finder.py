import subprocess as sp
import os as os
import itertools as itertools

#Runs strassenSeuil and returns the time it took to run the algorithm
def runProgram(m1, m2, threshold):
	return float(sp.check_output(['./strassenSeuil', '-f', str(m1), str(m2), '-s', str(threshold)]))

def runBenchmark():
	matSizes = [7, 8, 9, 10, 11]
	matNumber = 5
	
	print('Generating matrices...')
	filenames = [['mat_' + str(size) + '_' + str(i) + '.txt' for i in range(0, matNumber)] for size in matSizes]
	for index, matGroup in enumerate(filenames):
		for filename in matGroup:
			sp.call(['./Gen', str(matSizes[index]), filename])
			
	for index, matGroup in enumerate(filenames):
		for combination in itertools.combinations(matGroup, 2):
			print(combination)
	
	#To be completed...
			
	print("Removing generated matrices...")
	os.system("rm mat_*")
	
def findThreshold():
	matSize = 11
	
	#Generate a test matrix
	sp.call(['./Gen', str(matSize), 'mat.txt'])
	
	print('Starting tests...')
	for i in [(1 << n) for n in range(1, matSize+1)]:
		print('Threshold : ' + str(i) + ' : ' + str(runProgram('mat.txt', 'mat.txt', i)))
	
	#Removing the test matrix
	sp.call(['rm', 'mat.txt'])
	
findThreshold()
