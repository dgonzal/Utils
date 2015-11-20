# usage of script: number of cores, first file, second file, ...
# python readaMCatNLO.py 4 ../../common/datasets/MC_WJet.xml

import sys, multiprocessing, time
from ROOT import *

def read_xml(xmlFileDir):
    #try:
    xmlFile = open(xmlFileDir)
    rootFileStore = []
    for line in xmlFile:
        rootFileStore.append(line.split('"')[1])
    return rootFileStore
    #except:
    #    print "No able to read file Dir", xmlFileDir
    #    return 

def read_tree(rootDir):
    ntuple = TFile(str(rootDir))
    AnalysisTree = ntuple.Get("AnalysisTree")
    numberOfweightedEntries = 0 
    for event in AnalysisTree:
        #numberOfweightedEntries+=event.m_weights[0]
        for value in event.m_weights:
            numberOfweightedEntries += value
    return numberOfweightedEntries



def readEntries(worker, xmlfiles):
    print "number of workers",worker
    for xml in xmlfiles:
        pool = multiprocessing.Pool(int(worker))
        print "open XML file:",xml
        rootFileStore = read_xml(xml)
        numberXMLFiles = len(rootFileStore)
        result = pool.map_async(read_tree,rootFileStore)
        #time.sleep(30)
        print result._number_left ,numberXMLFiles,result._chunksize
        while result._number_left>0:
            sys.stdout.write("\033[F")
            #print result._number_left ,numberXMLFiles,result._chunksize
            missing = round(float(result._number_left)*float(result._chunksize)/float(numberXMLFiles)*100)
            if(missing > 100):
                missing =100
            print "Missing [%]", missing
            time.sleep(10)
        pool.close()
        pool.join()
        print "number of events in",xml,sum(result.get())


 
    
if __name__ == "__main__":
    readEntries(sys.argv[1], sys.argv[2:])
