function d = cylinderintersect(rays,ray0,radius,axispoint,axisvect)
%%cylinder axis: pcyl + ncyl*t, radius: r, point on surface: q
% (q-pcyl-dot(ncyl,q-pcyl)ncyl)^2-r^2 = 0

% ray: ray0 + ray*d
% intersection: ray0 - pcyl+ray*t-dot(ncyl,ray0-pcyl+ray*t)*ncyl)^2-r^2 = 0
% At^2 + B*t + C = 0
% A = (ray-dot(ray,ncyl)ncyl)^2
% B = 2*dot(ray-dot(ray,ncyl)ncyl,dp-dot(dp,ncyl)ncyl)
% C = (dp-dot(dp,ncyl)*ncyl)^2 - r^2
% dp = ray0-pcyl

nrays = size(rays,1);


dotvect = rays*axisvect;
Av = rays - repmat(dotvect,[1,3]).*(repmat(axisvect,[1,nrays])');

dp = ray0-axispoint;
dotpv = dp'*axisvect;
Cv = dp - dotpv*axisvect;

A = sum(Av.*Av,2);
B = 2*Av*Cv;
C = Cv'*Cv-radius*radius;

[d1,d2] = qroots(A,B,C);
d1(imag(d1)~=0) = -1;
d2(imag(d2)~=0) = -1;
d = min(d1,d2);