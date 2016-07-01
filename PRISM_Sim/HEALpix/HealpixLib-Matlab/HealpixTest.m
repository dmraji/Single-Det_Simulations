n = 16; %768 points

I = HealpixGenerateSampling(n, 'rindex');
S = HealpixGenerateSampling(n, 'scoord');
Sring = sortrows(S);
C = SphToCart(S);
plot3(C(:,1),C(:,2),C(:,3), '.')
%plot3(C(:,1),C(:,2),C(:,3))
axis equal


% Plot ring indexed points on at a time
ringplot = 1;  %plot if 1, dont plot if 0
if ringplot
    C = SphToCart(Sring);
    figure;
    for i = 1:size(S)
        plot3(C(1:i,1),C(1:i,2),C(1:i,3), '.')
        axis ([-1 1 -1 1 -1 1])
        drawnow
    end
end


% Write to file
fileID = fopen('HEALPix_PhiTheta_Nested.txt','w');
fileID2 = fopen('HEALPix_PhiTheta_Ring.txt','w');
for i = 1:size(S)
    fprintf(fileID,  '%6.5f %6.5f\n',S(i,1), S(i,2));
    fprintf(fileID2,'%6.5f %6.5f\n',Sring(i,1), Sring(i,2));
end
fclose(fileID);
fclose(fileID2);
