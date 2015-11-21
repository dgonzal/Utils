#!/usr/bin/env python

"""
*
* Usage :
* ./CrabConfig.py ConfigFile [options] 
*
*
"""
import argparse
import os, glob, sys

from CrabScript import *
from create_dataset_xmlfile import create_dataset_xml
from readaMCatNloEntries import readEntries


if __name__ == '__main__':        
        parser = argparse.ArgumentParser(description='Scripts for a simple multicrab')
        parser.add_argument('ConfigFile', action='store', 
                            help='Config File which should be processed')
        parser.add_argument('--xml', dest='xml_create',action='store_true',
                            default=False,
                            help="create xml Files for sframe")
        parser.add_argument('--submit','-s', dest='submit_flag', action='store_true',
                            default=False,
                            help='Submit to the grid')
        parser.add_argument('--status','-c', dest='status_flag', action='store_true',
                            default=False,
                            help='check status of all jobs. Have a lok at the dashbord')
        parser.add_argument('--command', dest='crab_command', action='store',
                            default='',
                            help='execute any crab command you want')
        parser.add_argument('--options','-o',dest='crab_options', action='append',
                            default=[],
                            help='Hand any option to your crab command')
        parser.add_argument('--readEntries', dest='readEntries', action='store',
                            default=0,
                            help='read the all entries contained in all xml files. Specify how many cores you want to use.')
        parser.add_argument('--postfix','-p', dest='postfix', action='store',
                           default='',
                           help="Posibility to add a postfix to the request names. Don't forget to add it too if using --xml/--readEntries")
        args = parser.parse_args()

        for i in range(len(args.crab_options)):
                print args.crab_options[i]
                args.crab_options[i] = '--'+args.crab_options[i]
                
        if args.ConfigFile.endswith('.py'):
                args.ConfigFile = args.ConfigFile.replace('.py','')

	#sys.path.append(os.path.abspath("/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_15_patch1/src/UHH2/VLQToTopAndLepton/Utils/crab"))
        
        module_name = args.ConfigFile
        __import__(module_name)
        ConfigFile = sys.modules[module_name]

        if len(ConfigFile.requestNames) != len(ConfigFile.inputDatasets):
                print 'Number of Request-Names',len(ConfigFile.requestNames),' unequal to number of Input-Datasets',len(ConfigFile.inputDatasets)
                print 'prefere to exit'
                exit(100)
        
        print 'Goint to print the Request-Name / Input-Dataset pairs'
        for i in range(len(ConfigFile.requestNames)):
                print ConfigFile.requestNames[i],ConfigFile.inputDatasets[i]

        if args.submit_flag:
                work = CrabConfig(ConfigFile.config,'submit',args.crab_options)
                work.ByDatasets(ConfigFile.inputDatasets,ConfigFile.requestNames,args.postfix)
        if args.status_flag:
                work = CrabConfig(ConfigFile.config,'status',args.crab_options)
                work.ByDatasets(ConfigFile.inputDatasets,ConfigFile.requestNames,args.postfix)
        if args.crab_command:
                work = CrabConfig(ConfigFile.config,args.crab_command,args.crab_options)
                work.ByDatasets(ConfigFile.inputDatasets,ConfigFile.requestNames,args.postfix)

        if args.xml_create:
                for name in ConfigFile.requestNames:
                        dirname = '/pnfs/desy.de/cms/tier2/'+config.Data.outLFNDirBase+name+'/crab*'+postfix+'/**/**/*.root'
                        l = glob.glob(dirname)
                        xmlname += '.xml'
                        #print xmlname, l
                        create_dataset_xml(dirname,xmlname)

        if args.readEntries > 0:
                fileList =[]
                for name in ConfigFile.requestNames:
                        xmlname += '.xml'
                        fileList.append(xmlname)
                readEntries(args.readEntries,fileList)
                
