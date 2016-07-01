%%
% Outer View (from Z+)
%       Y
%   . . | . . 
% . . . | . 1 2
% . . . | 3 4 5
% ------------- X
% . . . | . . .
% . . . | . . .
%   . . | . . 

L = 9.88;  % [mm]
x0 = zeros(5,2,3);  % [five unique locations, 2 outer corners (low-XY, high-XY), xyz [mm]]
x0(1,:,:) = [30.00 29.99 58.65; 21.49 21.48 65.71];
x0(2,:,:) = [44.03 29.91 49.05; 38.34 20.67 57.83];
x0(3,:,:) = [14.06 14.04 69.60;  4.35  4.34 72.14];
x0(4,:,:) = [31.61 14.03 63.59; 22.97  4.24 68.53];
x0(5,:,:) = [46.98 14.03 53.24; 39.92  4.24 60.25];


%% Build 32-Detector Face

% 5 unique 2-corner pairs
x = zeros(5,2,3);
fi = 1:5;
ti = [9:10 14:16]; % DetectorIndex on each face
x(ti,:,:) = x0(fi,:,:);  % map old to new

% 5 -> 8, reflect 3 non-diagonal elements
n = [1 -1 0]';  % normal to x=y diagonal
n = n / norm(n);
A = eye(3) - 2*(n*n');  % reflection matrix
fi = [16 10 15];
ti = [3 4 8];
for i = 1:length(fi)
   x(ti(i),:,:) = [A * squeeze(x(fi(i),1,:)), A * squeeze(x(fi(i),2,:))]';
end

% 8 -> 16 reflection
n = [1 0 0]';  % normal to x=y diagonal
A = eye(3) - 2*(n*n');  % reflection matrix
fi = [3:4 8:10 14:16];
ti = [2:-1:1 7:-1:5 13:-1:11];
for i = 1:length(fi)
   x(ti(i),:,:) = [A * squeeze(x(fi(i),1,:)), A * squeeze(x(fi(i),2,:))]';
end

% 16 -> 32 rotation 
R = rotationmat3D(pi,[0 0 1]);
fi = [1:16];
ti = [32:-1:17]; % reverse indices
for i = 1:length(fi)
   x(ti(i),:,:) = [R * squeeze(x(fi(i),1,:)), R * squeeze(x(fi(i),2,:))]';
end

%% Place 6 Faces - based on 1st Face above
% First Face (#6 below) [hor, vert, out-of-face] = [2 1 3] = [+Y +X +Z]
% Orientation matrix of 6 Faces
M = [
    2  3 -1
   -2  3  1
   -1  3 -2
    1  3  2
   -2  1 -3
    2  1  3
  ];

% [FaceIndex, DetectorIndex, 2-Points, XYZ] - [i j k :]
x3 = zeros(6,size(x,1),2,3);  
for i = 1:size(x3,1);
    C = coordTrans(M(6,:),M(i,:));
    for j = 1:size(x3,2)
        for k = 1:size(x3,3)
            x3(i,j,k,:) = C' * squeeze(x(j,k,:));
        end
    end
end

%% Plot x3
figure(1); clf; hold on
color = {'r','b','g','m','c','k'};
for i = 1:size(x3,1)
    for j = 1:size(x3,2)
        %plot3(squeeze(x3(i,j,:,1)), squeeze(x3(i,j,:,2)), squeeze(x3(i,j,:,3)),'-','Color',color{i});
        c = mean(squeeze(x3(i,j,:,:)));
         text(c(1),c(2),c(3),sprintf('%d',j),'Color',color{i});
    end
end
hold on;
a = squeeze(mean(x0,2));
%plot3(a(:,1),a(:,2),a(:,3),'ok','MarkerSize',10)

xlabel x; ylabel y; zlabel z
axis equal; xlim([-75,75]); ylim(xlim); zlim(xlim)

%% Draw CZT from 2 pts

a = squeeze(x(1,:,:));
c = mean(a);
R = rotationmat3D(pi/2,c);
b = R * a'; b = b';
a = [a;b];
% a = sortrows(a,1);
F = [1 2 3; 1 2 4];

%% Spherical Detector Plot
figure(2);clf
hold on

F =[1 3 2 4; 
    5 7 6 8;
    1 5 7 3;
    3 7 6 2;
    2 6 8 4;
    4 8 5 1];
cmap = 0.7*hsv(6);

for i = 1:size(x3,1)
%     if mod(i,2)
%         U = U4;
%     else
%         U = setdiff(1:32,U4);
%     end
    for j = 1:size(x3,2)
        a = squeeze(x3(i,j,:,:));  % 2 unique xyz points [2x3]
        c = 1.01 * mean(a);
        a = [a; (rotationmat3D(pi/2,mean(a)) * a')'];
        a = [a; a - repmat(L * mean(a(1:2,:)) / norm(mean(a(1:2,:))),[size(a,1),1])];
        %text(c(1),c(2),c(3),sprintf('%d',j),'Color','k');
        %plot3q(mean(a(5:end,:)),'.')
        patch('Faces',F,'Vertices',a,'FaceColor',cmap(i,:),'FaceAlpha',0.35,'EdgeColor',cmap(i,:)*0.75)
%         patch('Faces',F,'Vertices',a,'FaceColor',cmap(i,:),'FaceAlpha',0.35,'EdgeColor','none')
%         patch('Faces',F,'Vertices',a,'FaceColor','k','EdgeColor','none')
    end
end

plot3(0,0,0,'ok')
xlabel 'x (mm)'; ylabel 'y (mm)'; zlabel 'z (mm)'
axis equal; xlim([-75,75]); ylim(xlim); zlim(xlim)

%return
%% ===========================================================================


%% Spherical Detector Plot (coded)
figure(3);clf
hold on

F =[1 3 2 4; 
    5 7 6 8;
    1 5 7 3;
    3 7 6 2;
    2 6 8 4;
    4 8 5 1];
cmap = 0.7*hsv(6);

randommask = [zeros(1,16), ones(1,16)];
randommask = randommask(randperm(32));

for i = 1:size(x3,1)
%     if mod(i,2)
%         U = U4;
%     else
%         U = setdiff(1:32,U4);
%     end
    for j = 1:size(x3,2)
        a = squeeze(x3(i,j,:,:));  % 2 unique xyz points [2x3]
        c = 1.01 * mean(a);
        a = [a; (rotationmat3D(pi/2,mean(a)) * a')'];
        a = [a; a - repmat(L * mean(a(1:2,:)) / norm(mean(a(1:2,:))),[size(a,1),1])];
        %text(c(1),c(2),c(3),sprintf('%d',j),'Color','k');
        %plot3q(mean(a(5:end,:)),'.')
        if  randommask(j) == 1
        patch('Faces',F,'Vertices',a,'FaceColor',cmap(i,:),'FaceAlpha',0.4,'EdgeColor',cmap(i,:)*0.75)
%         patch('Faces',F,'Vertices',a,'FaceColor',cmap(i,:),'FaceAlpha',0.35,'EdgeColor','none')
%         patch('Faces',F,'Vertices',a,'FaceColor','k','EdgeColor','none')
        end
    end
end

plot3(0,0,0,'ok')
xlabel 'x (mm)'; ylabel 'y (mm)'; zlabel 'z (mm)'
axis equal; xlim([-75,75]); ylim(xlim); zlim(xlim)

return
%% ===========================================================================



%% Build Spherical CZT Array for MEGALib
% GeoMega requires xyz rotations

L = 0.988; % [cm]

x0 = zeros(5,4,3);  % [five unique locations, 2 outer corners [high-XY, low-XY], xyz [cm]] (can be any two diagonal corners)
x0(1,1:2,:) = [3.000 2.999 5.865; 2.149 2.148 6.571];
x0(2,1:2,:) = [4.403 2.991 4.905; 3.834 2.067 5.783];
x0(3,1:2,:) = [1.406 1.404 6.960; 0.435 0.434 7.214];
x0(4,1:2,:) = [3.161 1.403 6.359; 2.297 0.424 6.853];
x0(5,1:2,:) = [4.698 1.403 5.324; 3.992 0.424 6.025];

% Add 2 more points to complete top square of detector
for i = 1:size(x0,1)
    xct = mean(squeeze(x0(i,:,:)));  % Top Center of Detector
    R = rotationmat3D(pi/2, xct);
    x0(i,3,:) = R * squeeze(x0(i,1,:));
    x0(i,4,:) = R * squeeze(x0(i,2,:));
end

%%  Array 32 Detectors from Original 5

% 5 unique outer detector faces
x1 = zeros(32,4,3);
fi = 1:5; % fromIndex
ti = [9 10  14 15 16]; % toIndex
x1(ti,:,:) = x0(fi,:,:);  % map old to new

% 5 -> 8, reflect 3 non-diagonal elements
xyn = [1 -1 0]' / norm([1 -1 0]);  % normal to x=y diagonal
A = eye(3) - 2 * (xyn * xyn');  % reflection matrix
fi = [16 10 15];
ti = [3 4 8];
for i = 1:length(fi)
    for j = 1:size(x1,2)
        x1(ti(i),j,:) = A * squeeze(x1(fi(i),j,:));
    end
end

% 8 -> 16 reflection
xn = [1 0 0]';  % x
A = eye(3) - 2 * (xn * xn');  % reflection matrix
fi = [3 4  8 9 10  14 15 16];
ti = [2 1  7 6 5  13 12 11];
for i = 1:length(fi)
    for j = 1:size(x1,2)
        x1(ti(i),j,:) = A * squeeze(x1(fi(i),j,:));
    end
end

% 16 -> 32 rotation 
R = rotationmat3D(pi,[0 0 1]);
fi = [1:16]; 
ti = [32:-1:17]; % reverse indices
for i = 1:length(fi)
    for j = 1:size(x1,2)
        x1(ti(i),j,:) = R * squeeze(x1(fi(i),j,:));
    end
end


%% Locate Detector Center and Coordinate Rotation Angles

x = struct;
for i = 1:size(x1,1)
    x(i).id = i;
    x(i).p = squeeze(x1(i,:,:)); % 4x3, where [1:2,:] are diagonally opposed
    
    % 1. Axis-3: Detector center to Origin (not normalized)
    % 2. Detector Center Location
    x(i).w = zeros(3,3);  
    x(i).w(3,:) = mean(x(i).p);  % Top Center of Detector
    x(i).xcd = x(i).w(3,:)  * (1 - L/2 / norm(x(i).w(3,:)));  % Center of Detector

    % 3. Axis-1: -x-y:+x-y,  (not normalized)
    % 4. Axis-2: +x-y:+x+y,  (not normalized)
    P = x(i).p - repmat(x(i).w(3,:),[4,1]); % put points in 4 quadrants
    p1 = x(i).p((P(:,1) > 0) & (P(:,2) > 0),:);  % find +x,+y
    p2 = x(i).p((P(:,1) < 0) & (P(:,2) < 0),:);  % find -x,-y
    p3 = x(i).p((P(:,1) > 0) & (P(:,2) < 0),:);  % find +x,-y
    x(i).w(1,:) = p3 - p2;
    x(i).w(2,:) = p1 - p3;
    
    % 5. Rotation Matrix between Cartesian axes and detector axes (Wahba's Problem)
    [U,~,V] = svd(x(i).w');
    x(i).R = U * V';
    
    % 6. (x,y,z) Angles (roll,pitch,yaw)
    x(i).ang = atand([ +x(i).R(3,2) / x(i).R(3,3); ...
                       -x(i).R(3,1) / norm(x(i).R(1:2,1)); ...
                       +x(i).R(2,1) / x(i).R(1,1)]);
end


%% Write the PRISM_Face.geo file

% Random Mask
ZO = [zeros(1,16), ones(1,16)];
ZO = ZO(randperm(32));

% The Six Unique 50% Masks
% ZO = [0 1 1 0, 0 0 1 1 0 0, 1 1 0 0 1 1]; 
% ZO = [1 1 1 1, 1 0 0 0 0 1, 1 0 0 0 0 1]; 
% ZO = [1 0 0 1, 1 0 1 1 0 1, 0 1 0 0 1 0]; 
ZO = [0 1 1 0, 0 1 0 0 1 0, 1 0 1 1 0 1];
% ZO = [0 0 0 0, 0 1 1 1 1 0, 0 1 1 1 1 0];
% ZO = [1 0 0 1, 1 1 0 0 1 1, 0 0 1 1 0 0];
ZO = [ZO,fliplr(ZO)];

g = {};
for i = 1:length(x)
    if ZO(i)
        g = [g, sprintf('volCZT.Copy volCZT_%u \n',x(i).id)];
        g = [g, sprintf('volCZT_%u.Position %g %g %g \n',i,x(i).xcd - [0 0 7.25/2])]; % center of trap
        g = [g, sprintf('volCZT_%u.Rotation %g %g %g \n',i,x(i).ang)];
        g = [g, sprintf('volCZT_%u.Mother volFace \n\n',i)];    
    end
end
fp = '/Users/Hellfeld/Documents/School/UCB/Research/Coded\ Aperature/PRISM/GeometryFiles';
fn = 'PRISM_CZTs.geo';
dlmwrite([fp fn],g,'')
% open([fp fn])


%% Write PRISM_Faces.geo
TP = {'0 0 ZRAD';
    '0 0 -ZRAD';
    'ZRAD 0 0' ;
    '-ZRAD 0 0' ;
    '0 -ZRAD 0';
    '0 ZRAD 0';
    };
TR = [...
    0   0   0;   % +Z
    180 0   0;   % -Z
    0  90   0;   % +X
    0 -90   0;   % -X
    90  0   0;   % -Y
    -90 0   0];  % +Y
g = {};
for i = 1:6
    g = [g, sprintf('volFace.Copy volFace%u \n',i)];
    g = [g, sprintf('volFace%u.Visibility 0 \n',i)];
    g = [g, sprintf('volFace%u.Mother volWorld \n',i)];
    g = [g, sprintf('volFace%u.Position %s \n',i,TP{i})];
    g = [g, sprintf('volFace%u.Rotation %g %g %g \n',i,TR(i,:))];
    g = [g, sprintf('volFace%u.Virtual true \n\n',i)];
end
fp = '/Users/Hellfeld/Documents/School/UCB/Research/Coded Aperature/PRISM/GeometryFiles';
fn = 'PRISM_Faces.geo';
dlmwrite([fp fn],g,'')
% open([fp fn])


%#ok<*NBRAK>