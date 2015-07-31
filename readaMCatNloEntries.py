import sys
from ROOT import *


def main():
    try:
        for arg in sys.argv[1:]:
            print "open XML file:",arg
            xmlFile = open(arg)
            numberOfweightedEntries =0 
            for line in xmlFile:
                ntupleLocation = line.split('"')[1]
                print "new file",ntupleLocation,"current result",numberOfweightedEntries
                ntuple = TFile(str(ntupleLocation))
                AnalysisTree = ntuple.Get("AnalysisTree")
                for event in AnalysisTree:
                    for value in event.m_weights:
                        numberOfweightedEntries += value
            print numberOfweightedEntries
    except:
        print "could not read cmd arguments or other problem"

if __name__ == "__main__":
    main()
