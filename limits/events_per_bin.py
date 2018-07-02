#! /usr/bin/env python

from injection_merge import * 
import sys

if __name__=="__main__":
    if len(sys.argv) == 2:
        events_per_width(sys.argv[1], '')
    elif len(sys.argv) == 3:
        events_per_width(sys.argv[1], sys.argv[2])
    else:
        print 'wrong number of arguments, usage: filename postfix'
        
    
    
