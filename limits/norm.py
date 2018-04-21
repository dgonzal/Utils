#! /usr/bin/env python

import math, multiprocessing, glob
import ROOT
from ROOT import TObject

def read_xml(xmlFileDir):
    #try:
    xmlFile = open(xmlFileDir)
    rootFileStore = []
    for line in xmlFile:
        if '<!--' in line:continue
        rootFileStore.append(line.split('"')[1])
    xmlFile.close()
    return rootFileStore

def pdf_norm(filename, treename="AnalysisTree"):
    work_file = ROOT.TFile(filename)
    tree = work_file.Get(treename)
    
    cross_section = 0
    pdf_sum = 0
    for event in tree:
        original = event.genInfo.originalXWGTUP()
        cross_section += event.genInfo.weights()[0]
        sys = 0
        for weight in event.genInfo.systweights()[9:109]:
            sys += (weight/original-1)*(weight/original-1)
        pdf_sum += math.sqrt(sys/99)* original/math.fabs(original)
    #print 'yield nominal',  cross_section,  'yield pdf ', pdf_sum, 'ratio', pdf_sum/cross_section
    work_file.Close()
    return cross_section, pdf_sum
 


def scale_norm(filename, treename="AnalysisTree"):
    work_file = ROOT.TFile(filename)
    tree = work_file.Get(treename)
    
    cross_section = 0
    envelop_up_sum = 0
    envelop_down_sum = 0
    for event in tree:
        original = event.genInfo.originalXWGTUP()
        cross_section += event.genInfo.weights()[0]
        sys = 0
        up = event.genInfo.systweights()[1]/original
        down  = event.genInfo.systweights()[1]/original
        # just use certain bits
        # entries: 1,2,3,4,6,8 # take care to shift properly
        weight_list = event.genInfo.systweights()[1:9]
        counter =0
        for weight in weight_list:
            counter +=1
            if counter == 5 or counter == 7:continue
            weight = weight/original
	    #print weight
            if(up  <weight): up   = weight
            if(down>weight): down = weight
        envelop_up_sum += up
        envelop_down_sum +=down
    #print 'cross section',cross_section, 'up', envelop_up_sum, 'down', envelop_down_sum, ' ratio up', envelop_up_sum/cross_section, 'raio down', envelop_down_sum/cross_section
    work_file.Close()
    return cross_section, envelop_up_sum, envelop_down_sum


def combi_norm(filename):
    return [scale_norm(filename),pdf_norm(filename)]


def multithread_read(filestore, worker=6):
    pool = multiprocessing.Pool(worker)
    result = pool.map_async(combi_norm,filestore)
    pool.close()
    pool.join()
    result = result.get()
    cross_section = 0.
    scale_up = 0.
    scale_down = 0.
    pdf_sum=0.    
    for i in result:
        #print i 
        cross_section += i[0][0]
        scale_up += i[0][1] 
        scale_down += i[0][2] 
        pdf_sum += i[1][1]    

    print 'cross section', cross_section, 'scale up', scale_up, 'scale down', scale_down, 'PDF', pdf_sum
    return  [cross_section,scale_up,scale_down,pdf_sum]


def find_xmlfile(directory, signal, chirality, width =''):
    print 'find xml in',directory,'for',signal,chirality,width
    mass_width = ''
    if width:
        mass_width = width
        if 'p' not in width: mass_width +='p'
    search_string = directory+'/*'+signal+'*'+mass_width+'*'+chirality+'.xml'
    print search_string
    files = []
    for name in glob.glob(search_string):
        if 'Width' not in name and not width:
            files.append(name)
        elif width and width in name:
            files.append(name)
    print 'found files',files
    return files


def scale_pdf_reweight(root_dir,xml_dir, signal, chirality, width =''):
    xmlfiles = find_xmlfile(xml_dir, signal, chirality, width)
    work_file = ROOT.TFile(root_dir,'UPDATE')
    keys = work_file.GetListOfKeys()
    
    for xmlfile in xmlfiles:
        digit_search = xmlfile.split('/')[-1]
        print digit_search
        digit_search = digit_search.replace(width+'p','')
        digit_search = digit_search.replace('X53','X')
        mass = str(filter(str.isdigit, digit_search))
        print xmlfile, 'mass',mass
        filestore = read_xml(xmlfile)
        result = multithread_read(filestore)
        nominal =      result[0]
        scale_up =     result[1]
        scale_down =   result[2]
        pdf =          result[3]
        
        hist_list =[]
        for key in keys:  
            key = str(key.GetName())
            if '_'+mass+'_' not in key: continue
            split_key = key.split('__')
            #print split_key
            if len(split_key) !=4: continue
            if 'PDF' in key:
                hist = work_file.Get(key).Clone()
                if 'plus'  in key:
                    print 'scaling',key,'by',nominal/(nominal+pdf)
                    hist.Scale(nominal/(nominal+pdf))
                if 'minus' in key:
                    hist.Scale(nominal/(nominal-pdf))
                    print 'scaling',key,'by',nominal/(nominal-pdf)
                hist_list.append(hist)
            if 'scale' in key:
                hist = work_file.Get(key).Clone()
                if 'plus'  in key:
                    hist.Scale(nominal/scale_up)
                    print 'scaling',key,'by',nominal/(scale_up)
                if 'minus' in key:
                    hist.Scale(nominal/scale_down)
                    print 'scaling',key,'by',nominal/(scale_down)
                hist_list.append(hist)
        for hist in hist_list:
    	    hist.Write('',TObject.kOverwrite)
    work_file.Close()


if __name__ == "__main__":
    print 'using the pdf and scale reweighting script' 
    
    masses = ['800','1000','1200','1400','1600','1800'] 
    
    for mass in masses:
        xmldir = '/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/common/datasets/RunII_80X_v3/Bprime/BprimeBToTW_M-'+mass+'_RH.xml'

	
        print xmldir
        print '='*20
        #find_xmlfile('/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/common/datasets/RunII_80X_v3/Bprime/','BprimeB','LH','10')
        print '@'*10
        
        #continue
        print xmldir        
        file_store = read_xml(xmldir)
        #multithread_read(file_store)
        #continue
        for filename in file_store: 
            scale_norm(filename)
            #pdf_norm(filename)
     
