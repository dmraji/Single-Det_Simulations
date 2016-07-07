import numpy as np
import matplotlib.pyplot as plt
import pickle as pickle


# ---------------------------------------------
# Read from binary file
# ---------------------------------------------

loadfromfile = False

if (loadfromfile):
	dt = np.dtype([('evtN', np.uint32), ('HitNum', np.int8), ('Energy', np.float32), ('DetID',np.uint8),('DOI', np.uint8),('HPindex',np.uint16)])
	fName = '../output/HP16Ring_DetRing_Response_20k_2152Mask.bin'
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
detbins = 192
HPbins = 3072 #768
if (loadfromfile):
	response, xedges, yedges, img = plt.hist2d(data['DetID'],data['HPindex'], bins=[detbins,HPbins], range=[[0.5,detbins+0.5],[0.5,HPbins+0.5]])
	plt.close()
	pickle.dump(response, open("HP16_2152Mask_20k_Response.p","wb"))
else:
	response=pickle.load(open("HP16_2152Mask_20k_Response.p","rb"))

plt.figure(1)
im = plt.imshow(response, cmap=plt.cm.jet, origin='lower', interpolation='nearest', extent=[0.5,HPbins+0.5,0.5,detbins+0.5], aspect='auto')
plt.colorbar(im)
#plt.clim(0,175)
plt.xticks(np.arange(0,HPbins+1,200))
plt.xlabel("HEALPix Index")
plt.yticks(np.arange(0,detbins+1,12))
plt.ylabel("Detector Index")


# Plot transpose(response)*response
plt.figure(2)
im = plt.imshow(np.dot(np.transpose(response),response), cmap=plt.cm.jet, origin='lower', interpolation='nearest', extent=[0.5,HPbins+0.5,0.5,HPbins+0.5], aspect='auto')
plt.colorbar(im)
plt.xticks(np.arange(0,HPbins+1,500))
plt.xlabel("HEALPix Index")
plt.yticks(np.arange(0,HPbins+1,500))
plt.ylabel("HEALPix Index")




#
# MLEM
#
mlem = True

if (mlem):
	sourcebin = 1500
	signal = response[:,sourcebin]
	
	# Number of iterations
	itr = 20;
	print "\nRunning MLEM with",itr,"iterations..."
	
	# Initialize the image to ones
	image = np.ones(HPbins)
	
	# Iterate (see Lange and Carson, 1984)
	S = np.sum(response, axis=0)            # nice to have this separate, could put directly into image line though...
	for iteration in range(1,itr+1):
	    image = (image/S)*np.dot(signal/np.dot(response,image),response)
	image[np.isnan(image)] = 0

	# Normalize image
	image = image/np.sum(image)
	
	plt.figure(3)
	plt.hist(range(0,HPbins), weights=image, bins=HPbins)
	plt.title("MLEM image - %s iterations ... Source at HPindex = %s"%(itr, sourcebin))
	plt.xlabel("HPindex")
	plt.ylabel("MLEM Intensity")



# Show plots
plt.show()




'''
# Create/save the image plot
f2 = plt.figure(3)
MLEMplot = plt.imshow(image.reshape((61,61)), origin='lower', interpolation='nearest', extent=[-30.5,30.5,-30.5,30.5], cmap='jet')
plt.colorbar(MLEMplot)
plt.title("MLEM image - %s iterations"%(itr))
plt.xlabel("Phi (deg)")
plt.ylabel("Theta (deg)")
if (save):
    f2.savefig("images/MLEM/mlem(%s,%s).pdf"%(theta[i],phi[i]))
'''



