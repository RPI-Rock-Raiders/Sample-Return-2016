ypixels = 480;
xpixels = 640;
theta = 110;
phi = ypixels/xpixels*theta;
xmax = 15;

tilt = 10;

R = [1 0 0;
    0 cos(tilt*pi/180) -sin(tilt*pi/180);
    0 sin(tilt*pi/180) cos(tilt*pi/180)];

close all;
% 
% xmin = height/tan(phi/2*pi/180);
% xwidth = @(x) 2*depth*tan(theta/2*pi/180); %inline function
% 
% maxfloory = 

center = [(xpixels+1)/2, (ypixels+1)/2];

xdif = (ones(ypixels,1)*(1:xpixels) - center(1))*(tan(theta/2*pi/180)/center(1));
ydif = (center(2) - (1:ypixels)'*ones(1,xpixels))*(tan(phi/2*pi/180)/center(2));
rays = [xdif(:), ydif(:), ones(length(xdif(:)),1)];
ray0 = [0;1;0];
nrays = length(xdif(:));

rays = rays*R';

%cylinder1:
radius = 1;
axisvect = [0;1;0];
axispoint = [0;0;10];
dc1 = cylinderintersect(rays,ray0,radius,axispoint,axisvect);

%cylinder2:
radius2 = 1;
axisvect2 = [0;1;0];
axispoint2 = [-3;0;6];
dc2 = cylinderintersect(rays,ray0,radius2,axispoint2,axisvect2);

%sphere:
Sradius = 2;
Scenter = [2;0;8];
ds = sphereintersect(rays,ray0,Sradius,Scenter);

%%ground
%ground plane
groundnormal = [0;1;0];
groundpoint = [0;0;0];
dg = planeintersect(rays,ray0,groundnormal,groundpoint);
%pothole
Pradius = 1;
Pcenter = [0;0;4];
[d_,dp] = sphereintersect(rays,ray0,Pradius,Pcenter);
%combine
d_(d_ < 0) = nan;
dg(dg < 0) = nan;
dp(dp < 0) = nan;
ind_ph = find(d_ < dg & dg < dp);
dg(ind_ph) = dp(ind_ph);

%%
%select d's
dc1(dc1 < 0) = nan;
dc2(dc2 < 0) = nan;
ds(ds < 0) = nan;
dg(dg < 0) = nan;
d = min([dc1,dc2,ds,dg],[],2);
d(isnan(d)) = -1;


pc = repmat(d,[1,3]).*rays+repmat(ray0',[nrays,1]);
pc = pc(d > -1*10^-10,:);
maxdepth = [0;0;1];
maxdepth = R*maxdepth;
% pc = pc(pc(:,1) < maxdepth(1) & pc(:,2) < maxdepth(2) & pc(:,3) < maxdepth(3),:);
% pc = pc(pc(:,3) < maxdepth(3),:);
pc = pc(pc*maxdepth < 15,:);
figure;
plot3(pc(:,1),pc(:,2),pc(:,3),'.');
axis equal

depth = repmat(d,[1,3]).*rays+repmat(ray0',[nrays,1]);
% depth = depth(:,3);
depth = depth*maxdepth;
depth(d < 0) = 15;
depth = min(depth,15);
depth = reshape(depth,ypixels,xpixels);
depth = (15-depth)./15;
figure;
imshow(depth);