function data = processData(path)
    fid = fopen(path);
    mydata = textscan(fid,'%d %s %s');
    data.all = zeros(length(mydata{2}),3);
    data.all(:,1) = mydata{1};
    for k = 1:length(mydata{2})
        data.all(k,2) = mydata{2}{k};
        data.all(k,3) = mydata{3}{k};
    end
    fclose(fid);
    data.all(:,2) = data.all(:,2) - 33;
    data.all(:,3) = data.all(:,3) - 33;
    data.pinsH{1} = data.all(1:6:length(data.all),2);
    data.pinsH{2} = data.all(2:6:length(data.all),2);
    data.pinsH{3} = data.all(3:6:length(data.all),2);
    data.pinsH{4} = data.all(4:6:length(data.all),2);
    data.pinsH{5} = data.all(5:6:length(data.all),2);
    data.pinsH{6} = data.all(6:6:length(data.all),2);
    data.pinsL{1} = data.all(1:6:length(data.all),3);
    data.pinsL{2} = data.all(2:6:length(data.all),3);
    data.pinsL{3} = data.all(3:6:length(data.all),3);
    data.pinsL{4} = data.all(4:6:length(data.all),3);
    data.pinsL{5} = data.all(5:6:length(data.all),3);
    data.pinsL{6} = data.all(6:6:length(data.all),3);
    data.pins{1} = smooth(bitshift(data.pinsH{1},8)+ data.pinsL{1},10);
    data.pins{2} = smooth(bitshift(data.pinsH{2},8)+ data.pinsL{2},10);
    data.pins{3} = smooth(bitshift(data.pinsH{3},8)+ data.pinsL{3},10);
    data.pins{4} = smooth(bitshift(data.pinsH{4},8)+ data.pinsL{4},10);
    data.pins{5} = smooth(bitshift(data.pinsH{5},8)+ data.pinsL{5},10);
    data.pins{6} = smooth(bitshift(data.pinsH{6},8)+ data.pinsL{6},10);
    data.names = {'Pin0','Pin1','Pin2','Pin3','Pin4','Pin5'};
    for k = 1:6
        h = plot(data.pins{k});
        set(h, 'DisplayName', data.names{k});
        hold all;
    end
end



