import numpy as np
import matplotlib.pyplot as plt


# ---------------------------------------------
# Read from binary file
# ---------------------------------------------


dt = np.dtype([('evtN', np.uint32), ('HitNum', np.int8), ('Energy', np.float64), ('DetID',np.uint8),('DOI', np.uint8),('HPindex',np.uint16)])
fName = '../output/HP8Ring_DetRing_Response_200.bin'
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


# Plot DetID vs HPindex

#H, xedges, yedges, img = plt.hist2d(data['DetID'],data['HPindex'], bins=[192,3072], range=[[0.5,192.],[0.5,3072.5]])
H, xedges, yedges, img = plt.hist2d(data['DetID'],data['HPindex'], bins=[192,768], range=[[0.5,192.],[0.5,768.5]])
extent = [yedges[0], yedges[-1], xedges[0], xedges[-1]]
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
#im = ax.imshow(H, cmap=plt.cm.jet, origin='lower', interpolation='nearest', extent=[0.5,3072.5,0.5,192.5], aspect='auto')
im = ax.imshow(H, cmap=plt.cm.jet, origin='lower', interpolation='nearest', extent=[0.5,768.5,0.5,192.5], aspect='auto')
fig.colorbar(im, ax=ax)
plt.show()

