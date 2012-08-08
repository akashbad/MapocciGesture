function Z = getComprehensiveData(file)
    fid = fopen(file);
    myData = textscan(fid,'%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f');
    figure;
    hold all;
    Z = zeros(17,360);
    for i=1:16
        temp = arrayfun(@removeDeads,myData{i});
        Z(i,:) = filter([1 0], [1 0],temp(1:360))';
    end 
    Z(17,:) = sum(Z(1:16,:));
    imagesc(Z);
    colormap(bone);
    caxis([0 60]);
    axis([0 60 .5 17.5]);
    
%     figure;
%     hold all;
%     for i=17:19
%         plot(myData{i});
%     end
%     axis([0 60 200 400]);
%     
%     avgs = getMean(Z);
%     figure;
%     stem(avgs, 'DisplayName', 'mean');
%     axis([0 60 1 15]);
%     
%     [fits, norms] = linearFit(avgs);
%     figure;
%     stem(fits, 'DisplayName', 'fits');
%     axis([0 60 -5 5]);
%     
%     figure;
%     stem(norms, 'DisplayName', 'norms');
%     axis([0 60 -5 5]);
%     
%     stds = zeros(1,360);
%     
%     modes = zeros(1,360);
%     for i=1:360
%         stds(i) = getStd(avgs(i), Z(:,i));
%         modes(i) = find(Z(:,i)==max(Z(:,i)),1);
%     end
%      figure;
%      stem(stds, 'DisplayName', 'std');
%      xlim([0 60]);
%     figure;
%     stem(modes, 'DisplayName', 'mode');
%     xlim([0 60]);
%     tempDiff = arrayfun(@digital,diff(avgs));
%     diffs = filter([1 0], [1 -1], tempDiff);
% %   diffs = diff(avgs);
%     figure;
%     stem(diffs, 'DisplayName', 'diff');
%     xlim([0 60]);
end

function data = removeDeads(input)
    if(input > 500)
        data = 0;
    else
        data = input - 250;
    end
    if(data < 20)
        data = 0;
    end
end

function data = getMean(input)
    total = sum(input);
    data = zeros(1,size(input,2));
    for i=1:size(input,1)
        data = data + input(i,:)*i;
    end
    data = data./total;
end

function data = getStd(mean, input)
    variance = 0;
    total = sum(input);
    for i=1:length(input)
        variance = variance + input(i)*(i-mean)^2;
    end
    variance = variance / total;
    data = sqrt(variance);
end


function signals = digital(number)
    if(number > 0)
            signals = 1;
    elseif (number < 0)
        signals = -1;
    else
        signals = 0;
    end
end

function [output, normrs] = linearFit(input)
    output = zeros(1,length(input));
    normrs = zeros(1,length(input));
    for i=4:length(input)-3
        y = input(i-3:i+3);
        x = i-3:i+3;
        ns = isnan(y);
        y(ns) = [];
        x(ns) = [];
        [coeffs, S] = polyfit(x,y,1);
        output(i) = coeffs(1);
        normrs(i) = S.normr;
    end
end