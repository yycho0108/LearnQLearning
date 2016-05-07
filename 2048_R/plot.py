#!/usr/bin/python
from matplotlib import pyplot as plt
import numpy as np
import csv

sectorSize = 100

def plotcsv(name):
    with open(name,'rb') as f:
        #pivot = int(sys.argv[1])*0.3
        reader = csv.reader(f)
        
        data = np.asarray(list(reader),dtype=np.float32)
        sector = len(data)/sectorSize

        data_av = np.arange(sectorSize, dtype=np.float32)
        for i in range(sectorSize):
            print "processing {}".format(i)
            data_av[i] = np.average(data[i*sector:(i+1)*sector])

        time = np.arange(sectorSize)
        #fit = np.polyfit(time,data_av,deg=3)
        print("MAX : {}".format(max(data)))
        plt.plot(data_av,'o')
        #plt.plot(time,fit[0]*time**3+fit[1]*time**2+fit[2]*time+fit[3],color='red')
        plt.title(name + ': {} iterations'.format(len(data)))
        plt.show()

plotcsv('build/score.csv')
plotcsv('build/loss.csv')
