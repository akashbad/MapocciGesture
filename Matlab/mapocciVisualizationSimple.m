function data = mapocciVisualizationSimple(s)
    figure;
    axis([0 40 0 200]);
    while true
        line = fgetl(s);
        nums = regexp(line, '\t','split');
        data = str2double(nums);
        clf;
%         if(length(data)==40)
%             data(40) = data(40)*5;
%         end
        stem(data);
        axis([0 40 0 200]);
        drawnow;
    end
end
    
    