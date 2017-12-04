#! /usr/bin/env python

import math, multiprocessing, glob
import ROOT

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
 


def scale_norm( filename, treename="AnalysisTree"):
    work_file = ROOT.TFile(filename)
    tree = work_file.Get(treename)
    
    cross_section = 0
    envelop_up_sum = 0
    envelop_down_sum = 0
    for event in tree:
        original = event.genInfo.originalXWGTUP()
        cross_section += event.genInfo.weights()[0]
        sys = 0
        up = event.genInfo.systweights()[0]
        down  = event.genInfo.systweights()[0]
        for weight in event.genInfo.systweights()[1:9]:
            weight = weight/original
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
    cross_setcion = 0.
    scale_up = 0.
    scale_down = 0.
    pdf_sum=0.    
    for i in result:
        #print i 
        cross_setcion += i[0][0]
        scale_up += i[0][1] 
        scale_down += i[0][2] 
        pdf_sum += i[1][1]    

    print 'cross section', cross_setcion, 'scale up', scale_up, 'scale down', scale_down, 'PDF', pdf_sum
    return  [cross_setcion,scale_up,scale_down,pdf_sum]


def find_xmlfile(directory, signal, chirality, width =''):
    mass_width = ''
    if width:
        mass_width = width
        if 'p' not in width: mass_width +='p'
    search_string = directory+'/*'+signal+'*'+mass_width+'*'+chirality+'.xml'
    print search_string
    name = glob.glob(search_string)
    print 'found files',name
    return name


def scale_pdf_reweight(root_dir,xml_dir, signal, chirality, width =''):
    xmlfiles = find_xmlfile(xml_dir, signal, chirality, width ='')
    filestore = read_xml(xmlfile)


    work_file = ROOT.TFile(root_dir)
    keys = work_file.GetListOfKeys()

    for xmlfile in  xmlfiles:
        digit_search = xmlfile.replace(width)
        mass = int(filter(str.isdigit, digit_search))
        print mass
        result = multithread_read(xmlfile)
        nominal =      result[0]
        selfcale_up =  result[1]
        scale_down =   result[2]
        pdf =          result[3]
    
        for key in keys:  
            key = str(key.GetName())
            if mass not in key: continue
            split_key = key.split('__')
            if len(split_key) !=3: continue
            if 'PDF' in key:
                hist = work_file.Get(key)
                if 'plus'  in key: hist.Scale(nominal/(nominal+pdf))
                if 'minus' in key: hist.Scale(nominal/(nominal-pdf))
            if 'scale' in key:
                hist = work_file.Get(key)
                if 'plus'  in key: hist.Scale(nominal/scale_up)
                if 'minus' in key: hist.Scale(nominal/scale_down)
    work_file.Close()


if __name__ == "__main__":    
    masses = ['800','1000','1200','1400','1600','1800'] 
    
    for mass in masses:
        xmldir = '/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/common/datasets/RunII_80X_v3/Bprime/BprimeBToTW_M-'+mass+'_RH.xml'
        print xmldir
        print '='*20
        find_xmlfile('/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_8_0_24_patch1/src/UHH2/common/datasets/RunII_80X_v3/Bprime/','BprimeB',mass,'LH','10')
        print '@'*10
        
        continue
        print xmldir        
        file_store = read_xml(xmldir)
        multithread_read(file_store)
        continue
        for filename in file_store: 
            scale_norm(filename)
            pdf_norm(filename)
            
