%--------------------------PART-E----------------------------
%--------------Optional mirror table 24th gen----------------

%Initial matrix
table = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];

%Find the dimensions of the initial matrix so that the code works for any
%matrix of size MxN 
[r, c] = size(table);

%Output matrix containing final result 
%Needed because otherwise we will be altering the initial matrix as we go
output4 = zeros(15, 15);

for n = 1:24
    %Loops through every element in the entire matrix
    for i = 1:r
        for j = 1:c
    
            %Reset the sum back to zero for every element
            sum = 0;
    
            %Conditional statements to check each position (if there exists)
            %neighboring the center piece is a 1
            %Adds 1 to sum to keep track of total neighbors that is a 1
            if i ~= 1 && j ~= 1 && table(i-1, j-1) == 1
                sum = sum + 1;
            end
            if i ~= 1 && table(i-1, j) == 1
                sum = sum + 1;
            end
            if i ~= 1 && j ~= c && table(i-1, j+1) == 1
                sum = sum + 1;
            end
            if j ~= 1 && table(i, j-1) == 1
                sum = sum + 1;
            end
            if j ~= c && table(i, j+1) == 1
                sum = sum + 1;
            end
            if i ~= r && j ~= 1 && table(i+1, j-1) == 1
                sum = sum + 1;
            end
            if i ~= r && table(i+1, j) == 1
                sum = sum + 1;
            end
            if i ~= r && j ~= c && table(i+1, j+1) == 1
                sum = sum + 1;
            end
    
            %Conditional statements to check the four rules of the game
            %Updates output accordingly
            if table(i, j) == 1
                if sum < 2 || sum > 3
                    outpu4(i, j) = 0;
                else 
                    output4(i, j) = 1;
                end
            else
                if sum == 3
                    output4(i, j) = 1;
                else
                    output4(i, j) = 0;
                end
            end
        end
    end
    table = output4;
end

%Display the final output matrix to the screen
disp(output4)
imagesc(output4)
colormap(gray)