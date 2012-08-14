function myData = getData(file)
    fid = fopen(file);
    myData = textscan(fid,'%s');
    fclose(fid);
end