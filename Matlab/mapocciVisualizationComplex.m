function data = mapocciVisualizationComplex(s)
    %if(strcmp(get(s,'Status'),'open'))
     %   fclose(s);
    %end
    %fopen(s);f
    figure;
    axis([0 19 0 800]);
    while true
        line = fgetl(s);
        nums = regexp(line, '\t','split');
        data = str2double(nums);
        clf;
%         if(length(data)>16)
%             stem(data(1:16));
%             mean(1) = {'MEAN'};
%             mean(2) = {num2str(data(17))};
%             text(1, 200, mean);
%             if(length(data)>17)
%                 std(1) = {'STD'};
%                 std(2) = {num2str(data(18))};
%                 text(4, 200, std);
%                 if(length(data)>18)
%                     mode(1) = {'MODE'};
%                     mode(2) = {num2str(data(19))};
%                     text(7, 200, mode);
%                     if(length(data)>19)
%                         diff(1) = {'Diff'};
%                         diff(2) = {num2str(data(20))};
%                         text(10, 200, diff);
%                         if(length(data)>20)
%                             filt(1) = {'Filt'};
%                             filt(2)= {num2str(data(21))};
%                             text(13, 200, filt);
%                         end
%                     end
%                 
%                 end
%             end
%             
%         else
            stem(data);
%         end
        axis([0 19 0 800]);
        drawnow;
    end
end
    
    