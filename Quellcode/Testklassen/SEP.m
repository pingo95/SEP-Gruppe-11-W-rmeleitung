clc
clear all

m = 2;
n = 5; deltaX = 1/(n-1);
T = 1; deltaT = 1/m;

u0 = zeros(n*n,1); g = zeros(4*n,1); f = zeros(n*n,1);

for i=1:n
    g(i) = 300;
    g(4*n+1-i) = 300;
end
for i=1:n
    g(n+2*i-1) = 300;
    g(n+2*i) = 300;
end

for i=1:(n*n)
    u0(i) = 100;
end
for i=1:n
    u0(i) = g(i);
    u0(n*n-i+1) = g(4*n-i+1);
end
for i=1:n
    u0((i-1)*n+1) = g(n+2*i-1);
    u0(i*n) = g(n+2*i);
end

for i=1:(n*n)
    f(i) = 10; % -32
end
for i=1:n
    f(i) = 0;
    f(n*n-i+1) = 0;
end
for i=1:n
    f((i-1)*n+1) = 0;
    f(i*n) = 0;
end

U = reshape(u0,[n,n]);

A = zeros(n*n,n*n);

for i=2:n-1
    for j=2:n-1
        A((i-1)*n+j,(i-2)*n+j) = 1;
        A((i-1)*n+j,(i-2)*n+j+n-1) = 1;
        A((i-1)*n+j,(i-2)*n+j+n) = -4;
        A((i-1)*n+j,(i-2)*n+j+n+1) = 1;
        A((i-1)*n+j,(i-2)*n+j+2*n) = 1;
    end
end  
     

tC = zeros(n*n,1);
for i=1:n*n
    tC(i) = 5;
end
A = diag(tC) * A;

[t,u,steps] = HeatImpEuler(u0,T,m,A,g,f);
for i=1:size(u,2)
    U(:,:,i) = reshape(u(:,i),[n,n]);
end
%U
%surf(0:deltaX:1,0:deltaX:1,U(:,:,m))