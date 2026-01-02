clear;
clc;

[A, ~, C] = myfun(16, 4, 3)

function [out1,out2,out3] = myfun(in1,in2,in3)
 
out1 = 2*in1;
out2 = 4*in2;
out3 = 8*in3;
 
end