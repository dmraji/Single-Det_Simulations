import math
from math import pi, cos, sin
import numpy as np
from numpy import array, zeros, ones
import matplotlib
import matplotlib.pyplot as plt
import ROOT as ROOT
from scipy import arange
from matplotlib.colors import LogNorm
import time

start_time = time.clock()


# Select reconstruction method
crosscorr = False
mlem = True

writetofile = False

f1 = ROOT.TFile("../xcode/Debug/totalresponse.root")
response = f1.Get("totalresponse")



'''
print "Reading in system matrix..."
_response = zeros((64,3721))
for detbin in range(1,64+1):
    imagebin = 0
    for i in range(1,61+1):
        for j in range(1,61+1):
            response.GetZaxis().SetRange(detbin,detbin)
            _response[detbin-1][imagebin] = response.Project3D("xy").GetBinContent(i,j)
            imagebin += 1
_response /= np.sum(_response)
'''

'''
if (writetofile):
    f1 = open("systemarray.txt", 'w+')
    f1.write("_response={[")
    for detbin in range(1,64+1):
        for imagebin in range(1,3721+1):
            f1.write("%f, " % _response[detbin-1][imagebin-1])
        f1.write("],\n[")
    f1.write("]}")
    f1.close()

import pickle
pickle.dump(_response, open("blah.p","wb"))

'''

import pickle
_response=pickle.load(open("systemarray.p","rb"))


_signal = zeros(64)

theta = 10
phi = 3

binnumber = (theta + 30)*61 + (phi + 31)

for detbin in range(1,64+1):
    _signal[detbin-1] = _response[detbin-1][binnumber-1]


theta = -20
phi = 5

binnumber = (theta + 30)*61 + (phi + 31)

for detbin in range(1,64+1):
    _signal[detbin-1] += _response[detbin-1][binnumber-1]


if (mlem):

    #h_image = ROOT.TH2D("image","",61,-30.5,30.5, 61,-30.5, 30.5)

    print "Running MLEM..."
    _lambda = ones(3721) 
    for iteration in range(1,21):
        print "Iteration", iteration
        
        _lambda = _lambda/np.sum(_response, axis=0)*np.dot(_signal/np.dot(_response,_lambda) , _response)
        
        plt.imshow(_lambda.reshape((61,61)), origin='lower', interpolation='none', extent=[-30,30,-30,30])
        plt.xlabel("Phi (deg)")
        plt.ylabel("Theta (deg)")
        plt.show()
        

    '''
    for j in range(1,3721+1):
        _sum_ = 0.
        for detbin in range(1,64+1):
            _sum_ += (_response[detbin-1][j-1]*_signal[detbin-1]) / np.dot(_response[detbin-1],_lambda)
            
        _lambda[j-1] = (_lambda[j-1] * _sum_) / _response[:,j-1].sum()
    '''
    
    
    
    
    
    
    
    
    
    

    '''
    index = 0
    for i in range(1,61+1):
        for j in range(1,61+1):
            h_image.SetBinContent(i, j, _lambda[index])
            index += 1


    f3 = ROOT.TFile("MLEM_reconstruction.root", "recreate")
    h_image.Write()
    '''




'''

# Cross Correlation algorithm
if (crosscorr):

    print "Performing cross correlation"

    for j in range(1,181):
        mean = 0.
            meantest = 0.
            for i in range(0,h_backplane.GetSize()):
                mean += h_backplane.GetBinContent(i)
                    meantest += response.GetBinContent(j,i)
            mean /= h_backplane.GetSize()
            meantest /= response.GetYaxis().GetNbins()
            
            Nominator = 0.
            sig = 0.
            sigtest = 0.
            for i in range(0,h_backplane.GetSize()):
                Nominator += (h_backplane.GetBinContent(i)-mean)*(response.GetBinContent(j,i)-meantest)
                    sig += ((h_backplane.GetBinContent(i)-mean)**2)
                    sigtest += ((response.GetBinContent(j,i)-meantest)**2)
        
            value = Nominator/math.sqrt((sig*sigtest))
            if (value < 0):
                value = 0.0001
                #print value
        
            h_image.SetBinContent(j,value)

    f3 = ROOT.TFile("CrossCorr_reconstruction.root", "recreate")
    h_image.Draw()
    h_image.Write()

'''


'''
# MLEM algorithm
if (mlem):

    response.GetYaxis().SetRange(30,30)
    signal = response.Project3D("zx").ProjectionY("signal",30,30)
    norm = 1./signal.Integral()
    signal.Scale(norm)

    h_image =  ROOT.TH2D("image","",60,-30,30, 60,-30, 30)

    
    print "Performing MLEM"

    f3 = ROOT.TFile("MLEM_reconstruction.root", "recreate")

    _lambda = ones((60,60))
    detected_array = np.frombuffer(signal.GetArray(),'d', signal.GetNbinsX(),8)

    for iteration in range(1,5):
        print "Iteration # = ", iteration
    
        for w in range(1,h_image.GetNbinsX()+1):
    
            response.GetYaxis().SetRange(w,w)
            
            blah = response.Project3D("zx")
            norm = 1./blah.Integral()
            blah.Scale(norm)
            blah.Draw()
            blah.Write()
            
        
            for j in range(1,h_image.GetNbinsY()+1):
                
                response_array_dets = np.frombuffer(response.Project3D("zx").ProjectionY("dets",j,j).GetArray(),'d', response.Project3D("zx").ProjectionY("dets",j,j).GetNbinsX(),8)
                sum_ = 0.
                for i in range(1,response_array_dets.size+1):
                    response_array_angles = np.frombuffer(response.Project3D("zx").ProjectionX("angles",i,i).GetArray(),'d', response.Project3D("zx").ProjectionX("angles",i,i).GetNbinsX(),8)
                    sum_ += (response_array_dets[i-1]*detected_array[i-1])/np.dot(response_array_angles, _lambda[w-1])
    
                _lambda[w-1][j-1] = _lambda[w-1][j-1] * sum_ / np.sum(response_array_dets)
        


        for i in range(1,h_image.GetNbinsX()+1):
            for j in range(1,h_image.GetNbinsY()+1):
                h_image.SetBinContent(i,j,_lambda[i-1][j-1])
    
        h_image.Draw()
        h_image.Write()
'''



print
print time.clock() - start_time, "seconds"

