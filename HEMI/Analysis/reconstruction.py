# Imports
import math
import numpy as np
import matplotlib.pyplot as plt
import time
import pickle

'''
# Start clock
start_time = time.clock()
# Stop clock, print to screen
print "\nRun time ",time.clock() - start_time, "sec"
'''

'''
# Import system response matrix from ROOT file
import ROOT as ROOT
f1 = ROOT.TFile("../Geant4Sim/build/Debug/totalresponse.root")
response = f1.Get("totalresponse")
    
print "Reading in system matrix..."
_response = zeros((64,3721))
for detbin in range(1,64+1):
    imagebin = 0
    for i in range(1,61+1):cd ..
        for j in range(1,61+1):
            response.GetZaxis().SetRange(detbin,detbin)
            _response[detbin-1][imagebin] = response.Project3D("xy").GetBinContent(i,j)
        imagebin += 1
    _response /= np.sum(_response)

pickle.dump(_response, open("blah.p","wb"))
'''

# Load in system response matrix
response=pickle.load(open("systemarray.p","rb"))

# Define signal (pull from response matrix)

theta = [10,0]
phi = [20,10]
signal = np.zeros(64)
for i in range(0,len(theta)):
    signal += response[:,((theta[i] + 30)*61 + (phi[i] + 31))-1]
'''
signal=[0.,310.,0.,275.,193.,209.,114.,258.,0.,121.,0.,100.,253.,0.,0.,0.,321.,231.,120.,66.,115.,0.,223.,135.,0.,24.,297.,0.,102.,159.,0.,243.,37.,161.,138.,121.,136.,214.,96.,86.,0.,125.,0.,320.,320.,156.,0.,123.,138.,292.,125.,87.,99.,177.,0.,0.,278.,180.,0.,82.,183.,102.,82.,125.]
'''
signal = signal/np.sum(signal)

# Select reconstruction method
mlem = True
crosscorr = True

# Maximum Likelihood Expectation Maximization
if (mlem):
    
    # Number of iterations
    itr = 2;
    print "\nRunning MLEM with",itr,"iterations..."
    
    # Initialize the image to ones
    image = np.ones(3721)
    
    # Iterate (see Lange and Carson, 1984)
    S = np.sum(response, axis=0)            # nice to have this separate, could put directly into image line though...
    for iteration in range(1,itr+1):
        image = (image/S)*np.dot(signal/np.dot(response,image),response)

    # Normalize image
    image = image/np.sum(image)

    # Create the image plot
    f1 = plt.figure()
    MLEMplot = plt.imshow(image.reshape((61,61)), origin='lower', interpolation='nearest', extent=[-30.5,30.5,-30.5,30.5], cmap='jet')
    plt.colorbar(MLEMplot)
    plt.title("MLEM image - %s iterations"%(itr))
    plt.xlabel("Phi (deg)")
    plt.ylabel("Theta (deg)")


# Cross Correlation
if (crosscorr):

    print "Running cross correlation..."

    # Calculate cross correlation (from wikipedia and Andreas' HemiImager.cc)
    image = np.dot(signal-np.mean(signal), response-np.mean(response)) / (np.std(signal) * np.std(response))
    image[image<0]=1e-8
    
    # Normalize image
    image = image/np.sum(image)

    # Create the image plot
    f2 = plt.figure()
    CCplot = plt.imshow(image.reshape((61,61)), origin='lower', interpolation='nearest', extent=[-30.5,30.5,-30.5,30.5], cmap='jet')
    plt.colorbar(CCplot)
    plt.title("Cross Correlation image")
    plt.xlabel("Phi (deg)")
    plt.ylabel("Theta (deg)")



# Plot image(s)
plt.show()


