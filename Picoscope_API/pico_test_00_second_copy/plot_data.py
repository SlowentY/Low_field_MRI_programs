# -*- coding: utf-8 -*-
"""
Created on Wed Dec 13 14:38:57 2023

@author: michael.murzin
"""

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
data = pd.read_csv('test_pico.csv')
data['test_channel'] = data['test_channel']/32761 -0.2
sampling_interval_ns = 5
num_of_samples = 102400
time = np.linspace(0,5*102400*1e-9,102400)
plt.plot(time[0:400],data['test_channel'][0:400])
plt.xlabel("time,seconds")
plt.ylabel("Ampl,volts")

