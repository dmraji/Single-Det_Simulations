%% Define PRISM Detector Unique Points
% P.Barton 5/18/2016
%

% Outer View (from Z+)
%       Y
%   . . | . . 
% . . . | . 1 2
% . . . | 3 4 5
% ------------- X
% . . . | . . .
% . . . | . . .
%   . . | . . 

L = 9.88;  % depth of detector [mm]
x0 = zeros(5,2,3);  % [five unique locations, 2 outer corners (low-XY, high-XY), xyz [mm]] Extracted from Solidworks Measure Tool
x0(1,:,:) = [30.00 29.99 58.65; 21.49 21.48 65.71];
x0(2,:,:) = [44.03 29.91 49.05; 38.34 20.67 57.83];
x0(3,:,:) = [14.06 14.04 69.60;  4.35  4.34 72.14];
x0(4,:,:) = [31.61 14.03 63.59; 22.97  4.24 68.53];
x0(5,:,:) = [46.98 14.03 53.24; 39.92  4.24 60.25];


%%
D = struct;

% 5 unique 2-corner pairs
fi = 1:5;
ti = [9:10 14:16];
for i = 1:size(x0,1)
    a = squeeze(x0(i,:,:));  % 2 unique xyz points [2x3]
    a = [a; (rotationmat3D(pi/2, mean(a)) * a')'];  % 2 rotated corners [4x3]
    w = [a(1,:) - a(4,:); a(3,:) - a(1,:); 0 0 0];
    [U,~,V] = svd(w');  % Rotation Matrix between Cartesian axes and detector axes (Wahba's Problem)
    D(ti(i)).R = U * V';
    D(ti(i)).c = mean(a)';
end

% 5 -> 8, reflect 3 non-diagonal elements
n = [1 -1 0]' / sqrt(2);  % normal to x=y diagonal
A = eye(3) - 2*(n*n');  % reflection matrix
fi = [16 10 15];
ti = [3 4 8];
for i = 1:length(fi)
    D(ti(i)).R = A * D(fi(i)).R;
    D(ti(i)).c = A * D(fi(i)).c;
end

% 8 -> 16 reflection
n = [1 0 0]';  % normal to x=y diagonal
A = eye(3) - 2*(n*n');  % reflection matrix
fi = [3:4    8:10   14:16];
ti = [2:-1:1 7:-1:5 13:-1:11];
for i = 1:length(fi)
    D(ti(i)).R = A * D(fi(i)).R;
    D(ti(i)).c = A * D(fi(i)).c;
end

% 16 -> 32 rotation 
A = diag([-1 -1 1]); % rotate pi about [0 0 1]
fi = [1:16];
ti = [32:-1:17]; % reverse indices
for i = 1:length(fi)
    D(ti(i)).R = A * D(fi(i)).R;
    D(ti(i)).c = A * D(fi(i)).c;
end

% orient 6 faces of 32 detectors for 192
M = {}; % Orientation matrix of 6 Faces [hor, vert, out-of-face] = [2 1 3] = [+Y +X +Z]
M{1} = [ 0  1 0;  0 0 1; -1  0  0];
M{2} = [ 0 -1 0;  0 0 1;  1  0  0];
M{3} = [-1  0 0;  0 0 1;  0 -1  0];
M{4} = [ 1  0 0;  0 0 1;  0  1  0];
M{5} = [ 0 -1 0;  1 0 0;  0  0 -1];
M{6} = [ 0  1 0;  1 0 0;  0  0  1];
D0 = D;
for i = 1:length(M)
    for j = 1:length(D0)
        D(j + 32*(i-1)).R = M{i}' * D0(j).R;
        D(j + 32*(i-1)).c = M{i}' * D0(j).c;
    end
end

% add vertices
dx = [-1 1] * L/2;
[ax,ay,az] = meshgrid(dx,dx,dx);
a = [ax(:), ay(:), az(:)];
for i = 1:length(D)
    D(i).v = (D(i).R * a')' + repmat(D(i).c',[8 1]);  % rotate and translate
end

% add (x,y,z) Angles (roll,pitch,yaw)
for i = 1:length(D)
    D(i).ang = atan([ +D(i).R(3,2) / D(i).R(3,3); ...
                       -D(i).R(3,1) / norm(D(i).R(1:2,1)); ...
                       +D(i).R(2,1) / D(i).R(1,1)]);
end

% add (x,y,z) Angles (roll,pitch,yaw)
for i = 1:length(D)
    D(i).center = [(D(i).v(1,1)+D(i).v(8,1))/2; ...
                        (D(i).v(1,2)+D(i).v(8,2))/2; ...
                        (D(i).v(1,3)+D(i).v(8,3))/2];
end



% test plot
figure(1); clf; hold on
cmap = gray(256);
F =[1 2 4 3; % inner
    5 6 8 7; % outer
    1 3 7 5; % away from middle
    2 4 8 6; % toward middle
    1 2 6 5; % toward +
    3 4 8 7; % away from +
    ];
for i = 1:length(D)
    patch('Faces',F,'Vertices',D(i).v,'FaceColor',cmap(i,:),'EdgeColor','w')
end
axis vis3d equal off




% write rotations and cube centers to file

%fileID = fopen('rotationangles_Nested.txt','w');
%fprintf(fileID,'%6.8f %6.8f %6.8f\n',D.ang);
%fclose(fileID);

fileID = fopen('centervertices_Nested.txt','w');
fprintf(fileID,'%6.8f %6.8f %6.8f\n',D.center);
fclose(fileID);

fileID = fopen('rotationmatrices_Nested.txt','w');
fprintf(fileID,'%6.8f %6.8f %6.8f %6.8f %6.8f %6.8f %6.8f %6.8f %6.8f \n',D.R);
fclose(fileID);


%get centers
centers = [D.center];

%get rotations
rotations = zeros(192,9);
for i = 1:192
    rotations(i,:)= [D(i).R(1,1), D(i).R(2,1), D(i).R(3,1), D(i).R(1,2), D(i).R(2,2), D(i).R(3,2), D(i).R(1,3), D(i).R(2,3), D(i).R(3,3)];
end

% calc the theta and phi's
phi = acos(centers(3,:)./(sqrt(centers(1,:).^2+centers(2,:).^2+centers(3,:).^2)));
theta = atan2(centers(2,:),centers(1,:));
t = theta<0;
theta(t) = theta(t)+2*pi;

%fill a 2d array with original indicies
thetaphi = zeros(192, 3);
for i = 1:192
    thetaphi(i,1)=theta(i);
    thetaphi(i,2)=phi(i);
    thetaphi(i,3)=i;
end

% sort according to theta (-pi to pi)
thetaphi=sortrows(thetaphi);


sortedthetaphi = zeros(192, 3);
q=1;
for i = 1:size(thetaphi)
    if (thetaphi(i,2) > 0 && thetaphi(i,2) < 0.3)
        sortedthetaphi(q,:)=thetaphi(i,:);
        q = q+1;
    end
end
for i = 1:size(thetaphi)
    if (thetaphi(i,2) > 0.3 && thetaphi(i,2) <0.53 )
        sortedthetaphi(q,:)=thetaphi(i,:);
        q = q+1;
    end
end
for i = 1:size(thetaphi)
    if (thetaphi(i,2) > 0.53 && thetaphi(i,2) < 0.75)
        sortedthetaphi(q,:)=thetaphi(i,:);
        q = q+1;
    end
end
for i = 1:size(thetaphi)
    if (thetaphi(i,2) > 0.75 && thetaphi(i,2) < 1.1)
        sortedthetaphi(q,:)=thetaphi(i,:);
        q = q+1;
    end
end
for i = 1:size(thetaphi)
    if (thetaphi(i,2) > 1.1 && thetaphi(i,2) < 1.3)
        sortedthetaphi(q,:)=thetaphi(i,:);
        q = q+1;
    end
end
for i = 1:size(thetaphi)
    if (thetaphi(i,2) > 1.3 && thetaphi(i,2) < 1.5)
        sortedthetaphi(q,:)=thetaphi(i,:);
        q = q+1;
    end
end
for i = 1:size(thetaphi)
    if (thetaphi(i,2) > 1.5 && thetaphi(i,2) < 1.8)
        sortedthetaphi(q,:)=thetaphi(i,:);
        q = q+1;
    end
end
for i = 1:size(thetaphi)
    if (thetaphi(i,2) > 1.8 && thetaphi(i,2) < 2.1)
        sortedthetaphi(q,:)=thetaphi(i,:);
        q = q+1;
    end
end
for i = 1:size(thetaphi)
    if (thetaphi(i,2) > 2.1 && thetaphi(i,2) < 2.3)
        sortedthetaphi(q,:)=thetaphi(i,:);
        q = q+1;
    end
end
for i = 1:size(thetaphi)
    if (thetaphi(i,2) > 2.3 && thetaphi(i,2) < 2.55)
        sortedthetaphi(q,:)=thetaphi(i,:);
        q = q+1;
    end
end
for i = 1:size(thetaphi)
    if (thetaphi(i,2) > 2.55 && thetaphi(i,2) < 2.8)
        sortedthetaphi(q,:)=thetaphi(i,:);
        q = q+1;
    end
end
for i = 1:size(thetaphi)
    if (thetaphi(i,2) > 2.8 && thetaphi(i,2) < 3)
        sortedthetaphi(q,:)=thetaphi(i,:);
        q = q+1;
    end
end

Nested2RingMap = sortedthetaphi(:,3);
invcenters=centers';
RingedCenters=invcenters(Nested2RingMap,:);

RingedRotMats=rotations(Nested2RingMap,:);

plot_= 0;
if plot_
    figure(2); clf; hold on
    for i = 1:size(thetaphi)
        %plot(blah3(1:i,1),blah3(1:i,2),'.')
        %axis([-3.5,3.5,-0.5,3.5])
        %pause(1)
        plot3(RingedCenters(1:i,1),RingedCenters(1:i,2),RingedCenters(1:i,3),'.')
        axis([-80,80,-80,80,-80,80])
        drawnow
    end
end



% write rotations and cube centers to file

fileID = fopen('centervertices_Ring.txt','w');
for i=1:192
    fprintf(fileID,'%6.8f %6.8f %6.8f\n',RingedCenters(i,1),RingedCenters(i,2),RingedCenters(i,3));
end
fclose(fileID);

fileID = fopen('rotationmatrices_Ring.txt','w');
for i=1:192
    fprintf(fileID,'%6.8f %6.8f %6.8f %6.8f %6.8f %6.8f %6.8f %6.8f %6.8f \n',RingedRotMats(i,1), RingedRotMats(i,2), RingedRotMats(i,3), RingedRotMats(i,4), RingedRotMats(i,5), RingedRotMats(i,6), RingedRotMats(i,7), RingedRotMats(i,8), RingedRotMats(i,9));
end
fclose(fileID);



figure(3); clf; hold on
for i =1:192
    plot3(RingedCenters(:,1),RingedCenters(:,2),RingedCenters(:,3),'.');
    c=[RingedCenters(i,1),RingedCenters(i,2),RingedCenters(i,3)];
    text(c(1),c(2),c(3),sprintf('%d',i), 'Color',cmap(i,:));
end
hold on
xlabel x; ylabel y; zlabel z
axis equal; xlim([-75,75]); ylim(xlim); zlim(xlim)
