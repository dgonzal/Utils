import glob
from shutil import copyfile

for i in glob.glob('output_*_BprimeB_RH_*/limi*'):
	name = i.split("/")[0]
	new = name+'/'+name+'_limit.pdf'
	print 'copy', i, new
	copyfile(i,new)


