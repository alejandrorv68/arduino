# -*- coding: utf-8 -*-


import ctypes
import os
import math
import numpy as np
import numpy.ctypeslib as ctl
import matplotlib.pyplot as plt
import sys


    
    

class DMDControl:
    PatternDType = np.dtype([
        ('exposure', '<i4'), #us
        ('darktime', '<i4'), #us
        ('trigin', '<i4'),
        ('trigout', '<i4'),
        ('imageIndex', '<i4'),
        ('imageBitIndex', '<i4'),
        ('bits', '<i4')
    ])

    def __init__(self, debugMode=False):
        self.inst = None
        
        thispath = os.path.dirname(os.path.abspath(__file__))

        if ctypes.sizeof(ctypes.c_voidp) == 4:
            raise RuntimeError(f"The andorcam library can only be used with 64-bit python.")

        if debugMode:
            abs_dllpath = os.path.abspath(thispath + "/dlpc900/x64/Debug/dlpcontrol.dll")
        else:
            abs_dllpath = os.path.abspath(thispath + "/dlpc900/x64/Release/dlpcontrol.dll")

        print("Using " + abs_dllpath)
        self.debugMode = debugMode

        curpath = os.getcwd()
        os.chdir(os.path.dirname(abs_dllpath))
        self.lib = ctypes.CDLL(abs_dllpath)

        self._DMD_Create = self.lib.DMD_Create
        self._DMD_Create.restype = ctypes.c_void_p
        
        self._DMD_Free = self.lib.DMD_Free
        self._DMD_Free.argtypes = [ctypes.c_void_p]

        self._DMD_Enable= self.lib.DMD_Enable
        self._DMD_Enable.argtypes = [ctypes.c_void_p, ctypes.c_bool]

        #CDLL_EXPORT const char* DMD_GetInfo(DMDControl* c, int& width, int& height); // returns serial number
        self._DMD_GetInfo = self.lib.DMD_GetInfo
        self._DMD_GetInfo.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_int32), ctypes.POINTER(ctypes.c_int32)]
        self._DMD_GetInfo.restype = ctypes.c_char_p
        
        self._DMD_SendPatterns = self.lib.DMD_SendPatterns
        self._DMD_SendPatterns.argtypes = [
            ctypes.c_void_p, 
            ctl.ndpointer(np.uint32, flags="aligned, c_contiguous"),
            ctypes.c_int32, # num images 
            ctl.ndpointer(self.PatternDType, flags="aligned, c_contiguous"),
            ctypes.c_int32, # num patterns
        ]
        
        #CDLL_EXPORT DMDControl* DMD_Create();
        #CDLL_EXPORT void DMD_Free(DMDControl* c);
        #CDLL_EXPORT void DMD_Enable(DMDControl* c, bool enable); // run patterns on/off
        #CDLL_EXPORT const char* DMD_GetInfo(DMDControl* c, int& width, int& height); // returns serial number
        
        #// images are 32 bit, with the highest 8 bits ignored. (pixel & 0xffffff is what is passed to DMD)
        #// total compressed size is returned (total bytes sent to device)
        #CDLL_EXPORT int DMD_SendPatterns(DMDControl* c, uint32_t* images, int numImages, DMDPattern* patterns, int numpatterns);

        self.inst = self._DMD_Create()        
        w = ctypes.c_int32()
        h = ctypes.c_int32()
        usb_id = self._DMD_GetInfo(self.inst, ctypes.pointer(w), ctypes.pointer(h))
        self.usb_id = usb_id.decode('utf-8')
        
        self.width = w.value
        self.height = h.value
        
        self.shape = (self.height,self.width)
        
        print(f"DMD Serial#: {self.usb_id}. Width={self.width}, Height={self.height}")
        
        self.clear()
    
    def clear(self):
        self.patterns = np.zeros(0, dtype=self.PatternDType)
        self.images = None
        self.enable(False)
    
    def close(self):
        if self.inst is not None:
            self._DMD_Free(self.inst)
            self.inst = None
            
    
        if self.lib is not None:        
            # Free DLL so we can overwrite the file when we recompile
            ctypes.windll.kernel32.FreeLibrary.argtypes = [ctypes.wintypes.HMODULE]
            ctypes.windll.kernel32.FreeLibrary(self.lib._handle)
            self.lib = None

    def addPattern(self, imageIdx, imageBit=0, exposure=100000, 
                   numbits=8, trigin=False, trigout=True,darktime=0):
        npat = np.zeros(1+len(self.patterns), dtype=self.PatternDType)
        npat[:-1] = self.patterns
        npat[-1]['exposure'] = exposure
        npat[-1]['imageIndex'] = imageIdx
        npat[-1]['imageBitIndex'] = imageBit
        npat[-1]['trigin'] = trigin
        npat[-1]['trigout'] = trigout
        npat[-1]['bits'] = numbits
        npat[-1]['darktime'] = darktime
        
        self.patterns = npat
        
    def send(self):
        data = np.ascontiguousarray(self.images, dtype=np.uint32)
        
        expected_shape = [len(self.images), self.height, self.width]
        if not np.array_equal(data.shape, expected_shape):
            raise ValueError(f'Expecting images to have shape {expected_shape}. Given:{data.shape}')
        
        p = np.ascontiguousarray(self.patterns)
        self._DMD_SendPatterns(self.inst, data, len(self.images), p, len(p))
        
    def set_trigger_out2():
        ...
        
    def set_trigger_out1():
        ...
        
    def enable(self, active):
        self._DMD_Enable(self.inst, active)

    def __enter__(self):
        return self
    
    def __exit__(self, *args):
        self.close()
        


def randomBlocks(blocksize, shape):
    blocks = np.random.randint(2, size=(1+shape[0]//blocksize,1+shape[1]//blocksize))
    blocks = np.array(blocks, dtype=np.uint32)
    blocks *= 0x00ffffff
    X,Y=np.meshgrid(np.arange(shape[1]),np.arange(shape[0]))
    return blocks[Y//blocksize, X//blocksize]


if __name__=="__main__":
    
    with DMDControl(debugMode=False) as dmd:

        images = []

        # for i, bs in enumerate(np.arange(10, 100, 10)):
        #     images.append(randomBlocks(bs,dmd.shape))

        # for i in range(len(images)):
        #     dmd.addPattern(i, exposure=100000)


        AllOn = np.ones((1080,1920))
        AllOn *=0x00ffffff
        images.append(AllOn)
        images.append(AllOn)
        
        for i in range(len(images)):
            dmd.addPattern(len(images)-i-1, exposure=100000)
        dmd.images = images
        dmd.send()
        dmd.enable(True)
        
        print('all sent.')
        
        while True:
            ...
            
        