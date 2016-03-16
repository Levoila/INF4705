from subprocess import Popen, PIPE, call
import os
import csv

programLocation = 'Release/TP2.exe'
dataLocation = 'BTS/'
#algorithms = ['greedy', 'dynamic', 'local']
algorithms  = ['dynamic']

def main():
	filenames, nbFilenames = gatherDataFilenames()
	
	nbProcessed = 0
	timeData = []
	for algorithm in algorithms:
		for nbLocations in sorted(filenames):
			timeLine = [nbLocations]
			revenuLine = [nbLocations]
			for quantityRange in sorted(filenames[nbLocations]):
				totalTime = 0
				totalRevenu = 0
				for i in filenames[nbLocations][quantityRange]:
					nbProcessed += 1
					print('Progress :', nbProcessed, '/', nbFilenames * len(algorithms))
					
					filename = os.path.join(dataLocation, filenames[nbLocations][quantityRange][i])
					out = execute(['./'+programLocation, '-b', '-a', algorithm, '-f', filename])
					lines = str.splitlines(out.decode('utf-8'))
					revenu = float(lines[0])
					time = float(lines[1])
					availableQuantity = int(execute(['tail', '-1', filename]))
					
					timeData.append([algorithms.index(algorithm), nbLocations, quantityRange, availableQuantity, time])
		
		with open('results_' + algorithm + '_time.csv', 'w', newline='') as file:
			writer = csv.writer(file, delimiter=',')
			writer.writerows(timeData)
	
def gatherDataFilenames():
	filenames = [filename for filename in os.listdir(dataLocation) if os.path.isfile(os.path.join(dataLocation, filename))]
	
	dict = {}
	for filename in filenames:
		components = filename.split('-')
		nbLocations = int(components[1])
		quantityRange = int(components[2])
		num = int(components[3].split('.')[0])
		
		if nbLocations not in dict:
			dict[nbLocations] = {}
		if quantityRange not in dict[nbLocations]:
			dict[nbLocations][quantityRange] = {}
			
		dict[nbLocations][quantityRange][num] = filename
	
	return dict, len(filenames)

def execute(cmd):
	p = Popen(cmd, stdout=PIPE)
	out, err = p.communicate()
	return out
	
if __name__ == '__main__':
	main()