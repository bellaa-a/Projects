% Function to create a breaksafter vector given a template
% Goes day-by-day through the template. If the day is an off day,
% then appends the previous slot number to the breaksafter vector.
function [breaksafter] = getBreaksAfter(template)
breaksafter = [];
[rows,cols] = size(template);
prev = 0;
for r = 1:rows
    for c = 1:cols
        if template(r,c) == 0
            if prev ~= 0
                breaksafter = [breaksafter prev];
                prev = template(r,c);
            end
            % if prev == 0, nothing to do
        else
            prev = template(r,c);
        end
    end
end
breaksafter = breaksafter(1:end-1);
end