function [x1,x2] = qroots(a,b,c)
x1 = (-b + (b.*b - 4*a.*c).^(1/2))./(2*a);
x2 = (-b - (b.*b - 4*a.*c).^(1/2))./(2*a);