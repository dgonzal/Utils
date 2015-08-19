from CRABAPI.RawCommand import crabCommand
from CRABClient.ClientExceptions import ClientException
from httplib import HTTPException
from CRABClient.UserUtilities import config, getUsernameFromSiteDB

class submit_config:
    def __init__(self,config, options=None):
        self.config = config
        self.options = options
        
    def _submit_(self):
        try:
            crabCommand('submit', config = self.config)
        except HTTPException as hte:
            print "Failed submitting task: %s" % (hte.headers)
        except ClientException as cle:
            print "Failed submitting task: %s" % (cle)

    def submitByDatasets(self,listOfDatasets, listOfNames, namePostfix):
        #print "DataSets", listOfDatasets,"Request Name", listOfNames, "Postfix",namePostfix
        if(len(listOfNames)==len(listOfDatasets)):
            for i in range(0,len(listOfDatasets)):
                self.config.Data.inputDataset = listOfDatasets[i]
                self.config.General.requestName = listOfNames[i]+namePostfix
                print "DataSet", listOfDatasets[i],"Request Name", listOfNames[i]+namePostfix
                self._submit_()
        else:
            print "Number of Datasets & Names are unequal"
                
