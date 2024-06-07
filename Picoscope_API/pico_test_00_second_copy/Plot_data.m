A = importdata('D:\pico\pico_test_00_second_copy\test.txt');
Volts = A/32768 ;
timebase = 1000;
noOfPostTriggerSamples = 102400;
time = linspace(0,20e-9*(timebase-2)*noOfPostTriggerSamples,noOfPostTriggerSamples);
plot(time,Volts);
xlabel('t_{seconds}');
ylabel ('Ampl, Volts');
title('Test signal f = 1KHz, Ampl = 2Vpp')