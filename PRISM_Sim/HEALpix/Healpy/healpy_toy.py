from pylab import *
import healpy as hp


NSIDE = 16

print hp.nside2npix(NSIDE)

m = np.ones(3072)
m[1500:1600]=2

hp.mollview(m)
plt.show()