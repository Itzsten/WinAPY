from winapy_user import *
from winapy_gdi import *
from winapy_con import *
try: WIN32_MEAN_AND_LEAN
except:
	# Remove rarely used packages
	from winapy_mme import *
