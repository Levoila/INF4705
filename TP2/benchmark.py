from subprocess import Popen, PIPE, call
import os
import csv

programLocation = 'Release/TP2.exe'
dataLocation = 'BTS/'
algorithms = ['greedy']#, 'dynamic', 'local']

def main():
	filenames, nbFilenames = gatherDataFilenames()
	
	nbProcessed = 0
	for algorithm in algorithms:
		timeData = [['', 'Quantite maximale'], ["Nombre d'emplacements"] + [10, 100, 1000]]
		revenuData = [['', 'Quantite maximale'], ["Nombre d'emplacements"] + [10, 100, 1000]]
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
					totalRevenu += float(lines[0])
					totalTime += float(lines[1])
					
				averageTime = totalTime / len(filenames[nbLocations][quantityRange])
				averageRevenu = totalRevenu / len(filenames[nbLocations][quantityRange])
				
				timeLine.append(averageTime)
				revenuLine.append(averageRevenu)
				
			timeData.append(timeLine)
			revenuData.append(revenuLine)
		
		with open('results_' + algorithm + '_time.csv', 'w', newline='') as file:
			writer = csv.writer(file, delimiter=',')
			writer.writerows(timeData)
			
		with open('results_' + algorithm + '_revenu.csv', 'w', newline='') as file:
			writer = csv.writer(file, delimiter=',')
			writer.writerows(revenuData)
	
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