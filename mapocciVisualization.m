function data = mapocciVisualization(s)
    figure;
    axis([0 40 0 200]);
    while true
        line = fgetl(s);
        nums = regexp(line, '\t','split');
        data = str2double(nums);
        clf;
        stem(data);
        axis([0 40 0 200]);
        drawnow;
    end
end
    
    