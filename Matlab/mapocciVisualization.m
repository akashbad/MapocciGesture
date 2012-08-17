function data = mapocciVisualization(s)
    figure;
    axis([0 39 0 1000]);
    while true
        line = fgetl(s);
        nums = regexp(line, '\t','split');
        data = str2double(nums);
        clf;
        stem(data);
        axis([0 39 0 1000]);
        drawnow;
    end
end
    
    