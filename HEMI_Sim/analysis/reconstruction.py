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
# Select reconstruction method
mlem = True
crosscorr = True
    
# Select plot or save
plot = False
save = True

# Load in system response matrix
response=pickle.load(open("systemarray.p","rb"))

# Plot the system response and its autocorrelation
f1 = plt.figure()
responseplot = plt.imshow(response, origin='lower', interpolation='nearest', aspect="auto")
plt.colorbar(responseplot)
plt.title("System Response")
plt.xlabel("Incident Angle")
plt.ylabel("Detector ID")
if (save):
    f1.savefig("images/systemresponse.pdf")
# add code for autocorrelation



# Define angles of interest
theta = [-20,-10,0,10,20,-20,-10,0,10,20,-20,-10,0,10,20,-20,-10,0,10,20,-20,-10,0,10,20]
phi   = [-20,-20,-20,-20,-20,-10,-10,-10,-10,-10,0,0,0,0,0,10,10,10,10,10,20,20,20,20,20]
#theta = [0]
#phi =[0]

for i in range(0,len(theta)):

    #Define signal (pull from response)
    signal = np.zeros(64)
    signal += response[:,((theta[i] + 30)*61 + (phi[i] + 31))-1]
    #signal=[0.,310.,0.,275.,193.,209.,114.,258.,0.,121.,0.,100.,253.,0.,0.,0.,321.,231.,120.,66.,115.,0.,223.,135.,0.,24.,297.,0.,102.,159.,0.,243.,37.,161.,138.,121.,136.,214.,96.,86.,0.,125.,0.,320.,320.,156.,0.,123.,138.,292.,125.,87.,99.,177.,0.,0.,278.,180.,0.,82.,183.,102.,82.,125.]
    #signal = signal/np.sum(signal)
    
    # Maximum Likelihood Expectation Maximization
    if (mlem):
        
        # Number of iterations
        itr = 20;
        print "\nRunning MLEM with",itr,"iterations..."
        
        # Initialize the image to ones
        image = np.ones(3721)
        
        # Iterate (see Lange and Carson, 1984)
        S = np.sum(response, axis=0)            # nice to have this separate, could put directly into image line though...
        for iteration in range(1,itr+1):
            image = (image/S)*np.dot(signal/np.dot(response,image),response)
    
        # Normalize image
        image = image/np.sum(image)
    
        # Create/save the image plot
        f2 = plt.figure()
        MLEMplot = plt.imshow(image.reshape((61,61)), origin='lower', interpolation='nearest', extent=[-30.5,30.5,-30.5,30.5], cmap='jet')
        plt.colorbar(MLEMplot)
        plt.title("MLEM image - %s iterations"%(itr))
        plt.xlabel("Phi (deg)")
        plt.ylabel("Theta (deg)")
        if (save):
            f2.savefig("images/MLEM/mlem(%s,%s).pdf"%(theta[i],phi[i]))
    
    
    # Cross Correlation
    if (crosscorr):
    
        print "Running cross correlation..."
    
        # Calculate cross correlation (from wikipedia and Andreas' HemiImager.cc)
        image = np.dot(signal-np.mean(signal), response-np.mean(response)) / (np.std(signal) * np.std(response))
        image[image<0]=1e-8
        
        # Normalize image
        image = image/np.sum(image)
    
        # Create/save the image plot
        f3 = plt.figure()
        CCplot = plt.imshow(image.reshape((61,61)), origin='lower', interpolation='nearest', extent=[-30.5,30.5,-30.5,30.5], cmap='jet')
        plt.colorbar(CCplot)
        plt.title("Cross Correlation image")
        plt.xlabel("Phi (deg)")
        plt.ylabel("Theta (deg)")
        if (save):
            f3.savefig("images/Cross_Correlation/crosscorr(%s,%s).pdf"%(theta[i],phi[i]))

    if (plot):
        plt.show()



