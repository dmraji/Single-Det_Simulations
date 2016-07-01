import struct
import numpy as np
import matplotlib.pyplot as plt


# ---------------------------------------------
# Read from binary file
# ---------------------------------------------

def read_in_chunks(file_object, chunk_size=11):
    while True:
        data = file_object.read(chunk_size)
        if not data:
            break
        yield data

fp = open('../output/HP8Ring_Response_10k.bin', mode='rb')

evtN=[]
HitNum=[]
Energy=[]
DetID=[]
DOI=[]
HPindex=[]

for piece in read_in_chunks(fp):

    evtN.append(struct.unpack('i', piece[0:4])[0])
    HitNum.append(struct.unpack('B', piece[4:5])[0])
    Energy.append(struct.unpack('h', piece[5:7])[0])
    DetID.append(struct.unpack('B', piece[7:8])[0])
    DOI.append(struct.unpack('B', piece[8:9])[0])
    HPindex.append(struct.unpack('h', piece[9:11])[0])

'''
print evtN
print HitNum
print Energy
print DetID
print DOI
print HPindex
'''
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

'''
# Plot DOI histogram
plt.hist(DOI, bins=np.arange(0.5,21.5))
plt.axis([0,21,0,1500])
plt.show()

# Plot DetID histogram
plt.hist(DetID, bins=np.arange(0.5,192.5))
plt.axis([0,200,0,150])
plt.show()
'''

# Plot DetID vs HPindex
H, xedges, yedges, img = plt.hist2d(DetID,HPindex, bins=[192,768])
extent = [yedges[0], yedges[-1], xedges[0], xedges[-1]]
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
im = ax.imshow(H, cmap=plt.cm.jet, origin='lower', interpolation='nearest', extent=[0,768,0,192], aspect='auto')
fig.colorbar(im, ax=ax)
plt.show()

