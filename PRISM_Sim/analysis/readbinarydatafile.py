import numpy as np
import matplotlib.pyplot as plt


# ---------------------------------------------
# Read from binary file
# ---------------------------------------------


dt = np.dtype([('evtN', np.uint32), ('HitNum', np.int8), ('Energy', np.uint16), ('DetID',np.uint8),('DOI', np.uint8),('HPindex',np.uint16)])
fName = '../output/HP16Ring_DetRing_Response_20k_PBMask.bin'
data = np.fromfile(fName, dtype=dt)

# ---------------------------------------------


'''
# ---------------------------------------------
# read from text file
# -----

evtN=[]
HitNum=[]
Energy=[]
DetID=[]
DOI=[]
HPindex=[]

a = np.loadtxt('../output/test.txt', delimiter='\t')
for i in range(0,len(a)):
    evtN.append(a[i][0])
    HitNum.append(a[i][1])
    Energy.append(a[i][2])
    DetID.append(a[i][3])
    DOI.append(a[i][4])
    HPindex.append(a[i][5])

print evtN
print HitNum
print Energy
print DetID
print DOI
print HPindex

# ---------------------------------------------
'''


H=np.histogram2d(data['DetID'],data['HPindex'], bins=[192,3072])


'''
# Plot DetID vs HPindex
H, xedges, yedges, img = plt.hist2d(data['DetID'],data['HPindex'], bins=[192,3072], range=[[0.5,192.],[0.5,3072.5]])
extent = [yedges[0], yedges[-1], xedges[0], xedges[-1]]
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
im = ax.imshow(H, cmap=plt.cm.jet, origin='lower', interpolation='nearest', extent=[0.5,3072.5,0.5,192.5], aspect='auto')
fig.colorbar(im, ax=ax)
plt.show()
'''
