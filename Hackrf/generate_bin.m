clc()
clear all
clear global
close all
%clf
more off; %show output immediately (also fflush(stdout))
beep_on_error = true;

disp('start')

time_low = 500000; %time in us
time_high = 1000; %time in us
time_sampling = 0.5; %2000000 samples per second->two sample per us


filepath = '';
filename = 'prog_06_06_24.bin';

a1re = 127;  %I1
a1im = 0 ; %Q1
n1 = round(time_high/time_sampling);

a2re = 0;
a2im = 0;
n2 = round(time_low/time_sampling);

nrep = 100; %number of cycles

a(2*(n1+n2)*nrep) = int8(0);  %array preallocation

counter = 1;

for jj=1:nrep

for ii = 1:n2
 a(counter) = int8(a2re);
 a(counter+1) = int8(a2im);
 counter = counter + 2;
end  %ii

for ii = 1:n1
 a(counter) = int8(a1re);
 a(counter+1) = int8(a1im);
 counter = counter + 2;
end  %ii


end%jj

disp('data ready')

size(a)

fd = fopen([filepath filename], 'w')
disp('saving data')
fwrite(fd, a);
fclose(fd);
disp('data saved')
