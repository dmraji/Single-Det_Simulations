# Imports
import math
import numpy as np
import matplotlib.pyplot as plt
import time
import pickle


# Import system response matrix from ROOT file
import ROOT as ROOT
f1 = ROOT.TFile("../Geant4Sim/build/Debug/totalresponse.root")
sysresponse = f1.Get("totalresponse")
    
print "Reading in system matrix..."
_response = np.zeros((64,3721))
for detbin in range(1,64+1):
    imagebin = 0
    for i in range(1,61+1):
        for j in range(1,61+1):
            sysresponse.GetZaxis().SetRange(detbin,detbin)
            _response[detbin-1][imagebin] = sysresponse.Project3D("xy").GetBinContent(i,j)
            imagebin += 1

#_response /= np.sum(_response)

pickle.dump(_response, open("systemarray_new.p","wb"))



