ypixels = 480;
xpixels = 640;
theta = 110;
phi = ypixels/xpixels*theta;
xmax = 15;
% 
% xmin = height/tan(phi/2*pi/180);
% xwidth = @(x) 2*depth*tan(theta/2*pi/180); %inline function
% 
% maxfloory = 

center = [(xpixels+1)/2, (ypixels+1)/2];

xdif = (ones(ypixels,1)*(1:xpixels) - center(1))*(tan(theta/2*pi/180)/center(1));
ydif = ((1:ypixels)'*ones(1,xpixels) - center(2))*(tan(phi/2*pi/180)/center(2));
rays = [xdif(:), ydif(:), ones(length(xdif(:)),1)];
ray0 = [0;1;0];

%%plane
planenormal = [0;1;0];
planepoint = [0;0;0];

d = ((planepoint - ray0)'*planenormal)*((rays*planenormal).^-1);
pc = repmat(d,[1,3]).*rays+repmat(ray0',[length(xdif(:)),1]);

pc = pc(d > -1*10^-10,:);
pc = pc(pc(:,3) < 15,:);
plot3(pc(:,1),pc(:,2),pc(:,3),'.');


% plot3([zeros(4,1) [xangles(1,1);xangles(1,xpixels);xangles(ypixels,1);xangles(ypixels,xpixels)]]',...
%     [zeros(4,1) [yangles(1,1);yangles(1,xpixels);yangles(ypixels,1);yangles(ypixels,xpixels)]]',...
%     [zeros(4,1) ones(4,1)]');


%%cylinder axis: pcyl + ncyl*t, radius: r, point on surface: q
% (q-pcyl-dot(ncyl,q-pcyl)ncyl)^2-r^2 = 0

% ray: ray0 + ray*d
% intersection: ray0 - pcyl+ray*t-dot(ncyl,ray0-pcyl+ray*t)*ncyl)^2-r^2 = 0
% At^2 + B*t + C = 0
% A = (ray-dot(ray,ncyl)ncyl)^2
% B = 2*dot(ray-dot(ray,ncyl)ncyl,dp-dot(dp,ncyl)ncyl)
% C = (dp-dot(dp,ncyl)*ncyl)^2 - r^2
% dp = ray0-pcyl
