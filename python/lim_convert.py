#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys

class mass_point:
    def __init__(self, mass, exp, one_sigma, two_sigma, obs =None):
        self.mass = mass
        self.exp = exp
        self.one_sigma = one_sigma
        self.two_sigma = two_sigma
        self.obs =obs
    
def string_round(mystring):
    return str(round(float(mystring),2))

def isfloat(value):
  try:
    float(value)
    return True
  except ValueError:
    return False

def convert_file(file_name):
    limit_file = open(file_name,'r')
    fill_expected = None
    limits = []
    for line in limit_file:
        if "expected" in line:
            fill_expected = True
            continue
        elif "observed" in line:
            fill_expected = False
            continue
        if "#" in line:
            continue
        item_list = line.split()
        if len(item_list) == 0:
            continue
        elif not isfloat(item_list[0]):
            continue
        #print line, item_list
        #print len(item_list)
        if fill_expected:
            #print item_list[0],item_list[1],(item_list[4],item_list[5]),(item_list[2],item_list[3])
            limits.append(mass_point(item_list[0],item_list[1],[item_list[4],item_list[5]],[item_list[2],item_list[3]]))
        else:
            for lim in limits:
                if lim.mass == item_list[0]:
                    lim.obs = item_list[1]
                    break
    limit_file.close()
    result = open(file_name,'a')
    result.write("\n")
    result.write("This should be used to make tex tables\n")
    result.write("Process & Expected [pb] & Exp. $\pm 1 \sigma$ [pb] & Exp. $\pm 2 \sigma$ [pb] & Observed [pb]\\\ \hline \n")
    for item in limits:
        result.write(item.mass+" & "+item.exp+" & "+string_round(item.one_sigma[0])+" - "+string_round(item.one_sigma[1])+" & "+string_round(item.two_sigma[0])+" - "+string_round(item.two_sigma[1])+" & "+item.obs+"\\\ \n")
                     
if __name__ == "__main__":
    convert_file(sys.argv[1])
    
