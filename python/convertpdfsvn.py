#! /usr/bin/env python

from subprocess import call
import glob


def convert_pdf_svg(name):
    call(['pdf2svg '+name+' '+name.replace('pdf','svg')],shell=True)
def convert_eps_pdf(name):
    call(['epspdf '+name],shell=True)

if __name__ == '__main__':
    for item in glob.glob("*.eps"):
	convert_eps_pdf(item)
    for item in glob.glob("*/*.eps"):
 	convert_eps_pdf(item)
    for filename in glob.glob("*.pdf"):
        convert_pdf_svg(filename)
    for filename in glob.glob("*/*.pdf"):
        convert_pdf_svg(filename)

   

