function rawData = mapocciTestAnalysis(file)
    fid = fopen(file);
    rawData = textscan(fid, '%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f');
    len = length(rawData{1})-1
    time = rawData{1}(1:len);
    accel = zeros(3,len);
    gyro = zeros(1,len);
    pressure = zeros(39,len);
    touch = zeros(2,len);
    capacitive = zeros(3,len);
    for i=2:49
        if i<5
            accel(i-1,:) = rawData{i}(1:len);
        elseif i<6
            gyro(i-4,:) = rawData{i}(1:len);
        elseif i<45
            temp0 = arrayfun(@removeDeadPixels, rawData{i});
            pressure(i-5,:) = temp0(1:len);
        elseif i<47
            temp1 = arrayfun(@removeDeadCaps, rawData{i});
            touch(i-44,:) = temp1(1:len);
        else
            temp2 = arrayfun(@removeDeadCaps, rawData{i});
            capacitive(i-46,:) = temp2(1:len);
        end
    end
    figure;
    imagesc(time,1:3,accel);
    colormap(bone);
    caxis([0 675]);
    figure;
    plot(time, gyro);
    figure;
    plot(time, touch);
    figure;
    plot(time, capacitive);
    figure;
    imagesc(time, 1:39, pressure);
end


function data = removeDeadCaps(input)
    if(input > 500)
        data = 0;
    else
        data = input;
    end
end

function data = removeDeadPixels(input)
    data = input - 270;
    if(data > 250)
        data = 0;
    end
end