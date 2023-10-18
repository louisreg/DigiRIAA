import matplotlib.pyplot as plt
import numpy as np
import glob
import os
import pandas as pd

def find_nearest(array, value):
    array = np.asarray(array)
    idx = (np.abs(array - value)).argmin()
    return idx

AOUT_L= './data/RIAA_L_AOUT.txt'
AOUT_R = './data/RIAA_R_AOUT.txt'

THD_loopback= './data/loopback.txt'
THD_no_cover = './data/RIAA_L_AOUT_NOTOP_THD.txt'
THD_AOUT_L='./data/RIAA_L_AOUT_THD.txt'
THD_AOUT_R='./data/RIAA_R_AOUT_THD.txt'


R_color = '#AA3939'
L_color = '#226666'
REF_color = '#211919'

freq_RIAA = [10,20,30,40,50,60,80,100,120,150,180,200,300,400,500,600,800,1000,1200,1500
            ,1800,2000,3000,4000,5000,6000,8000,10000,12000,15000,18000,20000,30000,40000]
Ref_RIAA = [19.74,19.27,18.59,17.79,16.95,16.1,14.51,13.09,11.85,10.27,8.97,8.22,5.48,3.78,2.65,1.84,0.75,
            0,-0.61,-1.4,-2.12,-2.59,-4.74,-6.61,-8.21,-9.6,-11.89,-13.73,-15.26,-17.16,-18.72,-19.62,-23.12,-25.6]

AOUT_L = pd.read_csv(AOUT_L)
freq_L = AOUT_L['Freq(Hz)']
GAIN_L = AOUT_L[' SPL(dB)']
idx_1k = find_nearest(freq_L,1000)
g_1k = GAIN_L[idx_1k]
GAIN_L = np.array(GAIN_L)-g_1k

AOUT_R = pd.read_csv(AOUT_R)
freq_R = AOUT_R['Freq(Hz)']
GAIN_R = AOUT_R[' SPL(dB)']
idx_1k = find_nearest(freq_R,1000)
g_1k = GAIN_R[idx_1k]
GAIN_R = np.array(GAIN_R)-g_1k

#Plot Gain
plt.figure(0)
plt.semilogx(freq_RIAA,Ref_RIAA,color = REF_color,linestyle='--',alpha = 0.8,label ='Ref RIAA')
plt.semilogx(freq_L,GAIN_L,color = L_color,label ='Left Channel')
plt.semilogx(freq_R,GAIN_R,color = R_color,label ='Right Channel')
plt.grid()
plt.xlabel('Frequency (Hz)')
plt.ylabel('Gain (dB)')
plt.xlim(min(freq_RIAA),max(freq_RIAA))
plt.legend()
plt.tight_layout()
plt.savefig('GAIN_RIAA.png')


freq_RIAA = freq_RIAA [1:-2]
Ref_RIAA = Ref_RIAA [1:-2]
GAIN_L_interpolated = np.interp(freq_RIAA, freq_L, GAIN_L)
mismatch_L = np.abs(GAIN_L_interpolated-Ref_RIAA)
GAIN_R_interpolated = np.interp(freq_RIAA, freq_R, GAIN_R)
mismatch_R = np.abs(GAIN_R_interpolated-Ref_RIAA)
mismatch_L_R = np.abs(GAIN_L_interpolated-GAIN_R_interpolated)

plt.figure(1)
plt.semilogx(freq_RIAA,mismatch_L,color = L_color,label ='Left Channel')
plt.semilogx(freq_RIAA,mismatch_R,color = R_color,label ='Right Channel')
plt.semilogx(freq_RIAA,mismatch_L_R,color = REF_color,label ='Between Channels')
plt.grid()
plt.xlabel('Frequency (Hz)')
plt.ylabel('Mismatch Gain (dB)')
plt.xlim(min(freq_RIAA),max(freq_RIAA))
plt.legend()
plt.tight_layout()
plt.savefig('Error_RIAA.png')


#plot THD
THD_AOUT_L = pd.read_csv(THD_AOUT_L)
THD_freq_L = THD_AOUT_L['Freq(Hz)']
THD_AOUT_L = THD_AOUT_L[' THD (%)']

THD_AOUT_R = pd.read_csv(THD_AOUT_R)
THD_freq_R = THD_AOUT_R['Freq(Hz)']
THD_AOUT_R = THD_AOUT_R[' THD (%)']

THD_no_cover = pd.read_csv(THD_no_cover)
THD_Freq_no_cover = THD_no_cover['Freq(Hz)']
THD_no_cover = THD_no_cover[' THD (%)']


THD_loopback = pd.read_csv(THD_loopback)
THD_Freq_loopback = THD_loopback['Freq(Hz)']
THD_loopback = THD_loopback[' THD (%)']


plt.figure(2)
plt.semilogx(THD_freq_L,THD_AOUT_L,color = L_color,label ='Left Channel')
plt.semilogx(THD_freq_R,THD_AOUT_R,color = R_color,label ='Right Channel')
#plt.semilogx(THD_Freq_no_cover,THD_no_cover,color = REF_color,label ='No Cover')
plt.semilogx(THD_Freq_loopback,THD_loopback,color = REF_color,linestyle='--',alpha = 0.8,label ='Loopback')
plt.grid()
plt.xlabel('Frequency (Hz)')
plt.ylabel('THD (%)')
plt.xlim(min(THD_freq_R),max(THD_freq_R))
plt.legend()
plt.tight_layout()
plt.savefig('THD_RIAA.png')

#plt.show()