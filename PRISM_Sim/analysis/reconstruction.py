import numpy as np
import matplotlib.pyplot as plt
import pickle as pickle
import healpy as hp

plt.close("all")

#%%==============================================================================
# Read from binary file

loadfromfile = False

if (loadfromfile):
    dt = np.dtype([('evtN', np.uint32), ('HitNum', np.int8), ('Energy', np.float32), ('DetID', np.uint8), ('DOI', np.uint8),('HPindex', np.uint16)])
    fName = '../output/HP16Ring_DetRing_Response_20k_2152Mask.bin'
    data = np.fromfile(fName, dtype=dt)



#%%==============================================================================
# read from text file  (out of date)

loadfromtext = False

if loadfromtext:

    evtN, HitNum, Energy ,DetID, DOI, HPindex=[]
    
    fName = '../output/HP16Ring_DetRing_Response_20k_2152Mask.bin'
    a = np.loadtxt(fName, delimiter='\t')
    
    for i in range(0,len(a)):
        evtN.append(a[i][0])
        HitNum.append(a[i][1])
        Energy.append(a[i][2])
        DetID.append(a[i][3])
        DOI.append(a[i][4])
        HPindex.append(a[i][5])



#%%==============================================================================
# Get the system response matrix (detID vs. Healpix ID)

NSIDE = 16

detbins = 192
HPbins = 12 * NSIDE * NSIDE

if (loadfromfile):
    fName1 = "HP16_2152Mask_20k_Response.p"
    response = (np.histogram2d(data['DetID'],data['HPindex'], bins=[detbins,HPbins], range=[[0.5,detbins+0.5],[0.5,HPbins+0.5]]))[0]
    pickle.dump(response, open(fName1, "wb"))

else:
    fName2 = "HP16_2152Mask_20k_Response.p"
    response = pickle.load(open(fName2, "rb"))



#%%==============================================================================
# Plot the system response matrix 

plt.figure(1)
im = plt.imshow(response, cmap=plt.cm.jet, origin='lower', interpolation='nearest', extent=[0.5, HPbins + 0.5, 0.5, detbins + 0.5], aspect='auto')
plt.colorbar(im)
# plt.clim(0,175)
# plt.xticks(np.arange(0,HPbins+1,200))
plt.xlabel("HEALPix Index")
# plt.yticks(np.arange(0,detbins+1,12))
plt.ylabel("Detector Index")


#--------------------------------------------------------------------------------
# Plot transpose(response)*response

plt.figure(2)
im = plt.imshow(np.dot(np.transpose(response), response), cmap=plt.cm.jet, origin='lower', interpolation='nearest', extent=[0.5, HPbins + 0.5, 0.5, HPbins + 0.5], aspect='auto')
plt.colorbar(im)
# plt.xticks(np.arange(0,HPbins+1,500))
plt.xlabel("HEALPix Index")
# plt.yticks(np.arange(0,HPbins+1,500))
plt.ylabel("HEALPix Index")



#%%==============================================================================
# MLEM reconstruction

mlem = True

if (mlem):

    # initialize the mlem reconstruction matrix
    recon = np.zeros((HPbins, HPbins))

    # Remove all the empty rows from the response matrix
    response_ = response[~np.all(response == 0, axis=1)]

    # Loop through all healpix angles
    for i in range(0, HPbins):
        
        # Pull out a signal
        signal = response_[:, i]
        
        # Number of iterations
        itr = 20;
        # print "\nRunning MLEM with",itr,"iterations..."

        # Initialize the image to ones
        image = np.ones(HPbins)

        # Iterate (see Lange and Carson, 1984)
        S = np.sum(response, axis=0)  # nice to have this separate, could put directly into image line though...
        for iteration in range(1, itr + 1):
            image = (image / S) * np.dot(signal / np.dot(response_, image), response_)

        # Normalize image
        image = image / np.sum(image)

        # Push image into the reconstruction matrix
        recon[i, :] = image

    #--------------------------------------------------------------------------
    # Plot the reconstruction for each HPindex

    plt.figure(3)
    im2 = plt.imshow(recon, cmap=plt.cm.jet, interpolation='nearest', origin="lower", extent=[0.5, HPbins + 0.5, 0.5, HPbins + 0.5], aspect='auto')
    plt.colorbar(im2)
    plt.clim(0, 0.003)
    plt.xlabel("True HP Index")
    plt.ylabel("Reconstructed HP Index")
    
    #--------------------------------------------------------------------------

    
    #--------------------------------------------------------------------------
    # Interactively plot MLEM recons on HEALPix sphere
    
    from matplotlib.widgets import Slider
    
    hpi = np.arange(0, 12*NSIDE*NSIDE)
    plt.figure(4)
    hp.mollview(recon[hpi[0],:], fig=4, cbar=False, title="")
    
    slider = True   #include the slider to look interactively
    
    if slider:
        def update(val):
            hp.mollview(recon[hpi[int(shp.val)],:], fig=4, cbar=False, title="")
    
        axslide = plt.axes([0.25, 0.1, 0.65, 0.03])
        shp = Slider(axslide, "HP Index", 0, HPbins, valinit=0, valfmt="%i", dragging=True)
        shp.on_changed(update)

    #--------------------------------------------------------------------------


    #--------------------------------------------------------------------------
    # Plot just one MLEM recon on HEALPix sphere
       
    hpi_ = 45
    hp.mollview(recon[hpi_,:], cbar=True, title="MLEM Reconstruction")
    #hp.mollview(image, cbar=True, title="MLEM Reconstruction")
    
    #--------------------------------------------------------------------------



#%%==============================================================================
# Show all the plots

plt.show()

#==============================================================================



