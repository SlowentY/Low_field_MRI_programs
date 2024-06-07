%2023.06 hackrf I/Q file +-127

beep
%sound(2*sin(0:0.1:200))

clc()
clear all
clear global
close all
%clf
more off; %show output immediately (also fflush(stdout))
beep_on_error = true;

disp('start')

infostep = 10000+1

filepath = ''
filename = 'a.bin'

a1re = 127  %I1
a1im = 0  %Q1
n1 = 10

a2re = 0
a2im = 0
n2 = 20

nrep = 100000

a(2*(n1+n2)*nrep) = int8(0);  %array preallocation

counter = 1;

for jj=1:nrep

for ii = 1:n1
 a(counter) = int8(a1re);
 a(counter+1) = int8(a1im);
 counter = counter + 2;
end  %ii

for ii = 1:n2
 a(counter) = int8(a2re);
 a(counter+1) = int8(a2im);
 counter = counter + 2;
end  %ii

if (mod(counter, infostep)==0) jj  end  %if

end  %jj

disp('data ready')

size(a)

fd = fopen([filepath filename], 'w')
disp('saving data')
fwrite(fd, a);
fclose(fd);
disp('data saved')

disp('done')
beep

%------------

