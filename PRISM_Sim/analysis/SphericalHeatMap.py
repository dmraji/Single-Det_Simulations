### Mayavi rendering for spherical heat map

# Plotting the image (or whatever you'd like) on the 3D sphere.

from mayavi import mlab
import matplotlib.pylab as plt
import numpy as np
import healpy as hp

def sphericalheatmap(nside, map_, trueindex = 1e6):
    gridsize = 1000
    theta = np.linspace(np.pi, 0, gridsize)
    phi   = np.linspace(-np.pi, np.pi, gridsize)
    
    # project the map to a rectangular matrix
    PHI, THETA = np.meshgrid(phi, theta)
    grid_pix = hp.ang2pix(nside, THETA, PHI)
    grid_map = map_[grid_pix]
    
    # Create a XYZ sphere from the theta,phi grid
    x = np.sin(THETA)*np.cos(PHI)
    y = np.sin(THETA)*np.sin(PHI)
    z = np.cos(THETA)
    
    mlab.figure(1, bgcolor=(1, 1, 1), fgcolor=(0, 0, 0), size=(400, 300))
    mlab.clf()
    
    mp = mlab.mesh(x, y, z, scalars=grid_map, colormap="jet") 
    mlab.colorbar(mp, orientation="vertical")
    
    # Add a point to the true source location if we supply it
    if (trueindex != 1e6):
        x1,y1,z1 = hp.pix2vec(nside,1848)
        mlab.points3d(x1, y1, z1, mode="sphere", color=(1,1,1), scale_factor=0.02, resolution=20)
    
    # Plot axes (very clunky...)
    #mlab.xlabel("X", mp)
    #mlab.ylabel("Y", mp)
    #mlab.zlabel("Z", mp)
    
    # Plot a title
    mlab.text(0.005,0.05,"Spherical Heat Map")

    # animation (rotate)
    @mlab.animate(delay=60, ui=False)
    def anim():
        f = mlab.gcf()
        while 1:
            f.scene.camera.azimuth(5)
            f.scene.render()
            yield
    
    # Uncomment this line to animate the figure (it will spin in one direction right now...)
    #a = anim()     

    mlab.show()
