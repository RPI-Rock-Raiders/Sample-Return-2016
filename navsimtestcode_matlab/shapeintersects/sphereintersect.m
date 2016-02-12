function [dmin, dmax] = sphereintersect(rays,ray0,radius,center)

dp = ray0 - center;
A = sum(rays.*rays,2);
B = 2*rays*dp;
C = dp'*dp-radius*radius;

[d1,d2] = qroots(A,B,C);

d1(imag(d1)~=0) = -1;
d2(imag(d2)~=0) = -1;
dmin = min(d1,d2);
dmax = max(d1,d2);