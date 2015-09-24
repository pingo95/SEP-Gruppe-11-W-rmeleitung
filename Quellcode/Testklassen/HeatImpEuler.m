function [t,u,steps] = HeatImpEuler(u0,T,m,A,g,f)
% Liefert Approximation (impliziter Euler) der Loesung der durch A orts-diskretisierten PDE
% und Vektor mit berechneten Zeitpunkten (t)
dt = T/m;
Nx = size(A,1);
n = sqrt(Nx);
dx = 1/(n-1);

e = ones(Nx,1);
B = diag(e) - dt/(dx*dx) * A;

% Approximation fuer das gewuenschte Zeitintervall
t = zeros(m+1,1);
t(1) = 0;
u = zeros(Nx,m+1);
u(:,1) = u0;

for i=2:m+1
    [u(:,i),steps(i,1)] = JC2(B,(u(:,i-1)+dt*f)); 
    for j=1:n
        u(j,i) = g(i);
        u(n*n-j+1,i) = g(4*n-j+1);
    end
    for j=1:n
        u((j-1)*n+1,i) = g(n+2*j-1);
        u(j*n,i) = g(n+2*j);
    end
    t(i) = t(1) + (i-1) * dt;
end

end