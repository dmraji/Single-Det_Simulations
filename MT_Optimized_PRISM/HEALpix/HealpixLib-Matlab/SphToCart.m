% Spherical-Cartesian Coordinate Conversion

function OUT = SphToCart(IN)

n = size(IN, 1);

OUT = zeros(n, 3);

for i = 1:n
    phi = IN(i, 1);
    theta = IN(i, 2);
    rho = 1;
    
    OUT(i, :) = [rho * sin(phi) * cos(theta), rho * sin(phi) * sin(theta), rho * cos(phi)];
end

