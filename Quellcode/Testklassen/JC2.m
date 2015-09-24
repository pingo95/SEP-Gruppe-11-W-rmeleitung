function [u,steps] = JC(A,b)
% iterativer Loeser: Jacobi-Verfahren

maxIt = 100;
N = size(A,1); % Dimension des Problems
eps = 1e-5; % Abbruchkriterium
steps = 0; % Anzahl Iterationen
old = zeros(N,1); % Startvektor
res = norm(b-A*old,2); % r0 in der unendlich-Norm

while( res - eps > 0 && steps < maxIt)
    steps = steps+1;
    for i=1:N
        sum = 0;
        for j=1:i-1
            sum = sum + A(i,j) * old(j);
        end
        for j=i+1:N
            sum = sum + A(i,j) * old(j);
        end
        u(i,1) = 1/A(i,i) * (b(i) - sum);
    end
    old = u;
    res = norm(b-A*u,2); % rSteps
end

end

