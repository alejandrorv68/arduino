# -*- coding: utf-8 -*-
"""
Created on Mon Aug  3 20:57:07 2020

@author: arozenevallesp
"""


import ctypes
import os
import math
import numpy as np
import numpy.ctypeslib as ctl
import matplotlib.pyplot as plt
import sys

thispath = os.path.dirname(os.path.abspath(__file__))

abs_dllpath = os.path.abspath(thispath + "/usbdrvd.dll")


curpath = os.getcwd()

os.chdir(os.path.dirname(abs_dllpath))


#lib = ctypes.WinDLL.LoadLibrary(abs_dllpath)
#ctypes.cdll.LoadLibrary(abs_dllpath)


lib = ctypes.CDLL(abs_dllpath)