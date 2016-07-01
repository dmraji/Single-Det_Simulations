function c = coordTrans(A,B)
% Coordinate transformation matrix between two sets of orthoganol dimension indices

% Takes 3 dimension indices, e.g. [2 -3 1] and converts to unit vectors [0 1 0; 0 0 -1; 1 0 0]
ind2 = @(m) sign(repmat(m',[1,3])) .* (abs(repmat(m',[1,3])) == repmat(1:3,[3,1]));

a = ind2(A);
b = ind2(B);
c = zeros(3);

for i = 1:3;
    c(1:3,i) = dot(repmat(a(i,:),[3,1]), b, 2);
end


% Provides unit vector 
% 1 -> [1 0 0], -2 -> [0 -1 0]
% ind = @(a) sign(a) * (abs(a) == 1:3);