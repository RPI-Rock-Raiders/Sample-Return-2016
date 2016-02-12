function d = planeintersect(rays,ray0,planenormal,planepoint)
d = ((planepoint - ray0)'*planenormal)*((rays*planenormal).^-1);