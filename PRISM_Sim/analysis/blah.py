import numpy as np

dt = np.dtype([('evtN', np.uint32), ('HitNum', np.int8), ('Energy', np.uint16), ('DetID',np.uint8),('DOI', np.uint8),('HPindex',np.uint16)])
fName = '../output/HP16Ring_DetRing_Response_20k_PBMask.bin'
data = np.fromfile(fName, dtype=dt)

# access data as follows
data['evtN']